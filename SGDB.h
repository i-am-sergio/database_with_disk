#pragma once
#include "HeadersHDD/Disco.h"
#include "DiskController.h"
#include "BufferManager.h"
#include "tipos.cpp"
#include <iostream>
#include <fstream>
#include <sstream>


class SGDB
{
public:
    Disco *disco;
    DiskController *diskController;
    BufferManager *bufferManager;

    SGDB(Disco *disco, DiskController *diskController)
    {
        this->disco = disco;
        this->diskController = diskController;
        this->bufferManager = new BufferManager(this->diskController);
    }
    ~SGDB() {}

     void insertRegistroLongitudFija(string prompt){
        stringstream ss(prompt);
        string palabra; 
        string columnasDelRegistro;
        string tableName;
        int count = 0;

        while (ss >> palabra) {
            count++;;
            if (count == 3) {
                tableName = palabra;
                continue;
            }
            if(count > 3){
                columnasDelRegistro += palabra;
            }
        }

        //Solicito un frame;
        char * frame = bufferManager->getPageOfBuuferPool(1)->data;

        
        //titanic 12 passengerid,int,survived,int,Pclass,int,Name,str,90,Sex,str,6,age,double,SibSp,int,Parch,int,Ticket,str,20,Fare,double,Cabin,str,20,Embarked,str,1
        //889,0,3,"Johnston, Miss. Catherine Helen Carrie",female,,1,2,W./C. 6607,23.45,,S

        //889#0#3#Johnston, Miss. Catherine Helen Carrie#female##1#2#W./C. 6607#23.45##S#
    }


    void insertRegistroLongitudVariable(string prompt){
        stringstream ss(prompt);
        string tempPalabra; 
        string columnasDelRegistro;
        string tableName;
        int count = 0;

        while (ss >> tempPalabra) {
            count++;;
            if (count == 3) {
                tableName = tempPalabra;
                continue;
            }
            if(count > 3){
                columnasDelRegistro += tempPalabra;
            }
        }
        columnasDelRegistro += "\n";
        diskController->variableTableToVector(tableName);

        char caracter;
        bool reemplazarActivo = true;
        int indice=0;
        string word="";
        int natributos = diskController->info.size();
        string colsregistro_enTupla = "";

        for(int i=0; i<columnasDelRegistro.length(); i++){
            caracter = columnasDelRegistro[i];
            if (((caracter == ',')||(caracter =='\n')) && reemplazarActivo == true ) {
                if(indice<natributos){ //natributos
                    colsregistro_enTupla += word;
                    colsregistro_enTupla += '#';
                    word = "";
                    indice++;
                    if (indice >= natributos) { //natributos
                        //indice=0;
                        colsregistro_enTupla += '\n';
                    }
                }
            } else if(caracter == '"'){
                reemplazarActivo = (!reemplazarActivo);
            } else{
                if(caracter != '\r')
                    word += caracter;
            }
        }

        colsregistro_enTupla.erase(colsregistro_enTupla.length() - 1);
        cout<<"\n|"<<colsregistro_enTupla<<"|"<<endl;
        int sizeRegistro = 0; 
        stringstream stream(colsregistro_enTupla);
        string palabra;

        char c;
        int entero;
        double decimalGrande;

       //A partir de aqui arreglar
        //Solicito un frame;
        //char * frame = bufferManager->getPageOfBuuferPool(1)->data;
        char * frame = new char[4096];
        int offset = 0;


        while(getline(stream,palabra,'#')){ // palabra 5    20
            if(get<1>(diskController->info[indice])=="int"){
                if(palabra==""){
                    entero = -1;
                } else {
                    entero = stoi(palabra);
                }
                //salida.write(reinterpret_cast<char*>(&entero), sizeof(entero));
                // Copiar los bytes del entero en el frame
                cout<<"llego aqui???????\n";
                
                char* enteroBytes = reinterpret_cast<char*>(&entero);
                for (int i = 0; i < sizeof(entero); i++) {
                    frame[offset++] = enteroBytes[i];
                }
                sizeRegistro += 4;
                
            } else if (get<1>(diskController->info[indice])=="double"){
                if(palabra==""){
                    decimalGrande = -1;
                } else {
                    decimalGrande = stod(palabra);
                }
                //salida.write(reinterpret_cast<char*>(&decimalGrande), sizeof(decimalGrande));
                // Copiar los bytes del decimal grande en el frame
                char* decimalBytes = reinterpret_cast<char*>(&decimalGrande);
                for (int i = 0; i < sizeof(decimalGrande); i++) {
                    frame[offset++] = decimalBytes[i];
                }
                sizeRegistro += sizeof(decimalGrande);
            } else if (get<1>(diskController->info[indice])=="str"){
                string str = palabra;
                sizeRegistro += palabra.length();

                // Verificar si se requiere completar bytes
                // int bytesPorCompletar = get<2>(diskController->info[indice]) - str.size();
                // if (bytesPorCompletar > 0) {
                //     str += string(bytesPorCompletar, ' '); // Agregar bytes
                // }
                // Copiar los bytes de la cadena en el frame
                const char* strBytes = str.c_str();
                for (int i = 0; i < str.size(); i++) {
                    frame[offset++] = strBytes[i];
                }
            }
            indice++;
        }
        delete[] frame;
        diskController->WriteDataRegistroEnBloque(100,sizeRegistro,frame,tableName);
        //diskController->WriteDataRegistroEnBloque(1,sizeRegistro,frame,tableName);
    }

    
    void deleteRegistro(string prompt){

    }


    void createTable(string prompt)
    {
        ofstream file;
        
        string word;
        stringstream stream(prompt);
        getline(stream,word,' ');
        if(word=="f"){ // fijos
            file.open("esquema", ios::app);
            file.seekp(0, ios::beg);
        } else if(word=="v"){ //variables
            file.open("dictionary/variableSchema", fstream::app);
            file.seekp(0, ios::beg);
        }
        getline(stream, word, ' ');
        file << word << '#';
        getline(stream, word, ' ');
        file << word;
        // Escritura
        getline(stream, word, ' ');
        string wor;
        cout << "Atributos:" << endl;
        // v titanic 12 passengerid,int,survived,int,Pclass,int,Name,str,90,Sex,str,6,age,double,SibSp,int,Parch,int,Ticket,str,20,Fare,double,Cabin,str,20,Embarked,str,1
        // Muestra las columnas de las tablas
        stringstream stream1(word);
        cout << word << endl;
        while (getline(stream1, wor, ','))
        {
            file << "#" << wor;
        }
        file<<"\n";
        file.close();
    }


    void mostrarPage(int pageId) { // MostrarBloque
        this->diskController->tableToVector("titanic");     
        int comprobarRegistro; 
        int sizeRegistro = 0;
        for(auto& i : this->diskController->info){
            sizeRegistro += get<2>(i);
        }
        
        //cout<<">>>>>>>>> sizeRegisto = "<<sizeRegistro;

        //char * frame = new char[this->diskController->sizeBloque];
        //cout<<"aqui sizebloque->>>>>> "<<this->bufferManager->getPageOfBuuferPool(pageId)->sizePage<<endl;
        char * frame = bufferManager->getPageOfBuuferPool(pageId)->data;
        //cout<<"llego aqui"<<endl;
        ifstream file;
        file.open("disk/bloque"+to_string(pageId)+".bin",ios::in | ios::binary);
        file.read(frame,this->diskController->sizeBloque);
        
        char marcador;
        marcador = *reinterpret_cast<char *>(frame + sizeRegistro-5);
        int byte = 0;
        if (marcador != '*') {
          byte+=(sizeRegistro)-4;
          cout<<"Freelist -> ";
          fun_int(frame,byte,4);
          cout<<endl;
        }
        

        for (int i=1; i<(this->diskController->sizeBloque / sizeRegistro); i++) {
          char marcador;
          marcador = *reinterpret_cast<char *>(frame + ((i+1)*sizeRegistro)-5);
          //cout<<"aqui marcador ->"<<marcador<<endl;
          if (marcador != '*') {
            for(auto& i : this->diskController->info){
              if (get<1>(i)=="int"){
                  fun_int(frame,byte,mytipos::_INT);
              } else if(get<1>(i)=="double"){
                  fun_double(frame,byte,mytipos::_DOUBLE);
              } else if(get<1>(i)=="str"){
                  fun_char(frame,byte,get<2>(i));
              }
            }
            cout<<"\n";
          }
        }
        file.close();

        // Page* pagina = this->bufferManager->getPageOfBuuferPool(pageId);
        // ifstream file("disk/bloque"+to_string(pageId)+".bin", ios::binary);

        // this->diskController.

        // file.read(pagina->data, this->diskController->sizeBloque);
        // int * datos = reinterpret_cast<int*>(pagina->data);

        // for(auto i=0; i<875; i++){
        //     cout<<datos[i];
        // }

        // file.close();
    }
    void search(string atributo, int objetivo){
        this->diskController->tableToVector("titanic");     
        int aux=0;
        for(auto& i : this->diskController->info){
          if(get<0>(i) != atributo){
            aux += get<2>(i);
          }
          else {
            break;
          }
         }

        //cout<<"aux-> "<<aux;

        int comprobarRegistro; 
        int sizeRegistro = 0;
        for(auto& i : this->diskController->info){
            sizeRegistro += get<2>(i);
        }

        for (int i=1; i<=45; i++) {
          char * frame = bufferManager->getPageOfBuuferPool(i)->data;
          ifstream file;
          file.open("disk/bloque"+to_string(i)+".bin",ios::in | ios::binary);
          file.read(frame,this->diskController->sizeBloque);
          
          char marcador;
          marcador = *reinterpret_cast<char *>(frame + sizeRegistro-5);
          int byte = 0;
          if (marcador != '*') {
            byte+=(sizeRegistro);
          }

          for (int i=1; i<(this->diskController->sizeBloque / sizeRegistro); i++) {
            char marcador;

            marcador = *reinterpret_cast<char *>(frame + ((i+1)*sizeRegistro)-5);
            //cout<<"aqui marcador ->"<<marcador<<endl;
            if (marcador != '*') {
              int pru = get_integer(frame, byte, aux);
              if(pru == objetivo){
                for(auto& i : this->diskController->info){
                  if (get<1>(i)=="int"){
                      fun_int(frame,byte,mytipos::_INT);
                  } else if(get<1>(i)=="double"){
                      fun_double(frame,byte,mytipos::_DOUBLE);
                  } else if(get<1>(i)=="str"){
                      fun_char(frame,byte,get<2>(i));
                  }
                }
                cout<<"\n";
              }
              else {
                byte += sizeRegistro;
              }
              //cout<<"\n";
            }
          }
          file.close();
        }
    }

    void showTable(string nameTable){
        int numRegistros;
        this->diskController->tableToVector(nameTable);
        ifstream numRegistros_dictionary("dictionary/numRegistros.bin", std::ios::binary);
        numRegistros_dictionary.read(reinterpret_cast<char*>(&numRegistros), sizeof(int));
        numRegistros_dictionary.close(); 
        cout<<"\n-> numbloquesenuso = "<<diskController->NumBLoquesEnUso<<endl;
        for (int i = 1; i<=diskController->nTotalBloques; i++) {
            mostrarPage(i); 
        }
    }

    int get_integer(char *frame, int &byte, int sizeBytes)
    {
        int pru;
        pru = *reinterpret_cast<int *>(frame + byte + sizeBytes);
        return pru;
    }
    
    void fun_int(char *frame, int &byte, int sizeBytes)
    {
        int pru;
        pru = *reinterpret_cast<int *>(frame + byte);
        std::cout << pru << "|";
        byte += sizeBytes;
    }
    void fun_float(char *frame, int &byte, int sizeBytes)
    {
        int flo;
        flo = *reinterpret_cast<float *>(frame + byte);
        std::cout << flo << "|";
        byte += sizeBytes;
    }
    void fun_double(char *frame, int &byte, int sizeBytes)
    {
        int dou;
        dou = *reinterpret_cast<double *>(frame + byte);
        std::cout << dou << "|";
        byte += sizeBytes;
    }
    void fun_char(char *frame, int &byte, int sizeBytes)
    {
        char texto;
        for (int i = 0; i < sizeBytes; i++)
        {
            texto = *reinterpret_cast<char *>(frame + byte);
            std::cout << texto;
            byte += 1;
        }
        std::cout << "|";
    }

    int printUbicacionRegistro(int objetivo){
        this->diskController->tableToVector("titanic");     

        int sizeRegistro = 0;
        for(auto& i : this->diskController->info){
            sizeRegistro += get<2>(i);
        }

        for (int i=1; i<=diskController->NumBLoquesEnUso; i++) {
          int bloque = i-1;
          char * frame = bufferManager->getPageOfBuuferPool(i)->data;
          ifstream file;
          file.open("disk/bloque"+to_string(i)+".bin",ios::in | ios::binary);
          file.read(frame,this->diskController->sizeBloque);
          
          char marcador;
          marcador = *reinterpret_cast<char *>(frame + sizeRegistro-5);
          int byte = 0;
          if (marcador != '*') {
            byte+=(sizeRegistro);
          }

          for (int i=1; i<(this->diskController->sizeBloque / sizeRegistro); i++) {
            char marcador;

            marcador = *reinterpret_cast<char *>(frame + ((i+1)*sizeRegistro)-5);
            if (marcador != '*') {
              int pru = get_integer(frame, byte, 0);
              if(pru == objetivo){
                for(auto& i : this->diskController->info){
                  if (get<1>(i)=="int"){
                      fun_int(frame,byte,mytipos::_INT);
                  } else if(get<1>(i)=="double"){
                      fun_double(frame,byte,mytipos::_DOUBLE);
                  } else if(get<1>(i)=="str"){
                      fun_char(frame,byte,get<2>(i));
                  }
                }

                int nSector = ((i*sizeRegistro) % disco->capacidadDelSector == 0) ? ((i*sizeRegistro)/disco->capacidadDelSector) : ((i*sizeRegistro)/disco->capacidadDelSector)+1;
                nSector = nSector+(8*bloque);

                return bloque+1; 
              }
              else {
                byte += sizeRegistro;
              }
            }
          }
          file.close();
        }
        return 0;
    }
};
