#include <cmath>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <chrono>
#include <random>
using namespace std;

const int r[6] = { 257, 269, 277, 389, 359, 491 };

int xor_devide_hash(int sum, int M);
int xor_multiply_hash(int sum, int M);
void generate_keys(int num, std::vector<std::string>& result);
static int string_xor_sum(std::string key);
int find_nearby_prime(int);
bool is_prime(int);

enum class HashType { XOR_DEVIDE, XOR_MULTIPLY };

class LinearProbing {
public:
	LinearProbing(int _capacity, double _load_factor, HashType hash_type)
		: capacity(abs(_capacity)),
		load_factor(fabs(_load_factor)),
		real_capacity(capacity / load_factor),
		size(0) {
		c = real_capacity / 5 + 1;
		while (real_capacity % c == 0) c++;

		table = new std::string * [real_capacity];

		if (hash_type == HashType::XOR_DEVIDE)
			hash = xor_devide_hash;
		else
			hash = xor_multiply_hash;
	}
	~LinearProbing() { drop(); }
	bool insert(std::string value);
	std::pair<int, int> find(std::string value);
	void drop();

private:
	const double load_factor;
	const int capacity;
	const int real_capacity;
	int size;
	int c;
	int (*hash)(int, int);

	std::string** table;
};
class QudraticProbing {
public:
	QudraticProbing(int min_capacity, HashType hash_type)
		: load_factor(0.5),
		size(0),
		capacity(min_capacity),
		c(1),
		d(1),
		real_capacity(find_nearby_prime(capacity / load_factor)) {
		table = new std::string * [real_capacity];

		if (hash_type == HashType::XOR_DEVIDE)
			hash = xor_devide_hash;
		else
			hash = xor_multiply_hash;
	}
	~QudraticProbing() { drop(); }
	bool insert(std::string value);
	std::pair<int, int> find(std::string value);
	void drop();

private:
	const double load_factor;
	int capacity;
	int real_capacity;
	int size;
	int c;
	int d;
	int (*hash)(int, int);

	std::string** table;
};

//хеш-значение ключа
int xor_devide_hash(int sum, int M) {
	int hashed_key = sum % M;

	return hashed_key;
}

int xor_multiply_hash(int sum, int M) {
	const double A = (sqrt(5) - 1) / 2;
	int hashed_key =
		static_cast<int>(M * ((sum * A) - static_cast<int>(sum * A)));

	return hashed_key;
}

int find_nearby_prime(int x) {
	while (!is_prime(x)) x++;

	return x;
}

bool is_prime(int x) {
	for (int i = 2; i <= x / 2; i++) {
		if (x % 2 == 0) return false;
	}
	return true;
}
//сумма элементов сортировки
int string_xor_sum(string value) {
	int sum = 0;
	int counter = 0;
	for (auto symbol : value)
		sum += (static_cast<int>(symbol)) ^ r[counter++];

	return sum;
}
//генерация ключей
void generate_keys(int num, vector<string>& result) {
	result.clear();
	chrono::system_clock::time_point seed = chrono::system_clock::now();
	default_random_engine engine(seed.time_since_epoch().count());
	uniform_int_distribution<int> distrib(48, 122);
	int random;

	result.resize(num);
	for (int i = 0; i < num; i++) {
		for (int k = 0; k < 6; k++) {
			random = distrib(engine);
			if ((random >= 48 && random <= 57) ||
				(random >= 65 && random <= 90) ||
				(random >= 97 && random <= 122))
				result.at(i).push_back(
					static_cast<char>(random));
			else
				--k;
		}
	}
}
//вставка(линейное)
bool LinearProbing::insert(string value) {
	if (size >= capacity || find(value).second != -1) return false;

	int key = string_xor_sum(value);
	int hashed_key = hash(key, real_capacity);

	while (table[hashed_key] && *table[hashed_key] != value) {
		hashed_key = (hashed_key + c) % real_capacity;
	}
	if (!table[hashed_key]) {
		table[hashed_key] = new string(value);
		size++;
	}
	else if (*table[hashed_key] == value)
		return false;

	return true;
}

//поиск(линейное)
pair<int, int> LinearProbing::find(string value) {
	int key = string_xor_sum(value);
	int hashed_key = hash(key, real_capacity);

	int i = 0;
	while (table[hashed_key] && *table[hashed_key] != value) {
		hashed_key = (hashed_key + c) % real_capacity;
		i++;
	}

	if (!table[hashed_key]) return make_pair(-1, -1);

	return make_pair(i, hashed_key);
}
//очистка таблицы(линейное)
void LinearProbing::drop() {
	for (int i = 0; i < real_capacity; i++)
		if (table[i] != nullptr) {
			table[i] = nullptr;
		}
	size = 0;
}
//вставка(линейное)
bool QudraticProbing::insert(string value) {
	if (size >= capacity || find(value).second != -1) return false;

	int key = string_xor_sum(value);
	int hashed_key = hash(key, real_capacity);
	int tmp_key = hashed_key;

	int i = 0;
	while (table[tmp_key] && *table[tmp_key] != value) {
		tmp_key =
			(hashed_key + c * i + static_cast<int>(d * pow(i, 2))) %
			real_capacity;
		i++;
	}
	if (!table[tmp_key]) {
		table[tmp_key] = new string(value);
		size++;
	}
	else if (*table[tmp_key] == value)
		return false;

	return true;
}

//очистка таблицы(линейное)
void QudraticProbing::drop() {
	for (int i = 0; i < real_capacity; i++)
		if (table[i] != nullptr) {
			table[i] = nullptr;
		}

	size = 0;
}

//поиск(линейное)
pair<int, int> QudraticProbing::find(string value) {
	int key = string_xor_sum(value);
	int hashed_key = hash(key, real_capacity);
	int tmp_key = hashed_key;

	int i = 0;
	while (table[tmp_key] && *table[tmp_key] != value) {
		tmp_key =
			(hashed_key + c * i + static_cast<int>(d * pow(i, 2))) %
			real_capacity;
		i++;
	}

	if (!table[tmp_key]) return make_pair(-1, -1);

	return make_pair(i, tmp_key);
}

int sum(string key) {
	int sum = 0;

	int counter = 0;
	for (auto symbol : key) sum += static_cast<int>(symbol) ^ r[counter++];

	return sum;
}

int main() {
	int table_size;
	int keys_num;
	int experiments;
	vector<string> keys;

	cout << "Enter table size: ";
	cin >> table_size;
	cout << "Enter keys amount(mod 10): ";
	cin >> keys_num;
	if (keys_num % 10 != 0) {
		cout << "Enter more keys!";
		return 1;
	}

	LinearProbing lnProbingDev(table_size, 0.5, HashType::XOR_DEVIDE);
	LinearProbing lnProbingMul(table_size, 0.5, HashType::XOR_MULTIPLY);
	QudraticProbing qdProbingDev(table_size, HashType::XOR_DEVIDE);
	QudraticProbing qdProbingMul(table_size, HashType::XOR_MULTIPLY);

	int sum = 0;

	experiments = keys_num / 10;
	cout << "Linear probing (devide): " << endl;
	while (experiments) {
		for (int i = 0; i < 10000; i++) {
			lnProbingDev.drop();
			generate_keys(experiments * 10, keys);
			for (auto key : keys) lnProbingDev.insert(key);
			for (auto key : keys)
				sum += lnProbingDev.find(key).first;
		}
		cout << "\tFor " << experiments * 10 << " keys average length is " << (static_cast<double>(sum) / (10000 * experiments * 10)) << endl;

		sum = 0;
		experiments--;
	}

	experiments = keys_num / 10;
	cout << "Linear probing (multiply): " << endl;
	while (experiments) {
		for (int i = 0; i < 10000; i++) {
			lnProbingMul.drop();
			generate_keys(experiments * 10, keys);
			for (auto key : keys) lnProbingMul.insert(key);
			for (auto key : keys)
				sum += lnProbingMul.find(key).first;
		}
		cout << "\tFor " << experiments * 10
			<< " keys average length is "
			<< (static_cast<double>(sum) /
				(10000 * experiments * 10))
			<< endl;

		sum = 0;
		experiments--;
	}
	experiments = keys_num / 10;
	cout << "Qudratic probing (devide): " << endl;
	while (experiments) {
		for (int i = 0; i < 10000; i++) {
			qdProbingDev.drop();
			generate_keys(experiments * 10, keys);
			for (auto key : keys) qdProbingDev.insert(key);
			for (auto key : keys)
				sum += qdProbingDev.find(key).first;
		}
		cout << "\tFor " << experiments * 10
			<< " keys average length is "
			<< (static_cast<double>(sum) /
				(10000 * experiments * 10))
			<< endl;

		sum = 0;
		experiments--;
	}
	experiments = keys_num / 10;
	cout << "Qudratic probing (multiply): " << endl;
	while (experiments) {
		for (int i = 0; i < 10000; i++) {
			qdProbingMul.drop();
			generate_keys(experiments * 10, keys);
			for (auto key : keys) qdProbingMul.insert(key);
			for (auto key : keys)
				sum += qdProbingMul.find(key).first;
		}
		cout << "\tFor" << experiments * 10
			<< " keys average length is "
			<< (static_cast<double>(sum) /
				(10000 * experiments * 10))
			<< endl;

		sum = 0;
		experiments--;
	}
}
