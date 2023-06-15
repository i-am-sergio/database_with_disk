#include <iostream>
#include <map>
#include <vector>
#include <cstring>
using namespace std;

// Una pagina es un bloque de datos de tamaño fijo
// Cada pag tiene un identificador unico
// El DBMS utiliza una capa de direccionamiento indirecto para asignar ID de página a ubicaciones físicas
class Page{ 
public:
    int id;

    Page(){

    }
    ~Page(){

    }
};

class StorageManager{
public:
    void * LRUKReplacementPolicy;
    BufferPool bufferPool;
    vector<Page*> readPages;
    vector<Page*> WritePages;
};

class DiskManager{

};

class BufferPool{
public:
    vector<Page> pool;
    DiskManager diskManager;

    BufferPool(){}

    void movePageToDisk(){

    }

    void getPage(int page_id){
        // findPageInMemory(page_id);

    }

};


int main(){

    return 0;
}