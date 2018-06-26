#ifndef SEARCHTREE_H
#define SEARCHTREE_H

#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define INF INT_MAX

#define REP(i,n) for(int i = 0,n_=(n);i < n_;++i)
#define FOR(i,a,b) for(int i = a,n_=(b)+1;i < n_;++i)
#define FORD(i,b,a) for(int i = b,n_=(a);i > n_;++i)

/**
 * \brief Esta estructura guardar&aacute; todos los datos que necesitamos
 * para crear nuestro arbol.
 * \todo Para poder calcular los pesos sera necesario tener una
 * estructura lombriz que guarde todas las coordenadas que componen
 * a la lombriz.
 */
struct AdjListNode{
    int dest;
    int weight;
    std::vector<float> vectorScores;
    class Lombris *ptrLombris;
    // La siguiente informacion se encontrara dentro de la estructura Lombris
    //int frameInitial;//!< Guardamos el frame donde inicia
    //int frameFinal;//!< Guardamos el frame donde termina
    //int keypointId;//!< Guardamos el identificador de la lombriz
    //float xField;//!< x_field del frame inicial
    //float yField;//!< y_field del frame inicial
    //float xOriginal;//!< x_original del frame inicial
    //float yOriginal;//!< x_original del frame inicial
    struct AdjListNode* next;
};

struct AdjList{
    struct AdjListNode *head;
    int numberOfAdjNodes;
};

struct Graph{
    int V;
    //std::vector<struct AdjList> vectorAdj;
    struct AdjList *array;
};

struct MinHeapNode{
    int  v; //!< Identificador del nodo actual.
    int dist; //!< Valor acumulado al nodo actual.
};

struct MinHeap{
    int size;      //!< Número actual de nodos en el monticulo.
    int capacity;  //!< Capacidad de nodos en el monticulo.
    int *pos;      //!< Necesario para el descremento de llaves, aqui guardamos las posiciones.
    struct MinHeapNode **array; //!< Arreglo de nodos del monticulo.
};


class Lombris{
public:
    Lombris();
    Lombris(unsigned long int frameInitial_,float xField_,float yField_,float xOriginal_,float yOriginal_);
    ~Lombris();
    int numberOfVertexes;
    bool addPositionsOnFrame(float xField_,float yField_,float xOriginal_,float yOriginal_);
    unsigned long int frameInitial;//!< Guardamos el frame donde inicia
    unsigned long int frameFinal;//!< Guardamos el frame donde termina
    unsigned long int keypointId;//!< Guardamos el identificador de la lombriz
    std::vector<float> xField;//!< x_field del frame i
    std::vector<float> yField;//!< y_field del frame i
    std::vector<float> xOriginal;//!< x_original del frame i
    std::vector<float> yOriginal;//!< y_original del frame i
    int getNumberOfVertexes() const;
    void setNumberOfVertexes(int value);
    unsigned long getFrameInitial() const;
    void setFrameInitial(unsigned long value);
    unsigned long getFrameFinal() const;
    void setFrameFinal(unsigned long value);
    unsigned long getKeypointId() const;
    void setKeypointId(unsigned long value);
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

class searchtree{
public:
    searchtree();
    ~searchtree();
    searchtree(Node*);
    unsigned long int memoryRamBytes;
    Graph *createGraph(int V);
    AdjListNode *newAdjListNode(int dest, int weight);
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
    void dijkstra(Graph *graph, int src);
    /// Getter Setter
    unsigned long getMemoryRamBytes() const;
    void setMemoryRamBytes(unsigned long value);
    void addMemoryRamBytes(unsigned long value);
    void test_01();
    void test_02();
    void test_03();
    void printGraphAdjs(Graph *graph);
    void addEdge(std::vector<int> adj[], int u, int v);
};

/*class CGraphDirected{
    int V; //!< Numero de vertices actual
    //list< pair<int, int> > *adj;
public:
    Graph(int V);
    void addEdge(int u, int v, int w);
    void shortestPath(int s);
};*/

#endif // SEARCHTREE_H
