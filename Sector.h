#pragma once
#include <iostream>
using namespace std;

class Sector{
public:
    // Info
    int idSector;
    int numDePista;
    int numDeSuperficie;
    int numDePlato;

    int capacidadDeSector;
    int capacidadOcupada;
    int capacidadLibre;

    Sector(int idSector, int numDePista, int numDeSuperficie, int numDePlato){//(i+1,_pista,_superf,_plat);
        this->idSector = idSector;
        this->numDePista = numDePista;
        this->numDeSuperficie = numDeSuperficie;
        this->numDePlato = numDePlato;

        cout<<"["<<idSector<<"]"<<numDePista<<"|"<<numDeSuperficie<<"|"<<numDePlato<<"\n";
    }

    void write(){

    }
};