#pragma once
#include "Disco.h"
#include "DiskController.h"
#include "BufferManager.h"
#include <iostream> 
#include <fstream>
#include <sstream>

const int MAX_SIZE_PROMPT = 250 + 1;
const int MAX_SIZE_TABLENAME = 64 + 1;

class SGDB{
public:
    Disco * disco;
    DiskController * diskController;
    BufferManager * bufferManager;

    SGDB(Disco * disco, DiskController * diskController){
        this->disco = disco;
        this->diskController = diskController;
        this->bufferManager = new BufferManager(this->diskController);
    }
    ~SGDB(){}

    void createTable(string prompt){
        ofstream file;
        file.open("esquema", fstream::app);
        file.seekp(0,ios::beg);
        string word;
        stringstream stream(prompt);
        getline(stream,word,' ');
        file<<word<<'#';
        getline(stream,word,' ');
        file<<word;
        //Escritura
        getline(stream,word,' ');
        string wor;
        cout<<"Atributos:"<<endl;
        //titanic 12 passengerid,int,survived,int,Pclass,int,Name,str,90,Sex,str,6,age,double,SibSp,int,Parch,int,Ticket,str,20,Fare,double,Cabin,str,20,Embarked,str,1
        //Muestra las columnas de las tablas
        stringstream stream1(word);
        cout<<word<<endl;
        while (getline(stream1,wor,',')) {
            file<<"#"<<wor;
        }

        file.close();
    }

    void mostrarPage(int pageId){ // MostrarBloque
        Page* pagina = this->bufferManager->getPageOfBuuferPool(pageId);
        ifstream file("disk/bloque"+to_string(pageId)+".bin", ios::binary);
        int * ptrInt;
        int entero;
        double * ptrDouble;
        char * ptrChar;

        file.read(pagina->data, this->diskController->sizeBloque);
        int * datos = reinterpret_cast<int*>(pagina->data);

        for(auto i=0; i<875; i++){
            cout<<datos[i];
        }

        file.close();
        //this->bufferManager->showPageTable();
    }
};  