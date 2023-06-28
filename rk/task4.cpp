#include <iostream>
#include <cassert>
#include <string>

template <class T>
struct DefaultComparator {
    bool IsLess(const T& l, const T& r) {
        return l < r;
    }

    bool IsEqual(const T& l, const T& r) {
        return l == r;
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
    bool DeleteRecursion(Node *&node, T key);
    void DeleteNode(Node*& node);
    Node* FindMin(Node* node, T* key);

    int SearchPositionRecursion(Node* node, T key);
    int Count(Node* node);
    T GetKStatRecursion(Node* node, int pos);
    void PostOrderDelete(Node* node);
    bool IsExistRecursion(Node* node, T key);
    int NextRecursion(Node* node,T key);
    int PrevRecursion(Node* node,T key);

public:
    explicit AVLTree(const C& comp):Root(nullptr), Comp(comp){};
    AVLTree(const AVLTree&) = delete;
    ~AVLTree();

    void Add(T key);
    void Delete(T key);
    bool IsExist(T key);
    int SearchPosition(T key);
    T GetKStat(int pos);
    int Next(T key, bool &isExists);
    int Prev(T key, bool &isExists);

};

template <class T, class C>
void AVLTree<T, C>::PostOrderDelete(Node *node) {
    if (node == nullptr) {
        return;
    }
    PostOrderDelete(node->Left);
    PostOrderDelete(node->Right);
    delete node;
}

template<class T, class C>
AVLTree<T, C>::~AVLTree() {
    if (Root == nullptr) {
        return;
    }
    PostOrderDelete(Root);
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
    Balance(Root);
}

template <class T, class C>
typename AVLTree<T, C>::Node* AVLTree<T, C>::AddRecursion(AVLTree::Node *root, T key) {
    if (root == nullptr) {
        return new Node(key);
    }

    if (Comp.IsLess(key, root->Key)) {
        root->Left = AddRecursion(root->Left, key);
    } else if (Comp.IsLess(root->Key, key)){
        root->Right = AddRecursion(root->Right, key);
    }

    return Balance(root);
}

template <class T, class C>
void AVLTree<T, C>::Delete(T key) {
    DeleteRecursion(Root, key);
}

template <class T, class C>
bool AVLTree<T, C>::DeleteRecursion(AVLTree::Node *&node, T key) {

    if (node == nullptr) {
        return false;
    }

    if (Comp.IsLess(key, node->Key)) {
        if (node->Left == nullptr) {
            return true;
        }
        DeleteRecursion(node->Left, key);
        node = Balance(node);
        return true;
    } else if (Comp.IsLess(node->Key, key)) {
        if (node->Right == nullptr) {
            return true;
        }
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

template<class T, class C>
bool AVLTree<T, C>::IsExistRecursion(AVLTree::Node *node, T key) {

    if (Comp.IsEqual(key, node->Key)) {
        return true;
    }


    if (Comp.IsLess(key, node->Key)) {
        if (node->Left == nullptr) {
            return false;
        }
        return IsExistRecursion(node->Left, key);
    } else {
        if (node->Right == nullptr) {
            return false;
        }
        return IsExistRecursion(node->Right, key);
    }
}



template<class T, class C>
bool AVLTree<T, C>::IsExist(T key) {
    if (Root == nullptr) {
        return false;
    }
    return IsExistRecursion(Root, key);
}

template<class T, class C>
int AVLTree<T, C>::Next(T key, bool &isExists) {
    if (Root == nullptr) {
        isExists = false;
        return 0;
    }
    Node* node;
    node = Root;
    while (node->Key <= key) {
        if (node->Right == nullptr) {
            isExists = false;
            return 0;
        }
        node = node->Right;
    }
    return NextRecursion(Root, key);

}

template<class T, class C>
int AVLTree<T, C>::NextRecursion(Node* node, T key) {
    int temp = node->Key;

    while (node->Key <= key) {
        if (node->Right == nullptr) {
            break;
        }
        node = node->Right;
        temp = node->Key;
    }

    while (node->Left != nullptr && node->Left->Key > key) {
        node = node->Left;
        temp = node->Key;
    }

    if (node->Left != nullptr && node->Left->Right != nullptr) {
        int tempTwo = NextRecursion(node->Left->Right, key);
        if (tempTwo < temp && tempTwo > key) {
            temp = tempTwo;
        }
    }

    return temp;


}

template<class T, class C>
int AVLTree<T, C>::PrevRecursion(AVLTree::Node *node, T key) {
    int temp = node->Key;

    while (node->Key >= key) {
        if (node->Left == nullptr) {
            break;
        }
        node = node->Left;
        temp = node->Key;
    }

    while (node->Right != nullptr && node->Right->Key < key) {
        node = node->Right;
        temp = node->Key;
    }

    if (node->Right != nullptr && node->Right->Left != nullptr) {
        int tempTwo = NextRecursion(node->Right->Left, key);
        if (tempTwo > temp && tempTwo < key) {
            temp = tempTwo;
        }
    }

    return temp;
}

template<class T, class C>
int AVLTree<T, C>::Prev(T key, bool &isExists) {
    if (Root == nullptr) {
        isExists = false;
        return 0;
    }
    Node* node;
    node = Root;
    while (node->Key >= key) {
        if (node->Left == nullptr) {
            isExists = false;
            return 0;
        }
        node = node->Left;
    }
    return PrevRecursion(Root, key);
}

int main() {
//    int N = 0;
//    std::cin >> N;
    std::string action;
    int actionValue = 0;
    DefaultComparator<int> comp;
    AVLTree<int, DefaultComparator<int>> tree(comp);

//    for (int i = 0; i < N; ++i) {
    while(std::cin >> action >> actionValue) {
        if (action == "insert") {
            tree.Add(actionValue);
            continue;
        }

        if (action == "delete") {   // вроде раб
            tree.Delete(actionValue);
            continue;
        }

        if (action == "exists") {
            std::cout << ((tree.IsExist(actionValue)) ? "true" : "false") << std::endl;
            continue;
        }

        if (action == "next") {
            bool is = true;
            int res = tree.Next(actionValue, is);
            if (is) {
                std::cout << res << std::endl;
            } else {
                std::cout << "none" << std::endl;
            }
            continue;
        }

        if (action == "prev") {
            bool is = true;
            int res = tree.Prev(actionValue, is);
            if (is) {
                std::cout << res << std::endl;
            } else {
                std::cout << "none" << std::endl;
            }
            continue;
        }


    }

    return 0;
}