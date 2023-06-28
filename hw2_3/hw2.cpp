//Задача 2. Порядок обхода (4 балла)
//Обязательная задача
//Дано число N < 106 и последовательность целых чисел из [-231..231] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
//Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
//Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
//2_3. Выведите элементы в порядке post-order (снизу вверх).

#include <iostream>
#include <cassert>
#include <list>

class Node;

template <class T>
struct DefaultComparator {
    bool isLess(const T& l, const T& r) {
        return l < r;
    }
};

template <class T, class Compare>
class BitTree {
private:
    struct Node {
        T Key;
        Node* Left;
        Node* Right;
        bool isVisited;

        explicit Node(T key) : Key(key), Left(nullptr), Right(nullptr), isVisited(false) {};
    };
    Node* root;
    Compare cmp;

public:
    explicit BitTree(Compare& cmp):root(nullptr), cmp(cmp){};
    BitTree(const BitTree&) = delete;
    ~BitTree();

    void Add(T key);

    void PostOrderDFS(void visitNode(Node*), void visitKey(int));
};

template <class T, class Compare>
BitTree<T, Compare>::~BitTree() {
    PostOrderDFS([](Node* node){delete node;}, nullptr);
}

template <class T, class Compare>
void  BitTree<T, Compare>::Add(T key) {
    Node** current = &root;
    while (*current != nullptr) {
        Node& node = **current;
        if (cmp.isLess(key, node.Key)) {
            current = &node.Left;
        } else {
            current = &node.Right;
        }
    }
    *current = new Node(key);
}

template <class T, class Compare>
void BitTree<T, Compare>::PostOrderDFS(void (*visitNode)(Node*), void (*visitKey)(int)) {
    Node* current = root;
    std::list<Node*> nodesList;
    nodesList.push_back(current);
    bool isEnd;
    do {
        isEnd = true;
        auto it = nodesList.begin();

        for (; it != nodesList.end(); it++) {

            current = *it;

            if (current->isVisited) {
                continue;
            }

            if (current->Left != nullptr) {
                nodesList.insert(it, current->Left);
                isEnd = false;
                current->isVisited = true;
            }

            if (current->Right != nullptr) {
                nodesList.insert(it, current->Right);
                isEnd = false;
                current->isVisited = true;
            }
        }
    } while (!isEnd);

    auto it = nodesList.begin();

    for (;it != nodesList.end(); ++it) {
        current = *it;
        current->isVisited = false;
    }
    it = nodesList.begin();

    for (; it != nodesList.end(); ++it) {
        if (visitNode != nullptr) {
            visitNode(*it);

        }
        if (visitKey != nullptr) {
            visitKey((*it)->Key);
        }
    }
}

int main() {
    DefaultComparator<int> comparator;
    BitTree<int, DefaultComparator<int>> tree(comparator);
    int numbeNodes = 0;
    std::cin >> numbeNodes;
    int key;

    for (int i = 0; i < numbeNodes; ++i) {
        std::cin >> key;
        tree.Add(key);
    }

    tree.PostOrderDFS(nullptr, [](int key){std::cout << key << std::endl;});
    return 0;
}