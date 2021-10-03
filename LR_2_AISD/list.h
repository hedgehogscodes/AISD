#pragma once
#include <vector>
using namespace std;
template <typename T, typename K>
class List {
public:
	void InitList();
	void Insert(T data);
	bool Delete(T data);
	void DropList();
	std::vector<T> GetAll();
	std::vector<T> GetByCriteria(int criteria);
	bool Empty();
	~List();
private:
	template <typename J>
	class Node {
	public:
		Node() : data(J()) {
			for (int i = 0; i < 5; i++)
				ptr[i] = nullptr;
		}
		J data;
		Node<J>* ptr[5];
	};
	struct descriptor {
		Node<T>* head;
		Node<T>* tail;
		Node<T>* current;
	};

	bool find(T data);

	descriptor descriptor_array[5];
	K predicate;
	int size;
};


template <typename T, typename K>
void List<T, K>::InitList() {
	size = 0;
	for (int i = 0; i < 5; i++) {
		descriptor_array[i].head = nullptr;
		descriptor_array[i].tail = nullptr;
		descriptor_array[i].current = nullptr;
	}
}
template <typename T, typename K>
void List<T, K>::Insert(T data) {
	vector<int> criteria = predicate(data);
	Node<T>* newNode = new Node<T>;
	newNode->data = data;
	if (Empty()) {
		descriptor_array[0].head = descriptor_array[0].tail = newNode;
		for (auto x : criteria) {
			descriptor_array[x].head = descriptor_array[x].tail = newNode;
		}
	}
	else {
		descriptor_array[0].tail->ptr[0] = newNode;
		descriptor_array[0].tail = newNode;
		for (auto x : criteria) {
			if (descriptor_array[x].tail == nullptr) {
				descriptor_array[x].head = descriptor_array[x].tail = newNode;
			}
			else {
				descriptor_array[x].tail->ptr[x] = newNode;
				descriptor_array[x].tail = newNode;
			}
		}
	}
	size++;
}
template <typename T, typename K>
bool List<T, K>::Delete(T data) {
	if (Empty() || !find(data))
		return false;

	Node<T>* to_delete;
	if (descriptor_array[0].current->data == data)
		to_delete = descriptor_array[0].current;
	else
		to_delete = descriptor_array[0].current->ptr[0];

	for (int i = 0; i < 5; i++) {
		if (descriptor_array[i].current == nullptr)
			break;

		if (descriptor_array[i].current->data == data) {
			descriptor_array[i].head = descriptor_array[i].current->ptr[i];
			if (descriptor_array[i].head == nullptr)
				descriptor_array[i].tail = nullptr;
		}
		else {
			descriptor_array[i].current->ptr[i] = descriptor_array[i].current->ptr[i]->ptr[i];
			if (descriptor_array[i].current->ptr[i] == nullptr)
				descriptor_array[i].tail = descriptor_array[i].current;
		}
		descriptor_array[i].current = nullptr;
	}
	delete to_delete;
	size--;

	return true;
}
template <typename T, typename K>
void List<T, K>::DropList() {
	descriptor_array[0].current = descriptor_array[0].head;
	while (descriptor_array[0].current) {
		Node<T>* to_delete = descriptor_array[0].current;
		descriptor_array[0].current = descriptor_array[0].current->ptr[0];
		delete to_delete;
	}
	for (int i = 0; i < 5; i++) {
		descriptor_array[i].head =
			descriptor_array[i].current =
			descriptor_array[i].tail = nullptr;
	}
	size = 0;
}
template <typename T, typename K>
vector<T> List<T, K>::GetAll() {
	vector<T> result;
	descriptor_array[0].current = descriptor_array[0].head;
	while (descriptor_array[0].current) {
		result.push_back(descriptor_array[0].current->data);
		descriptor_array[0].current = descriptor_array[0].current->ptr[0];
	}

	return result;
}
template <typename T, typename K>
vector<T> List<T, K>::GetByCriteria(int criteria) {
	if (criteria == 0 || criteria > 4)
		return vector<T>();

	vector<T> result;
	descriptor_array[criteria].current = descriptor_array[criteria].head;
	while (descriptor_array[criteria].current) {
		result.push_back(descriptor_array[criteria].current->data);
		descriptor_array[criteria].current = descriptor_array[criteria].current->ptr[criteria];
	}

	return result;
}
template <typename T, typename K>
bool List<T, K>::find(T data) {
	bool found = false;
	for (int i = 0; i < 5; i++) {
		descriptor_array[i].current = descriptor_array[i].head;
		while (descriptor_array[i].current) {
			if (descriptor_array[i].current->data == data || (descriptor_array[i].current->ptr[i] != nullptr &&
				descriptor_array[i].current->ptr[i]->data == data)) {
				found = true;
				break;
			}
			descriptor_array[i].current = descriptor_array[i].current->ptr[i];
		}
	}

	return found;
}
template <typename T, typename K>
bool List<T, K>::Empty() {
	return (size == 0);
}
template <typename T, typename K>
List<T, K>::~List() {
	DropList();
}


#pragma once
