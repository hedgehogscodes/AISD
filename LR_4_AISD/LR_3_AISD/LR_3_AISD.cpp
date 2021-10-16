#include <chrono>
#include <random>
#include <cmath>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;
const int r[6] = { 257, 269, 277, 389, 359, 491 };

//аддитивный метод деления ключа
map<int, int> adddev_hash(vector<string>& keys, int M) {
    int sum = 0;
    map<int, int> result;
    int key;

    for (auto& str : keys) {
        sum = 0;
        for (auto x : str) {
            sum += static_cast<int>(x);
        }
        key = sum % M;
        if (result.find(key) == result.end())
            result[key] = 0;
        else
            result[key]++;
    }
    return result;
}

//аддитивный метод умножения ключа
map<int, int> addmult_hash(vector<string>& keys, int M) {
    int sum = 0;
    const double A = (sqrt(5) - 1) / 2;
    map<int, int> result;
    int key;

    for (auto& str : keys) {
        sum = 0;
        for (auto x : str) {
            sum += static_cast<int>(x);
        }
        key = static_cast<int>(M * ((sum * A) - static_cast<int>(sum * A)));
        if (result.find(key) == result.end())
            result[key] = 0;
        else
            result[key]++;
    }
    return result;
}
//метод исключающего "или" и метод деления ключа
map<int, int> xordev_hash(vector<string>& keys, int M) {
    int sum = 0;
    map<int, int> result;
    int key;

    for (auto& str : keys) {
        sum = 0;
        int counter = 0;
        for (auto x : str)
            sum += static_cast<int>(x) ^ r[counter++];
        key = sum % M;
        if (result.find(key) == result.end())
            result[key] = 0;
        else
            result[key]++;
    }
    return result;
}

//метод исключающего или и метод умножения ключа
map<int, int> xormult_hash(vector<string>& keys, int M) {
    int sum = 0;
    const double A = (sqrt(5) - 1) / 2;
    map<int, int> result;
    int key;

    for (auto& str : keys) {
        sum = 0;
        int counter = 0;
        for (auto x : str)
            sum += static_cast<int>(x) ^ r[counter++];
        key = static_cast<int>(M * ((sum * A) - static_cast<int>(sum * A)));
        if (result.find(key) == result.end())
            result[key] = 0;
        else
            result[key]++;
    }
    return result;
}
void generate_keys(int num, vector<string>& result) {
    result.clear();
    int random;

    result.resize(num);
    for (int i = 0; i < num; i++) {
        for (int k = 0; k < 6; k++) {
            random = 48 + rand() % (122 - 48 + 1);
            if ((random >= 48 && random <= 57) || (random >= 65 && random <= 90) || (random >= 97 && random <= 122))
                result.at(i).push_back(static_cast<char>(random));
            else
                --k;
        }
    }
}

int main(int argc, char** argv) {
    setlocale(LC_ALL, "Russian");
    if (argc < 3) {
        cout << "Введите количество ключей и размер таблицы!" << endl; //
        return 1;
    }

    int keys_num = atoi(argv[1]);
    int table_size = atoi(argv[2]);
    vector<string> keys;
    generate_keys(keys_num, keys);
    map<int, int> adddev = adddev_hash(keys, table_size);
    map<int, int> xordev = xordev_hash(keys, table_size);
    map<int, int> addmult = addmult_hash(keys, table_size);
    map<int, int> xormult = xormult_hash(keys, table_size);

    char input;

    while (true) {
        cout << "1. Сгенерировать коллизии" << endl;
        cout << "2. Общее число коллизий для каждого метода" << endl;
        cout << "3. Cреднее число коллизий на основании нескольких опытов" << endl;
        cout << "4. Сгенерировать новые ключи" << endl;
        cout << "5. Выйти" << endl;
        input = getchar();
        switch (input) {
        case '1': {

            ofstream os("colfile", ios::out | ios::trunc);
            if (!os.is_open()) {
                cout << "Невозможно открыть файл отчёта";
                break;
            }
            os << "Additive Divide" << endl;
            for (auto& x : adddev)
                os << x.second << endl;

            os << "Additive Multiply" << endl;
            for (auto& x : addmult)
                os << x.second << endl;

            os << "XOR Divide" << endl;
            for (auto& x : xordev)
                os << x.second << endl;

            os << "XOR Multiply" << endl;
            for (auto& x : xormult)
                os << x.second << endl;
            os.close();
            cout << "Файл успешно сформирован!" << endl;

            input = getchar();
            break;
        }
        case '2': {
            int collisions = 0;
            for (auto& x : adddev)
                collisions += x.second;
            cout << "Аддитивный метод деления имеет " << collisions << " коллизий" << endl;
            collisions = 0;

            for (auto& x : addmult)
                collisions += x.second;
            cout << "Аддитивный метод умножения имеет " << collisions << " коллизий" << endl;
            collisions = 0;

            for (auto& x : xordev)
                collisions += x.second;
            cout << "Xor метод деления имеет " << collisions << " коллизий" << endl;
            collisions = 0;

            for (auto& x : xormult)
                collisions += x.second;
            cout << "Xor метод умножения имеет " << collisions << " коллизий" << endl;

            input = getchar();
            break;
        }
        case '3': {
            int experiments = 1000;
            double adddev_collisions = 0;
            double addmult_collisions = 0;
            double xordev_collisions = 0;
            double xormult_collisions = 0;

            for (int i = 0; i < experiments; i++) {
                generate_keys(keys_num, keys);
                adddev = adddev_hash(keys, table_size);
                addmult = addmult_hash(keys, table_size);
                xordev = xordev_hash(keys, table_size);
                xormult = xormult_hash(keys, table_size);

                for (auto& x : adddev)
                    adddev_collisions += x.second;
                for (auto& x : addmult)
                    addmult_collisions += x.second;
                for (auto& x : xordev)
                    xordev_collisions += x.second;
                for (auto& x : xormult)
                    xormult_collisions += x.second;
            }
            cout << "Среднее количество коллизий:" << endl;
            cout << "аддитивный метод деления = " << adddev_collisions / experiments << endl;
            cout << "аддитивный метод умножения = " << addmult_collisions / experiments << endl;
            cout << "XOR метод деления = " << xordev_collisions / experiments << endl;
            cout << "XOR метод умножения = " << xormult_collisions / experiments << endl;

            input = getchar();
            break;
        }
        case '4': {

            generate_keys(keys_num, keys);

            adddev = adddev_hash(keys, table_size);
            addmult = addmult_hash(keys, table_size);
            xordev = xordev_hash(keys, table_size);
            xormult = xormult_hash(keys, table_size);

            cout << "Сгенерированно " << keys_num << " новых ключей" << endl;

            input = getchar();
            break;
        }
        case '5': {
            return 0;
        }
        }
    }
}
