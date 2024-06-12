#include <iostream>
using namespace std;

class AVL {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        int height;

        Node(int value) :data(value), left(nullptr), right(nullptr), height(1) {}
    };

    Node* root;

    Node* copy(Node* node) {
        if (node == nullptr) {
            return nullptr;
        }

        Node* newNode = new Node(node->data);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        newNode->height = node->height;
        return newNode;
    }

    void deallocate(Node* node) {
        if (node != nullptr) {
            deallocate(node->left);
            deallocate(node->right);
            delete node;
        }
    }

    int height(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* temporary = x->right;
        x->right = y;
        y->left = temporary;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    Node* rotateLeft(Node* x) {   
        Node* y = x->right;
        Node* temporary = y->left;
        y->left = x;
        x->right = temporary;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    int getBalanceFactor(Node* root) {
        if (root == nullptr) {
            return 0;
        }
        return height(root->left) - height(root->right);
    }

    Node* nodeWithMinValue(Node* node) {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }

    Node* insertNode(Node* node, int data) {
        if (node == nullptr) {
            return new Node(data);
        }
        if (data < node->data) {
            node->left = insertNode(node->left, data);
        }
        else if (data > node->data) {
            node->right = insertNode(node->right, data);
        }
        else return node;

        node->height = max(height(node->left), height(node->right)) + 1;
        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor > 1) {
            if (data < node->left->data) {
                return rotateRight(node);
            }
            else if (data > node->left->data) {
                node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
        }
        if (balanceFactor < -1) {
            if (data > node->right->data) {
                return rotateLeft(node);
            }
            else if (data < node->right->data) {
                node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
        }
        return node;
    }

    Node* deleteNode(Node* root, int key) {
        if (root == nullptr)
            return root;
        if (key < root->data) {
            root->left = deleteNode(root->left, key);
        }
        else if (key > root->data) {
            root->right = deleteNode(root->right, key);
        }
        else {
            if ((root->left == nullptr) || (root->right == nullptr)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                }
                else
                    *root = *temp;
                delete temp;
            }
            else {
                Node* temp = nodeWithMinValue(root->right);
                root->data = temp->data;
                root->right = deleteNode(root->right, temp->data);
            }
        }

        if (root == nullptr) return root;

        root->height = max(height(root->left), height(root->right)) + 1;
        int balanceFactor = getBalanceFactor(root);
        if (balanceFactor > 1) {
            if (getBalanceFactor(root->left) >= 0) {
                return rotateRight(root);
            }
            else {
                root->left = rotateLeft(root->left);
                return rotateRight(root);
            }
        }
        if (balanceFactor < -1) {
            if (getBalanceFactor(root->right) <= 0) {
                return rotateLeft(root);
            }
            else {
                root->right = rotateRight(root->right);
                return rotateLeft(root);
            }
        }
        return root;
    }

    Node* searchFor(Node* root, int value) {
        if (root == nullptr || root->data == value) {
            return root;
        }
        if (value < root->data) {
            return searchFor(root->left, value);
        }
        return searchFor(root->right, value);
    }
public:
    AVL() : root(nullptr) {}

    AVL(const AVL& other) {
        root = copy(other.root);
    }

    ~AVL() {
        deallocate(root);
    }

    AVL& operator=(const AVL& other) {
        if (this != &other) {
            deallocate(root);
            root = copy(other.root);
        }
        return *this;
    }

    void insert(int value) {
        root = insertNode(this->root, value);
    }

    void Delete(int value) {
        root = deleteNode(root, value);
    }

    bool search(int value) {
        return searchFor(root, value);
    }
};

int main()
{
    AVL tree;
    tree.insert(33);
    tree.insert(13);
    tree.insert(53);
    tree.insert(9);
    tree.insert(21);
    tree.insert(61);
    tree.insert(8);
    tree.insert(11);
    cout << "Enter element to check if it's in the tree: ";
    int element;
    cin >> element;
    cout << "Tree contains " << element <<": " << (tree.search(element) ? "Yes" : "No") << std::endl;
    if (tree.search(element)) {
        tree.Delete(element);
        cout << "Element has been deleted" << endl;
        cout << "Tree contains " << element << " after deletion: " << (tree.search(element) ? "Yes" : "No") << std::endl;
    }
}