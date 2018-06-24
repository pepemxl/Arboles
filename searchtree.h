#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <limits.h>

#define REP(i,n) for(int i = 0,n_=(n);i < n_;++i)
#define FOR(i,a,b) for(int i = a,n_=(b)+1;i < n_;++i)
#define FORD(i,b,a) for(int i = b,n_=(a);i > n_;++i)

struct AdjListNode{
    int dest;
    int weight;
    struct AdjListNode* next;
};

struct AdjList{
    struct AdjListNode *head;
};

struct Graph{
    int V;
    struct AdjList *array;
};

struct MinHeapNode{
    int  v;
    int dist;
};

struct MinHeap{
    int size;      //!< Number of heap nodes present currently.
    int capacity;  //!< Capacity of min heap.
    int *pos;      //!< This is needed for decreaseKey().
    struct MinHeapNode **array;
};


class Lombris{
public:
    Lombris(){}
    ~Lombris(){}
};

class Node{
public:
    Node(){}
    ~Node(){}
    int initialFrame,finalFrame;
    double x_field,y_field;
    double x_original,y_original;
    std::vector<Node*> vectorNodes;
    std::vector<float> vectorScores;
    float getScore(Node*);
private:
};

class searchtree
{
public:
    searchtree();
    ~searchtree();
    searchtree(Node*);

    AdjListNode *newAdjListNode(int dest, int weight);
    Graph *createGraph(int V);
    void addEdge(Graph *graph, int src, int dest, int weight);
    MinHeapNode *newMinHeapNode(int v, int dist);
    MinHeap *createMinHeap(int capacity);
    void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b);
    void minHeapify(MinHeap *minHeap, int idx);
    MinHeapNode *extractMin(MinHeap *minHeap);
    bool isInMinHeap(MinHeap *minHeap, int v);
    int isEmpty(MinHeap *minHeap);
    void decreaseKey(MinHeap *minHeap, int v, int dist);
    void printArr(int dist[], int n);
    void test_01();
    void dijkstra(Graph *graph, int src);
};

#endif // SEARCHTREE_H
