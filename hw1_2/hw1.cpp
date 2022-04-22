//Задача 1. Хеш-таблица (6 баллов)
//Обязательная задача
//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
//Хранимые строки непустые и состоят из строчных латинских букв.
//Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
//Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае,
//когда коэффициент заполнения таблицы достигает 3/4.
//Структура данных должна поддерживать операции добавления строки в множество,
//удаления строки из множества и проверки принадлежности данной строки множеству.
//1_2. Для разрешения коллизий используйте двойное хеширование.

#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using std::string;
using std::vector;

struct HasherString {
    unsigned int operator()(const string& str, bool mainHash) const {
        unsigned int hash = 0;
        int koef = 5;
        if (mainHash) {
            koef = 7;
        }
        for (char i : str) {
            hash = hash * koef + i;
        }
        return hash;
    }
};

template<class T, class H>
class HashTable {
public:
    explicit HashTable(const H& hasher = H());
    HashTable(const HashTable& hasher) = delete;
    HashTable& operator=(const HashTable& hasher) = delete;
    ~HashTable();

    bool Add(const T& data);
    bool Has(const T& data) const;
    bool Delete(const T& data);

    void TableSize(int size){keysCount = size;}

private:

    H hasher;
    enum Status {
        DELETED,
        EMPTY,
        FULL,
    };

    struct HashTableNode {
        T data;
        Status status;
        int hash1;
        int hash2;
        HashTableNode():status(EMPTY), hash1(0), hash2(0){}
        HashTableNode(const T& data, Status status, int hash1, int hash2):data(data), status(status), hash1(hash1), hash2(hash2){}

    };

    vector<HashTableNode*> table;
    unsigned int keysCount;

    void growTable();
};

template<class T, class H>
HashTable<T, H>::HashTable(const H &hasher):
    hasher(hasher),
    table(8, nullptr),
    keysCount(0)
    {
}

template<class T, class H>
HashTable<T, H>::~HashTable() {

}

template<class T, class H>
bool HashTable<T, H>::Add(const T &data) {
    if (keysCount > 0.75 * table.size()) {
        growTable();
    }
    unsigned int absHash1 = hasher(data, true);
    unsigned int hash1 = absHash1 % table.size();
    unsigned int absHash2 = hasher(data, false);
    unsigned int hash2 = absHash2 % table.size();

    if (hash2 == 0) {
        hash2++;
    }

    unsigned int indexDeleted = -1;

    HashTableNode *node = table[hash1];
    for (int i = 0; i < table.size() && node != nullptr && node->status != EMPTY; ++i) {
        if (node->data == data && node->status == FULL) {
            return false;
        }
        if (node->status == DELETED && indexDeleted == -1) {
            indexDeleted = hash1;
        }

        hash1 = (hash1 + i * hash2) % table.size();
        node = table[hash1];
    }

    if (indexDeleted == -1) {
        table[hash1] = new HashTableNode(data, FULL, absHash1, absHash2);
    } else {
        table[indexDeleted] = new HashTableNode(data, FULL, absHash1, absHash2);
    }
    ++keysCount;

    return true;
}

template<class T, class H>
bool HashTable<T, H>::Has(const T &data) const {
    unsigned int hash1 = hasher(data, true) % table.size();
    unsigned int hash2 = hasher(data, false) % table.size();
    if (hash2 == 0) {
        hash2++;
    }

    HashTableNode *node = table[hash1];
    for (int i = 0; i < table.size() && node != nullptr && node->status != EMPTY; ++i) {
        if (node->data == data && node->status == FULL) {
            return true;
        }
        hash1 = (hash1 + i * hash2) % table.size();
        node = table[hash1];
    }
    return false;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T &data) {
    unsigned int hash1 = hasher(data, true) % table.size();
    unsigned int hash2 = hasher(data, false) % table.size();
    if (hash2 == 0) {
        hash2++;
    }

    HashTableNode *node = table[hash1];
    for (int i = 0; i < table.size() && node != nullptr && node->status != EMPTY; ++i) {
        if (node->data == data && node->status == FULL) {
            node->status = DELETED;
            --keysCount;
            return true;
        }
        hash1 = (hash1 + i * hash2) % table.size();
        node = table[hash1];
    }

    return false;
}

template<class T, class H>   //  доделать
void HashTable<T, H>::growTable() {
    unsigned int newSize = table.size() * 2;
    vector<HashTableNode*> newTable(newSize, nullptr);
    for (int i = 0; i < table.size(); ++i) {
        HashTableNode *node = table[i];
        if (node != nullptr && node->status == FULL) {
            unsigned int hash1 = table[i]->hash1 % newTable.size();
            unsigned int hash2 = table[i]->hash2 % newTable.size();
            if (hash2 == 0) {
                hash2++;
            }

            HashTableNode *newNode = newTable[hash1];
            for (int j = 0; j < newTable.size() && newNode != nullptr && newNode->status != EMPTY; ++i) {
                hash1 = (hash1 + j * hash2) % newTable.size();
                newNode = newTable[hash1];
            }
            newTable[hash1] = new HashTableNode(table[i]->data, FULL, table[i]->hash1, table[i]->hash2);

        }
        delete table[i];
    }
    table = std::move(newTable);
    TableSize(newSize);

}

int main() {
    HashTable<string, HasherString> table;
    char operation = 0;
    string data;
    while (std::cin >> operation >> data) {
        switch (operation) {
            case '+':
                std::cout << (table.Add(data) ? "OK" : "FAIL") << "\n";
                break;
            case '-':
                std::cout << (table.Delete(data) ? "OK" : "FAIL") << "\n";
                break;
            case '?':
                std::cout << (table.Has(data) ? "OK" : "FAIL") << "\n";
                break;
            default:
                assert(true);
        }
    }

    return 0;
}
