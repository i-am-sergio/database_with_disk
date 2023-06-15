#include <iostream>
#include <fstream>

void escribirByteBitPorBit() {
    std::ofstream archivo("archivo_binario.bin", std::ios::binary);

    if (archivo) {
        unsigned char byte = 0; // Byte inicializado en 0

        // Escribir los bits en el byte
        byte |= 0 << 7;  // Bit más significativo (MSB)
        byte |= 0 << 6;  // Siguiente bit
        byte |= 0 << 5;
        byte |= 0 << 4;
        byte |= 1 << 3;
        byte |= 0 << 2;
        byte |= 1 << 1;
        byte |= 0 << 0;  // Bit menos significativo (LSB)

        // Escribir el byte en el archivo
        archivo.write(reinterpret_cast<char*>(&byte), sizeof(byte));

        archivo.close();
    } else {
        std::cout << "No se pudo abrir el archivo para escritura." << std::endl;
    }
}

int main() {
    escribirByteBitPorBit();
    return 0;
}
