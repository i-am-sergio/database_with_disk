#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <initializer_list>
using namespace std;

void escribirByteBitPorBit() {
    std::ofstream archivo("archivo_binario.bin", std::ios::binary);    

    if (archivo) {
        unsigned char byte = 0; // Byte inicializado en 0

        // Escribir los bits en el byte
        for(int i=7; i>=0; i--){
            if(i%2==0)
                byte |= 1 << i;
            else 
                byte |= 0 << i;
        }

        // byte |= 1 << 7;  // Bit mas significativo (MSB)
        // byte |= 1 << 6;  // Siguiente bit
        // byte |= 0 << 5;
        // byte |= 1 << 4;
        // byte |= 0 << 3;
        // byte |= 0 << 2;
        // byte |= 0 << 1;
        // byte |= 0 << 0;  // Bit menos significativo (LSB)

        // Escribir el byte en el archivo
        archivo.write(reinterpret_cast<char*>(&byte), sizeof(byte));

        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo para escritura." << std::endl;
    }
}

void leerByteBitPorBit() {
    std::ifstream archivo("archivo_binario.bin", std::ios::binary);

    if (archivo) {
        unsigned char byte = 0;
        archivo.read(reinterpret_cast<char*>(&byte), sizeof(byte));

        // Leer cada bit del byte
        
        for (int i = 7; i >= 0; i--) {
            bool bit = byte & (1 << i);
            std::cout << "Bit " << (7 - i) << ": " << bit << std::endl;
        }

        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo para lectura." << std::endl;
    }
}

void write_NullBitMap(vector<int> nullbitmap){//Al momento de insertar
    int numBytes_ASeparar = std::ceil(static_cast<double>(nullbitmap.size()) / 8);
    ofstream archivo("archivo_binario.bin", ios::binary);
    int contadorBits = 0;
    
    for(int k=0; k<numBytes_ASeparar; k++){
        unsigned char byte = 0; 
        for(int i = 7; i >= 0; i--){
            byte |= (nullbitmap[contadorBits]) << i;
            contadorBits++;
            if(contadorBits == nullbitmap.size()){
                break;
            }
        }
        archivo.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }
}

void read_NullBitMap(int sizeNullBitMap) {
    std::ifstream archivo("archivo_binario.bin", std::ios::binary);

    if (archivo) {
        // Leer bit por bit de cada byte
        for(int k=0; k<sizeNullBitMap; k++){
            unsigned char byte = 0;
            archivo.read(reinterpret_cast<char*>(&byte), sizeof(byte));
            for (int i = 7; i >= 0; i--) {
                bool bit = byte & (1 << i);
                std::cout << "Bit " << (7 - i) << ": " << bit << std::endl;
            }
        }
        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo para lectura." << std::endl;
    }
}

void getNumAtributos(){
    
}

void procesarLista(const std::initializer_list<int>& lista) {
    cout<<lista.size()<<"\n";
    for (const auto& elemento : lista) {
        std::cout << elemento << " ";
    }
    std::cout << std::endl;
}


int main() {
    // escribirByteBitPorBit();
    // leerByteBitPorBit();
    vector<int> mynull = {0,1,0,0,0,1,0,0,0,0,1,1};
    write_NullBitMap(mynull);
    read_NullBitMap(2);
    return 0;
}