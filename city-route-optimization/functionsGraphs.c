/* SERB Razvan-Gabriel -> 311CB */
#include "graphs.h"
#define MAXI 101

/* InitializeCorespondenceTable:
- initializeaza o matrice de caractere
- aceasta are rolul de a asocia fiecarui "string de pe linie" ca si corepondent "numarul liniei" */
char** InitializeCorespondenceTable(int N) {
    char** corespondence = (char **)malloc((N + 1) * sizeof(char*));
    if (!corespondence)
        return NULL;
    int i = 0;
    corespondence[i] = NULL;
    for (i = 1; i < N + 1; i++) {
        corespondence[i] = (char*)malloc(MAXI * sizeof(char));
        if (!corespondence[i]) {
            int j = 0;
            for (j = 1; j < i; j++)
                free(corespondence[j]);
            free(corespondence);
            return NULL;
        }
        snprintf(corespondence[i], strlen("NOTHING") + 1, "%s", "NOTHING");
    }
    return corespondence;
}

/* AssociatedNodeForString:
- daca string-ul "sir" exista in tabelul de corespondenta, se intoarce corespondentul sau
- daca string-ul "sir" nu exista in tabelul de corespondenta, intoarce 0 */
int AssociatedNodeForString(char **corespondence, int N, char *sir) {
    int i = 0;
    for (i = 1; i < N + 1; i++)
        if (!strcmp(corespondence[i], sir))
            return i;
    return 0;
}

/* CreateCorespondenceTable:
- tabel de corespondence
- aceasta are rolul de a asocia fiecarui "string de pe linie" ca si corepondent "numarul liniei" */
char** CreateCorespondenceTable(FILE *fin, int N) {
    char** corespondence = InitializeCorespondenceTable(N);
    if (!corespondence)
        return NULL;
    int M = 0;
    fscanf(fin, "%d", &M);
    char sir[MAXI];
    int cost = 0;
    int index = 0;
    int i = 0;
    for (i = 0; i < M; i++) {
        fscanf(fin, "%s", sir);
        if (!AssociatedNodeForString(corespondence, N, sir)) {
            index++;
            snprintf(corespondence[index], strlen(sir) + 1, "%s", sir);
        }
        fscanf(fin, "%s", sir);
        if (!AssociatedNodeForString(corespondence, N, sir)) {
            index++;
            snprintf(corespondence[index], strlen(sir) + 1, "%s", sir);
        }
        fscanf(fin, "%d", &cost);
    }
    return corespondence;
}

/* PrintCorespondenceTable:
- afisare tabelul de corespondenta */
void PrintCorespondenceTable(char **corespondence, int N, FILE *fout) {
    int i = 0;
    for (i = 1; i < N + 1; i++)
        fprintf(fout, "%d %s\n", i, corespondence[i]);
}

/* DestroyCorespondenceTable:
- distruge tabelul de corespondenta */
void DestroyCorespondenceTable(char **corespondence, int N) {
    if (!corespondence) return;
    int i = 0;
    for (i = 0; i < N + 1; i++)
        free(corespondence[i]);
    free(corespondence);
}

/* AllocCellListAdj:
- aloca spatiu pentru o celula dintr-o lista de adiacenta a unui nod */
TListAdj AllocCellListAdj(int destination, int cost) {
    TListAdj aux = (TListAdj)malloc(sizeof(TCellListAdj));
    if (!aux)
        return NULL;
    aux->destination = destination;
    aux->cost = cost;
    aux->next = NULL;
    return aux;
}

/* AddFinalListAdj:
- adauga un nod la final in lista de adiacenta */
void AddFinalListAdj(TListAdj *list, int destination, int cost) {
    if (!(*list)) {
        *list = AllocCellListAdj(destination, cost);
        return;
    }
    TListAdj p = *list;
    for (p = p; p->next != NULL; p = p->next)
        continue;
    p->next = AllocCellListAdj(destination, cost);
}

/* DestroyListAdj:
- distruge lista de adiacenta a unui node */
void DestroyListAdj(TListAdj *list) {
    if (!(*list)) return;
    TListAdj p = *list, aux = NULL;
    while (p != NULL) {
        aux = p;
        p = p->next;
        free(aux);
    }
    *list = NULL;
}

/* ReadUndirectedGraph:
- citeste lista muchiilor pentru un graf neorientat si creeaza listele de adiacenta pentru el */
TGraph* ReadUndirectedGraph(FILE *fin, char **corespondence) {
    TGraph *graph = (TGraph *)malloc(sizeof(TGraph));
    if (!graph)
        return NULL;
    int N = 0, M = 0;
    fscanf(fin, "%d", &N);
    graph->num_nodes = N;
    graph->la = (TListAdj *)malloc((N + 1) * sizeof(TListAdj));
    if (!(graph->la))
        return NULL;
    int i = 0;
    for (i = 0; i < N + 1; i++)
        graph->la[i] = NULL;
    fscanf(fin, "%d", &M);
    char sir[MAXI];
    int cost = 0;
    for (i = 0; i < M; i++) {
        fscanf(fin, "%s", sir);
        int source = AssociatedNodeForString(corespondence, N, sir);
        fscanf(fin, "%s", sir);
        int destination = AssociatedNodeForString(corespondence, N, sir);
        fscanf(fin, "%d", &cost);
        AddFinalListAdj(&(graph->la[source]), destination, cost);
        AddFinalListAdj(&(graph->la[destination]), source, cost);
    }
    return graph;
}

/* ReadDirectedGraph:
- citeste lista muchiilor pentru un graf orientat si creeaza listele de adiacenta pentru el */
TGraph* ReadDirectedGraph(FILE *fin, char **corespondence) {
    TGraph *graph = (TGraph *)malloc(sizeof(TGraph));
    if (!graph)
        return NULL;
    int N = 0, M = 0;
    fscanf(fin, "%d", &N);
    graph->num_nodes = N;
    graph->la = (TListAdj *)malloc((N + 1) * sizeof(TListAdj));
    if (!(graph->la))
        return NULL;
    int i = 0;
    for (i = 0; i < N + 1; i++)
        graph->la[i] = NULL;
    fscanf(fin, "%d", &M);
    char sir[MAXI];
    int cost = 0;
    for (i = 0; i < M; i++) {
        fscanf(fin, "%s", sir);
        int source = AssociatedNodeForString(corespondence, N, sir);
        fscanf(fin, "%s", sir);
        int destination = AssociatedNodeForString(corespondence, N, sir);
        fscanf(fin, "%d", &cost);
        AddFinalListAdj(&(graph->la[source]), destination, cost);
    }
    return graph;
}

/* PrintListsAdjGraph:
- afiseaza listele de adiacenta pentru un graf */
void PrintListsAdjGraph(TGraph *graph, FILE *fout) {
    int i = 0;
    for (i = 1; i < graph->num_nodes + 1; i++) {
        fprintf(fout, "%d: ", i);
        TListAdj p = graph->la[i];
        for (p = p; p != NULL; p = p->next)
            fprintf(fout, "(%d, %d) ", p->destination, p->cost);
        fprintf(fout, "\n");
    }
}

/* DestroyGraph:
- distruge un graf */
void DestroyGraph(TGraph **graph, int N) {
    if (!(*graph)) return;
    int i = 0;
    for (i = 0; i < N + 1; i++)
        DestroyListAdj(&((*graph)->la[i]));
    free((*graph)->la);
    free(*graph);
    *graph = NULL;
}

/* ReadDepthVector:
- citeste vectorul de adancimi ale nodurilor, prezentat in cerinta */
int* ReadDepthVector(FILE *fin, int N, char **corespondence) {
    int *depth = (int *)malloc((N + 1) * sizeof(int));
    char sir[MAXI];
    int i = 0; depth[0] = 0;
    for (i = 1; i < N + 1; i++) {
        fscanf(fin, "%s", sir);
        fscanf(fin, "%d", &depth[AssociatedNodeForString(corespondence, N, sir)]);
    }
    return depth;
}

/* IntializeQueue:
- functie ce initializeaza o coada
- intoarce coada daca s-a reusit initializarea sau NULL daca nu s-a reusit */
TQueue IntializeQueue() {
    TQueue queue = (TQueue) malloc(sizeof(TCellQueue));
    if (!queue)
        return NULL;
    queue->info = 0;
    queue->prev = queue;
    queue->next = queue;
    return queue;
}

/* InsertQueue:
- functie ce insereaza un pointer la un nod in coada */
int InsertQueue(TQueue queue, int info) {
    TQueue aux = (TQueue) malloc(sizeof(TCellQueue));
    if (!(aux))
        return 0;
    aux->info = info;
    aux->prev = queue->prev;
    aux->next = queue;
    queue->prev = aux;
    aux->prev->next = aux;
    return 1;
}

/* ExtractQueue:
- functie ce extrage un pointer la un nod din coada */
int ExtractQueue(TQueue queue, int *info) {
    if (queue->next == queue)
        return 0;
    *info = queue->next->info;
    TQueue aux = queue->next;
    queue->next = queue->next->next;
    queue->next->prev = queue;
    free(aux);
    return 1;
}

/* PrintQueue:
- functie ce afiseaza continutul din coada */
void PrintQueue(TQueue queue, FILE *fout) {
    fprintf(fout, "COADA: ");
    TQueue sentinel = queue;
    for (queue = queue->next; queue != sentinel; queue = queue->next)
        fprintf(fout, "%d ", queue->info);
    fprintf(fout, "\n");
}

/* DestroyQueue:
- functie ce distruge o coada */
void DestroyQueue(TQueue *queue) {
    if (!(*queue))
        return;
    TQueue sentinel = (*queue);
    (*queue) = (*queue)->next;
    while ((*queue) != sentinel) {
        TQueue aux = *queue;
        (*queue) = (*queue)->next;
        free(aux);
    }
    free(sentinel);
    *queue = NULL;
}

/* BFS:
- algoritmul BFS de parcurgere al unui graf */
void BFS(TGraph *graph, int node, int *visited) {
    TQueue queue = IntializeQueue();
    if (!queue) exit(-1);
    if (!InsertQueue(queue, node)) exit(-1);
    while (ExtractQueue(queue, &node)) {
        visited[node] = 1;
        TListAdj p = graph->la[node];
        for (p = p; p != NULL; p = p->next)
            if (!visited[p->destination]) {
                if (!InsertQueue(queue, p->destination)) exit(-1);
                continue;
            }
    }
    DestroyQueue(&queue);
}

/* NumRelatedComponents:
- functie ce returneaza numarul de componente conexe ale unui graph */
int NumRelatedComponents(TGraph *graph) {
    int num_related_components = 0;
    int *visited = (int*)malloc((graph->num_nodes + 1) * sizeof(int));
    int i = 0;
    for (i = 0; i < graph->num_nodes + 1; i++)
        visited[i] = 0;
    for (i = 1; i < graph->num_nodes + 1; i++)
        if (!visited[i]) {
            num_related_components++;
            BFS(graph, i, visited);
        }
    free(visited);
    return num_related_components;
}

/* UnvisitedNodeMinimumCost:
- functie ce returneaza nodul nevizitat de cost minim */
int UnvisitedNodeMinimumCost(int *visited, int *distante, int num_nodes) {
    int node_minimum_cost = 0;
    int minimum_cost = INT_MAX;
    int i = 0;
    for (i = 1; i < num_nodes + 1; i++)
        if (visited[i] == 0 && (distante[i] < minimum_cost)) {
            node_minimum_cost = i;
            minimum_cost = distante[i];
        }
    return node_minimum_cost;
}

/* CostTreePrim:
- functie ce returneaza costul APCM Prim aplicat unui graph conex */
int CostTreePrim(TGraph *graph, int start_node) {
    int *visited = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    int *distante = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    int i = 0;
    for (i = 0; i < graph->num_nodes + 1; i++) {
        visited[i] = 0;
        distante[i] = INT_MAX;
    }
    distante[start_node] = 0;
    for (i = 1; i < graph->num_nodes + 1; i++) {
        int node = UnvisitedNodeMinimumCost(visited, distante, graph->num_nodes);
        if (node == 0) break;
        visited[node] = 1;
        TListAdj p = graph->la[node];
        for (p = p; p != NULL; p = p->next)
            if (visited[p->destination] == 0 && (p->cost < distante[p->destination]))
                distante[p->destination] = p->cost;
    }
    int cost_prim = 0;
    for (i = 1; i < graph->num_nodes + 1; i++)
        if (distante[i] != INT_MAX)
            cost_prim = cost_prim + distante[i];
    free(visited);
    free(distante);
    return cost_prim;
}

int compareInt(const void* a, const void* b) {
    return ((*(int*)a) - (*(int*)b));
}

/* CostTreePrimRelatedComponents:
- functie ce returneaza costul APCM Prim pentru fiecare componenta conexa a unui graph */
int* CostTreePrimRelatedComponents(TGraph *graph, int num_related_components) {
    int cnt = 0;
    int *v_cost_prim = (int *)malloc((num_related_components + 1) * sizeof(int));
    int *visited = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    int i = 0;
    for (i = 0; i < graph->num_nodes + 1; i++)
        visited[i] = 0;
    for (i = 1; i < graph->num_nodes + 1; i++)
        if (!visited[i]) {
            v_cost_prim[cnt++] = CostTreePrim(graph, i);
            BFS(graph, i, visited);
        }
    qsort(v_cost_prim, cnt, sizeof(int), compareInt);
    free(visited);
    return v_cost_prim;
}

/* UnvisitedNodeMinimumScore:
- functie ce returneaza nodul nevizitat de scor minim */
int UnvisitedNodeMinimumScore(int *visited, float *score, int num_nodes) {
    int node_minimum_score = 0;
    float scor_minim = INT_MAX;
    int i = 0;
    for (i = 1; i < num_nodes + 1; i++)
        if (visited[i] == 0 && (score[i] < scor_minim)) {
            node_minimum_score = i;
            scor_minim = score[i];
        }
    return node_minimum_score;
}

/* RoadsDijkstra:
- functie ce retutneaza o list de liste
- listele sunt drumuri de la nodul de start la toate celelalte noduri (obtinute din Dijkstra) */
TListAdj* RoadsDijkstra(TGraph *graph, int *depth, int start_node) {
    int *visited = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    float *score = (float *)malloc((graph->num_nodes + 1) * sizeof(float));
    TListAdj *roads = (TListAdj *)malloc((graph->num_nodes + 1) * sizeof(TListAdj));
    int i = 0;
    for (i = 0; i < graph->num_nodes + 1; i++) {
        visited[i] = 0;
        score[i] = INT_MAX;
        roads[i] = NULL;
        if (i) AddFinalListAdj(&(roads[i]), start_node, 0);
    }
    score[start_node] = 0;
    for (i = 1; i < graph->num_nodes + 1; i++) {
        int node = UnvisitedNodeMinimumScore(visited, score, graph->num_nodes);
        visited[node] = 1;
        TListAdj p = graph->la[node];
        for (p = p; p != NULL; p = p->next) {
            float neighbour_score = (float) p->cost / (float) depth[p->destination];
            if (visited[p->destination] == 0 && (score[node] + neighbour_score < score[p->destination])) {
                DestroyListAdj(&(roads[p->destination]));
                TListAdj q = roads[node];
                for (q = q; q != NULL; q = q->next)
                    AddFinalListAdj(&(roads[p->destination]), q->destination, q->cost);
                AddFinalListAdj(&(roads[p->destination]), p->destination, p->cost);
                score[p->destination] = score[node] + neighbour_score;
            }
        }
    }
    free(visited);
    free(score);
    return roads;
}

/* RoadTreasure:
- functie ce afiseaza drumul comorii */
void RoadTreasure(TGraph* graph, int* depth, int G, char** corespondence, FILE* fout) {
    int start_node = AssociatedNodeForString(corespondence, graph->num_nodes, "Insula");
    int finish_node = AssociatedNodeForString(corespondence, graph->num_nodes, "Corabie");
    int *visited = (int *)malloc((graph->num_nodes + 1) * sizeof(int));
    int i = 0;
    for (i = 0; i < graph->num_nodes + 1; i++)
        visited[i] = 0;
    BFS(graph, finish_node, visited);
    if (!visited[start_node]) {
        fprintf(fout, "Echipajul nu poate ajunge la insula");
        free(visited);
        return;
    }
    for (i = 0; i < graph->num_nodes + 1; i++)
        visited[i] = 0;
    BFS(graph, start_node, visited);
    if (!visited[finish_node]) {
        fprintf(fout, "Echipajul nu poate transporta comoara inapoi la corabie");
        free(visited);
        return;
    }
    free(visited);
    TListAdj* roads = RoadsDijkstra(graph, depth, start_node);
    int cost_route = 0;
    int minimum_depth = INT_MAX;
    TListAdj p = roads[finish_node];
    for (p = p; p != NULL; p = p->next) {
        fprintf(fout, "%s ", corespondence[p->destination]);
        cost_route = cost_route + p->cost;
        if (depth[p->destination] != 1 && depth[p->destination] < minimum_depth)
            minimum_depth = depth[p->destination];
    }
    fprintf(fout, "\n");
    fprintf(fout, "%d\n", cost_route);
    fprintf(fout, "%d\n", minimum_depth);
    fprintf(fout, "%d\n", G / minimum_depth);
    for (i = 0; i < graph->num_nodes + 1; i++)
        DestroyListAdj(&(roads[i]));
    free(roads);
}
