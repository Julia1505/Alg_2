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

struct MyNode {
    std::string product_name;
    int color;
    int size;

    bool operator==( const MyNode& rh){
        if (product_name == rh.product_name && color == rh.color && size == rh.size) {
            return true;
        }
        return false;
    }
};

std::istream& operator>>(std::istream &in, MyNode node) {
    in >> node.product_name >> node.color >> node.size;
    return in;
}

struct HasherString {
    unsigned int operator()(const string& str, bool mainHash) const {
        unsigned int hash = 0;
        if (mainHash) {
            for (char i : str) {
                hash = hash * 7 + i;
            }
        } else {
            hash = str.size();
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

    void PrintAll();

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
        unsigned int hash1;
        unsigned int hash2;
        HashTableNode():status(EMPTY), hash1(0), hash2(0){}
        HashTableNode(const T& data, Status status, unsigned int hash1, unsigned int hash2):data(data), status(status), hash1(hash1), hash2(hash2){}
        void Change(T newData, Status newStatus, int newHash1, int newHash2) {
            data = newData;
            status = newStatus;
            hash1 = newHash1;
            hash2 = newHash2;
        }
    };

    vector<HashTableNode> table;
    unsigned int keysCount;
    unsigned int allKeysCount;

    void growTable();
    void Rehash();
};

template<class T, class H>
HashTable<T, H>::HashTable(const H &hasher):
        hasher(hasher),
        table(8),
        keysCount(0),
        allKeysCount(0)
{}

template<class T, class H>
HashTable<T, H>::~HashTable() {

}

template<class T, class H>
bool HashTable<T, H>::Add(const T &data) {
    if (keysCount > 0.75 * table.size()) {
        growTable();
    }
    unsigned int absHash1 = hasher(data.product_name, true);
    unsigned int hash1 = (absHash1 * 2 - 1) % table.size();
    unsigned int absHash2 = hasher(data.product_name, false);
    unsigned int hash2 = (absHash2 * 2 - 1) % table.size();

    unsigned int indexDeleted = -1;

    HashTableNode node = table[hash1];
    if (node.status != EMPTY) {
        return false;
    }
    for (int i = 0; i < table.size() && node.status != EMPTY; ++i) {
        if (node.data == data && node.status == FULL) {
            return false;
        }

        if (node.status == DELETED && indexDeleted == -1) {
            indexDeleted = hash1;
        }

        hash1 = (hash1 + i * hash2) % table.size();
        node = table[hash1];
    }

    if (indexDeleted != -1) {
        hash1 = indexDeleted;
    }

    table[hash1].Change(data, FULL, absHash1, absHash2);

    ++keysCount;
    ++allKeysCount;

    return true;
}

template<class T, class H>
bool HashTable<T, H>::Has(const T &data) const {
    unsigned int hash1 = (hasher(data.product_name, true) * 2 - 1) % table.size();
    unsigned int hash2 = (hasher(data.product_name, false) * 2 - 1) % table.size();

    HashTableNode node = table[hash1];
    for (int i = 0; i < table.size() && node.status != EMPTY; ++i) {
        if (node.data == data && node.status == FULL) {
            return true;
        }

        hash1 = (hash1 + i * hash2) % table.size();
        node = table[hash1];
    }
    return false;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T &data) {
    unsigned int hash1 = (hasher(data.product_name, true) * 2 - 1) % table.size();
    unsigned int hash2 = (hasher(data.product_name, false) * 2 - 1) % table.size();

    HashTableNode node = table[hash1];
    for (int i = 0; i < table.size() && node.status != EMPTY; ++i) {
        if (node.data == data && node.status == FULL) {
            table[hash1].status = DELETED;
            --keysCount;

            if (float(allKeysCount - keysCount) / allKeysCount > 0.5) {
                Rehash();
            }
            return true;
        }
        hash1 = (hash1 + i * hash2) % table.size();
        node = table[hash1];
    }



    return false;
}

template<class T, class H>
void HashTable<T, H>::growTable() {
    unsigned int newSize = table.size() * 2;
    vector<HashTableNode> newTable(newSize);
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].status == FULL) {
            unsigned int hash1 = (table[i].hash1 * 2 - 1) % newTable.size();
            unsigned int hash2 = (table[i].hash2 * 2 - 1) % newTable.size();

            HashTableNode newNode = newTable[hash1];
            for (int j = 0; j < newTable.size() && newNode.status != EMPTY; ++j) {
                hash1 = (hash1 + j * hash2) % newTable.size();
                newNode = newTable[hash1];
            }
            newTable[hash1].Change(table[i].data, table[i].status, table[i].hash1, table[i].hash2);

        }

    }
    table = std::move(newTable);
}

template<class T, class H>
void HashTable<T, H>::PrintAll() {
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].status == FULL) {
            std::cout << table[i].data << " ";
        }
    }
    std::cout << std::endl;
}

template<class T, class H>
void HashTable<T, H>::Rehash() {
    vector<HashTableNode> newTable(table.size());
    for (int i = 0; i < table.size(); ++i) {
        if (table[i].status == FULL) {
            unsigned int hash1 = (table[i].hash1 * 2 - 1) % newTable.size();
            unsigned int hash2 = (table[i].hash2 * 2 - 1) % newTable.size();

            HashTableNode newNode = newTable[hash1];
            for (int j = 0; j < newTable.size() && newNode.status != EMPTY; ++j) {
                hash1 = (hash1 + j * hash2) % newTable.size();
                newNode = newTable[hash1];
            }

            newTable[hash1].Change(table[i].data, table[i].status, table[i].hash1, table[i].hash2);
        }

    }
    table = std::move(newTable);
    allKeysCount = keysCount;
}

int main() {
    HashTable<MyNode, HasherString> table;
    char operation = 0;
    MyNode data;
    while (std::cin >> operation >> data) {
        switch (operation) {
            case '+':
                std::cout << (table.Add(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Delete(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has(data) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(true);
        }
    }

    return 0;
}
