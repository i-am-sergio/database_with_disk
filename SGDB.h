#pragma once
#include "Disco.h"
#include "DiskController.h"
#include "BufferManager.h"
#include "tipos.cpp"
#include <iostream>
#include <fstream>
#include <sstream>

const int MAX_SIZE_PROMPT = 250 + 1;
const int MAX_SIZE_TABLENAME = 64 + 1;

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

    void createTable(string prompt)
    {
        ofstream file;
        file.open("esquema", fstream::app);
        file.seekp(0, ios::beg);
        string word;
        stringstream stream(prompt);
        getline(stream, word, ' ');
        file << word << '#';
        getline(stream, word, ' ');
        file << word;
        // Escritura
        getline(stream, word, ' ');
        string wor;
        cout << "Atributos:" << endl;
        // titanic 12 passengerid,int,survived,int,Pclass,int,Name,str,90,Sex,str,6,age,double,SibSp,int,Parch,int,Ticket,str,20,Fare,double,Cabin,str,20,Embarked,str,1
        // Muestra las columnas de las tablas
        stringstream stream1(word);
        cout << word << endl;
        while (getline(stream1, wor, ','))
        {
            file << "#" << wor;
        }

        file.close();
    }

    void mostrarPage(int pageId) { // MostrarBloque
        this->diskController->tableToVector("titanic");     
        
        int sizeRegistro = 0;
        for(auto& i : this->diskController->info){
            sizeRegistro += get<2>(i);
        }
        
        cout<<">>>>>>>>> sizeRegisto = "<<sizeRegistro;

        char * frame = new char[this->diskController->sizeBloque];

        //char * frame = this->bufferManager->getPageOfBuuferPool(1)->data;

        ifstream file;
        file.open("disk/bloque"+to_string(pageId)+".bin",ios::in | ios::binary);
        file.read(frame,this->diskController->sizeBloque);
        int byte = 0;
        byte+=(sizeRegistro)-4;
        cout<<"Freelist -> ";
        fun_int(frame,byte,4);
        cout<<endl;
        for (int i=1; i<(this->diskController->sizeBloque / sizeRegistro); i++) {
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
};