#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Disco.h"
using namespace std;

class DiskController{
public:
    Disco * disco;
    int nTotalBloques;
    int sizeBloque;
    
    DiskController(Disco* disco){
        this->disco = disco;
        this->nTotalBloques = disco->getNumBloques();
        this->sizeBloque = disco->capacidadDelSector * disco->sectoresPorBloque;

        system("mkdir disk");
        for(int i=1; i<= nTotalBloques; i++){
            string name_bloque = "bloque";
            name_bloque += to_string(i);
            ofstream archivo("disk/"+name_bloque+".bin", std::ios::binary);

            // std::ofstream archivo("datos.bin", std::ios::binary);
            if (!archivo) { std::cout << "No se pudo abrir el archivo." << std::endl; return; }
            // char buffer[sizeBloque];
            char * buffer = new char[sizeBloque];
            // Rellenar el buffer con datos (en este ejemplo, caracteres 'A')
            for (int j = 0; j < sizeBloque; j++) {
                buffer[j] = '\0';
            }
            archivo.write(buffer, sizeBloque);
            
            delete [] buffer;
            //delete buffer;
            
            archivo.close();
        }
        std::cout << "Se han generado los bloques correctamente" << std::endl;
    }
    ~DiskController(){
        system("rmdir /s /q disk");
    }

    void uploadTableToDisk(const char * tablaNameFile, int sizeFile){
        const char* carpetaDisco = "disco\\";
        string path = string(carpetaDisco)+string(tablaNameFile); // disco/titanic
        
        ifstream file(tablaNameFile);
        ifstream esquema("esquema");
        ofstream escribirEnDisco(path,ios::app);
        if (!file.is_open()) { cout << "No se pudo abrir el archivo " << path << endl; return; }
        if (!escribirEnDisco.is_open()) { cout << "No se pudo abrir el archivo " << path << endl; return; }
        
        char c;
        
        char caracter;
        int entero;
        float decimal;
        double decimalGrande;

        int pos = findEsquemaDeTabla("titanic"); // ubicacion de esquema de tabla

        int contador = 0;
        while (file.get(c)) {
            if(c=='\n') continue;
            //escribirEnDisco<<c;
            //sector.writeData();
            contador++;
            //contador-=2;
            
            /*if(contador==this->numBytes){
                break;
            }*/
        }
        
        /*
        for (auto& plato : platos) {
            for (auto& pista : plato.superficie1) {
                for (auto& sector : pista.sectores) {
                    //if(sector.id==(sizeFile/sizeRegistro)) return;
                    //cout<<"|Sector path: "<<sector.path<<"|\n";
                    int contador = 0;
                    while (file.get(c)) {
                        if(c=='\n') continue;
                        escribirEnDisco<<c;
                        sector.writeData();
                        contador++;
                        //contador-=2;
                        if(contador==this->numBytes){
                            break;
                        }
                    }
                    if (file.eof()) { break; }
                }
                if (file.eof()) { break; }
            }
            if (file.eof()) { break; }

            for (auto& pista : plato.superficie2) {
                for (auto& sector : pista.sectores) {
                    //cout<<"|Sector path: "<<sector.path<<"|\n";
                    int contador = 0;
                    while (file.get(c)) {
                        if(c=='\n') continue;
                        escribirEnDisco<<c;
                        sector.writeData();
                        contador++;
                        if(contador==this->numBytes){
                            break;
                        }
                    }
                    if (file.eof()) { break; }
                }
                if (file.eof()) { break; }
            }
            if (file.eof()) { break; }
        }
        
        */
        file.close();
        escribirEnDisco.close();
    }


    int findEsquemaDeTabla(const char * tablaName){
        ifstream esquemaTabla("esquema.txt");
        if ( !esquemaTabla.is_open()) { std::cout << "Error al abrir el archivo." << std::endl; }

        char c;
        int contadorSizeTablaName = 0;
        int indice = 0;
        char tablaNameDelEsquema[50];
        char * ptr = tablaNameDelEsquema;
        streampos posicion;
        // buscar tabla en esquema
        while(true){
            contadorSizeTablaName = 0;
            while(esquemaTabla.get(c)){ // obtiene el nombre hasta el simbolo #
                indice++;
                if(c=='#') 
                    break;
                tablaNameDelEsquema[contadorSizeTablaName] = c;
                contadorSizeTablaName++;
            }
            tablaNameDelEsquema[contadorSizeTablaName] = '\0';

            if(strcmp(ptr,tablaName)==0){ // Si es la tabla?
                cout<<"encontro la tabla:\n";
                cout<<"\n|"<<tablaName<<"|"<<endl;
                cout<<"\n|"<<ptr<<"|"<<endl; 
                posicion = esquemaTabla.tellg(); // esta correcto
                //cout<<"extraer los sizes de los tipos de datos\n";
                break;
            } else {
                do{
                    esquemaTabla.get(c);
                    indice++;
                } while(c!='\n');
                indice++;// IMPORTANTE CONTAR EL SALTO DE LINEA
            }
        }
        esquemaTabla.close();
        return indice;
    }
};