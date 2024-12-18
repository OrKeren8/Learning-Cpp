#include "smartGraph.h"
#include "graph.h"
#include "Queue.h"


vector<size_t> GraphAlgorithems::BFS(Graph& g, size_t source_id) {
	Queue q;
	vector<size_t> verticeVal(g.getNumOfVertices(), -1);
	
	q.enQueue(&g.getVertice(source_id));
	verticeVal[source_id-1] = 0;

	while (!q.isEmpty()) {
		Vertice* u = q.deQueue();
		for (size_t neighbor : u->neighbors) {
			if (verticeVal[neighbor-1] == -1) {
				verticeVal[neighbor-1] = verticeVal[u->id-1] + 1;
				q.enQueue(&g.getVertice(neighbor));
			}
		}
	}
	return verticeVal;
}


Graph GraphAlgorithems::BFS_ShortestPathGraph(Graph& g, size_t source_id) {
	vector<size_t> verticeVal = BFS(g, source_id);
	Graph shortestPathsGraph(g.getNumOfVertices(), 0, {});
	for (Vertice v : g) {
		for (size_t neighbor : v.neighbors) {
			
			if ((verticeVal[neighbor - 1] - verticeVal[v.id - 1]) == 1) {
				shortestPathsGraph.addArc({ v.id, neighbor });
			}
		}
	}
	return shortestPathsGraph;
}