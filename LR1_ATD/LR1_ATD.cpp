#include <iostream>
#include <queue>
#include <string.h>
#include <vector>
#include "queue.h"
//#include "queue2.h"
using namespace std;



struct detail {
	char name[5];
	unsigned time;
};

int main() {
	setlocale(LC_ALL, "rus");
	Queue<detail> queue;
	queue.InitQueue();
	char input;
	while (true) {

		cout << "1 - поставить деталь на обработку" << endl;
		cout << "2 - сделать шаг модельного времени" << endl;
		cout << "3 - снять обрабатывающуюся деталь" << endl;
		cout << "4 - вывести список обрабатывающихся деталей" << endl;
		cout << "5 - сбросить моделирование" << endl;
		cout << "6 - выйти" << endl;
		cin >> input;
		switch (input) {
		case '1': {

			detail det;
			cout << "Введите номер детали (максимум 4 цифры): ";
			cin >> det.name;
			cout << "Введите время, требующееся для обработки данной детали: ";
			cin >> det.time;

			if (queue.EnQueue(det))
				cout << "Деталь успешное добавлена" << endl;
			else
				cout << "Очередь переполнена" << endl;

			break;
		}
		case '2': {

			detail* det;
			det = queue.GetFirst();
			if (det != nullptr) {
				det->time--;
				if (det->time == -1) {
					detail tmp;
					queue.DeQueue(tmp);
					cout << "Деталь " << tmp.name << " успешно обработана" << endl;
				}
				else {
					cout << "Деталь " << det->name << " все еще обрабатывается" << endl;
					cout << "Осталось " << det->time << " шагов" << endl;
				}
			}
			else {
				cout << "Очередь пуста, вы тратите время впустую!!!" << endl;
			}

			break;
		}
		case '3': {

			detail det;
			if (queue.DeQueue(det)) {
				cout << "Деталь " << det.name << " была принудительно снята с производства" << endl;
			}
			else {
				cout << "Очередь пуста" << endl;
			}

			break;
		}
		case '4': {

			vector<detail> container = queue.GetAll();
			for (auto det : container)
				cout << "Деталь " << det.name << endl;

			break;
		}
		case '5': {

			queue.InitQueue();
			cout << "Очередь деталей сброшена" << endl;


			break;
		}
		case '6': {
			return 0;
		}

		}
	}
}
