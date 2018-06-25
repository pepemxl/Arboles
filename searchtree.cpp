#include "searchtree.h"

searchtree::searchtree()
{
    this->setMemoryRamBytes(0);
}

searchtree::~searchtree()
{

}

unsigned long searchtree::getMemoryRamBytes() const
{
    return memoryRamBytes;
}

void searchtree::setMemoryRamBytes(unsigned long value)
{
    memoryRamBytes = value;
}

void searchtree::addMemoryRamBytes(unsigned long value)
{
    memoryRamBytes += value;
    std::cout << "Current Memory: " << memoryRamBytes << " Bytes" << std::endl;
}

/**
 * \brief Función que crea un nuevo nodo(adjacensy).
 * \todo Pasar la lista dinámica a una estructura STL(vector).
*/
struct AdjListNode* searchtree::newAdjListNode(int dest, int weight){
    struct AdjListNode* newNode = (struct AdjListNode*)malloc(sizeof(struct AdjListNode));
    newNode->dest   = dest;
    newNode->weight = weight;
    newNode->next   = NULL;
    this->addMemoryRamBytes(sizeof(newNode));
    this->addMemoryRamBytes(sizeof(struct AdjListNode));
    return newNode;
}

/**
 * \brief Función que crea un grafo con V nodos.
 * Creamos un arreglo con $V$ listas de relaciones entre nodos, comunmente llamadas adyacencias(adjacensy).
 * Inicializamos cada una de las listas de relaciones al conjunto vacio.
 * \todo Pensar si es mejor meter el arreglo de listas a un std::vector.
*/
struct Graph* searchtree::createGraph(int n){
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = n;
    graph->array = (struct AdjList*)malloc(n*sizeof(struct AdjList));
    this->addMemoryRamBytes(sizeof(graph->array));
    this->addMemoryRamBytes(n*sizeof(struct AdjList));
    REP(i,n){
        graph->array[i].head = NULL;
    }
    return graph;
}

/**
 * \brief Función que agrega una arista a un grafo no dirigido.
 * La arista va del nodo $src$ al nodo $dest$.
 *
 */
void searchtree::addEdge(struct Graph* graph, int src, int dest, int weight){
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    //!<  Las siguientes lineas solo se agregan cuando el grafo no es dirigido se agrega tambien una arista de dest a src.
//    newNode = newAdjListNode(src, weight);
//    newNode->next = graph->array[dest].head;
//    graph->array[dest].head = newNode;
}


/**
 * \brief Función para crear un Nodo de "costo minimo".
 */
struct MinHeapNode* searchtree::newMinHeapNode(int v, int dist){
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    this->addMemoryRamBytes(sizeof(minHeapNode));
    this->addMemoryRamBytes(sizeof(struct MinHeapNode));
    return minHeapNode;
}

/**
 * \brief A utility function to create a Min Heap.
 */
struct MinHeap* searchtree::createMinHeap(int capacity){
    struct MinHeap* minHeap = (struct MinHeap*)malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    this->addMemoryRamBytes(sizeof(minHeap));
    this->addMemoryRamBytes(sizeof(struct MinHeap));
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    this->addMemoryRamBytes(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

/**
 * \brief Función para realizar el intercambio en dos nodos de min heap.
 */
void searchtree::swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

/**
 * \brief A standard function to heapify at given idx
 * This function also updates position of nodes when they are swapped.
 * Position is needed for decreaseKey()
 */
void searchtree::minHeapify(struct MinHeap* minHeap, int idx){
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist < minHeap->array[smallest]->dist )
      smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist < minHeap->array[smallest]->dist )
      smallest = right;

    if(smallest != idx){
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

/**
 * \brief Función para revisar si minHeap es vacio o no.
 *
 */
int searchtree::isEmpty(struct MinHeap* minHeap){
    return minHeap->size == 0;
}

/**
 * \brief Función para extraer el nodo minimo de heap.
 *
 */
struct MinHeapNode* searchtree::extractMin(struct MinHeap* minHeap){
    if(isEmpty(minHeap)){
        return NULL;
    }
    struct MinHeapNode* root = minHeap->array[0];
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

void searchtree::decreaseKey(struct MinHeap* minHeap, int v, int dist){
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while(i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool searchtree::isInMinHeap(struct MinHeap *minHeap, int v){
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

void searchtree::printArr(int dist[], int n){
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}

/**
 * \brief La función Dijkstra calcula la distancia más corta
 * de $src$ a cualquier otro vertice, es de orden
 *            $O(E\cdot Log V)$.
 */
void searchtree::dijkstra(struct Graph* graph, int src){
    int V = graph->V; //!< Obtenemos el numero de vertices en el grafo
    int dist[V]; //!< dist values used to pick minimum weight edge in cut
    struct MinHeap* minHeap = createMinHeap(V);
    REP(i,V){
        dist[i] = INT_MAX;
        minHeap->array[i] = newMinHeapNode(i, dist[i]);
        minHeap->pos[i] = i;
    }
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;
    while (!isEmpty(minHeap)){
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL){
            int v = pCrawl->dest;
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v]){
                dist[v] = dist[u] + pCrawl->weight;
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    printArr(dist, V);
}


void searchtree::test_01(){
    int V = 9;
    struct Graph* graph = this->createGraph(V);
    this->addEdge(graph, 0, 1, 4);
    this->addEdge(graph, 0, 7, 8);
    this->addEdge(graph, 1, 2, 8);
    this->addEdge(graph, 1, 7, 11);
    this->addEdge(graph, 2, 3, 7);
    this->addEdge(graph, 2, 8, 2);
    this->addEdge(graph, 2, 5, 4);
    this->addEdge(graph, 3, 4, 9);
    this->addEdge(graph, 3, 5, 14);
    this->addEdge(graph, 4, 5, 10);
    this->addEdge(graph, 5, 6, 2);
    this->addEdge(graph, 6, 7, 1);
    this->addEdge(graph, 6, 8, 6);
    this->addEdge(graph, 7, 8, 7);

    this->dijkstra(graph, 0);
    //this->dijkstra(graph, 1);
    //this->dijkstra(graph, 2);
    //this->dijkstra(graph, 3);
}

int Lombris::getNumberOfVertexes() const
{
    return numberOfVertexes;
}

void Lombris::setNumberOfVertexes(int value)
{
    numberOfVertexes = value;
    this->setFrameFinal(this->getFrameFinal()+value);
}

Lombris::Lombris(){
    this->setNumberOfVertexes(0);
}

Lombris::Lombris(unsigned long frameInitial_, float xField_, float yField_, float xOriginal_, float yOriginal_)
{
    this->setFrameFinal(frameInitial_);
    this->addPositionsOnFrame(xField_,yField_,xOriginal_,yOriginal_);
}

Lombris::~Lombris(){

}

bool Lombris::addPositionsOnFrame(float xField_,float yField_,float xOriginal_,float yOriginal_){
    this->xField.push_back(xField_);
    this->yField.push_back(yField_);
    this->xOriginal.push_back(xOriginal_);
    this->yOriginal.push_back(yOriginal_);
    this->setNumberOfVertexes(this->getNumberOfVertexes()+1);
    this->setFrameFinal(this->getFrameInitial()+this->getNumberOfVertexes());
}

unsigned long Lombris::getKeypointId() const
{
    return keypointId;
}

void Lombris::setKeypointId(unsigned long value)
{
    keypointId = value;
}

unsigned long Lombris::getFrameFinal() const
{
    return frameFinal;
}

void Lombris::setFrameFinal(unsigned long value)
{
    frameFinal = value;
}

unsigned long Lombris::getFrameInitial() const
{
    return frameInitial;
}

void Lombris::setFrameInitial(unsigned long value)
{
    frameInitial = value;
}
