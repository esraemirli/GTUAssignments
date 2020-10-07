// A C Program to demonstrate adjacency list
// representation of graphs
#include <stdio.h>
#include <stdlib.h>

// A structure to represent an adjacency list node
struct AdjListNode
{
	int dest;
	struct AdjListNode *next;
};

// A structure to represent an adjacency list
struct AdjList
{
	struct AdjListNode *head;
};

// A structure to represent a graph. A graph
// is an array of adjacency lists.
// Size of array will be V (number of vertices
// in graph)
struct Graph
{
	int V;
	struct AdjList *array;
};
// A utility function to create a new adjacency list node
struct AdjListNode *newAdjListNode(int dest)
{
	struct AdjListNode *newNode =
		(struct AdjListNode *)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph *createGraph(int V)
{
	struct Graph *graph =
		(struct Graph *)malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists. Size of
	// array will be V
	graph->array =
		(struct AdjList *)malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by
	// making head as NULL
	int i;
	for (i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph *graph, int src, int dest)
{
	// Add an edge from src to dest. A new node is
	// added to the adjacency list of src. The node
	// is added at the begining
	struct AdjListNode *newNode = newAdjListNode(dest);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;
}

struct Result
{
	int *arr;
	int size;
};

struct Result
bfs(struct Graph *graph, int s)
{
	// Mark all the vertices as not visited
	int *visited = malloc(graph->V * sizeof(int));
	int i;
	for (i = 0; i < graph->V; i++)
		visited[i] = 0;

	// Create a queue for BFS
	int *queue = malloc(graph->V * sizeof(int));

	struct Result result;
	result.arr = malloc(graph->V * sizeof(int));
	result.size = 0;

	int q = 0;

	// Mark the current node as visited and enqueue it
	visited[s] = 1;
	queue[q++] = s;

	while (q != 0)
	{
		// Dequeue a vertex from queue and print it
		s = queue[0];
		result.arr[result.size++] = s;
		//kaydır
		int i;
		for (i = 0; i < q - 1; i++)
		{
			queue[i] = queue[i + 1];
		}
		q--;
		// Get all adjacent vertices of the dequeued
		// vertex s. If a adjacent has not been visited,
		// then mark it visited and enqueue it

		i = 0;
		struct AdjListNode *pCrawl = graph->array[s].head;
		while (pCrawl)
		{
			int n = pCrawl->dest;

			if (!visited[n])
			{
				visited[n] = 1;
				queue[q++] = n;
			}
			pCrawl = pCrawl->next;
		}
	}
	free(visited);
	free(queue);
	return result;
}
int nerden(struct Graph *graph, int s, int d)
{
	// Mark all the vertices as not visited
	int *visited = malloc(graph->V * sizeof(int));
	int i;
	for (i = 0; i < graph->V; i++)
		visited[i] = 0;

	// Create a queue for BFS
	int *queue = malloc(graph->V * sizeof(int));

	int q = 0;

	// Mark the current node as visited and enqueue it
	visited[s] = 1;
	queue[q++] = s;
	while (q != 0)
	{
		// Dequeue a vertex from queue and print it
		s = queue[0];
		//kaydır
		int i;
		for (i = 0; i < q - 1; i++)
		{
			queue[i] = queue[i + 1];
		}
		q--;
		// Get all adjacent vertices of the dequeued
		// vertex s. If a adjacent has not been visited,
		// then mark it visited and enqueue it

		i = 0;
		struct AdjListNode *pCrawl = graph->array[s].head;
		while (pCrawl)
		{
			int n = pCrawl->dest;
			int found = 0;
			if (n == d)
			{
				found = 1;
			}
			if (!visited[n])
			{
				visited[n] = 1;
				queue[q++] = n;
				if (found)
				{
					free(visited);
					free(queue);
					return s;
				}
			}
			pCrawl = pCrawl->next;
		}
	}
	free(visited);
	free(queue);
	return -1;
}

struct Result findPath(struct Graph *graph, int s, int d)
{

	if (s >= graph->V)
	{

		struct Result noResult;
		noResult.size = 0;
		noResult.arr = NULL;
		return noResult;
	}
	struct Result result = bfs(graph, s);
	int i, found = 0;
	for (i = 0; i < result.size; i++)
	{
		if (result.arr[i] == d)
			found = 1;
	}
	if (found == 0)
	{
		result.size = 0;
		return result;
	}

	struct Result temp;
	temp.size = 0;
	temp.arr = malloc(graph->V * sizeof(int));

	// dest'e nerden geldin..
	int item = nerden(graph, s, d);
	temp.arr[temp.size++] = item;
	while (item != s && item != -1)
	{
		item = nerden(graph, s, item);
		temp.arr[temp.size++] = item;
	}

	struct Result returnRes;
	returnRes.arr = malloc((temp.size + 1) * sizeof(int));
	returnRes.size = 0;
	for (i = temp.size - 1; i >= 0; i--)
	{
		returnRes.arr[returnRes.size++] = temp.arr[i];
	}
	returnRes.arr[returnRes.size++] = d;

	free(result.arr);
	result.size = 0;
	free(temp.arr);

	return returnRes;
}
