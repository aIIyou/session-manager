#pragma once
#define NUMS_SPAN 8

#define SPAN0   64
#define SPAN1   128
#define SPAN2   256
#define SPAN3   512
#define SPAN4   1024
#define K       1024
#define SPAN5   (2*K)
#define SPAN6   (4*K)
#define SPAN7   (8*K)    //现在最大的块是8K，大于等于16K的还没做
#define SPAN8   (16*K)
#define SPAN9   (32*K)

#define SIZE_PTR 8

//32字节
class Span {
    public:
    Span* next;
    Span* prev;
    int spanCap;
    void* datap;      //通过一个Span的datap可以快速找到这个Span上的对象
};



//这个类型的实例不需要内存管理，整个程序的生命周期内存在
class XAllocator {
    public:
    int level2Cap[NUMS_SPAN];

    //首先需要一个结构O(1)时间找到SpanList中的空闲块，可以把used状态和free状态分开，这样链表头上的块直接用
    Span used[NUMS_SPAN];
    int usedListLength[NUMS_SPAN];

    //这里有一根问题需要考虑freeList中的块怎么释放（先考虑单线程的设计）
    Span free[NUMS_SPAN];
    int freeListLength[NUMS_SPAN];

    
    XAllocator();//先申请若干个Page进行分割
    int splitBySpanCap(void* mem,int spanCap,int totalCap,int level);
    Span* formatSpan(char* span,int spanCap);
    void connectSpan(Span* prev,Span* cur);


};

