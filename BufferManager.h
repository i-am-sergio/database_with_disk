#pragma once
#include <iostream>
#include <unordered_map>
#include "DiskController.h"
#include "Page.h"

class BufferManager {
public:
    DiskController * diskController;

    vector<Page> bufferPool;
    int sizeBufferPool;
    int sizePage;
    
    vector< vector<int> > pageTable; //pageTable
    vector<int> free_list;
    
    BufferManager(DiskController * diskController){
        this->sizeBufferPool = 4;
        this->diskController = diskController;
        this->sizePage = this->diskController->sizeBloque;
        bufferPool.resize(this->sizeBufferPool);

        for (auto& frame : bufferPool){
            frame = Page(this->sizePage);
        }

        pageTable.resize(sizeBufferPool);
        for(auto& frame : pageTable){
            frame.resize(5); // frameId,Page,id,DirtyBit,PinCount,LastUsed
        }
        
        for (int i=0; i<pageTable.size(); i++){
            for (int j=0; j<pageTable[0].size(); j++){
                if(j==0){
                    pageTable[i][j] = i;
                } else {
                    pageTable[i][j] = -1;
                }
            }
        }


    }
    ~BufferManager() { }


    auto getPageOfBuuferPool(int pageId){
        Page* pagina = LRU(pageId);
        return pagina;
    }


    Page* LRU(int pageId){
        for (int i=0; i<pageTable.size(); i++){
            if(pageTable[i][1]==pageId){
                pageTable[i][3]++;
                for(int k=0; k<pageTable.size(); k++){
                    if(pageTable[k][1]!=-1){
                        pageTable[k][4]++;
                    }
                }
                pageTable[i][4] = 0;
                return &bufferPool[i];
            }
        }
        for (int i=0; i<pageTable.size(); i++){
            if(pageTable[i][1]==-1){
                for(int k=0; k<pageTable.size(); k++){
                    if(pageTable[k][1]!=-1){
                        pageTable[k][4]++;
                    }
                }
                pageTable[i][1] = pageId;
                pageTable[i][2] = 0;
                pageTable[i][3] = 0;
                pageTable[i][4] = 0;
                
                return &bufferPool[i];
            }  
        }

        int mayor = pageTable[0][4];
        int pos=0;
        for (int i=0; i<pageTable.size(); i++){
            if(pageTable[i][2]==0 && pageTable[i][4]>mayor){
                mayor = pageTable[i][4];
                pos = i;
            }
        }

        if(pos!=-1){
            for(int k=0; k<pageTable.size(); k++){
                if(pageTable[k][1]!=-1){
                    pageTable[k][4]++;
                }
            }
            pageTable[pos][1] = pageId;
            pageTable[pos][2] = 0;
            pageTable[pos][3] = 0;
            pageTable[pos][4] = 0;
            return &bufferPool[pos];
        }
        //Implementar el pin y unpin para el dirty bit
        //
       return nullptr;
    }

    Page* Clock(int pageId){
        return nullptr;
    }

    void showPageTable(){
        cout<<"\n LRU tabla \n";
        for (int i=0; i<pageTable.size(); i++){
            for (int j=0; j<pageTable[0].size(); j++){
                cout<<pageTable[i][j]<<"\t";
            }
            cout<<"\n";
        }
        cout<<"\n";
    }
};

