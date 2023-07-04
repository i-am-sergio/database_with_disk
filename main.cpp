#include <iostream>
#include <cstring>
#include <string>
#include "DiskController.h"
#include "HeadersHDD/Disco.h"
#include "SGDB.h"
using namespace std;


int main(){
    Disco myDisk;
    DiskController myDiskController(&myDisk);
    SGDB sistema(&myDisk,& myDiskController);

    cin.get();

    int opc;
    bool salir = false;
    while(salir==false){
        cout<<"\n-------------------| MENU |--------------------\n";
        cout<<"\nIngrese una opcion:\n";
        cout<<"[1] Menu de Disco\n";
        cout<<"[2] Menu del sistema de Base de Datos\n";
        cin>>opc;
        if(opc==1){
            int opc2;
            cout<<"[1] Imprimir Metadata Sector n\n";
            cout<<"[2] Imprimir Metadata Bloque n\n";
            cout<<"[3] Imprimir Metadata Registro n\n";
            cout<<"[4] Mostrar informacion del disco n\n";
            cout<<"[5] Mostrar diccionario n\n";
            cout<<"[6] Prueba SlottedPage\n";
            cout<<"[7] Agregar Tabla al diccionario\n";
            cout<<"[8] Setear slottedPages\n";
            cin>>opc2;
            cin.ignore();
            if(opc2==1){
                int numSector;
                cout<<"Ingrese un numero de sector => ";
                cin>>numSector;
                myDiskController.printSector(numSector);
            } else if(opc2==2){
                int numBloque;
                cout<<"Ingrese un numero de bloque => ";
                cin>>numBloque;
                myDiskController.printBloque(numBloque);
            }else if(opc2==3){
                int registro,ubi;
                cout<<"Ingrese el numero de registro => ";
                cin>>registro;
                ubi = sistema.printUbicacionRegistro(registro);
                myDiskController.printBloque(ubi);
            } else if (opc2 == 4) {
                myDiskController.disco->showInfoDisco();
            } else if (opc2 == 5) {
                myDiskController.showDictionary();
            } else if (opc2 == 6) {
                myDiskController.setBloque_SlottedPage(1);
            } else if (opc2 == 7) {
                myDiskController.AgregarTablaVariable();
                myDiskController.showDictionary();
            } else if(opc2==8){
                myDiskController.setearBloques();
            }
        } else if(opc==2){

            int opc2;
            cout<<"[1] Crear Tabla\n";
            cout<<"[2] Insert File ()\n";
            cout<<"[3] Mostrar Bloque n\n";
            cout<<"[4] Insert 1 registro n\n"; // ident si es fijo o variable de acuerdo al esquema de tablas, e insertarlo en binario
            cout<<"[5] Delete 1 registro n\n";
            cout<<"[6] SELECT * FROM titanic\n";
            cout<<"[7] SELECT * FROM titanic where atributo = n\n";
            cout<<"[8] Elegir Tabla a trabajar\n";
            cout<<"[9] insertar RegistroVariable\n";
            cout<<"[10] Crear Tabla variable\n";
            cin>>opc2;
            cin.ignore();

            if(opc2==1){
                cout<<" >> ";
                string prompt;
                getline(cin,prompt);
                sistema.createTable(prompt);
            } else if(opc2==2){
                string prompt, prompt2;
                getline(cin,prompt);
                getline(cin,prompt2);
                myDiskController.uploadTableToDisk(prompt,prompt2);
            } else if(opc2==3){
                cout<<" >> Ingrese el numero de bloque: ";
                string prompt;
                getline(cin,prompt);
                sistema.mostrarPage(stoi(prompt));
                sistema.bufferManager->showpageTableCLOCK();
                //cout<<"MUESTRA BLOQUE 100 ----\n";
                //sistema.mostrarPage(1);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(2);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(4);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(4);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(5);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(1);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(5);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(3);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(1);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(2);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(3);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(1);
                //sistema.bufferManager->showpageTableCLOCK();
                //sistema.mostrarPage(5);
                //sistema.bufferManager->showpageTableCLOCK();
                //cout<<"\n---\n";
            } else if(opc2==4){
                sistema.insertarRegistro();
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==5){
                string prompt;  
                cout<<"Ingrese el objetivo -> ";
                getline(cin,prompt);
                sistema.deleteRegistro(stoi(prompt));
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==6){
                sistema.showTable();//cambios
                //sistema.showTable("titanic");
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==7){
                string prompt, prompt2;
                cout<<"Ingrese el nombre del atributo -> ";
                getline(cin,prompt);
                cout<<"Ingrese el objetivo -> ";
                getline(cin,prompt2);

                sistema.search(prompt,stoi(prompt2));
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==8){
                string prompt;
                cout<<"Ingrese el nombre de la tabla -> ";
                getline(cin,prompt);
                myDiskController.nameTable = prompt;
            } else if(opc2==9){
                try{
                    string prompt;
                    // string prompt = "INSERT INTO titanic 1,0,3,\"Johnston, Miss. Catherine Helen Carrie\",female,,1,2,W./C. 6607,23.45,,S";
                    // string promptMovie = "INSERT INTO movie Alien,,,2,,5,4,,,4,,,,,,2,,,4,3,,,2,5,,4";
                    cout<<" >> ";
                    getline(cin,prompt);
                    // sistema.diskController->setBloque_SlottedPage(1);
                    // string prompt = "INSERT INTO titanic 892,0,3,\"Johnston, Miss. Catherine Helen Carrie\",female,,1,2,W./C. 6607,23.45,,S";
                    
                    
                    sistema.insertRegistroLongitudVariable(prompt);
                    // sistema.insertRegistroLongitudVariable(promptMovie);

                    // string prompt2 = "INSERT INTO titanic 893,1,2,\"Richards, Master. George Sibley\",male,0.83,1,1,29106,18.75,,S";
                    // sistema.insertRegistroLongitudVariable(prompt2);
                    // string prompt3 = "INSERT INTO titanic 894,1,3,\"Backstrom, Mrs. Karl Alfred (Maria Mathilda Gustafsson)\",female,33,3,0,3101278,15.85,,S";
                    // sistema.insertRegistroLongitudVariable(prompt3);
                    
                    // sistema.insertRegistroLongitudVariable(prompt);
                    // sistema.insertRegistroLongitudVariable(prompt2);
                    // sistema.insertRegistroLongitudVariable(prompt3);
                    // sistema.insertRegistroLongitudVariable(prompt3);
                    // sistema.insertRegistroLongitudVariable(prompt);
                    // sistema.insertRegistroLongitudVariable(prompt2);
                    // sistema.insertRegistroLongitudVariable(prompt2);
                    // sistema.insertRegistroLongitudVariable(prompt);
                    // sistema.insertRegistroLongitudVariable(prompt3);

                } catch(const exception& e){
                    cout << "Se produjo una excepcion: " << e.what() << endl;
                }
            } else if(opc2==10){
                cout<<" >> ";
                string prompt;
                getline(cin,prompt);
                sistema.createTable_Variable(prompt);
            }

        } else if(opc==0){
            salir = true;
        }
    }
    return 0;
}
