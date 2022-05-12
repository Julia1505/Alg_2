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
    bool compare(const T& l, const T& r) {
        return l < r;
    }
};

struct SoldierComparator {
    bool compare(const Soldier& l, const Soldier& r) {
        return l.height > r.height; ////  TODO мб неправильный компаратор
    }
};

class AVLTree {
private:

    struct Node {
        int Key;
        int SumNodes;
        unsigned char Height;
        Node* Left;
        Node* Right;
        Node(int key) : Key(key), Height(1), Left(nullptr), Right(nullptr){};
    };

    Node* Root;

    unsigned char Height(Node* node) {return node ? node->Height : 0;};
    int BalanceFactor(Node* node);;
    void FixHeight(Node* node);

    Node* RotateRight(Node* node);
    Node* RotateLeft(Node* node);
    Node* Balance(Node* node);

    Node* AddRecursion(Node *root, int key);
    Node* DeleteRecursion(Node *root, int key);
    Node* FindMin(Node*);


public:
    explicit AVLTree():Root(nullptr){};
    AVLTree(const AVLTree&) = delete;
    ~AVLTree();

    void Add(int key);
    void Delete(int key);
    int Search(int key);

};

AVLTree::~AVLTree() {

}

void AVLTree::FixHeight(AVLTree::Node *node) {
    unsigned char heightLeft = Height(node->Left);
    unsigned char heightRight = Height(node->Right);
    node->Height = (heightLeft > heightRight ? heightLeft : heightRight) + 1;
}

AVLTree::Node *AVLTree::RotateRight(AVLTree::Node *node) {
    Node* newCurrentRoot = node->Left;
    node->Left = newCurrentRoot->Right;
    newCurrentRoot->Right = node;
    FixHeight(node);
    FixHeight(newCurrentRoot);
    return newCurrentRoot;
}

AVLTree::Node *AVLTree::RotateLeft(AVLTree::Node *node) {
    Node* newCurrentRoot = node->Right;
    node->Right = newCurrentRoot->Left;
    newCurrentRoot->Left = node;
    FixHeight(node);
    FixHeight(newCurrentRoot);
    return newCurrentRoot;
}

AVLTree::Node *AVLTree::Balance(AVLTree::Node *node) {
    FixHeight(node);
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

void AVLTree::Add(int key) {
    Root = AddRecursion(Root, key);
    Balance(Root);    //// мб не надо
}

AVLTree::Node* AVLTree::AddRecursion(AVLTree::Node *root, int key) {
    if (root == nullptr) {
        return new Node(key);
    }

    if (key < root->Key) {
        root->Left = AddRecursion(root->Left, key);
    } else {
        root->Right = AddRecursion(root->Right, key);
    }

    return Balance(root);
}

void AVLTree::Delete(int key) {
    Root = DeleteRecursion(Root, key);
}

AVLTree::Node *AVLTree::DeleteRecursion(AVLTree::Node *root, int key) {
    if (root == nullptr) {
        return nullptr;
    }

    if (key < root->Key) {
        root->Left = DeleteRecursion(root->Left, key);
    } else if (key > root->Key) {
        root->Right = DeleteRecursion(root->Right, key);
    } else {
        Node* leftNode = root->Left;
        Node* rightNode = root->Right;
        delete root;

        if (rightNode == nullptr) {
            return leftNode;
        }

        Node* min = FindMin(rightNode);
//        min->Right = rightNode;
        min->Left = leftNode;
        return Balance(min);
    }
    return root;
}

AVLTree::Node *AVLTree::FindMin(AVLTree::Node *node) {
    Node* foundNode = nullptr;
    if (node->Left != nullptr) {
        foundNode = FindMin(node->Left);
        foundNode->Right = node;
    }

    if (node->Left == nullptr) {
        foundNode = node;
        node = node->Right;
    }

//    Balance(node);

    return foundNode;
}

int AVLTree::Search(int key) {

    return 0;
}

int AVLTree::BalanceFactor(AVLTree::Node *node) {return Height(node->Right) - Height(node->Left);}




int main() {
    int N = 0;
    std::cin >> N;
    char action;
    int actionValue = 0;
    AVLTree tree;

    for (int i = 0; i < N; ++i) {
        std::cin >> action >> actionValue;
        switch (action) {
            case '1':
                tree.Add(actionValue);
//                std::cout << tree.Search(actionValue);
                break;
            case '2':
                tree.Delete(actionValue);
                break;
            default:
                assert(false);
        }
    }

    return 0;
}