#include <iostream>
#include <cstring>
#include "DiskController.h"
#include "Disco.h"
#include "SGDB.h"
using namespace std;

char input[MAX_SIZE_PROMPT];


int main(){
    Disco myDisk("structDisk.bin",5,8,20,100,10);
    DiskController myDiskController(&myDisk);
    SGDB sistema(&myDisk,& myDiskController);

    system("pause");

    int opc;
    bool salir = false;
    while(salir==false){
        cout<<"\n-------------------| MENU |--------------------\n";
        cout<<"\nIngrese una opcion:\n";
        cout<<"[1] Menu de Disco\n";
        cout<<"[2] Menu del sistema de Base de Datos\n";
        cin>>opc;
        if(opc==1){
            cout<<"etc\n";
        } else if(opc==2){

            int opc2;
            cout<<"[1] Crear Tabla\n";
            cout<<"[2] Insert File ()\n";
            cout<<"[3] Insert 1 registro n\n"; // ident si es fijo o variable de acuerdo al esquema de tablas, e insertarlo en binario
            cout<<"[4] Select 1 registro n\n";
            cout<<"[5] Delete 1 registro n\n";
            cin>>opc2;
            cin.ignore();

            if(opc2==1){
                cout<<" >> ";
                // fgets(input,MAX_SIZE_PROMPT,stdin); // stdin: entrada por terminal
                // input[strcspn(input, "\n")] = '\0'; // Eliminar el caracter de nueva linea
                // sistema.createTable(input);
                string prompt;
                getline(cin,prompt);
                sistema.createTable(prompt);
            } else if(opc2==2){
                //myDiskController.uploadTableToDisk("titanic.csv");
                string prompt, prompt2;
                getline(cin,prompt);
                getline(cin,prompt2);
                myDiskController.uploadTableToDisk(prompt,prompt2);
            }

        } else if(opc==0){
            salir = true;
        }
    }
    return 0;
}