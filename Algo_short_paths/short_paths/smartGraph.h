#pragma once
#include "graph.h"

class GraphAlgorithems {

private:

public:
	vector<size_t> BFS(Graph& G, size_t  source_id);
	Graph BFS_ShortestPathGraph(Graph& g, size_t source_id);
};