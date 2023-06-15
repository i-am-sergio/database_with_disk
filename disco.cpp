#include <iostream>
#include <fstream>
using namespace std;

void leerDeByteAByte(int byteIni){
    std::fstream archivo("datos.bin", std::ios::in | std::ios::out | std::ios::binary);
    archivo.seekg(byteIni);
    char c[10];
    archivo.read(reinterpret_cast<char*>(&c), 10);
    cout<<c;
    archivo.close();
}

void leerDeByteAByteConIfstream(int byteIni) {
    std::ifstream archivo("datos.bin", std::ios::in | std::ios::binary);
    archivo.seekg(byteIni);
    char c;
    for (int i = 0; i < 10 && archivo.read(&c, sizeof(char)); ++i) {
        cout << c;
    }
    archivo.close();
}

int main(){
    std::fstream archivo("datos.bin", std::ios::in | std::ios::out | std::ios::binary);
    //FileHeader header;
    int numRegistros1 = 0;
    int tamBloque1 = 1024;
    int primerEliminado1 = -1;
    char buffer1[] = "abcdefghij";
    archivo.write(reinterpret_cast<char*>(&numRegistros1), sizeof(numRegistros1));
    archivo.write(reinterpret_cast<char*>(&tamBloque1), sizeof(tamBloque1));
    archivo.write(reinterpret_cast<char*>(&primerEliminado1), sizeof(primerEliminado1));
    archivo.write(reinterpret_cast<char*>(&buffer1), sizeof(char)*10);

    cout<<"|"<<archivo.tellp()<<"|\n";

    archivo.close();
    
    leerDeByteAByteConIfstream(12);

    cin.get(); cin.get();
    system("del datos.bin");
    //std::fstream archivo2("datos.bin", std::ios::in | std::ios::out | std::ios::binary);

    // int numRegistros, tamBloque, primerEliminado;
    // char a;
    // archivo2.read(reinterpret_cast<char*>(&numRegistros), sizeof(numRegistros));
    // //std::cout << "|"<<archivo2.tellg()<<"|\n";

    // archivo2.read(reinterpret_cast<char*>(&tamBloque), sizeof(tamBloque));
    // archivo2.read(reinterpret_cast<char*>(&primerEliminado), sizeof(primerEliminado));
    // archivo2.read(reinterpret_cast<char*>(&a), sizeof(a));
    // std::cout << "Número de registros: " << numRegistros << std::endl;
    // std::cout << "Tamaño de bloque: " << tamBloque << std::endl;
    // std::cout << "Dirección del primer eliminado: " << primerEliminado << std::endl;
    // std::cout << "letra: " << a << std::endl;

    //archivo2.close();
    
    return 0;
}