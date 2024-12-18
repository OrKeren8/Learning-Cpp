#include "iostream"
#include "graph.h"
#include "smartGraph.h"
#include <vector>


int main() {
	GraphAlgorithems graphAlgorithems;
	int numOfVertices, numOfArcs;

	std::cin >> numOfVertices >> numOfArcs;
	

	std::vector<Arc> arcs;

	for (int i = 0; i < numOfArcs; i++) {
		size_t source, destination;
		std::cin >> source >> destination;
		arcs.push_back({ source, destination });
	}

	try {
		Graph g(numOfVertices, numOfArcs, arcs);
		Graph g2 = graphAlgorithems.BFS_ShortestPathGraph(g, 1);
		g2 = g2.transpose();
		g2 = graphAlgorithems.BFS_ShortestPathGraph(g2, g2.getNumOfVertices());
		g2 = g2.transpose();

		for (Vertice v : g) {
			for (size_t neighbor : v.neighbors) {
				if (!g2.hasArc({ v.id, neighbor })) {
					g.delArc({ v.id, neighbor });
				}
			}
		}

		if (g.getNumOfArcs() > 0) {
			std::cout << "arcs of paths are:" << std::endl;
			g.printGraph();
		}
		else {
			std::cout << "No path from " << 1 << " to " << g.getNumOfVertices() << std::endl;
		}
	}catch (...) {
	std::cerr << "invalid input" << std::endl;
	}
		
	return 0;
}