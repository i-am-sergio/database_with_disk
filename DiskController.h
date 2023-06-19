#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include "Disco.h"
#include "tipos.cpp"
using namespace std;

class DiskController{
public:
    Disco * disco;
    int nTotalBloques;
    int sizeBloque;
    vector<tuple<string,string,int>> info;
    
    DiskController(Disco* disco){
        this->disco = disco;
        this->nTotalBloques = disco->getNumBloques();
        this->sizeBloque = disco->capacidadDelSector * disco->sectoresPorBloque;

        system("mkdir disk");
        for(int i=1; i<= nTotalBloques; i++){
            string name_bloque = "bloque";
            name_bloque += to_string(i);
            ofstream archivo("disk/"+name_bloque+".bin", std::ios::binary);

            // std::ofstream archivo("datos.bin", std::ios::binary);
            if (!archivo) { std::cout << "No se pudo abrir el archivo." << std::endl; return; }
            // char buffer[sizeBloque];
            char * buffer = new char[sizeBloque];
            // Rellenar el buffer con datos (en este ejemplo, caracteres 'A')
            for (int j = 0; j < sizeBloque; j++) {
                buffer[j] = '\0';
            }
            archivo.write(buffer, sizeBloque);
            
            delete [] buffer;
            //delete buffer;
            
            archivo.close();
        }
        std::cout << "Se han generado los bloques correctamente" << std::endl;
    }
    ~DiskController(){
        system("rmdir /s /q disk");
    }

    void tableToVector(string nameTable){
      ifstream data;
      data.open("esquema",ios::in);
      string linea;
      while(getline(data,linea)){
        stringstream stream(linea);
        string word;
        getline(stream,word,'#');
        if (word == nameTable) {
          break;
        }
      }
      //Lleva a la memoria dinamica la ubicacion de los bytes por cada atributo
      //cout<<linea<<endl;
      stringstream stream(linea);
      string w1,w2,w3;
      getline(stream,w1,'#');
      getline(stream,w1,'#');
      while (!stream.eof()) {
          getline(stream,w1,'#');
          getline(stream,w2,'#');
          if ((w2 == "int")){
            info.push_back(make_tuple(w1,w2,mytipos::_INT));
            continue;
          } else if(w2 == "double"){
            info.push_back(make_tuple(w1,w2,mytipos::_DOUBLE));
            continue;
          } else if(w2 == "float"){
            info.push_back(make_tuple(w1,w2,mytipos::_FLOAT));
            continue;
          } else if(w2 == "bool"){
            info.push_back(make_tuple(w1,w2,mytipos::_BOOL));
            continue;
          }
          getline(stream,w3,'#');
          info.push_back(make_tuple(w1,w2,stoi(w3)));
      }
      cout<<info.size()<<"tam"<<endl;
    //   for(int i=0; i<(int)info.size(); i++){
    //     cout<<get<0>(info[i])<<"-"; 
    //     cout<<get<1>(info[i])<<"-"; 
    //     cout<<get<2>(info[i])<<"\n"; 
    //   }
      data.close();
    }

    void uploadTableToDisk(string fileToImport, string tablaName){ //(const char * tablaNameFile, int sizeFile)
        // prepareCSV_inTuplas(tablaNameFile);

        tableToVector(tablaName);
        convertCSV_inTuplas(fileToImport,tablaName,info.size());

        const char* carpetaDisco = "disco\\";

        int contador = 0;
        
        char caracter;
        int entero;
        float decimal;
        double decimalGrande;

        ifstream archivoTupla("titanic");
        ofstream salida("titanicbinario.bin", std::ios::binary);
        
        char c;
        char z;

        string linea;
        string palabra;
        int contadorBytes = 0;
        while(getline(archivoTupla,linea)){
            stringstream stream(linea);
            int indice = 0;
            while(getline(stream,palabra,'#')){ // palabra 5    20
                if(get<1>(info[indice])=="int"){
                    if(palabra==""){
                        entero = -1;
                    } else {
                        entero = stoi(palabra);
                    }
                    salida.write(reinterpret_cast<char*>(&entero), sizeof(entero));
                } else if (get<1>(info[indice])=="double"){
                    if(palabra==""){
                        decimalGrande = -1;
                    } else {
                        decimalGrande = stod(palabra);
                    }
                    salida.write(reinterpret_cast<char*>(&decimalGrande), sizeof(decimalGrande));
                } else if (get<1>(info[indice])=="str"){
                    for (auto& i : palabra){
                        salida.write(reinterpret_cast<char*>(&i), sizeof(char));
                    }
                    int bytesPorCompletar =  get<2>(info[indice]) - palabra.size();
                    for (auto i=0; i<bytesPorCompletar; i++){
                        z = ' ';
                        salida.write(reinterpret_cast<char*>(&z), sizeof(char));
                    }
                }
                indice++;
            }
        }


        for ( auto& atributo : info ){
            cout<<get<0>(atributo)<<"-"; 
            cout<<get<1>(atributo)<<"-"; 
            cout<<get<2>(atributo)<<"\n"; 
        }
        
        archivoTupla.close();
        salida.close();

        int sizeRegistro = 0;
        for(auto& i : info){
            if(get<1>(i)=="int"){
                sizeRegistro += 4;
            } else if (get<1>(i)=="double"){
                sizeRegistro += 8;
            } else if (get<1>(i)=="str"){
                sizeRegistro += get<2>(i);
            }
        }
        setDataInBloques(sizeRegistro);
    }

    void setDataInBloques(int sizeRegistro){
        int contador = 1;
        int contadorBytesTotal;
        int contAux = 0;
        char caract;
        int entero;
        double decimalGrande;
        ifstream file("titanicbinario.bin", std::ios::binary);
        
        file.seekg(0,ios::end);
        int sizeFile = file.tellg();
        file.seekg(0,ios::beg);
        
        while(contAux <= sizeFile){
            ofstream bloque("disk/bloque"+std::to_string(contador)+".bin", std::ios::binary);
            bloque.seekp(sizeRegistro-sizeof(int));
            int freelist = 0; //puntero inicial de la freelist que se inicializara en 0
            bloque.write(reinterpret_cast<char*>(&freelist), sizeof(int));

            contadorBytesTotal = sizeRegistro*2;
            while(contadorBytesTotal <= sizeBloque){ // sizeRegistro <= (sizeBloque-contadorBytesTotal)
                char* buffer = new char[sizeRegistro];
                file.read(buffer,sizeRegistro);
                bloque.write(buffer,sizeRegistro);
                contadorBytesTotal += sizeRegistro;
                contAux += contadorBytesTotal;
                delete [] buffer;
            }

            char caracterVacio = '\0';

            cout<<"\t\t aquiiiii->>>"<<sizeBloque-contadorBytesTotal+sizeRegistro<<"<-----"<<endl;
            char * buffer = new char[sizeBloque-contadorBytesTotal+sizeRegistro];
            // Rellenar el buffer con datos (en este ejemplo, caracteres 'A')
            for (int j = 0; j < (sizeBloque - contadorBytesTotal+sizeRegistro); j++) {
                buffer[j] = ' ';
            }
            bloque.write(buffer, (sizeBloque - contadorBytesTotal+sizeRegistro));
            delete [] buffer;

            /*for(int i=0;i<sizeBloque-contadorBytesTotal;i++){
                bloque.write(reinterpret_cast<char*>(caracterVacio),sizeof(char));
            }
            cout<<"\nllego Aqui ------------\n";*/

            bloque.close();
            contador++;
            
        }
        file.close();
    }

    void convertCSV_inTuplas(string fileAimportar, string newfile,int natributos){

        ifstream i(fileAimportar, ios::in); 
        ofstream f(newfile,ios::out); 
        bool reemplazarActivo = true;
        int indice=0;
        string word="";
        if (i && f) {
        char caracter;
        while (i.get(caracter)) {
            if (((caracter == ',')||(caracter =='\n')) && reemplazarActivo == true ) {
                if(indice<natributos){ //natributos
                    f<<word;
                    f<<'#';
                    word="";
                    indice++;
                    if (indice >= natributos) { //natributos
                        indice=0;
                        f<<'\n';
                    }
                }
            } else if(caracter == '"'){
                reemplazarActivo = (!reemplazarActivo);
            } else{
                if(caracter != '\r')
                    word+=caracter;
            }
        }
        cout << "Archivo importado exitosamente." << endl;
        }
        i.close(); 
        f.close(); 
    }

    void prepareCSV_inTuplas(const char * CSVName)
    {
        ifstream archivoCSV(CSVName);
        ofstream salida("tupla_titanic");

        if (!archivoCSV)
        {
            cout << "No se pudo abrir el archivo de entrada." << endl;
            return;
        }
        if (!salida)
        {
            cout << "No se pudo abrir el archivo de salida." << endl;
            return;
        }

        char c;
        bool dentroDeComillas = false;
        while (archivoCSV.get(c))
        {
            if (c == '"' && dentroDeComillas == false) {
                dentroDeComillas = true;
            }
            else if (c == '"' && dentroDeComillas == true) {
                dentroDeComillas = false;
            }
            if (c == ',' && dentroDeComillas == false) {
                salida << '#';
            }
            else {
                salida << c;
            }
        }
        archivoCSV.close();
        salida.close();
        cout << "Se ha generado tupla_" << CSVName << endl;
    }

    int findEsquemaDeTabla(const char * tablaName){
        ifstream esquemaTabla("esquema.txt");
        if ( !esquemaTabla.is_open()) { std::cout << "Error al abrir el archivo." << std::endl; }

        char c;
        int contadorSizeTablaName = 0;
        int indice = 0;
        char tablaNameDelEsquema[50];
        char * ptr = tablaNameDelEsquema;
        streampos posicion;
        // buscar tabla en esquema
        while(true){
            contadorSizeTablaName = 0;
            while(esquemaTabla.get(c)){ // obtiene el nombre hasta el simbolo #
                indice++;
                if(c=='#') 
                    break;
                tablaNameDelEsquema[contadorSizeTablaName] = c;
                contadorSizeTablaName++;
            }
            tablaNameDelEsquema[contadorSizeTablaName] = '\0';

            if(strcmp(ptr,tablaName)==0){ // Si es la tabla?
                cout<<"encontro la tabla:\n";
                cout<<"\n|"<<tablaName<<"|"<<endl;
                cout<<"\n|"<<ptr<<"|"<<endl; 
                posicion = esquemaTabla.tellg(); // esta correcto
                //cout<<"extraer los sizes de los tipos de datos\n";
                break;
            } else {
                do{
                    esquemaTabla.get(c);
                    indice++;
                } while(c!='\n');
                indice++;// IMPORTANTE CONTAR EL SALTO DE LINEA
            }
        }
        esquemaTabla.close();
        return indice;
    }
};