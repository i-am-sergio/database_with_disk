#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include "Sector.h"
using namespace std;

class Disco{
    public:
        int capacidadTotalBytes;
        uint64_t capacidadTotalBits;

        int numPlatos;
        int numPistasPorSuperficie;
        int numSectoresPorPista;
        int capacidadDelSector;
        
        int sectoresPorBloque;
        int sizeBloque;

        int totalSectoresEnDisco;
        int totalPistasEnDisco;
        int totalSuperficiesEnDisco;
        
        vector <Sector> sectores;

        int getNumBloques(){
            return (this->capacidadTotalBytes/(this->capacidadDelSector*this->sectoresPorBloque));
        }
        
        Disco(string nameStructDisk,int numPlatos,int numPistasPorSuperficie,int numSectoresPorPista,int capacidadDelSector,int sectoresPorBloque){
            std::fstream archivo("structDisk.bin", std::ios::in | std::ios::binary);
            //archivo.seekg(0, std::ios::end);
            if(archivo){
                archivo.seekg(0, std::ios::beg);
                archivo.read(reinterpret_cast<char*>(&this->numPlatos), sizeof(numPlatos));
                archivo.read(reinterpret_cast<char*>(&this->numPistasPorSuperficie), sizeof(numPistasPorSuperficie));
                archivo.read(reinterpret_cast<char*>(&this->numSectoresPorPista), sizeof(numSectoresPorPista));
                archivo.read(reinterpret_cast<char*>(&this->capacidadDelSector), sizeof(capacidadDelSector));
                archivo.read(reinterpret_cast<char*>(&this->sectoresPorBloque), sizeof(sectoresPorBloque));
                cout<<this->numPlatos<<"-"<<this->numPistasPorSuperficie<<"-"<<this->numSectoresPorPista<<"-"<<this->capacidadDelSector<<"-"<<this->sectoresPorBloque<<"\n";
                archivo.close();
            } else {
                std::fstream archivo("structDisk.bin", std::ios::out | std::ios::binary);
                this->numPlatos = numPlatos;
                this->numPistasPorSuperficie = numPistasPorSuperficie;
                this->numSectoresPorPista = numSectoresPorPista;
                this->capacidadDelSector = capacidadDelSector;
                this->sectoresPorBloque = sectoresPorBloque;
                archivo.write(reinterpret_cast<char*>(&numPlatos), sizeof(numPlatos));
                archivo.write(reinterpret_cast<char*>(&numPistasPorSuperficie), sizeof(numPistasPorSuperficie));
                archivo.write(reinterpret_cast<char*>(&numSectoresPorPista), sizeof(numSectoresPorPista));
                archivo.write(reinterpret_cast<char*>(&capacidadDelSector), sizeof(capacidadDelSector));
                archivo.write(reinterpret_cast<char*>(&sectoresPorBloque), sizeof(sectoresPorBloque));
                cout<<this->numPlatos<<"-"<<this->numPistasPorSuperficie<<"-"<<this->numSectoresPorPista<<"-"<<this->capacidadDelSector<<"-"<<this->sectoresPorBloque<<"\n";
            }
            buildSectores();
        };
        
        void buildSectores(){ //5,8,20,100,10
            this->capacidadTotalBytes = capacidadDelSector * numSectoresPorPista * numPistasPorSuperficie * 2 * numPlatos; 
            this->capacidadTotalBits = this->capacidadTotalBytes * 8;
            this->totalSectoresEnDisco = capacidadTotalBytes / capacidadDelSector;
            this->totalPistasEnDisco = totalSectoresEnDisco / numSectoresPorPista;
            this->totalSuperficiesEnDisco = numPlatos * 2;
            showInfoDisco();
            int _pista,_superf,_plat;
            for(int i=0; i<totalSectoresEnDisco;i++){
                _pista = ((i+1) % numSectoresPorPista == 0) ? ((i+1)/numSectoresPorPista) : ((i+1)/numSectoresPorPista)+1;
                _superf = (_pista % numPistasPorSuperficie == 0) ? (_pista/numPistasPorSuperficie) : (_pista/numPistasPorSuperficie)+1;
                _plat = (_superf % 2 == 0) ? (_superf/2) : (_superf/2)+1;
                sectores.emplace_back(i+1,_pista,_superf,_plat);
                //cout<<"Sector "<<i+1<<"\n";
            }
        }

        void showInfoDisco() {
            std::cout << "capacidadTotalBytes: " << capacidadTotalBytes << std::endl;
            std::cout << "capacidadTotalBits: " << capacidadTotalBits << std::endl;
            std::cout << "numPlatos: " << numPlatos << std::endl;
            std::cout << "numPistasPorSuperficie: " << numPistasPorSuperficie << std::endl;
            std::cout << "numSectoresPorPista: " << numSectoresPorPista << std::endl;
            std::cout << "capacidadDelSector: " << capacidadDelSector << std::endl;
            std::cout << "sectoresPorBloque: " << sectoresPorBloque << std::endl;
            std::cout << "totalSectoresEnDisco: " << totalSectoresEnDisco << std::endl;
            std::cout << "totalPistasEnDisco: " << totalPistasEnDisco << std::endl;
            std::cout << "totalSuperficiesEnDisco: " << totalSuperficiesEnDisco << std::endl;
        }


        ~Disco(){}

};