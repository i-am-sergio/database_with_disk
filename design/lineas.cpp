#pragma once
#include <iostream>

namespace lineas {
    std::string linea50(50, (char) 196);
    std::string linea100(100, (char) 196);
    std::string linea200(200, (char) 196);
    
    std::string drawLinea(int size){
        std::string linea(size, (char) 196);
        return linea;
    }

}
