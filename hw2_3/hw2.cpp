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

class BitTree {
private:
    struct Node {
        int Key;
        Node* Left;
        Node* Right;
        bool isVisited;

        explicit Node(int key) : Key(key), Left(nullptr), Right(nullptr), isVisited(false) {};
    };
    Node* root;

    void PostOrderDFS(void visit(Node*));

public:
    BitTree():root(nullptr){};
    ~BitTree();

    void Add(int key);

    void ShowTree();
};

BitTree::~BitTree() {
    PostOrderDFS([](Node* node){delete node;});
}

void  BitTree::Add(int key) {
    Node** current = &root;
    while (*current != nullptr) {
        Node& node = **current;
        if (key < node.Key) {
            current = &node.Left;
        } else {
            current = &node.Right;
        }
    }
    *current = new Node(key);

}

void BitTree::PostOrderDFS(void (*visit)(Node*)) {
    Node* current = root;
    std::list<Node*> nodesList;
    nodesList.push_back(current);
    bool isEnd;
    do {
        isEnd = true;
        auto it = nodesList.begin();
        auto itEnd = nodesList.end();

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
    } while (isEnd == false);

    auto it = nodesList.begin();

    for (;it != nodesList.end(); ++it) {
        current = *it;
        current->isVisited = false;
    }
    it = nodesList.begin();

    for (; it != nodesList.end(); ++it) {
        visit(*it);
    }

}

void BitTree::ShowTree() {
    PostOrderDFS([](Node* node){std::cout << node->Key << " ";});
}


int main() {
    BitTree tree;
    int numbeNodes = 0;
    std::cin >> numbeNodes;
    int key;
    for (int i = 0; i < numbeNodes; ++i) {
        std::cin >> key;
        tree.Add(key);
    }

    tree.ShowTree();

    return 0;
}