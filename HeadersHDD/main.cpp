#include <iostream>
#include "Disco.h"
using namespace std;

int main(int argc, char *argv[]){
  Disco hdd;
  for (auto& plato : hdd.platos) {
    for(auto& superficie: plato.superficies){
      for(auto& pista : superficie.pistas){
        for(auto& sector : pista.sectores){
          sector.showInfoSector();
        }
      }
    }
  }

  //for (auto& plato : hdd.platos) {
  //  for(auto& superficie: plato.superficies){
  //    for(auto& pista : superficie.pistas){
  //      pista.showInfo();
  //    }
  //  }
  //}
  return 0;
}
