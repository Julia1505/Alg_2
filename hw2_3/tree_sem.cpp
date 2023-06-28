#include <iostream>
#include <cassert>

class BitTree {
public:
    BitTree();
    ~BitTree();

    void Add(int key);
    void InOrderDFS(void visit(int));

private:
    struct Node {
        int Key;
        Node* Left;
        Node* Right;

        Node(int key) : Key(key), Left(nullptr), Right(nullptr) {}

    };
    Node* root;

    void add(Node*& node, int key);
    void inOrderDFS(Node*& node, void visit(int));
    void postOrder(Node*& node, void visit(Node*));
};

BitTree::BitTree(): root(nullptr) {
}

BitTree::~BitTree() {
    postOrder(root, [](Node* node){delete node;});
//    assert(false);
}

void BitTree::Add(int key) {
    add(root, key);
}

void BitTree::InOrderDFS(void (*visit)(int)) {
    inOrderDFS(root, visit);
}

void BitTree::add(BitTree::Node *&node, int key) {
    if (node == nullptr) {
        node = new Node(key);
        return;
    }

    if (key < node->Key) {
        add(node->Left, key);
    } else {
        add(node->Right, key);
    }
}

void BitTree::inOrderDFS(BitTree::Node *&node, void (*visit)(int)) {
    if (node == nullptr) {
        return;
    }
    inOrderDFS(node->Left, visit);
    visit(node->Key);
    inOrderDFS(node->Right, visit);
}

void BitTree::postOrder(BitTree::Node *&node, void (*visit)(Node*)) {
    if (node == nullptr) {
        return;
    }
    postOrder(node->Left, visit);
    postOrder(node->Right, visit);
    visit(node);
}


int main() {
    BitTree tree;
    tree.Add(10);
    tree.Add(5);
    tree.Add(1);
    tree.Add(7);
    tree.Add(6);
    tree.Add(15);
    tree.Add(25);
    tree.Add(20);
    tree.Add(22);
    tree.Add(1);

    tree.InOrderDFS([](int key){std::cout << key << " ";});

    return 0;
}
