#pragma once
#include "Disco.h"
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

    SGDB(Disco *disco, DiskController * diskController)
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
        
    }

    void deleteRegistro(string prompt){

    }

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

    //Retorna si la tabla ha sido creada para almacenar registros de longitud variable o fija
    int get_TipoDeTabla(){
        /*
        1 Recorrer el diccionario que indica <tablaName><tipo> tipo puede ser un caracter c o v
        
        */
        // ifstream esquemaBloques();
    }

    void mostrarPage(int pageId)
    { // MostrarBloque
        this->diskController->tableToVector("titanic");
        int comprobarRegistro;
        int sizeRegistro = 0;
        for (auto &i : this->diskController->info)
        {
            sizeRegistro += get<2>(i);
        }

        // cout<<">>>>>>>>> sizeRegisto = "<<sizeRegistro;

        // char * frame = new char[this->diskController->sizeBloque];
        // cout<<"aqui sizebloque->>>>>> "<<this->bufferManager->getPageOfBuuferPool(pageId)->sizePage<<endl;
        char *frame = bufferManager->getPageOfBuuferPool(pageId)->data;
        // cout<<"llego aqui"<<endl;
        ifstream file;
        file.open("disk/bloque" + to_string(pageId) + ".bin", ios::in | ios::binary);
        file.read(frame, this->diskController->sizeBloque);

        char marcador;
        marcador = *reinterpret_cast<char *>(frame + sizeRegistro - 5);
        int byte = 0;
        if (marcador != '*')
        {
            byte += (sizeRegistro)-4;
            cout << "Freelist -> ";
            fun_int(frame, byte, 4);
            cout << endl;
        }

        for (int i = 1; i < (this->diskController->sizeBloque / sizeRegistro); i++)
        {
            char marcador;
            marcador = *reinterpret_cast<char *>(frame + ((i + 1) * sizeRegistro) - 5);
            // cout<<"aqui marcador ->"<<marcador<<endl;
            if (marcador != '*')
            {
                for (auto &i : this->diskController->info)
                {
                    if (get<1>(i) == "int")
                    {
                        fun_int(frame, byte, mytipos::_INT);
                    }
                    else if (get<1>(i) == "double")
                    {
                        fun_double(frame, byte, mytipos::_DOUBLE);
                    }
                    else if (get<1>(i) == "str")
                    {
                        fun_char(frame, byte, get<2>(i));
                    }
                }
                cout << "\n";
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
    
    void search(string atributo, int objetivo)
    {
        this->diskController->tableToVector("titanic");
        int aux = 0;
        for (auto &i : this->diskController->info)
        {
            if (get<0>(i) != atributo)
            {
                aux += get<2>(i);
            }
            else
            {
                break;
            }
        }

        // cout<<"aux-> "<<aux;

        int comprobarRegistro;
        int sizeRegistro = 0;
        for (auto &i : this->diskController->info)
        {
            sizeRegistro += get<2>(i);
        }

        for (int i = 1; i <= 41; i++)
        {
            char *frame = bufferManager->getPageOfBuuferPool(i)->data;
            ifstream file;
            file.open("disk/bloque" + to_string(i) + ".bin", ios::in | ios::binary);
            file.read(frame, this->diskController->sizeBloque);

            char marcador;
            marcador = *reinterpret_cast<char *>(frame + sizeRegistro - 5);
            int byte = 0;
            if (marcador != '*')
            {
                byte += (sizeRegistro);
            }

            for (int i = 1; i < (this->diskController->sizeBloque / sizeRegistro); i++)
            {
                char marcador;

                marcador = *reinterpret_cast<char *>(frame + ((i + 1) * sizeRegistro) - 5);
                // cout<<"aqui marcador ->"<<marcador<<endl;
                if (marcador != '*')
                {
                    int pru = get_integer(frame, byte, aux);
                    if (pru == objetivo)
                    {
                        for (auto &i : this->diskController->info)
                        {
                            if (get<1>(i) == "int")
                            {
                                fun_int(frame, byte, mytipos::_INT);
                            }
                            else if (get<1>(i) == "double")
                            {
                                fun_double(frame, byte, mytipos::_DOUBLE);
                            }
                            else if (get<1>(i) == "str")
                            {
                                fun_char(frame, byte, get<2>(i));
                            }
                        }
                        cout << "\n";
                    }
                    else
                    {
                        byte += sizeRegistro;
                    }
                    // cout<<"\n";
                }
            }
            file.close();
        }
    }

    void showTable(string nameTable)
    {
        int numRegistros;
        this->diskController->tableToVector("titanic");
        ifstream numRegistros_dictionary("dictionary/numRegistros.bin", std::ios::binary);
        numRegistros_dictionary.read(reinterpret_cast<char *>(&numRegistros), sizeof(int));
        numRegistros_dictionary.close();

        for (int i = 1; i <= 41; i++)
        {
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

    int printUbicacionRegistro(int objetivo)
    {
        this->diskController->tableToVector("titanic");

        int sizeRegistro = 0;
        for (auto &i : this->diskController->info)
        {
            sizeRegistro += get<2>(i);
        }

        for (int i = 1; i <= 41; i++)
        {
            int bloque = i - 1;
            char *frame = bufferManager->getPageOfBuuferPool(i)->data;
            ifstream file;
            file.open("disk/bloque" + to_string(i) + ".bin", ios::in | ios::binary);
            file.read(frame, this->diskController->sizeBloque);

            char marcador;
            marcador = *reinterpret_cast<char *>(frame + sizeRegistro - 5);
            int byte = 0;
            if (marcador != '*')
            {
                byte += (sizeRegistro);
            }

            for (int i = 1; i < (this->diskController->sizeBloque / sizeRegistro); i++)
            {
                char marcador;

                marcador = *reinterpret_cast<char *>(frame + ((i + 1) * sizeRegistro) - 5);
                // cout<<"aqui marcador ->"<<marcador<<endl;
                if (marcador != '*')
                {
                    int pru = get_integer(frame, byte, 0);
                    if (pru == objetivo)
                    {
                        cout << " \nRegistro:\n";
                        for (auto &i : this->diskController->info)
                        {
                            if (get<1>(i) == "int")
                            {
                                fun_int(frame, byte, mytipos::_INT);
                            }
                            else if (get<1>(i) == "double")
                            {
                                fun_double(frame, byte, mytipos::_DOUBLE);
                            }
                            else if (get<1>(i) == "str")
                            {
                                fun_char(frame, byte, get<2>(i));
                            }
                        }
                        cout << "\n";

                        int nSector = ((i * sizeRegistro) % disco->capacidadDelSector == 0) ? ((i * sizeRegistro) / disco->capacidadDelSector) : ((i * sizeRegistro) / disco->capacidadDelSector) + 1;
                        nSector = nSector + (8 * bloque);

                        cout << "\n"
                             << lineas::drawLinea(75) << " | INFO SECTOR | " << lineas::drawLinea(75) << "\n";
                        this->disco->sectores[nSector - 1].showInfoSector(); // Imprime info: en que plato, superficie, pista esta
                        return bloque + 1;
                    }
                    else
                    {
                        byte += sizeRegistro;
                    }
                    // cout<<"\n";
                }
            }
            file.close();
        }
        return 0;
    }
};