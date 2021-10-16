#pragma once
#include <vector>
//----------Очередь в последовательной памяти---------
template <typename T>
class Queue {
public:
	void InitQueue();
	bool EnQueue(T elem);
	bool DeQueue(T& result);
	std::vector<T> GetAll();
	T* GetFirst();
	bool Empty();
	bool Full();
private:
	T element[6];
	int head, tail;
};


//----------Очередь в последовательной памяти---------
template <typename T>
void Queue<T>::InitQueue() {
	head = tail = 0;
};
template <typename T>
bool Queue<T>::EnQueue(T elem) {
	if (this->Full()) {
		return false;
	}
	else {
		this->element[this->tail] = elem;
		this->tail = (this->tail + 1) % 6;
	}

	return true;
};
template <typename T>
bool Queue<T>::DeQueue(T& result) {
	if (this->Empty()) {
		return false;
	}
	else {
		result = element[head];
		this->head = (this->head + 1) % 6;
	}

	return true;
};
template <typename T>
std::vector<T> Queue<T>::GetAll() {
	std::vector<T> container;
	int head_back = head;
	int tail_back = tail;
	int counter = 0;
	while (tail != head) {
		container.push_back(element[head]);
		head = (head + 1) % 6;
	}
	head = head_back;
	tail = tail_back;
	return container;
};
template <typename T>
T* Queue<T>::GetFirst() {
	if (Empty())
		return nullptr;
	return &element[head];
}
template <typename T>
bool Queue<T>::Empty() {
	if (this->head == this->tail)
		return true;
	else
		return false;
};
template <typename T>
bool Queue<T>::Full() {
	if (this->head == (this->tail + 1) % 6)
		return true;
	else
		return false;
};



