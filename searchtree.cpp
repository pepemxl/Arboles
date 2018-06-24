#include "searchtree.h"

searchtree::searchtree()
{

}

searchtree::~searchtree()
{

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
    return newNode;
}

/**
 * \brief Función que crea un grafo con V nodos.
 * \todo Pensar si es mejor meter el arreglo de listas a un std::vector.
*/
struct Graph* searchtree::createGraph(int V){
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->V = V;
    //!< Creamos un arreglo con $V$ listas de relaciones(adjacensy).
    graph->array = (struct AdjList*)malloc(V * sizeof(struct AdjList));
    //!< Inicializamos cada una de las listas de relaciones.
    REP(i,V){
        graph->array[i].head = NULL;
    }
    return graph;
}

/**
 * \brief Función que agrega una arista a un grafo no dirigido. Va de src a dest.
 *
 */
void searchtree::addEdge(struct Graph* graph, int src, int dest, int weight){
    //!< A new node is added to the adjacency
    //!< La estructura es de una pila.
    struct AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    //!<  Como el grafo no es dirigido se agrega tambien una arista de dest a src.
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}


/**
 * \brief Función para crear un Nodo de "costo minimo".
 */
struct MinHeapNode* searchtree::newMinHeapNode(int v, int dist){
    struct MinHeapNode* minHeapNode = (struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
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
    minHeap->array = (struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}

/**
 * \brief A utility function to swap two nodes of min heap. Needed for min heapify.
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

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *idxNode = minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

/**
 * \brief A utility function to check if the given minHeap is ampty or not.
 */
int searchtree::isEmpty(struct MinHeap* minHeap){
    return minHeap->size == 0;
}

/**
 * \brief Standard function to extract minimum node from heap.
 *
 */
struct MinHeapNode* searchtree::extractMin(struct MinHeap* minHeap){
    if(isEmpty(minHeap)){
        return NULL;
    }
    // Store the root node
    struct MinHeapNode* root = minHeap->array[0];
    // Replace root node with last node
    struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;
    // Update position of last node
    minHeap->pos[root->v] = minHeap->size-1;
    minHeap->pos[lastNode->v] = 0;
    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);
    return root;
}

/**
 * \brief Function to decreasy dist value of a given vertex v. This function
 * uses pos[] of min heap to get the current index of node in min heap.
 */
void searchtree::decreaseKey(struct MinHeap* minHeap, int v, int dist){
    // Get the index of v in  heap array
    int i = minHeap->pos[v];
    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;
    // Travel up while the complete tree is not hepified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist){
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
        minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],  &minHeap->array[(i - 1) / 2]);
        // move to parent index
        i = (i - 1) / 2;
    }
}

/**
 * A utility function to check if a given vertex.
 * 'v' is in min heap or not.
 */
bool searchtree::isInMinHeap(struct MinHeap *minHeap, int v){
   if (minHeap->pos[v] < minHeap->size)
     return true;
   return false;
}

/**
 * \brief A utility function used to print the solution.
 */
void searchtree::printArr(int dist[], int n){
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}


/**
 * \brief The main function that calulates distances of shortest paths from src to all
 * vertices. It is a O(ELogV) function.
 */
void searchtree::dijkstra(struct Graph* graph, int src){
    int V = graph->V;// Get the number of vertices in graph
    int dist[V];      // dist values used to pick minimum weight edge in cut

    // minHeap represents set E
    struct MinHeap* minHeap = createMinHeap(V);

    // Initialize min heap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src]   = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the followin loop, min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with minimum distance value
        struct MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v; // Store the extracted vertex number

        // Traverse through all adjacent vertices of u (the extracted
        // vertex) and update their distance values
        struct AdjListNode* pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX &&
                                          pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances
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
