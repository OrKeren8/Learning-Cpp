#include "Queue.h"


void Queue::makeEmpty() {
	q.clear();
	size = 0;
}

bool Queue::isEmpty() {
	if (size == 0)
		return true;
	return false;
}
void Queue::enQueue(Vertice *v) {
	q.push_front(v);
	size++;
}	

Vertice* Queue::deQueue() {
	Vertice* v = q.back();
	q.pop_back();
	size--;
	return v;
}