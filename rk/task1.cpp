#include <iostream>
#include <cassert>
#include <list>

class Node;

template <class T>
struct DefaultComparator {
    bool isLess(const T& l, const T& r) {
        return l < r;
    }

    bool isNotEqual(const T& l, const T& r) {
        return l != r;
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

    void PostOrderDFS(void visit(Node*));

    int CountR(Node* node);

public:
    explicit BitTree(Compare& cmp):root(nullptr), cmp(cmp){};
    BitTree(const BitTree&) = delete;
    ~BitTree();

    void Add(T key);

    void ShowTree();
    int PostOrderDFS();
    int Count();
};

template <class T, class Compare>
BitTree<T, Compare>::~BitTree() {
    PostOrderDFS([](Node* node){delete node;});
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
void BitTree<T, Compare>::PostOrderDFS(void (*visit)(Node*)) {
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


template <class T, class Compare>
void BitTree<T, Compare>::ShowTree() {
    PostOrderDFS([](Node* node){std::cout << node->Key << " ";});
}

template<class T, class Compare>
int BitTree<T, Compare>::PostOrderDFS() {
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
    } while (isEnd == false);

    auto it = nodesList.begin();

    for (;it != nodesList.end(); ++it) {
        current = *it;
        current->isVisited = false;
    }
    it = nodesList.begin();

    int temp = root->Key;
    for (; it != nodesList.end(); ++it) {
        current = *it;
        if (cmp.isNotEqual(temp, current->Key)) {
            return 0;
        }
    }
    return 1;

}



template<class T, class Compare>
int BitTree<T, Compare>::CountR(Node *node) {
    if (node == nullptr) {
        return 1;
    }
    if (node->Left != nullptr && node->Right != nullptr) {
        int l = CountR(node->Left) + 1;
        int r = CountR(node->Right) + 1;
        return ((l < r) ? l : r);
    }

    if (node->Left == nullptr) {
        return CountR(node->Right) + 1;
    }

    if (node->Right == nullptr) {
        return CountR(node->Left) + 1;
    }
}

template<class T, class Compare>
int BitTree<T, Compare>::Count() {
    return CountR(root) - 1;
}


int main() {
    DefaultComparator<int> comparator;
    BitTree<int, DefaultComparator<int>> tree(comparator);
//    int numbeNodes = 0;
    int key = 0;
    while (std::cin >> key) {
        tree.Add(key);
    }
//    std::cin >> numbeNodes;

//
//    for (int i = 0; i < numbeNodes; ++i) {
//        std::cin >> key;
//        tree.Add(key);
//    }
    std::cout << tree.Count();
//    std::cout << tree.PostOrderDFS() << " ";

//    std::cin >> numbeNodes;
//    int key;
//
//    for (int i = 0; i < numbeNodes; ++i) {
//        std::cin >> key;
//        tree.Add(key);
//    }

//    tree.ShowTree();
    return 0;
}