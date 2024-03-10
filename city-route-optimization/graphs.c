/* SERB Razvan-Gabriel -> 311CB */
#include "graphs.h"

int main(int argc, char const *argv[]) {
    FILE *fin = fopen("tema3.in", "rt");
    if (!fin) exit(-1);
    FILE *fout = fopen("tema3.out", "wt");
    if (!fout) exit(-1);
    FILE *fintable = fopen("tema3.in", "rt");
    if (!fintable) exit(-1);
    int N = 0;
    fscanf(fintable, "%d", &N);
    char **corespondence = CreateCorespondenceTable(fintable, N);
    if (!corespondence) exit(-1);
    // ListCorespondenceTable(corespondence, N, fout);
    fclose(fintable);
    if (!strcmp(argv[1], "1")) {
        TGraph *graph = ReadUndirectedGraph(fin, corespondence);
        if (!graph) exit(-1);
        // PrintListsAdjGraph(graph, fout);
        int num_related_components = NumRelatedComponents(graph);
        fprintf(fout, "%d\n", num_related_components);
        int *v_cost_prim = CostTreePrimRelatedComponents(graph, num_related_components);
        int i = 0;
        for (i = 0; i < num_related_components; i++)
            fprintf(fout, "%d\n", v_cost_prim[i]);
        free(v_cost_prim);
        DestroyGraph(&graph, N);
    }
    if (!strcmp(argv[1], "2")) {
        TGraph *graph = ReadDirectedGraph(fin, corespondence);
        if (!graph) exit(-1);
        // PrintListsAdjGraph(graph, fout);
        int *depth = ReadDepthVector(fin, N, corespondence);
        int G = 0; fscanf(fin, "%d", &G);
        RoadTreasure(graph, depth, G, corespondence, fout);
        free(depth);
        DestroyGraph(&graph, N);
    }
    DestroyCorespondenceTable(corespondence, N);
    fclose(fin);
    fclose(fout);
    return 0;
}
