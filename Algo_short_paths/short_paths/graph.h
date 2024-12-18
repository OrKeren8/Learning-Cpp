#pragma once
#include <list>
#include <vector>

using std::vector;
using std::list;

struct Arc {
	size_t source;
	size_t destination;
};

struct Vertice{
	size_t id = NULL;
	list<size_t> neighbors;
};

class Graph {

private:
	vector<Vertice> neighborsList;
	size_t numOfVertices = 0;
	size_t numOfArcs = 0;

public:
	Graph(size_t numOfVertices, size_t numOfArcs, vector<Arc> arcs);
	size_t getNumOfVertices() const { return numOfVertices; }
	size_t getNumOfArcs() const { return numOfArcs; }
	Vertice& getVertice(size_t id){ return neighborsList[id-1]; }
	void addArc(Arc arc) {
		neighborsList[arc.source - 1].neighbors.push_back(arc.destination);
		numOfArcs++;
	}
	bool checkVerticeExsitance(size_t id);
	Graph transpose();
	void printGraph();
	bool hasArc(Arc arc);
	void delArc(Arc arc);

	vector<Vertice>::const_iterator begin() const { return neighborsList.begin(); }
	vector<Vertice>::const_iterator end() const { return neighborsList.end(); }
};