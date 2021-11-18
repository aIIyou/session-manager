#include "xmalloc.h"
#include <cstdlib>

XAllocator::XAllocator(){
    for(int i=0;i<NUMS_SPAN;i++){
        this->used[i].datap = 0;
        this->used[i].prev= 0;
        this->used[i].next = 0;
        this->usedListLength[i]=0;
    }
    for(int i=0;i<NUMS_SPAN;i++){//这个for循环后边需要优化
        if (i==0){
            this->level2Cap[i]=SPAN0;
        }else{
            this->level2Cap[i]=2*this->level2Cap[i-1];
        }  
    }
    void *mem = 0;
    int spanCap = 0;
    int numOfSpan = 0;

    for(int i=0;i<NUMS_SPAN;i++){
        mem = malloc(16*1024);//每次固定分配16KB
        spanCap = this->level2Cap[i];
        this->splitBySpanCap(mem,spanCap,16*1024,i);
        this->freeListLength[i]=16*1024/spanCap;
    }
}

int XAllocator::splitBySpanCap(void* mem,int spanCap,int totalCap,int level){
    Span* prev = &this->free[level];
    Span* cur = 0;
    char* base = (char*)mem;
    for(int i=0;i<totalCap/spanCap;i++){
        cur = (Span*)(base+i*spanCap);
        this->connectSpan(prev,cur);
        prev = prev->next;
    }
    this->connectSpan(prev,&this->free[level]);
    return 0;
}

Span* XAllocator::formatSpan(char* span,int spanCap){
    Span* res = (Span*)span;
    res->spanCap = spanCap;
    res->datap = (void*)(span+spanCap);
    return res;
}

void XAllocator::connectSpan(Span* prev,Span* cur){
    prev->next = cur;
    cur->prev = prev;
}
