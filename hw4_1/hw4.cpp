//Задача 4. Использование АВЛ-дерева (5 баллов)
//Обязательная задача
//Требование для всех вариантов Задачи 4
//Решение должно поддерживать передачу функции сравнения снаружи.
//
//4_1. Солдаты. В одной военной части решили построить в одну шеренгу по росту.
//Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги.
//Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены по росту – сначала самые высокие, а в конце – самые низкие.
//За расстановку солдат отвечал прапорщик, который заметил интересную особенность – все солдаты в части разного роста.
//Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, а именно для каждого приходящего солдата указывать, перед каким солдатом в строе он должен становится.

#include <iostream>
#include <cassert>

struct Soldier {
    int height;

    Soldier(int height):height(height){};
};

template <class T>
struct DefaultComparator {
    bool IsLess(const T& l, const T& r) {
        return l < r;
    }

    bool IsEqual(const T& l, const T& r) {
        return l == r;
    }
};

struct SoldierComparator {
    bool IsLess(const Soldier& l, const Soldier& r) {
        return l.height > r.height; ////  TODO мб неправильный компаратор
    }

    bool IsEqual(const Soldier& l, const Soldier& r) {
        return l.height == r.height;
    }
};

template <class T, class C>
class AVLTree {
private:

    struct Node {
        T Key;
        int SumNodes;
        unsigned char Height;
        Node* Left;
        Node* Right;
        Node(T key) : Key(key), SumNodes(1), Height(1), Left(nullptr), Right(nullptr){};
    };

    Node* Root;
    C Comp;

    unsigned char Height(Node* node) {return node ? node->Height : 0;};
    int BalanceFactor(Node* node);
    void FixHeight(Node* node);
    void FixSumNodes(Node* node);

    Node* RotateRight(Node* node);
    Node* RotateLeft(Node* node);
    Node* Balance(Node* node);

    Node* AddRecursion(Node *root, T key);
    bool DeleteRecursion(Node *node, T key);
    void DeleteNode(Node*& node);
    Node* FindMin(Node* node, T* key);

    int SearchPositionRecursion(Node* node, T key);
    int Count(Node* node);
    T GetKStatRecursion(Node* node, int pos);

public:
    explicit AVLTree(const C& comp):Root(nullptr), Comp(comp){};
    AVLTree(const AVLTree&) = delete;
    ~AVLTree();

    void Add(T key);
    void Delete(T key);
    int SearchPosition(T key);
    T GetKStat(int pos);
};

template<class T, class C>
AVLTree<T, C>::~AVLTree() {

}

template <class T, class C>
void AVLTree<T, C>::FixHeight(AVLTree::Node *node) {
    unsigned char heightLeft = Height(node->Left);
    unsigned char heightRight = Height(node->Right);
    node->Height = (heightLeft > heightRight ? heightLeft : heightRight) + 1;
}

template <class T, class C>
void AVLTree<T, C>::FixSumNodes(AVLTree::Node *node) {
    if (node == nullptr) {
        return;
    }

    int countLeft = Count(node->Left);
    int countRight = Count(node->Right);
    node->SumNodes = countLeft + countRight + 1;
}

template <class T, class C>
typename AVLTree<T, C>::Node* AVLTree<T, C>::RotateRight(AVLTree::Node *node) {
    Node* newCurrentRoot = node->Left;
    node->Left = newCurrentRoot->Right;
    newCurrentRoot->Right = node;
    FixHeight(node);
    FixSumNodes(node);
    FixHeight(newCurrentRoot);
    FixSumNodes(newCurrentRoot);
    return newCurrentRoot;
}

template <class T, class C>
typename AVLTree<T, C>::Node *AVLTree<T, C>::RotateLeft(AVLTree::Node *node) {
    Node* newCurrentRoot = node->Right;
    node->Right = newCurrentRoot->Left;
    newCurrentRoot->Left = node;
    FixHeight(node);
    FixSumNodes(node);
    FixHeight(newCurrentRoot);
    FixSumNodes(newCurrentRoot);
    return newCurrentRoot;
}

template <class T, class C>
typename AVLTree<T, C>::Node *AVLTree<T, C>::Balance(AVLTree::Node *node) {
    FixHeight(node);
    FixSumNodes(node);
    if (BalanceFactor(node) == 2) {
        if (BalanceFactor(node->Right) < 0) {
            node->Right = RotateRight(node->Right);
        }
        return RotateLeft(node);
    }

    if (BalanceFactor(node) == -2) {
        if (BalanceFactor(node->Left) > 0) {
            node->Left = RotateLeft(node->Left);
        }
        return RotateRight(node);
    }

    return node;
}

template <class T, class C>
void AVLTree<T, C>::Add(T key) {
    Root = AddRecursion(Root, key);
    Balance(Root);    //// мб не надо
}

template <class T, class C>
typename AVLTree<T, C>::Node* AVLTree<T, C>::AddRecursion(AVLTree::Node *root, T key) {
    if (root == nullptr) {
        return new Node(key);
    }

    if (Comp.IsLess(key, root->Key)) {
        root->Left = AddRecursion(root->Left, key);
    } else {
        root->Right = AddRecursion(root->Right, key);
    }

    return Balance(root);
}

template <class T, class C>
void AVLTree<T, C>::Delete(T key) {
    DeleteRecursion(Root, key);
}

template <class T, class C>
bool AVLTree<T, C>::DeleteRecursion(AVLTree::Node *node, T key) {

    if (node == nullptr) {
        return false;
    }

    if (Comp.IsLess(key, node->Key)) {
        DeleteRecursion(node->Left, key);
        node = Balance(node);
        return true;
    } else if (Comp.IsLess(node->Key, key)) {
        DeleteRecursion(node->Right, key);
        node = Balance(node);
        return true;
    } else {
        DeleteNode(node);
        return true;
    }
}

template <class T, class C>
void AVLTree<T, C>::DeleteNode(AVLTree::Node *&node) {
    if (node->Left == nullptr) {
        Node* right = node->Right;
        delete node;
        node = right;
    } else if (node->Right == nullptr) {
        Node* left = node->Left;
        delete node;
        node = left;
    } else {
        node->Right = FindMin(node->Right, &node->Key);
    }

    if (node != nullptr) {
        node = Balance(node);
    }
}

template <class T, class C>
typename AVLTree<T, C>::Node *AVLTree<T, C>::FindMin(AVLTree::Node *node, T *key) {
    if (node->Left != nullptr) {
        node->Left = FindMin(node->Left, key);
        return Balance(node);
    }

    *key = node->Key;
    Node* temp = node->Right;
    delete node;

    if (temp == nullptr) {
        return nullptr;
    }

    return Balance(temp);
}

template <class T, class C>
int AVLTree<T, C>::SearchPosition(T key) {
    return SearchPositionRecursion(Root, key);

}

template <class T, class C>
int AVLTree<T, C>::SearchPositionRecursion(AVLTree::Node *node, T key) {
    if (Comp.IsEqual(key, node->Key)) {
        return Count(node->Left);
    }

    if (Comp.IsLess(key, node->Key)) {
        return SearchPositionRecursion(node->Left, key);
    } else {
        return SearchPositionRecursion(node->Right, key) + Count(node->Left) + 1;
    }
}


template <class T, class C>
int AVLTree<T, C>::BalanceFactor(AVLTree::Node *node) {return Height(node->Right) - Height(node->Left);}

template <class T, class C>
int AVLTree<T, C>::Count(AVLTree::Node *node) {
    if (node == nullptr) {
        return 0;
    }
    return node->SumNodes;
}

template <class T, class C>
T AVLTree<T, C>::GetKStat(int pos) {
    return GetKStatRecursion(Root, pos);
}

template <class T, class C>
T AVLTree<T, C>::GetKStatRecursion(AVLTree::Node *node, int pos) {
    int nodePos = Count(node->Left);

    if (nodePos == pos) {
        return node->Key;
    }

    if (nodePos > pos) {
        return GetKStatRecursion(node->Left, pos);
    } else {
        return GetKStatRecursion(node->Right, pos - Count(node->Left) - 1);
    }
}

int main() {
    int N = 0;
    std::cin >> N;
    char action;
    int actionValue = 0;
    DefaultComparator<int> comp;
    AVLTree<int, DefaultComparator<int>> tree(comp);

    for (int i = 0; i < N; ++i) {
        std::cin >> action >> actionValue;
        switch (action) {
            case '1':
                tree.Add(actionValue);
                std::cout << tree.SearchPosition(actionValue) << " ";
                break;
            case '2':
                tree.Delete(tree.GetKStat(actionValue));
                break;
            default:
                assert(false);
        }
    }

    return 0;
}