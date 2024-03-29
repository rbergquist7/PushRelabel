#include <stdlib.h>
#include <stdio.h>

int NODES = 6;
int INFINITE = 999;

int min(int a, int b) {
    return a<b ? a : b;
}
void push(int **C, int ** F, int *excess, int u, int v) {
  	int send = min(excess[u], C[u][v] - F[u][v]);
  	F[u][v] += send;
  	F[v][u] -= send;
  	excess[u] -= send;
  	excess[v] += send;
}
 
void relabel(int **C,int **F, int *height, int u) {
  	int v;
  	int min_height = INFINITE;
  	for (v = 0; v < NODES; v++) {
    		if (C[u][v] - F[u][v] > 0) {
      			min_height = min(min_height, height[v]);
      			height[u] = min_height + 1;
    		}
  	}
}
 
void discharge(int **C, int ** F, int *excess, int *height, int *seen, int u) {
  	while (excess[u] > 0) {
    		if (seen[u] < NODES) {
      			int v = seen[u];
      			if ((C[u][v] - F[u][v] > 0) && (height[u] > height[v])){
    				push(C, F, excess, u, v);
      			}
      		else
    			seen[u] += 1;
    		} else {
      			relabel(C, F, height, u);
      			seen[u] = 0;
    		}
  	}
}
 
void moveToFront(int i, int *A) {
  	int temp = A[i];
  	int n;
  	for (n = i; n > 0; n--){
    		A[n] = A[n-1];
  	}
  	A[0] = temp;
}
 
int pushRelabel(int **C, int ** F, int source, int sink) {
  	int *excess, *height, *list, *seen, i, p;
 
  	excess = (int *) calloc(NODES, sizeof(int));
  	height = (int *) calloc(NODES, sizeof(int));
  	seen = (int *) calloc(NODES, sizeof(int));
 
  	list = (int *) calloc((NODES-2), sizeof(int));
 
  	for (i = 0, p = 0; i < NODES; i++){
    		if((i != source) && (i != sink)) {
      			list[p] = i;
      			p++;
    		}
  	}
 
  	height[source] = NODES;
  	excess[source] = INFINITE;
  	for (i = 0; i < NODES; i++)
    		push(C, F, excess, source, i);
 
  	p = 0;
  	while (p < NODES - 2) {
    		int u = list[p];
    		int old_height = height[u];
    		discharge(C, F, excess, height, seen, u);
    		if (height[u] > old_height) {
      			moveToFront(p,list);
      			p=0;
    		}
    		else
      			p += 1;
  	}
  	int maxflow = 0;
  	for (i = 0; i < NODES; i++)
    		maxflow += F[source][i];
 
  	free(list);
 
  	free(seen);
  	free(height);
  	free(excess);
 
  	return maxflow;
}
 
void printMatrix(int ** M) {
  	int i,j;
  	for (i = 0; i < NODES; i++) {
    		for (j = 0; j < NODES; j++)
      			printf("%d\t",M[i][j]);
    			printf("\n");
  	}
}
 
int main(void) {
  	int **flow, **capacities, i;
  	flow = (int **) calloc(NODES, sizeof(int*));
  	capacities = (int **) calloc(NODES, sizeof(int*));
  	for (i = 0; i < NODES; i++) {
    		flow[i] = (int *) calloc(NODES, sizeof(int));
    		capacities[i] = (int *) calloc(NODES, sizeof(int));
  	}

  	capacities[0][1] = 3;
  	capacities[0][2] = 3;
  	capacities[0][3] = 2;
  	capacities[1][4] = 4;
  	capacities[2][3] = 1;
  	capacities[2][5] = 2;
  	capacities[3][5] = 2;
  	capacities[4][3] = 1;  
  	capacities[4][5] = 1;
  	printf("Capacity:\n");
  	printMatrix(capacities);
 
  	printf("Max Flow:\n%d\n", pushRelabel(capacities, flow, 0, 5));
 
  	printf("Flows:\n");
  	printMatrix(flow);
 
  return 0;
}
