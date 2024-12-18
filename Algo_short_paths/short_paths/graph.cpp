#include <iostream>
#include "graph.h"


Graph::Graph(size_t _numOfVertices, size_t _numOfArcs, vector<Arc> arcs) {
	
	numOfVertices = _numOfVertices;
	numOfArcs = _numOfArcs;

	neighborsList.reserve(_numOfVertices);
	
	for (size_t i = 1; i <= numOfVertices; i++) {
		neighborsList.push_back(Vertice());
		neighborsList[i-1].id = i;
	}

	for (Arc arc : arcs) {
		if (!checkVerticeExsitance(arc.source) or !checkVerticeExsitance(arc.destination)) {
			throw std::runtime_error("");
		}
		neighborsList[arc.source-1].neighbors.push_back(arc.destination);
	}
}

Graph Graph::transpose() {
	
	vector<Arc> _arcs;
	_arcs.reserve(numOfArcs);
	
	for (Vertice v : neighborsList) {
		for (size_t neighbor : v.neighbors) {
			_arcs.push_back({ neighbor, v.id }); //the transpose of each arc
		}
	}

	Graph gt(numOfVertices, numOfArcs, _arcs);

	return gt;
}

void Graph::printGraph() {
	for (Vertice v : neighborsList) {
		for (size_t neighbor : v.neighbors) {
			std::cout << v.id << " " << neighbor << std::endl;
		}
	}
}

bool Graph::checkVerticeExsitance(size_t id) {
	if (id < 1 or id > getNumOfVertices()) {
		return false;
	}
	return true;
}

bool Graph::hasArc(Arc arc) {
	bool res = false;
	for (size_t neighbor : neighborsList[arc.source - 1].neighbors) {
		if (neighbor == arc.destination) {
			res = true;
		}
	}
	return res;
}

void Graph::delArc(Arc arc) {
	list<size_t> &lst = neighborsList[arc.source - 1].neighbors;

	auto it = std::find(lst.begin(), lst.end(), arc.destination);

	if (it != lst.end()) {
		lst.erase(it); 
	}
	numOfArcs--;
}

