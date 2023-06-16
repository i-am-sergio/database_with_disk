#pragma once
#include <iostream>
#include "DiskController.h"

class BufferManager {
public:
    DiskController * diskController;
    char * buffer; // 1 bloque -> 1 pagina
    
    BufferManager(DiskController * diskController){
        this->diskController = diskController;
        buffer = new char[this->diskController->sizeBloque];
    }
    ~BufferManager(){}
};


