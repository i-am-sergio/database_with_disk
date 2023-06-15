#include <iostream>
#include <fstream>

void mostrarBitsDelByte() {
    std::ifstream archivo("archivo_binario.bin", std::ios::binary);

    if (archivo) {
        unsigned char byte;
        
        // Leer el byte del archivo
        archivo.read(reinterpret_cast<char*>(&byte), sizeof(byte));

        // Mostrar cada bit del byte
        for (int i = 7; i >= 0; --i) {
            unsigned char bit = (byte >> i) & 1;
            std::cout << "Bit " << i << ": " << static_cast<int>(bit) << std::endl;
        }

        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo para lectura." << std::endl;
    }
}

int main() {
    mostrarBitsDelByte();
    return 0;
}
