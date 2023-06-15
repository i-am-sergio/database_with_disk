#include <iostream>
#include "DiskController.h"
#include "Disco.h"
using namespace std;


int main(){
    Disco myDisk("structDisk.bin",5,8,20,100,10);
    DiskController myDiskController(myDisk.getNumBloques());
    system("pause");
    return 0;
}