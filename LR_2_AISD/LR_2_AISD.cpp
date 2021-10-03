#include <iostream>
#include <queue>
#include <string.h>
#include <string>
#include <vector>
#include "list.h"
using namespace std;
const string UNI_CITY = "Moscow";
struct abiturient {
	string surname;
	int marks[3];
	int certificate_with_honors;
	string town;
	int dormitory;

	bool operator==(abiturient abi) {
		return (this->surname.compare(abi.surname) == 0);
	}
};
class predicate {
public:
	vector<int> operator()(abiturient abi) {
		vector<int> criteria;
		if (abi.marks[0] == 5 && abi.marks[0] == abi.marks[1] && abi.marks[1] == abi.marks[2])
			criteria.push_back(1);
		if (abi.certificate_with_honors)
			criteria.push_back(2);
		if (abi.town.compare(UNI_CITY) == 0)
			criteria.push_back(3);
		if (abi.dormitory)
			criteria.push_back(4);

		return criteria;
	}
};

int main() {
	List<abiturient, predicate> list;
	list.InitList();
	setlocale(LC_ALL, "rus");
	int input = 0;
	while (true) {
		cout << "1 - добавить нового абитуриента" << endl;
		cout << "2 - вывести список по критерию" << endl;
		cout << "3 - вывести полный список абитуриентов" << endl;
		cout << "4 - удалить абитуриента" << endl;
		cout << "5 - очистить список" << endl;
		cout << "6 - выйти\n";
		cin>>input;
		switch (input) {
		case 1: {

			abiturient abi;
			cout << "Введите фамилию абитуриента: ";
			cin >> abi.surname;
			cout << "Первая оценка за вступительный экзамен: ";
			cin >> abi.marks[0];
			cout << "Вторая оценка за вступительный экзамен: ";
			cin >> abi.marks[1];
			cout << "Третья оценка за вступительный экзамен: ";
			cin >> abi.marks[2];
			cout << "Сертификат с отличием(0/1): ";
			cin >> abi.certificate_with_honors;
			cout << "Город, в котором проживает абитуриент: ";
			cin >> abi.town;
			cout << "Требуется общежитие(0/1): ";
			cin >> abi.dormitory;


			try {
				list.Insert(abi);
				cout << "Абитуриент успешно добавлен";
			}
			catch (bad_alloc) {
				cout << "Невозможно вставить данные. Переполнение физической памяти";
			}

			break;
		}
		case 2: {
			char input1;
			cout << "1 - вывести абитуриентов, которые сдали все экзамены на отлично" << endl;
			cout << "2 - вывести абитуриентов, которые имеют аттестат с отличием" << endl;
			cout << "3 - вывести абитуриентов, которые проживают в другом городе" << endl;
			cout << "4 - вывести абитуриентов, которые нуждаются в общежитии" << endl;

			cin >> input1;
			if (input1 < '1' || input1 > '4') {
				cout << "Неверная опция";
				break;
			}
			int criteria = static_cast<int>(input1 - 48);
			vector<abiturient> result = list.GetByCriteria(criteria);
			if (result.empty()) {
				cout << "Абитуриентов, удовлетворяющих запросу, не найдено";
				input1 = getchar();
			}
			else {
				for (auto abi : result) {
					cout << abi.surname << ", проживающий в " << abi.town << endl;
				}
			}

			break;
		}
		case 3: {
			vector<abiturient> result = list.GetAll();
			for (auto abi : result)
				cout << abi.surname << ", проживающий в городе " << abi.town << endl;

			break;
		}
		case 4: {

			abiturient abi;
			cout << "Введите фамилию абитуриента: ";
			cin >> abi.surname;


			if (list.Delete(abi)) {
				cout << "Абитуриент успешно удалён";
			}
			else {
				cout << "Абитуриент не найден";
			}
			break;
		}
		case 5: {

			list.DropList();
			cout << "Список успешно сброшен";

			break;
		}
		case 6: {
			return 0;
		}

		}
	}
}
