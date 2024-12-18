#pragma once
#include <list>
#include "graph.h"

using std::list;

class Queue{
private:
	list<Vertice*> q;
	size_t size=0;
	
public:
	void makeEmpty();
	bool isEmpty();
	void enQueue(Vertice *v);
	Vertice* deQueue();
};

