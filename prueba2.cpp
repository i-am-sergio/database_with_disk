#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(){

    char * data = new char[1000];
    ifstream file("disk/bloque1.bin", ios::binary);
    file.read(data,1000);

    file.close();

    int* ptrInt;
    double* ptrDouble;
    char* ptrChar;

    string cadena;

    vector<pair<string,string>> tipos = {
        {"int","4"},
        {"int","4"},
        {"int","4"},

        {"str","90"},
        {"str","6"},
        {"double","8"},

        {"int","4"},
        {"int","4"},
        {"str","20"},

        {"double","8"},
        {"str","20"},
        {"str","1"},
    };

    vector<int> byteInicio;
    byteInicio.emplace_back(0);

    ptrInt = (reinterpret_cast<int*>(&data[0]));
    cout<<*ptrInt<<"|";
    ptrInt = (reinterpret_cast<int*>(&data[4]));
    cout<<*ptrInt<<"|";
    ptrInt = (reinterpret_cast<int*>(&data[8]));
    cout<<*ptrInt<<"|";
    return 1;
    /*
    for(int i=0; i<tipos.size(); i++){
        if(tipos[i].first == "int"){
            byteInicio.emplace_back(stoi(tipos[i].second));
        } else if (tipos[i].first == "double"){

        } else if (tipos[i].first == "str"){

        }
    }*/

    int contador = 0;
    for (int k=0; k<5; k++){
        for(int i=0; i<tipos.size(); i++){
            // cout<<tipos[i].first<<endl;
            if(tipos[i].first == "int"){
                ptrInt = (reinterpret_cast<int*>(data[contador]));
                contador+=4;
                cout<<*ptrInt<<"|";
            } else if (tipos[i].first == "double"){
                ptrDouble = (reinterpret_cast<double*>(data));
                cout<<*ptrDouble<<"|";
            } else if (tipos[i].first == "str"){
                for(int j = 0; j<stoi(tipos[i].second); j++){
                    ptrChar = (reinterpret_cast<char*>(data));
                    cout<<*ptrChar;
                    //cadena += caracter;
                }
                cout<<"|";
            }
        }
        cout<<"\n";
    }

    delete[] data;
    return 0;
}