#pragma once
#include <iostream>
#include "design/symbols.cpp"
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
    }
    
    void showInfoSector() {
        std::cout << "\t" <<symbols::guionCuadrado << " IdSector: " << idSector << std::endl;
        std::cout << "\t" <<symbols::guionCuadrado << " numDePista: " << numDePista << std::endl;
        std::cout << "\t" <<symbols::guionCuadrado << " numDeSuperficie: " << numDeSuperficie << std::endl;
        std::cout << "\t" <<symbols::guionCuadrado << " numDePlato: " << numDePlato << std::endl;
    }

};