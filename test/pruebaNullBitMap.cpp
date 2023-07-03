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
    int numBytes_ASeparar = std::ceil(static_cast<double>(nullbitmap.size()) / 8); // redondear a techo
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



std::vector<char> generate_NullBitMap(const std::vector<int>& nullbitmap) {
    int numBytes_ASeparar = std::ceil(static_cast<double>(nullbitmap.size()) / 8);
    std::vector<char> bytes;

    int contadorBits = 0;
    for (int k = 0; k < numBytes_ASeparar; k++) {
        unsigned char byte = 0;
        for (int i = 7; i >= 0; i--) {
            byte |= (nullbitmap[contadorBits]) << i;
            contadorBits++;
            if (contadorBits == nullbitmap.size()) {
                break;
            }
        }
        bytes.push_back(byte);
    }

    return bytes;
}

void writeBytesToFile(const std::vector<char>& bytes, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(bytes.data(), bytes.size());
        file.close();
        std::cout << "Bytes written to file: " << filename << std::endl;
    } else {
        std::cout << "Error opening file: " << filename << std::endl;
    }
}

std::vector<char> readBytesFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    std::vector<char> bytes;

    if (file.is_open()) {
        // Obtener el tamaño del archivo
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Leer los bytes del archivo y almacenarlos en el vector
        bytes.resize(fileSize);
        file.read(bytes.data(), fileSize);

        file.close();
    } else {
        std::cout << "Error opening file: " << filename << std::endl;
    }

    return bytes;
}

void readBitsFromBytes(const std::vector<char>& bytes) {
    for (const char byte : bytes) {
        for (int i = 7; i >= 0; i--) {
            bool bit = ((byte >> i) & 1);
            std::cout << bit;
        }
    }
    std::cout << std::endl;
}


int main() {
    // escribirByteBitPorBit();
    // leerByteBitPorBit();
    // vector<int> mynull = {0,1,0,0,0,1,0,0,0,0,1,1};
    // write_NullBitMap(mynull);
    // read_NullBitMap(2);

    vector<int> nullbitmap = {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
    vector<char> bytes = generate_NullBitMap(nullbitmap);
    cout<<"size: "<<bytes.size()<<endl;
    for (const char byte : bytes) {
        std::cout << static_cast<int>(byte) << " ";
    }

    cout<<"\n---------------\n";
    writeBytesToFile(bytes, "archivo_binario.bin");
    std::vector<char> bytesExtraidos = readBytesFromFile("archivo_binario.bin");
    cout<<"size bytesExtraidos = "<<bytesExtraidos.size()<<"\n";
    readBitsFromBytes(bytesExtraidos); // Leer los bits de cada byte

    return 0;
}