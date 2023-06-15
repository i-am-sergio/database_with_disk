#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class DiskController{
public:
    int nTotalBloques;
    
    DiskController(int nTotalBloques){
        this->nTotalBloques = nTotalBloques;

        system("mkdir disk");
        for(int i=1; i<= nTotalBloques; i++){
            string name_bloque = "bloque";
            name_bloque += to_string(i);
            ofstream file("disk/"+name_bloque);
            file.close();
        }
    }
    ~DiskController(){
        system("rmdir /s /q disk");
    }
};