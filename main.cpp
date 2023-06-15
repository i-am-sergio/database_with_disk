#include <iostream>
#include "DiskController.h"
#include "Disco.h"
using namespace std;


int main(){
    Disco myDisk("structDisk.bin",5,8,20,100,10);
    DiskController myDiskController(myDisk.getNumBloques());
    system("pause");

    int opc, opc2;
    while(true){
        cout<<"\n-------------------| MENU |--------------------\n";
        cout<<"\nIngrese una opcion:\n";
        cout<<"[1] Menu de Disco\n";
        cout<<"[2] Menu del sistema de Base de Datos\n";
        cin>>opc;
        if(opc==1){
            
        } else if(opc==2){
            cout<<"[1] Crear Tabla\n";
            cout<<"[2] Insert File ()\n";
            cout<<"[3] Insert 1 registro n\n"; // ident si es fijo o variable de acuerdo al esquema de tablas, e insertarlo en binario
            cout<<"[4] Select 1 registro\n";
            cout<<"[5] Delete 1 registro\n";
            cin>>opc2;
            if(opc==1){
                
            } else if(opc==2){

            }
        }

    return 0;
}