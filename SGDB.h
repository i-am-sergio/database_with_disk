#pragma once
#include "Disco.h"
#include "DiskController.h"
#include <fstream>
#include <sstream>

const int MAX_SIZE_PROMPT = 250 + 1;
const int MAX_SIZE_TABLENAME = 64 + 1;

class SGDB{
public:
    Disco * disco;
    DiskController * diskController;

    SGDB(Disco * disco, DiskController * diskController){
        this->disco = disco;
        this->diskController = diskController;
    }
    ~SGDB(){}
/*
    void createTable(char input[]){
        char * ptr = &input[13];
        char numAtributos[100];
        int i2 = 0;
        while(true){
            if(ptr[0]== ' ') break;
            numAtributos[i2] = ptr[0];
            ptr += sizeof(char);
            i2++;
        }
        
        numAtributos[i2] = '\0';
        int numAtributos_int = std::stoi(numAtributos);

        //columns += sizeof(char)*i2;


        char nombreTabla[MAX_SIZE_TABLENAME];
        int i = 0;
        while(true){
            if(ptr[0]== ' ') break;
            nombreTabla[i] = ptr[0];
            ptr += sizeof(char);
            i++;
        }
        extraerPalabra(input, 13+i2-1, 13+i+i2-2,nombreTabla);// extrae una palabra de un char a otro char
        cout<<"\n|"<<nombreTabla<<"|\n";

        char * columns = &input[13+i+2+i2]; // apunta al primer caracter del conjunto de columnas
        ofstream nuevaTabla(nombreTabla);
        nuevaTabla.close();
        //CREATE TABLE students (nombre str 100,id int,depart str 50)
        ofstream esquema("esquema", ios::app);
        esquema<<nombreTabla<<'#';
        esquema<<numAtributos_int<<'#';


        while(columns[0] != '\0' && columns[0] != ')'){                     
            if(columns[0]==' '){
                // if(columns[1]=='i' && columns[2]=='n' && columns[3]=='t'){
                //     esquema<<"#int#";
                //     esquema<<"4";
                //     columns += (sizeof(char)*4); continue;
                // } else if(columns[1]=='d' && columns[2]=='o' && columns[3]=='u' && columns[4]=='b' && columns[5]=='l' && columns[6]=='e'){
                //     esquema<<"#double#";
                //     esquema<<"8";
                //     columns += (sizeof(char)*7); continue;
                // } else if(columns[1]=='b' && columns[2]=='o' && columns[3]=='o' && columns[4]=='l'){
                //     esquema<<"#bool#";
                //     esquema<<"1";
                //     columns += (sizeof(char)*5); continue;
                // }
            }
            if(columns[0]==' '){
                esquema<<'#';//' '
                columns += sizeof(char);
                continue;
            } else if (columns[0]==',') {
                esquema<<'#';
                columns += sizeof(char);
                continue;
            }
            esquema<<columns[0];
            columns += sizeof(char);
        }
        esquema<<"#\n"; // importante agregar # al final
        esquema.close();

        cout<<"Query Ok! new table created\n";
    }
*/

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

    void extraerPalabra(const char* input, int inicio, int fin, char* palabra) {
        int longitud = fin-inicio+1;
        for (int i=inicio, j=0; i<= fin; i++, j++) {
            palabra[j] = input[i];
        }
        palabra[longitud] = '\0'; // Agregar carácter nulo de terminación
    }
};  