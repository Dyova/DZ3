#include <stdio.h>
#include <stdlib.h>

#define WORD_MAX_SIZE 20

typedef struct Graph {
    int** matrix;
    char** words;
} Graph_t;

void initGraph(Graph_t* graph, int size) {
    graph->matrix = (int**)malloc(size*sizeof(int *));
    graph->words = (char**)malloc(sizeof(char*)*size);

    for(int i = 0; i < size; i++) {
        graph->words[i] = (char*)malloc(WORD_MAX_SIZE*sizeof(char));
        graph->matrix[i] = (int*)calloc(size,sizeof(int));
    }
}

int fillGraph (Graph_t* graph, int size) {
    int recievingVerticesNum;
    int vertexValue;

    for (int i = 0; i < size; i++) {
        printf("Value(max is %d) of the %d vertex: ",WORD_MAX_SIZE ,i + 1);
        scanf("%s", graph->words[i]);
    }

    for (int i = 0; i < size; i++) {
        system("clear");

        for (int k = 0; k < size; k++) {
            printf("%d.%s ", k + 1, graph->words[k]);
        }

        printf("\nNumber of outgoing or linked edges of the %d.%s vertex: ", i + 1, graph->words[i]);
        scanf("%d", &recievingVerticesNum);
        
        if (recievingVerticesNum != 0) {
            printf("\nEnter the %d values of the vertices: ", recievingVerticesNum);
        }
        for (int j = 0; j < recievingVerticesNum; j++) {
            scanf("%d", &vertexValue);
            if (vertexValue > 0 && vertexValue <= size) {
                graph->matrix[i][vertexValue - 1]++;
            } 
            else if(vertexValue == 0) {
                continue;
            }
            else {
                return 1;
            }
        }
    }
    system("clear");
    return 0;
}

void drawUndirected(Graph_t* graph, int size) {
    int copyGraph[size][size];                              //copy graphMatrix 
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            copyGraph[i][j] = graph->matrix[i][j];
        }
    }

    FILE* S = fopen("Graph.dot", "w");                 //write in .dot file
    int checkUncon = 0;
    fprintf(S, "graph Hey {\n");                        
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < copyGraph[i][j]; k++) {     
                fprintf(S, "\t%s -- %s;\n", graph->words[i], graph->words[j]);
                if (i != j) {
                    copyGraph[j][i]--;
                }
            }
            if (graph->matrix[i][j] != 0) {
                checkUncon++;
            }
        }
        if (checkUncon == 0) {
            fprintf(S, "\t%s;\n", graph->words[i]);
        }
        checkUncon = 0;
    }
    fprintf(S, "}");
    fclose(S);

    system("dot -Tpng Graph.dot -o undirGraph.png");  //call system for png
}

void shellSort(int elemNum, int* sortArr) {
    int k, j;

    int* copySort = (int*)calloc(elemNum, sizeof(int));                              
    for(int i = 0; i < elemNum; i++) {
        copySort[i] = sortArr[i];
    }

    for (int step = elemNum / 2; step > 0; step /= 2) {
        for (int i = step; i < elemNum; i++) {
            k = sortArr[i];
            for ( j = i; j >= step; j -= step) {
                if (k > sortArr[j - step]) {
                    sortArr[j] = sortArr[j - step];
                }
                else {
                    break;
                }
            }
            sortArr[j] = k;
        }
    }

    printf("\nSorted: ");
    for (int i = 0; i < elemNum; i++) {
        for (int j = 0; j < elemNum; j++) {
            if (sortArr[i] == copySort[j] && sortArr[i] != sortArr[i+1]) {
                printf("%d(n=%d) ", j + 1, sortArr[i]);
            }
        }
    }
    free(copySort);
}

void sortVertices(Graph_t* graph, int size) {
    int* sortArr = (int*)calloc(size, sizeof(int)); 

    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if (graph->matrix[i][j] != 0 && i == j) {
                sortArr[i] = sortArr[i] + 2*graph->matrix[i][j];
                continue;
            }
            if (graph->matrix[i][j] != 0) {
                sortArr[i] = sortArr[i] + graph->matrix[i][j];
            }
        }
    }

    printf("Unsorted: ");
    for (int i = 0; i < size; i++) {
        printf("%d(n=%d) ", i + 1, sortArr[i]);
    }

    shellSort(size, sortArr);

    free(sortArr);
}

int main(void) {
    int size;
    printf("Vertices number: ");
    scanf("%d", &size);

    Graph_t graph;
    initGraph(&graph, size);


    if(fillGraph(&graph, size) == 1) {
        system("clear");
        printf("Wrong value");
        return 1;
    };

    system("clear");
    
    drawUndirected(&graph, size);

    printf("Matrix:\n");
    for (int j = 0; j < size; j++) {
        for(int i = 0; i < size; i++) {
            printf("%d ", graph.matrix[j][i]);
        }
        printf("\n");
    }

    printf("\n");
    sortVertices(&graph, size);

    for(int i = 0; i < size; i++) {
        free(graph.matrix[i]);
        free(graph.words[i]);
    }
    free(graph.matrix);
    free(graph.words);

    return 0;
}