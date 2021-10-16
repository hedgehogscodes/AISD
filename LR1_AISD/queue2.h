//----------Очередь в связной памяти---------
#include <vector>
template <typename T>
class Queue {
public:
	void InitQueue();
	bool EnQueue(T elem);
	bool DeQueue(T& result);
	std::vector<T> GetAll();
	T* GetFirst();
	bool Empty();
	~Queue();
private:
	template <typename K>
	class Node {
	public:
		Node(K _data, Node<K>* _next = nullptr) :
			data(_data),
			next(_next) {};
		K data;
		Node<K>* next;
	};
	Node<T>* head;
	Node<T>* tail;
};


//----------Очередь в связной памяти---------
template <typename T>
void Queue<T>::InitQueue() {
	head = tail = nullptr;
}

template <typename T>
bool Queue<T>::EnQueue(T elem) {
	if (Empty()) {
		head = tail = new Node<T>(elem);
		return true;
	}
	else {
		tail->next = new Node<T>(elem);
		tail = tail->next;
	}


	return true;
}
template <typename T>
bool Queue<T>::DeQueue(T& result) {
	if (Empty())
		return false;
	result = head->data;
	Node<T>* to_delete = head;
	head = head->next;
	delete to_delete;
	if (head == nullptr)
		tail = nullptr;

	return true;
}
template <typename T>
std::vector<T> Queue<T>::GetAll() {
	std::vector<T> container;
	Node<T>* current = head;
	while (current) {
		container.push_back(current->data);
		current = current->next;
	}

	return container;
}
template <typename T>
T* Queue<T>::GetFirst() {
	if (Empty())
		return nullptr;
	return &head->data;
}
template <typename T>
bool Queue<T>::Empty() {
	if (head == tail && head == nullptr)
		return true;
	else
		return false;
}



