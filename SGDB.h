#pragma once
#include "HeadersHDD/Disco.h"
#include "DiskController.h"
#include "BufferManager.h"
#include "tipos.cpp"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

class SGDB
{
public:
  Disco *disco;
  DiskController *diskController;
  BufferManager *bufferManager;
  int bloqueInicial, bloqueFinal;

  SGDB(Disco *disco, DiskController *diskController)
  {
    this->disco = disco;
    this->diskController = diskController;
    this->bufferManager = new BufferManager(this->diskController);
  }
  ~SGDB() {}

  void createTable_Variable(string prompt)
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
    // titanic 1 passengerid,int,survived,int,Pclass,int,Name,str,90,Sex,str,6,age,double,SibSp,int,Parch,int,Ticket,str,20,Fare,double,Cabin,str,20,Embarked,str,1
    // Muestra las columnas de las tablas
    stringstream stream1(word);
    cout << word << endl;
    while (getline(stream1, wor, ','))
    {
      file << "#" << wor;
    }
    file << "\n";

    file.close();

    // Seteamos el nombre de la tabla y si es fijo o variable
    ifstream data;
    data.open("esquema", ios::in);
    string linea;
    while (getline(data, linea))
    {
      stringstream stream(linea);
      string word;
      getline(stream, word, '#');
      if (word == diskController->nameTable)
      {
        break;
      }
    }

    // Lleva a la memoria dinamica la ubicacion de los bytes por cada atributo
    // cout<<linea<<endl;
    stringstream stream2(linea);
    string w1;
    int inicializador = 0;
    getline(stream2, w1, '#');
    fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | ios::binary);
    // Aumenta el numero de tablas
    int nTablas;
    dictionary.seekg(8);
    dictionary.read(reinterpret_cast<char *>(&nTablas), sizeof(int));
    dictionary.seekp(8);
    cout << "tablas-> " << nTablas << endl;
    nTablas++;
    dictionary.write(reinterpret_cast<char *>(&nTablas), sizeof(int));
    cout << "tablas-> " << nTablas << endl;
    dictionary.seekp(0, ios::end);
    w1.resize(100, ' ');
    dictionary.write(w1.c_str(), w1.length());
    dictionary.write(reinterpret_cast<char *>(&inicializador), sizeof(int));
    dictionary.write(reinterpret_cast<char *>(&inicializador), sizeof(int));
    dictionary.write(reinterpret_cast<char *>(&inicializador), sizeof(int));
    getline(stream2, w1, '#');
    inicializador = stoi(w1);
    dictionary.write(reinterpret_cast<char *>(&inicializador), sizeof(int));
    dictionary.close();

    data.close();
  }

  void mostrarPage(int pageId)
  { // MostrarBloque
    this->diskController->tableToVector();
    // this->diskController->tableToVector("titanic");
    int comprobarRegistro;
    int sizeRegistro = 0;
    for (auto &i : this->diskController->info)
    {
      sizeRegistro += get<2>(i);
    }

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
      // cout<<"marcador -> "<<marcador<<endl;
      // cout<<"aqui marcador ->"<<marcador<<endl;
      if (marcador != '*')
      {
        if (marcador == '-')
        {
          cout << "-> ";
          int pru;
          pru = *reinterpret_cast<int *>(frame + ((i + 1) * sizeRegistro) - 4);
          cout << pru << endl;
          byte += sizeRegistro;
        }
        else
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
    }
    file.close();
  }
  void search(string atributo, int objetivo)
  {
    // this->diskController->tableToVector("titanic");
    this->diskController->tableToVector();
    int posicion = this->diskController->buscarTablaenDictionary(this->diskController->nameTable);
    posicion += 104;
    fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
    dictionary.seekg(posicion);
    dictionary.read(reinterpret_cast<char *>(&bloqueInicial), sizeof(int));
    dictionary.read(reinterpret_cast<char *>(&bloqueFinal), sizeof(int));
    dictionary.close();

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

    for (int i = bloqueInicial; i <= bloqueFinal; i++)
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

  void showTable()
  { // cambios
    int numRegistros;
    this->diskController->tableToVector();
    int posicion = this->diskController->buscarTablaenDictionary(this->diskController->nameTable);
    posicion += 104;
    fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
    dictionary.seekg(posicion);
    dictionary.read(reinterpret_cast<char *>(&bloqueInicial), sizeof(int));
    dictionary.read(reinterpret_cast<char *>(&bloqueFinal), sizeof(int));
    dictionary.close();

    for (int i = bloqueInicial; i <= bloqueFinal; i++)
    {
      mostrarPage(i);
    }
  }
  int get_integer(char *frame, int byte, int sizeBytes)
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
    this->diskController->tableToVector();
    int posicion = this->diskController->buscarTablaenDictionary(this->diskController->nameTable);
    posicion += 104;
    fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
    dictionary.seekg(posicion);
    dictionary.read(reinterpret_cast<char *>(&bloqueInicial), sizeof(int));
    dictionary.read(reinterpret_cast<char *>(&bloqueFinal), sizeof(int));
    dictionary.close();

    int sizeRegistro = 0;
    for (auto &i : this->diskController->info)
    {
      sizeRegistro += get<2>(i);
    }

    for (int i = bloqueInicial; i <= bloqueFinal; i++)
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

  void deleteRegistro(int objetivo)
  {
    this->diskController->tableToVector();

    int posicion = this->diskController->buscarTablaenDictionary(this->diskController->nameTable);
    posicion += 104;
    fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
    dictionary.seekg(posicion);
    dictionary.read(reinterpret_cast<char *>(&bloqueInicial), sizeof(int));
    dictionary.read(reinterpret_cast<char *>(&bloqueFinal), sizeof(int));
    dictionary.close();

    int sizeRegistro = 0;
    for (auto &i : this->diskController->info)
    {
      sizeRegistro += get<2>(i);
    }

    for (int i = bloqueInicial; i <= bloqueFinal; i++)
    {
      bool SeEncontro = false;
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
      else
      {
        return;
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
            SeEncontro = true;
            // cout<<"lo encontro"<<endl;
            for (int i = 0; i < sizeRegistro - 4; i++)
            {
              frame[byte + 1] = '-';
              byte++;
            }
            byte += 4;
            break;
          }
          else
          {
            byte += sizeRegistro;
          }
        }
      }
      if (SeEncontro)
      {
        int sustiByte = sizeRegistro - 4;
        int aux = 0;
        while (true)
        {
          char c = *reinterpret_cast<char *>(frame + aux + sustiByte - 1);
          // cout<<"c-> "<<c<<endl;
          if ((c != '*') && (c == '-'))
          {
            int free = get_integer(frame, sustiByte + aux, 0);
            if (free == 0)
            {
              // cout<<"ingresa = 0"<<endl;
              // cout<<"reemplaza "<<sustiByte+aux<<"-> "<<byte-sizeRegistro;
              *reinterpret_cast<int *>(&frame[sustiByte + aux]) = byte - sizeRegistro;
              file.close();

              *reinterpret_cast<int *>(&frame[byte - 4]) = 0;
              bufferManager->PinFrame(i);
              ofstream save;
              save.open("disk/bloque" + to_string(i) + ".bin", ios::out | ios::binary);
              save.write(frame, diskController->sizeBloque);
              save.close();
              return;
            }
            else
            {
              aux = free;
            }
          }
        }
      }
      file.close();
    }
  }

  void insertarRegistro()
  {
    this->diskController->tableToVector();
    int posicion = this->diskController->buscarTablaenDictionary(this->diskController->nameTable);
    posicion += 104;
    fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
    dictionary.seekg(posicion);
    dictionary.read(reinterpret_cast<char *>(&bloqueInicial), sizeof(int));
    dictionary.read(reinterpret_cast<char *>(&bloqueFinal), sizeof(int));
    dictionary.close();
    // this->diskController->tableToVector("titanic");
    int sizeRegistro = 0;
    for (auto &i : this->diskController->info)
    {
      sizeRegistro += get<2>(i);
    }

    // cout<<1<<endl;
    int i;
    for (i = bloqueInicial; i <= bloqueFinal; i++)
    {
      char *frame = bufferManager->getPageOfBuuferPool(i)->data;
      ifstream file;
      file.open("disk/bloque" + to_string(i) + ".bin", ios::in | ios::binary);
      file.read(frame, this->diskController->sizeBloque);

      char marcador;
      int ptr;
      marcador = *reinterpret_cast<int *>(frame + sizeRegistro - 5);
      if (marcador == '*')
      {
        break;
      }

      ptr = *reinterpret_cast<int *>(frame + sizeRegistro - 4);
      // cout<<"ptr-> "<<ptr<<endl;
      if (ptr == 0)
      {

        // cout<<2<<endl;
        continue;
      }
      else
      {
        int entero;
        double doble;
        string cad;
        int prePtr = ptr;
        int aux = 0;
        int sustiByte = sizeRegistro - 4;

        // cout<<2<<endl;
        while (true)
        {
          ptr = get_integer(frame, sustiByte + ptr, 0);
          // cout<<"ptr-> "<<ptr<<endl;
          if (ptr == 0)
          {

            // cout<<"preptr-> "<<prePtr<<endl;
            *reinterpret_cast<int *>(&frame[aux + sustiByte]) = 0;

            cout << " Ingrese los datos: \n";
            for (auto &i : this->diskController->info)
            {
              cout << "Ingrese " << get<0>(i) << "-> ";
              if (get<1>(i) == "int")
              {
                getline(cin, cad);
                if (cad == "")
                {
                  entero = -1;
                }
                else
                {
                  entero = stoi(cad);
                }
                *reinterpret_cast<int *>(&frame[prePtr]) = entero;
                prePtr += get<2>(i);
              }
              else if (get<1>(i) == "double")
              {
                getline(cin, cad);
                if (cad == "")
                {
                  doble = -1;
                }
                else
                {
                  doble = stod(cad);
                }
                *reinterpret_cast<double *>(&frame[prePtr]) = doble;
                prePtr += get<2>(i);
              }
              else if (get<1>(i) == "str")
              {
                getline(cin, cad);
                cad.resize(get<2>(i), ' ');
                int tam = get<2>(i);
                for (int i = 0; i < tam; i++)
                {
                  frame[prePtr] = cad[i];
                  prePtr++;
                }
              }
            }
            bufferManager->PinFrame(i);
            ofstream save;
            save.open("disk/bloque" + to_string(i) + ".bin", ios::out | ios::binary);
            save.write(frame, diskController->sizeBloque);
            save.close();
            return;
          }
          else
          {
            aux = prePtr;
            prePtr = ptr;
          }
        }
      }
    }
    ifstream file;
    file.open("disk/bloque" + to_string(i - 1) + ".bin", ios::in | ios::binary);
    char *frame = bufferManager->getPageOfBuuferPool(i)->data;
    file.read(frame, this->diskController->sizeBloque);

    char marcador;
    int aux = 0;
    string cad;
    int entero;
    double doble;
    int ptr;
    while (true)
    {
      marcador = *reinterpret_cast<int *>(frame + aux);
      if (marcador == '*')
      {
        break;
      }
      aux += sizeRegistro;
    }
    cout << " Ingrese los datos: \n";
    for (auto &i : this->diskController->info)
    {
      cout << "Ingrese " << get<0>(i) << "-> ";
      if (get<1>(i) == "int")
      {
        getline(cin, cad);
        if (cad == "")
        {
          entero = -1;
        }
        else
        {
          entero = stoi(cad);
        }
        *reinterpret_cast<int *>(&frame[aux]) = entero;
        aux += get<2>(i);
      }
      else if (get<1>(i) == "double")
      {
        getline(cin, cad);
        if (cad == "")
        {
          doble = -1;
        }
        else
        {
          doble = stod(cad);
        }
        *reinterpret_cast<double *>(&frame[aux]) = doble;
        aux += get<2>(i);
      }
      else if (get<1>(i) == "str")
      {
        getline(cin, cad);
        cad.resize(get<2>(i), ' ');
        int tam = get<2>(i);
        for (int i = 0; i < tam; i++)
        {
          frame[aux] = cad[i];
          aux++;
        }
      }
    }

    bufferManager->PinFrame(i - 1);
    ofstream save;
    save.open("disk/bloque" + to_string(i - 1) + ".bin", ios::out | ios::binary);
    save.write(frame, diskController->sizeBloque);
    save.close();
  }

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

  void verRegistro_Test(vector<char> &registroVariable)
  {

    char c;
    int entero;
    double _double;
    short int offset;
    short int len;
    int index = 0;
    int itr_esquemaTabla = 0;
    for (int i = 0; i < registroVariable.size(); i++)
    {
      // cout << "\t\t==> " << palabra << "\n"; // comentar esta linea
      // if (nullBitMap_INT[itr_esquemaTabla] == 1)
      // { // si nullBitmap es 1 quiere decir que el atributo es null
      //   itr_esquemaTabla++;
      //   continue;
      // }
      // if (IsAtributoVariable(get<1>(diskController->info[itr_esquemaTabla]))) // si el atributo es variable poner en cola
      // {
      //   colaAux.emplace_back(make_tuple(contadorDeBytes, palabra)); // posicion donde empiezan los 4 bytes que guarda el par(offset, lenght)
      //   for (int i = 0; i < 4; i++)
      //   { // Se separa 4 bytes para el par(offset, lenght)
      //     registroVariable.emplace_back('\0');
      //   }
      //   contadorDeBytes += (4); // par (offset 2 bytes, longitud 2 bytes)
      // }
      // else if (get<1>(diskController->info[itr_esquemaTabla]) == "int")
      // {
      //   entero = stoi(palabra);
      //   agregarIntARegistroVariable(entero, registroVariable);
      //   contadorDeBytes += (sizeof(int));
      // }
      // else if (get<1>(diskController->info[itr_esquemaTabla]) == "double")
      // {
      //   decimalGrande = stod(palabra);
      //   agregarDoubleARegistroVariable(decimalGrande, registroVariable);
      //   contadorDeBytes += (sizeof(double));
      // }
      itr_esquemaTabla++;
    }
    // Extraer los primeros 4 bytes y convertirlos a un int
    std::copy(registroVariable.begin() + 2, registroVariable.begin() + 6, reinterpret_cast<char *>(&entero));
    std::cout << "Primer entero: " << entero << std::endl;

    // Extraer los siguientes 4 bytes y convertirlos a un int
    std::copy(registroVariable.begin() + 6, registroVariable.begin() + 10, reinterpret_cast<char *>(&entero));
    std::cout << "Segundo entero: " << entero << std::endl;

    // Extraer los siguientes 4 bytes y convertirlos a un int
    std::copy(registroVariable.begin() + 10, registroVariable.begin() + 14, reinterpret_cast<char *>(&entero));
    std::cout << "Tercer entero: " << entero << std::endl;

    // Extraer los siguientes 2 bytes y convertirlos a un short
    std::copy(registroVariable.begin() + 14, registroVariable.begin() + 16, reinterpret_cast<char *>(&offset));
    std::cout << "offset primer string: " << offset << std::endl;

    // Extraer los siguientes 2 bytes y convertirlos a un short
    std::copy(registroVariable.begin() + 16, registroVariable.begin() + 18, reinterpret_cast<char *>(&len));
    std::cout << "len primer string: " << len << std::endl;

    std::copy(registroVariable.begin() + 22, registroVariable.begin() + 30, reinterpret_cast<char *>(&_double));
    std::cout << "double: " << _double << std::endl;

    // Extraer los últimos 3 bytes caracter por caracter
    std::vector<char> cadena(registroVariable.begin() + 54, registroVariable.begin() + 70);
    for (const char &c : cadena)
    {
      cout << c << endl;
    }
    // while( index < registroVariable.size()){

    // }
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
    cout << "NULLBITMAP => ";
    for (auto &it : nullBitMap_INT)
    {
      cout << it << " ";
    }
    cout << "\n";
    // Esquema
    /*for (const auto &tupla : diskController->info)
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
    }*/
    /* hasta aqui */

    // 892#0#3#Johnston,Miss.CatherineHelenCarrie#female##1#2#W./C.6607#23.45##S#
    int contadorDeBytes = nullBitMap_BITS.size(); // = 2
    int itr_esquemaTabla = 0;

    cout << "\n";                         // comentar esta linea
    while (getline(stream, palabra, '#')) // palabra contiene el atributo 1 de 12 (titanic)
    {
      cout << "\t\t==> " << palabra << "\n"; // comentar esta linea
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
    cout << "\nRegistroVariable (Size-parteFija): " << registroVariable.size() << endl;

    int auxx = 0;
    for (const auto &tupla : colaAux) // Inserta data de los atributos variables
    {
      int pos = get<0>(tupla);
      string data = get<1>(tupla);

      cout << "atributo variable a insertar (str): |" << data << "| - size:" << data.length() << endl;
      for (auto &_char : data)
      {
        registroVariable.emplace_back(_char); // Inserta data al final
      }

      short int _offset = contadorDeBytes; // Posicion donde comienza data del atributo variable
      short int _length = data.length();   // longitud de la data del atributo variable
      // auxx++;
      // if (auxx == 1)
      // {
      //     cout << "offset------------------------------->" << _offset << endl;
      //     cout << "length------------------------------->" << _length << endl;
      // }
      // Convertir _offset y _length a bytes
      unsigned char byteOffset[2];
      unsigned char byteLength[2];
      byteOffset[0] = static_cast<unsigned char>(_offset);
      byteOffset[1] = static_cast<unsigned char>(_offset >> 8);
      byteLength[0] = static_cast<unsigned char>(_length);
      byteLength[1] = static_cast<unsigned char>(_length >> 8);

      std::copy(byteOffset, byteOffset + 2, registroVariable.begin() + pos); // especifica directamente las posiciones
      std::copy(byteLength, byteLength + 2, registroVariable.begin() + pos + 2);
      contadorDeBytes += _length;
    }
    // cout << "colaAux size => " << colaAux.size() << endl;
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

  /*
  FUNCION: insertRegistroLongitudVariable
      1 Lee prompt con el registro (INSERT INTO ...)
      2 Extrae nombreTabla y extrae las columnas del registro a una tupla #
      4 convertTuplaA_RegistroVariable (le da la estructura: nullbitmap|parte_fija|parte_variable)
  */
  void insertRegistroLongitudVariable(string prompt)
  {
    string columnasDelRegistro;
    string tableName = "";
    int count = 0;

    bool todaviaEsTablaName = true;

    for (auto &caracter : prompt)
    {
      count++;
      if (count > 12) // INSERT INTO
      {
        if (todaviaEsTablaName)
        {
          if (caracter == ' ')
          {
            todaviaEsTablaName = false;
            continue;
          }
          tableName += caracter;
        }
        else
        {
          columnasDelRegistro += caracter;
        }
      }
    }
    columnasDelRegistro += "\n";

    diskController->variableTableToVector();

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

    cout << "\n|" << colsregistro_enTupla << "|" << endl;
    colsregistro_enTupla.erase(colsregistro_enTupla.length() - 1); // quita el salto de linea al final

    vector<char> registroVariable = convertTuplaA_RegistroVariable(colsregistro_enTupla, tableName);
    int sizeRegistro = registroVariable.size();

    cout << "size registroVariable => " << sizeRegistro << "\nRegistro en bytes";

    for (const char byte : registroVariable)
    {
      std::cout << static_cast<int>(byte) << " ";
    }

    cout << "\nRegistro desbinario:\n";
    //verRegistro_Test(registroVariable); // muestra el registro

    // diskController->setBloque_SlottedPage(1);
    // diskController->printBloque_SlottedPage(1);

    /****************************************************************************/

    int bloqueObtenidoPorFreeSpaceMap = diskController->getBloque_Of_Free_space_map();
    cout << "bloque obtenido por free_space_map => " << bloqueObtenidoPorFreeSpaceMap << endl;

    // diskController->setBloque_SlottedPage(bloqueObtenidoPorFreeSpaceMap);
    diskController->insertarRegistroVariableEnBloque_SlottedPage(bloqueObtenidoPorFreeSpaceMap, registroVariable);
    diskController->printBloque_SlottedPage(bloqueObtenidoPorFreeSpaceMap);

    cout << "EspacioLibre_SlottedPage => " << diskController->espacioLibreBloque(bloqueObtenidoPorFreeSpaceMap) << endl;
    cout << lineas::linea100 << endl;
    // return;
    // Solicito un frame (Se encarga el buffer manager):
    // getPageOfBuuferPool funciona con => clock | LRU | MRU
    char *frame = bufferManager->getPageOfBuuferPool(bloqueObtenidoPorFreeSpaceMap)->data;
    int iterdorFrame = 0;
    for (char c : registroVariable)
    {
      c = frame[iterdorFrame];
      iterdorFrame++;
    }
  }

  void deleteRegistro(string prompt)
  {
  }
};
