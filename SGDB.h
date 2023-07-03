#pragma once
#include "HeadersHDD/Disco.h"
#include "DiskController.h"
#include "BufferManager.h"
#include "tipos.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

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

    void insertRegistroLongitudFija(string prompt)
    {
        stringstream ss(prompt);
        string palabra;
        string columnasDelRegistro;
        string tableName;
        int count = 0;

        while (ss >> palabra)
        {
            count++;
            ;
            if (count == 3)
            {
                tableName = palabra;
                continue;
            }
            if (count > 3)
            {
                columnasDelRegistro += palabra;
            }
        }

        // Solicito un frame;
        char *frame = bufferManager->getPageOfBuuferPool(1)->data;

        // titanic 12 passengerid,int,survived,int,Pclass,int,Name,str,90,Sex,str,6,age,double,SibSp,int,Parch,int,Ticket,str,20,Fare,double,Cabin,str,20,Embarked,str,1
        // 889,0,3,"Johnston, Miss. Catherine Helen Carrie",female,,1,2,W./C. 6607,23.45,,S

        // 889#0#3#Johnston, Miss. Catherine Helen Carrie#female##1#2#W./C. 6607#23.45##S#
    }

    vector<int> extraerNullBitmap(string registroTupla)
    {
        stringstream stream(registroTupla);
        string palabra;
        vector<int> nullBitMap;
        while (getline(stream, palabra, '#'))
        {
            if (palabra.empty())
            {
                nullBitMap.emplace_back(1);
            }
            else
            {
                nullBitMap.emplace_back(0);
            }
        }
        return nullBitMap;
    }

    vector<char> generate_NullBitMap_BIT(const std::vector<int> &nullbitmap)
    {
        int numBytes_ASeparar = std::ceil(static_cast<double>(nullbitmap.size()) / 8);
        std::vector<char> bytes;

        int contadorBits = 0;
        for (int k = 0; k < numBytes_ASeparar; k++)
        {
            unsigned char byte = 0;
            for (int i = 7; i >= 0; i--)
            {
                byte |= (nullbitmap[contadorBits]) << i;
                contadorBits++;
                if (contadorBits == nullbitmap.size())
                {
                    break;
                }
            }
            bytes.push_back(byte);
        }

        return bytes;
    }

    vector<char> convertTuplaA_RegistroVariable(string registroTupla, string tablaName)
    {
        vector<char> registroVariable; // sera retornado al final
        vector<tuple<int, string>> colaAux;
        int sizeRegistro = 0;
        stringstream stream(registroTupla);
        string palabra;

        char c;
        int entero;
        double decimalGrande;

        vector<int> nullBitMap_INT = extraerNullBitmap(registroTupla);          // Si funciona
        vector<char> nullBitMap_BITS = generate_NullBitMap_BIT(nullBitMap_INT); // convierte nullBitMap_INT en nullBitMap_BITS
        /* nullBitMap_BITS esta listo para insertarse en REGISTRO VARIABLE */
        for (char &c : nullBitMap_BITS)
        {
            char byteOfNullBitMap = c;
            registroVariable.emplace_back(byteOfNullBitMap);
        }
        /* comentar desde aqui  */
        for (auto &it : nullBitMap_INT)
        {
            cout << it << " ";
        }
        cout << "\n";
        // Esquema
        for (const auto &tupla : diskController->info)
        {
            // Obtener los elementos de la tupla
            std::string elemento1 = get<0>(tupla);
            std::string elemento2 = get<1>(tupla);
            int elemento3 = get<2>(tupla);

            // Imprimir los elementos de la tupla
            std::cout << elemento1 << " ";
            std::cout << elemento2 << " ";
            std::cout << elemento3 << " ";
            std::cout << std::endl;
        }
        /* hasta aqui */

        // 892#0#3#Johnston,Miss.CatherineHelenCarrie#female##1#2#W./C.6607#23.45##S#
        int contadorDeBytes = nullBitMap_BITS.size(); // = 2
        int itr_esquemaTabla = 0;

        cout << "\n";                         // comentar esta linea
        while (getline(stream, palabra, '#')) // palabra contiene el atributo 1 de 12 (titanic)
        {
            cout << "===> " << palabra << "\n"; // comentar esta linea
            if (nullBitMap_INT[itr_esquemaTabla] == 1)
            { // si nullBitmap es 1 quiere decir que el atributo es null
                itr_esquemaTabla++;
                continue;
            }
            if (IsAtributoVariable(get<1>(diskController->info[itr_esquemaTabla]))) // si el atributo es variable poner en cola
            {
                colaAux.emplace_back(make_tuple(contadorDeBytes, palabra)); // posicion donde empiezan los 4 bytes que guarda el par(offset, lenght)
                for (int i = 0; i < 4; i++)
                { // Se separa 4 bytes para el par(offset, lenght)
                    registroVariable.emplace_back('\0');
                }
                contadorDeBytes += (4); // par (offset 2 bytes, longitud 2 bytes)
            }
            else if (get<1>(diskController->info[itr_esquemaTabla]) == "int")
            {
                entero = stoi(palabra);
                agregarIntARegistroVariable(entero, registroVariable);
                contadorDeBytes += (sizeof(int));
            }
            else if (get<1>(diskController->info[itr_esquemaTabla]) == "double")
            {
                decimalGrande = stod(palabra);
                agregarDoubleARegistroVariable(decimalGrande, registroVariable);
                contadorDeBytes += (sizeof(double));
            }
            itr_esquemaTabla++;
        }

        cout << "registroVariable (parte fija): ";
        for (const char byte : registroVariable)
        {
            cout << static_cast<int>(byte) << " ";
        }
        cout << "\nsize RegistroVariable (parteFija): " << registroVariable.size() << endl;

        for (const auto &tupla : colaAux) // Inserta data de los atributos variables
        {
            int pos = get<0>(tupla);
            string data = get<1>(tupla);

            cout << "string a insertar: |" << data << "| - size:" << data.length() << endl;
            for (auto &_char : data)
            {
                registroVariable.emplace_back(_char); // Inserta data al final
            }

            short int _offset = itr_esquemaTabla; // Posicion donde comienza data del atributo variable
            short int _length = data.length();    // longitud de la data del atributo variable

            // Convertir _offset y _length a bytes
            unsigned char byteOffset[2];
            unsigned char byteLength[2];
            byteOffset[0] = static_cast<unsigned char>(_offset);
            byteOffset[1] = static_cast<unsigned char>(_offset >> 8);
            byteLength[0] = static_cast<unsigned char>(_length);
            byteLength[1] = static_cast<unsigned char>(_length >> 8);

            // Reemplazar los bytes en registroVariable a partir de la posición 11
            std::copy(byteOffset, byteOffset + 2, registroVariable.begin() + pos);// especifica directamente las posiciones
            std::copy(byteLength, byteLength + 2, registroVariable.begin() + pos+2);
            itr_esquemaTabla += _length;
        }

        cout << "colaAux size => " << colaAux.size() << endl;
        return registroVariable;
    }

    bool IsAtributoVariable(string tipo)
    {
        if (tipo == "str")
            return true;
        return false;
    }

    void agregarIntARegistroVariable(int dato, vector<char> &registroVariable)
    {
        for (int i = 0; i < 4; i++)
        {
            char byte = (dato >> (i * 8)) & 0xFF;
            registroVariable.emplace_back(byte);
        }
    }

    void agregarDoubleARegistroVariable(double dato, vector<char> &registroVariable)
    {
        char *bytes = reinterpret_cast<char *>(&dato); // Convertir el double en 8 bytes
        for (int i = 0; i < sizeof(dato); i++)
        { // Agregar los bytes al final del vector registroVariable
            registroVariable.emplace_back(bytes[i]);
        }
    }

    void insertRegistroLongitudVariable(string prompt)
    {
        stringstream ss(prompt);
        string tempPalabra;
        string columnasDelRegistro;
        string tableName;
        int count = 0;

        while (ss >> tempPalabra)
        {
            count++;
            ;
            if (count == 3)
            {
                tableName = tempPalabra;
                continue;
            }
            if (count > 3)
            {
                columnasDelRegistro += tempPalabra;
            }
        }
        columnasDelRegistro += "\n";
        diskController->variableTableToVector(tableName);

        char caracter;
        bool reemplazarActivo = true;
        int indice = 0;
        string word = "";
        int natributos = diskController->info.size();
        string colsregistro_enTupla = "";

        for (int i = 0; i < columnasDelRegistro.length(); i++)
        {
            caracter = columnasDelRegistro[i];
            if (((caracter == ',') || (caracter == '\n')) && reemplazarActivo == true)
            {
                if (indice < natributos)
                { // natributos
                    colsregistro_enTupla += word;
                    colsregistro_enTupla += '#';
                    word = "";
                    indice++;
                    if (indice >= natributos)
                    { // natributos
                        // indice=0;
                        colsregistro_enTupla += '\n';
                    }
                }
            }
            else if (caracter == '"')
            {
                reemplazarActivo = (!reemplazarActivo);
            }
            else
            {
                if (caracter != '\r')
                    word += caracter;
            }
        }

        colsregistro_enTupla.erase(colsregistro_enTupla.length() - 1); // quita el salto de linea
        cout << "\n|" << colsregistro_enTupla << "|" << endl;

        vector<char> registroVariable = convertTuplaA_RegistroVariable(colsregistro_enTupla, tableName);
        int sizeRegistro = registroVariable.size();

        cout << "registroVariable size => " << sizeRegistro << endl;

        for (const char byte : registroVariable)
        {
            std::cout << static_cast<int>(byte) << " ";
        }
        return;
        // Solicito un frame;
        char * frame;
        // char * frame = bufferManager->getPageOfBuuferPool(1)->data;
        // char *frame = new char[4096];
        //delete[] frame;
        diskController->WriteDataRegistroEnBloque(100, sizeRegistro, frame, tableName);
        // diskController->WriteDataRegistroEnBloque(1,sizeRegistro,frame,tableName);
    }

    void deleteRegistro(string prompt)
    {
    }

    void createTable(string prompt)
    {
        ofstream file;

        string word;
        stringstream stream(prompt);
        getline(stream, word, ' ');
        if (word == "f")
        { // fijos
            file.open("esquema", ios::app);
            file.seekp(0, ios::beg);
        }
        else if (word == "v")
        { // variables
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
        file << "\n";
        file.close();
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

        for (int i = 1; i <= 45; i++)
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
        this->diskController->tableToVector(nameTable);
        ifstream numRegistros_dictionary("dictionary/numRegistros.bin", std::ios::binary);
        numRegistros_dictionary.read(reinterpret_cast<char *>(&numRegistros), sizeof(int));
        numRegistros_dictionary.close();
        cout << "\n-> numbloquesenuso = " << diskController->NumBLoquesEnUso << endl;
        for (int i = 1; i <= diskController->nTotalBloques; i++)
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

        for (int i = 1; i <= diskController->NumBLoquesEnUso; i++)
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
                if (marcador != '*')
                {
                    int pru = get_integer(frame, byte, 0);
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

                        int nSector = ((i * sizeRegistro) % disco->capacidadDelSector == 0) ? ((i * sizeRegistro) / disco->capacidadDelSector) : ((i * sizeRegistro) / disco->capacidadDelSector) + 1;
                        nSector = nSector + (8 * bloque);

                        return bloque + 1;
                    }
                    else
                    {
                        byte += sizeRegistro;
                    }
                }
            }
            file.close();
        }
        return 0;
    }
};
