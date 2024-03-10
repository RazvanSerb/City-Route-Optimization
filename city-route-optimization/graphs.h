/* SERB Razvan-Gabriel -> 311CB */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/* *TListAdj: Structura folosita cu scopul de a retine lista de adiacenta pentru un node */
typedef struct cellListAdj {
    int destination;
    int cost;
    struct cellListAdj *next;
} TCellListAdj, *TListAdj;

/* *TGraph: Structura folosita pentru a retine un graph sub forma de liste de adiacenta */
typedef struct graph {
    int num_nodes;
    TListAdj* la;
} TGraph;

/* *TQueue: Lista dublu inlantuita circulara cu santinela folosita pentru coada */
typedef struct cellQueue {
    int info;
    struct cellQueue *prev;
    struct cellQueue *next;
} TCellQueue, *TQueue;

char** InitializeCorespondenceTable(int N);
int AssociatedNodeForString(char** corespondence, int N, char* sir);
char** CreateCorespondenceTable(FILE* fin, int N);
void PrintCorespondenceTable(char** corespondence, int N, FILE* fout);
void DestroyCorespondenceTable(char** corespondence, int N);
TListAdj AllocCellListAdj(int destination, int cost);
void AddFinalListAdj(TListAdj* list, int destination, int cost);
void DestroyListAdj(TListAdj* list);
TGraph* ReadUndirectedGraph(FILE* fin, char** corespondence);
TGraph* ReadDirectedGraph(FILE* fin, char** corespondence);
void PrintListsAdjGraph(TGraph* graph, FILE* fout);
void DestroyGraph(TGraph** graph, int N);
int* ReadDepthVector(FILE* fin, int N, char** corespondence);
TQueue InitializeQueue();
int InsertQueue(TQueue queue, int info);
int ExtractQueue(TQueue queue, int *info);
void PrintQueue(TQueue queue, FILE *fout);
void DestroyQueue(TQueue* queue);
void BFS(TGraph* graph, int node, int* visited);
int NumRelatedComponents(TGraph* graph);
int UnvisitedNodeMinimumCost(int* visited, int* distante, int num_nodes);
int CostTreePrim(TGraph* graph, int node_start);
int* CostTreePrimRelatedComponents(TGraph* graph, int num_related_components);
int UnvisitedNodeMinimumScore(int* visited, float* score, int num_nodes);
TListAdj* RoadsDijkstra(TGraph* graph, int* depth, int node_start);
void RoadTreasure(TGraph* graph, int* depth, int G, char** corespondence, FILE* fout);
