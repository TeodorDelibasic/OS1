#ifndef LIST_H_
#define LIST_H_

#include "util.h"

template<class T> class List {

public:

	List() {
		curr = first = last = nullptr;
	}

	List& pushBack(const T&);
	T popFront();

	int empty() {
		return first == nullptr;
	}

	void setCurr(){
		curr = first;
	}
	int hasCurr() {
		return curr != nullptr;
	}
	void moveCurr() {
		curr = curr->next;
	}

	void deleteCurr();

	T getCurrData() {
		return curr->data;
	}

	void removeData(T t);

	~List();

private:

	struct Node {
		T data;

		Node* next;
		Node* prev;

		Node(T new_data) {
			data = new_data;
			next = prev = nullptr;
		}
	};

	Node* first;
	Node* last;

	Node* curr;
};

template<class T> List<T>& List<T>::pushBack(const T& t) {
	Node* new_node = new Node(t);
	new_node->prev = (last ? last : nullptr);
	last = (last ? last->next : first) = new_node;

	return *this;
}

template<class T> T List<T>::popFront() {
	Node* i = this->first->next;
	T data = this->first->data;
	delete this->first;
	this->first = i;

	if (this->first != nullptr)
		this->first->prev = nullptr;
	else
		this->last = nullptr;

	return data;
}

template<class T> void List<T>::removeData(T t) {
	Node* i = this->first;

	while(i != 0 && i->data != t) i = i->next;

	if(this->first == i) this->first = i->next;
	if(this->last == i) this->last = i->prev;
	if(i->next != nullptr) i->next->prev = i->prev;
	if(i->prev != nullptr) i->prev->next = i->next;

	delete i;
}

template<class T> void List<T>::deleteCurr() {
	Node* i = this->curr;

	this->curr = this->curr->next;

	if(this->first == i) this->first = i->next;
	if(this->last == i) this->last = i->prev;
	if(i->next != nullptr) i->next->prev = i->prev;
	if(i->prev != nullptr) i->prev->next = i->next;

	delete i;
}

template<class T> List<T>::~List() {
	Node* i = this->first;

	while(i != nullptr) {
		Node* old = i;
		i = i->next;
		delete old;
	}

	this->first = this->last = this->curr = nullptr;
}

#endif
