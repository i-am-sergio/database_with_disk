#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <cmath>
#include "Disco.h"
#include "tipos.cpp"
using namespace std;

class DiskController{
public:
    Disco * disco;
    int nTotalBloques;
    int sizeBloque;
    int numSectoresPorBloque;
    vector<tuple<string,string,int>> info;
    
    DiskController(Disco* disco){
        this->disco = disco;
        this->nTotalBloques = disco->getNumBloques();
        this->sizeBloque = disco->capacidadDelSector * disco->sectoresPorBloque;
        this->numSectoresPorBloque = disco->sectoresPorBloque;

        if(!system("mkdir disk")){ // si no existen bloques, los crea. 
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
        } else {
            std::cout << "Se ha cargado la estructura de los bloques existente" << std::endl;
        }

    }
    ~DiskController(){
        //system("rmdir /s /q disk");
    }

    void tableToVector(string nameTable){
        this->info.clear();
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
        int numRegistrosDeLaTabla = 0;
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
            numRegistrosDeLaTabla++;
        }

        cout<<"\n-------numRegistrosDeLaTabla: "<<numRegistrosDeLaTabla<<"\n";
        ofstream numRegistrosPorTabla_dictionary("dictionary/numRegistros.bin", std::ios::binary | std::ios::app);
        numRegistrosPorTabla_dictionary.seekp(0,ios::end);
        numRegistrosPorTabla_dictionary.write(reinterpret_cast<char*>(&numRegistrosDeLaTabla), sizeof(int));
        numRegistrosPorTabla_dictionary.close();

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
        readSchemaBloquesFijos();
    }

    void setDataInBloques(int sizeRegistro){
        int contador = 1;
        int contadorBytesTotal;
        int contAux = 0;
        int numRegistros;

        ifstream numRegistros_dictionary("dictionary/numRegistros.bin", std::ios::binary);
        numRegistros_dictionary.read(reinterpret_cast<char*>(&numRegistros), sizeof(int));
        numRegistros_dictionary.close();

        ifstream file("titanicbinario.bin", std::ios::binary);
        ofstream schemaBloques("schemaBloquesFijos.bin", std::ios::binary);
        
        file.seekg(0,ios::end);
        int sizeFile = file.tellg();
        file.seekg(0,ios::beg);

        int almacenamientoTotal = this->nTotalBloques*this->sizeBloque;
        
        int contadorRegistros = 0;

        while((contAux < almacenamientoTotal) && (contadorRegistros <= numRegistros)){ //sizeFile
            ofstream bloque("disk/bloque"+std::to_string(contador)+".bin", std::ios::binary);
            bloque.seekp(sizeRegistro-sizeof(int));
            int freelist = 0; //puntero inicial de la freelist que se inicializara en 0
            bloque.write(reinterpret_cast<char*>(&freelist), sizeof(int));

            contadorBytesTotal = sizeRegistro*2;
            
            while((contadorBytesTotal <= sizeBloque) && (contadorRegistros <= numRegistros)){ // sizeRegistro <= (sizeBloque-contadorBytesTotal)
                char* buffer = new char[sizeRegistro];
                file.read(buffer,sizeRegistro);
                bloque.write(buffer,sizeRegistro);
                contadorBytesTotal += sizeRegistro;
                delete [] buffer;
                contadorRegistros++;
            }

            contAux += (contadorBytesTotal-sizeRegistro);
            int numRegistrosEnBloque = (contAux / (sizeRegistro*contador)) - 1;
            schemaBloques.write(reinterpret_cast<char*>(&numRegistrosEnBloque), sizeof(int));


            cout<<"\t\t bloque "<<contador<<"->>>"<<sizeBloque-contadorBytesTotal+sizeRegistro<<"<-----"<<endl;
            char * buffer = new char[sizeBloque-contadorBytesTotal+sizeRegistro];
            // Rellenar el buffer con datos (en este ejemplo, caracteres 'A')
            for (int j = 0; j < (sizeBloque - contadorBytesTotal+sizeRegistro); j++) {
                buffer[j] = ' ';
            }
            bloque.write(buffer, (sizeBloque - contadorBytesTotal+sizeRegistro));
            delete [] buffer;

            contAux += (sizeBloque-contadorBytesTotal+sizeRegistro);
            cout<<"contAux: "<<contAux<<"\t\t";

            bloque.close();
            contador++;
            
        }
        file.close();
        schemaBloques.close();
    }

    void readSchemaBloquesFijos(){
        int numRegistros;
        ifstream schemaBloques("schemaBloquesFijos.bin", std::ios::binary);
        int i = 1;
        while( i <= this->nTotalBloques){
            schemaBloques.read(reinterpret_cast<char*>(&numRegistros), sizeof(int));
            cout<<"bloque "<<i<<" => "<<numRegistros<<"\n";
            i++;
        }
        schemaBloques.close();
    }

    void printSector(int numSector){
        cout<<"\n--------------- | Sector "<<numSector<<" | ---------------\n";
        cout<<"numBloques"<<this->disco->getNumBloques()<<" \n";

        int findNumBloqueQueApuntaAlSector = std::ceil(static_cast<double>(numSector) / this->numSectoresPorBloque);
        int numSectorDentroDelBloque = numSector % this->numSectoresPorBloque;
        int posicionDelSectorEnArchivo = numSectorDentroDelBloque * this->disco->capacidadDelSector;
        
        cout<<"findNumBloqueQueApuntaAlSector: "<<findNumBloqueQueApuntaAlSector<<"\n";
        ifstream bloqueUbicado("disk/bloque"+std::to_string(findNumBloqueQueApuntaAlSector)+".bin",ios::binary);
        bloqueUbicado.seekg(posicionDelSectorEnArchivo);
        char c;
        while(!bloqueUbicado.eof() ){
            bloqueUbicado.read(static_cast<char*>(&c),sizeof(char));
            cout << static_cast<int>(c);
        }
        bloqueUbicado.close();
        
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