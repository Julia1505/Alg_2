#include <iostream>
#include <string>
#include <vector>
#include <cassert>

using std::string;
using std::vector;

struct Hasher {
    unsigned int operator()(const string& str) const {
        unsigned int hash = 0;
        for (char i : str) {
            hash = hash * 7 + i;
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

private:
    struct HashTableNode{
        T Data;
        HashTableNode* Next;
        unsigned int Hash;
        HashTableNode():Hash(0), Next(nullptr){};
        HashTableNode(const T& data, unsigned int hash, HashTableNode* next):Hash(hash), Data(data), Next(nullptr){};
    };
    H hasher;
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
    for (int i = 0; i < table.size(); ++i) {
        HashTableNode* node = table[i];
        while (node != nullptr) {
            HashTableNode *nextNode = node->Next;
            delete node;
            node = nextNode;
        }
    }
}

template<class T, class H>
bool HashTable<T, H>::Add(const T &data) {
    if (keysCount > 3 * table.size()) {
        growTable();
    }

    unsigned int absHash = hasher(data);

    unsigned int hash = absHash % table.size();
    HashTableNode* node = table[hash];
    while (node != nullptr && node->Data != data) {
        node = node ->Next;
    }

    if (node != nullptr) {
        return false;
    }
    table[hash] = new HashTableNode(data, absHash, table[hash]);
    ++keysCount;

    return true;
}

template<class T, class H>
bool HashTable<T, H>::Has(const T &data) const {
    unsigned int hash = hasher(data) % table.size();
    HashTableNode* node = table[hash];
    while (node != nullptr && node->Data != data) {
        node = node ->Next;
    }
    return node != nullptr;
}

template<class T, class H>
bool HashTable<T, H>::Delete(const T &data) {
    unsigned int hash = hasher(data) % table.size();
    HashTableNode *node = table[hash];
    HashTableNode *prevNode = nullptr;

    while (node != nullptr && node->Data != data) {
        prevNode = node;
        node = node ->Next;
    }

    if (node != nullptr) {
        return false;
    }

    if (prevNode == nullptr) {
        table[hash] = node->Next;
    } else {
        prevNode->Next = node->Next;
    }

    delete node;
    --keysCount;
    return true;
}

template<class T, class H>
void HashTable<T, H>::growTable() {
    vector<HashTableNode*> newTable(table.size() * 2, nullptr);
    for (int i = 0; i < table.size(); ++i) {
        HashTableNode* node = table[i];
        while (node != nullptr) {
            HashTableNode *nextNode = node->Next;
            unsigned int newHash = node->Hash % newTable.size();
            node->Next = newTable[newHash];
            newTable[newHash] = node;
            node = nextNode;
        }
    }
    table = std::move(newTable);
}

int main() {
    HashTable<string, Hasher> table;
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
