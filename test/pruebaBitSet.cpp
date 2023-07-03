#include <iostream>
#include <bitset>
#include <string>
using namespace std;

string convert_str_a_binario(std::string registroTupla) {
    std::string binario;

    // Convierte los caracteres a su representación binaria
    for (char c : registroTupla) {
        std::bitset<8> bits(c);
        binario += bits.to_string();
    }

    // Convierte los números a su representación binaria
    for (int i = 0; i < 2; i++) {
        int numero = registroTupla[10 + i] - '0';  // Convierte el carácter numérico a entero
        std::bitset<8> bits(numero);
        binario += bits.to_string();
    }

    return binario;
}




int main(){
    string registroTupla = "abcdefghij12";
    string resultado = convert_str_a_binario(registroTupla);

    // Imprimir el resultado
    std::cout << "Resultado: " << resultado << std::endl;

    // Obtener el tamaño del resultado
    std::cout << "Tamaño: " << resultado.size() << " bits" << std::endl;


    return 0;
}