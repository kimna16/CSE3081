#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int MAX_WEIGHT;
struct Edge{
	int v1, v2, weight;
};
struct Graph {
	int V, E;
	struct Edge* edge;
};
struct Subset {
	int parent, rank, numOfvertices;
	long long int sumOfweights;
};
int Find(struct Subset subsets[], int i) {
	if (subsets[i].parent != i)
		subsets[i].parent = Find(subsets, subsets[i].parent);
	return subsets[i].parent;
}
void Union(struct Subset subsets[], int x, int y, int weight) {
	int xroot = Find(subsets, x), yroot = Find(subsets, y);

	if (subsets[xroot].rank > subsets[yroot].rank) {
		subsets[yroot].parent = xroot;
		subsets[xroot].sumOfweights += weight;
		subsets[xroot].sumOfweights += subsets[yroot].sumOfweights;
		subsets[xroot].numOfvertices += subsets[yroot].numOfvertices;
	}
	else if (subsets[xroot].rank < subsets[yroot].rank) {
		subsets[xroot].parent = yroot;
		subsets[yroot].sumOfweights += weight;
		subsets[yroot].sumOfweights += subsets[xroot].sumOfweights;
		subsets[yroot].numOfvertices += subsets[xroot].numOfvertices;
	}
	else{
		subsets[xroot].parent = yroot;
		subsets[yroot].rank++;
		subsets[yroot].sumOfweights += weight;
		subsets[yroot].sumOfweights += subsets[xroot].sumOfweights;
		subsets[yroot].numOfvertices += subsets[xroot].numOfvertices;
	}
}
int compareFunc(const void* a, const void* b) {
	struct Edge* a1 = (struct Edge*) a;
	struct Edge* b1 = (struct Edge*)b;
	return a1->weight - b1->weight;
}
void adjust(struct Edge* heapEdge, int root, int n) {
	int child, rootweight;
	struct Edge temp = heapEdge[root];
	rootweight = heapEdge[root].weight;
	child = 2 * root;
	while (child <= n) {
		if ((child < n) && (heapEdge[child].weight < heapEdge[child + 1].weight)) {
			child++;
		}
		if (rootweight > heapEdge[child].weight)
			break;
		else {
			heapEdge[child / 2] = heapEdge[child];
			child *= 2;
		}
	}
	heapEdge[child / 2] = temp;
}
void heapsort(struct Edge* heapEdge, int n) {
	struct Edge temp;
	for (int i = n/2; i > 0; i--) {
		adjust(heapEdge, i, n);
	}
	for (int i = n-1; i > 0; i--) {
		temp = heapEdge[1];
		heapEdge[1] = heapEdge[i + 1];
		heapEdge[i + 1] = temp;
		adjust(heapEdge, 1, i);
	}
}
void kruskalMST(struct Graph* graph, struct Edge* heapEdge, char outputFile[]) {

	//time_t start, end;
	//start = time(NULL);
	clock_t start, end;
	start = clock();


	struct Edge* result;
	result = (struct Edge*)malloc(sizeof(struct Edge) * (graph->V - 1));
	int* connectedVertex = (int*)malloc(sizeof(int) * graph->V);
	int e = 0, i = 0;

	//qsort(graph->edge, graph->E, sizeof(graph->edge[0]), compareFunc);
	//printf("Quick sort completes\n");

	heapsort(heapEdge, graph->E);
	for (int i = 0; i < graph->E; i++) {
		graph->edge[i] = heapEdge[i + 1];
		//printf("%d %d %d\n", heapEdge[i + 1].weight, heapEdge[i + 1].v1, heapEdge[i + 1].v2);
	}
	printf("Heap sort completes\n");

	struct Subset* subsets = (struct Subset*)malloc(graph->V * (sizeof(Subset)));
	for (int v = 0; v < graph->V; v++) {
		connectedVertex[v] = 0;
		subsets[v].parent = v;
		subsets[v].rank = 0;
		subsets[v].sumOfweights = 0;
		subsets[v].numOfvertices = 1;
	}
	while (e < (graph->V - 1) && i < (graph->E)) {
		//printf("e = %d, i = %d\n", e, i);
		struct Edge next_edge = graph->edge[i++];
		int x = Find(subsets, next_edge.v1);
		int y = Find(subsets, next_edge.v2);

		if (x != y) {
			result[e++] = next_edge;
			connectedVertex[next_edge.v1] = 1;
			connectedVertex[next_edge.v2] = 1;
			Union(subsets, x, y, next_edge.weight);
		}
	}

	//end = time(NULL);
	//double time = (double)(end - start);

	end = clock();
	double time = (double)(end - start);

	printf("수행시간 : %f\n", time);

	printf("While loop completes\n");
	printf("kscanned = %d\n", i);
	int unconnected = 0, connected = 0;
	for (int v = 0; v < graph->V; v++) {
		if (connectedVertex[v] == 0)
			unconnected++;
		if (subsets[v].parent == v)
			connected++;
	}
	FILE* fp_out = fopen(outputFile, "w");
	printf("unconnected = %d, connected = %d\n", unconnected, connected);
	fprintf(fp_out, "%d\n", connected);
	for (int v = 0; v < graph->V; v++) {
		if (subsets[v].parent == v) {
			fprintf(fp_out, "%d %lld\n", subsets[v].numOfvertices, subsets[v].sumOfweights);
		}
	}
	fclose(fp_out);
	free(subsets);
	free(connectedVertex);
	free(result);
}
int main() {

	char inputFile[128] = "Graphs_HW4/", outputFile[128]= "Graphs_HW4/";

	FILE* fp = fopen("Graphs_HW4/commands.txt", "r");
	if (!fp) {
		fprintf(stderr, "^^^ Error: cannot open the file %s.\n", inputFile);
		exit(-1);
	}
	//printf("NO ERROR\n");
	int index = 11;
	char ch;
	while (1) {
		ch = fgetc(fp);
		//printf("%c\n", ch);
		if (ch == '\n') {
			inputFile[index]='\0';
			break;
		}
		inputFile[index++] = ch;
	}
	index = 11;
	while (1) {
		ch = fgetc(fp);
		//printf("%c\n", ch);
		if (ch == '\n' || ch == EOF) {
			outputFile[index] = '\0';
			break;
		}
		outputFile[index++] = ch;
	}
	fclose(fp);

	FILE* fp_in;
	int v_from, v_to, weight, n_vertices, n_edges;

	printf("%s\n%s\n", inputFile, outputFile);
	fp_in = fopen(inputFile, "r");
	if (!fp_in) {
		fprintf(stderr, "^^^ Error: cannot open the file %s.\n", inputFile);
		exit(-1);
	}

	fscanf(fp_in, "%d %d %d", &n_vertices, &n_edges, &MAX_WEIGHT);

	fprintf(stdout, "*** N_VERTICES = %d, N_EDGES = %d, MAX_WEIGHT = %d\n", n_vertices, n_edges, MAX_WEIGHT);

	struct Graph* graph = (struct Graph*)malloc(sizeof(Graph));
	graph->V = n_vertices;
	graph->E = n_edges;
	graph->edge = (struct Edge*)malloc(sizeof(Edge) * graph->E);
	struct Edge* heapEdge = (struct Edge*)malloc(sizeof(Edge) * (graph->E + 1));

	int v_max = -1, tmp, check = 0;
	char line[40];
	ch = fgetc(fp_in);
	for (int i = 0; i < n_edges; i++) {
		index = 0;
		check = 0;
		while (1) {
			ch = fgetc(fp_in);
			if (ch == ' ' && check == 1) {
				line[index] = '\0';
				break;
			}
			else if (ch != ' ') {
				line[index++] = ch;
				check = 1;
			}
		}
		v_from = atoi(line);
		index = 0;
		check = 0;
		while (1) {
			ch = fgetc(fp_in);
			if (ch == ' ' && check == 1) {
				line[index] = '\0';
				break;
			}
			else if (ch != ' ') {
				line[index++] = ch;
				check = 1;
			}
		}
		v_to = atoi(line);
		index = 0;
		check = 0;
		while (1) {
			ch = fgetc(fp_in);
			if ((ch == ' ' || ch == EOF) && check == 1) {
				line[index] = '\0';
				break;
			}
			else if (ch != ' ') {
				line[index++] = ch;
				check = 1;
			}
		}
		weight = atoi(line);
		//printf("%d %d %d\n", v_from, v_to, weight);
		//if (v_from > v_max) v_max = v_from;
		//if (v_to > v_max) v_max = v_to;
		if (v_from > v_to) {
			tmp = v_to;
			v_to = v_from;
			v_from = tmp;
		}
		graph->edge[i].v1 = v_from;
		graph->edge[i].v2 = v_to;
		graph->edge[i].weight = weight;
		heapEdge[i + 1] = graph->edge[i];
	}
	fclose(fp_in);
	kruskalMST(graph, heapEdge, outputFile);
	free(graph->edge);
	free(graph);
}