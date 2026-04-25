#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

enum Color { //wanted to use enum because set values and organized
  RED, 
  BLACK 
};

//node struct
struct Node {
  int data;
  Color color;
  Node* left;
  Node* right;
  Node* parent;
  Node(int val);
};

//tree class + prototypes
class RBTree {
private:
  Node* root;
  void rotateLeft(Node* x);
  void rotateRight(Node* x);
  void fixInsert(Node* node);
  void printHelper(Node* node, int space);
public:
  RBTree();
  void add(int val);
  void readFromFile(char filename[]);
  void print();
};

//main functions will all commmands(add,print,read,quit)
int main() {
  RBTree tree;
  int choice;
  int value;
  char filename[300];
  bool running = true;
  while (running) {
    cout << "--- MENU ---" << endl;
    cout << "1. Add number" << endl;
    cout << "2. Read from file" << endl;
    cout << "3. Print tree" << endl;
    cout << "4. Exit " << endl;
    cout << "Choice:";
    cin >> choice;
    if (choice == 1) {
      cout << "Enter number (1–999): ";
      cin >> value;
      tree.add(value);
    }
    else if (choice == 2) {
      cout << "Enter filename: ";
      cin >> filename;
      tree.readFromFile(filename);
    }
    else if (choice == 3) {
      tree.print();
    }
    else if (choice == 4) {
      running = false;
    }
    else {
      cout << "Invalid choice.";
    }
  }
  return 0;
}

//node implement
Node::Node(int val) {
  data = val;
  color = RED;
  left = right = parent = NULL;
}

RBTree::RBTree() {
  root = NULL;
}
//node moves left
void RBTree::rotateLeft(Node* x) {
  Node* y = x->right;
  x->right = y->left;

  if (y->left != NULL)
    y->left->parent = x;
  y->parent = x->parent;

  if (x->parent == NULL)
    root = y;

  else if (x == x->parent->left)
    x->parent->left = y;

  else
    x->parent->right = y;

  y->left = x;
  x->parent = y;
}
//node moves right
void RBTree::rotateRight(Node* x) {
  Node* y = x->left;
  x->left = y->right;

  if (y->right != NULL)
    y->right->parent = x;
  y->parent = x->parent;

  if (x->parent == NULL)
    root = y;

  else if (x == x->parent->right)
    x->parent->right = y;

  else
    x->parent->left = y;

  y->right = x;
  x->parent = y;
}
//inserting node
void RBTree::fixInsert(Node* node) {
  while (node != root && node->parent->color == RED) {
    Node* parent = node->parent;
    Node* grandparent = parent->parent;

    if (parent == grandparent->left) {
      Node* uncle = grandparent->right;
      if (uncle != NULL && uncle->color == RED) {
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        node = grandparent;
      }
      else {
        if (node == parent->right) {
          rotateLeft(parent);
	  node = parent;
          parent = node->parent;
        }
        parent->color = BLACK;
        grandparent->color = RED;
        rotateRight(grandparent);
      }
    }

    else {
      Node* uncle = grandparent->left;
      if (uncle != NULL && uncle->color == RED) {
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        node = grandparent;
      }
      else {
        if (node == parent->left) {
          rotateRight(parent);
	  node = parent;
	  parent = node->parent;
        }
        parent->color = BLACK;
        grandparent->color = RED;
        rotateLeft(grandparent);
      }
    }

  }
  root->color = BLACK;
}
//adding node using insertion function and parents
void RBTree::add(int val) {
  if (val < 1 || val > 999) {
    cout << "Invalid number.";
    return;
  }

  Node* newNode = new Node(val);
  Node* parent = NULL;
  Node* current = root;

  while (current != NULL) {
    parent = current;
    if (val < current->data) {
      current = current->left;
    }
    else {
      current = current->right;
    }
  }

  newNode->parent = parent;
  if (parent == NULL) {
    root = newNode;
  }
  else if (val < parent->data) {
    parent->left = newNode;
  }
  else {
    parent->right = newNode;
  }
  fixInsert(newNode);
}
//reading inputs from file
void RBTree::readFromFile(char filename[]) {
  ifstream file(filename);
  if (!file) {
    cout << "Error opening file.";
    return;
  }
  int num;
  while (file >> num) {
    add(num);
  }
  file.close();
}
//print helper function to ease coding in print
void RBTree::printHelper(Node* node, int space) {
  if (node == NULL) {
    return;
  }
  space += 5;
  printHelper(node->right, space);
  cout << endl;
  for (int i = 5; i < space; i++) {
    cout << " ";
  }
  cout << node->data << "(";
  if (node->color == RED) {
    cout << "R";
  }
  else {
    cout << "B";
  }
  cout << ")";
  if (node->parent != NULL) {
    cout << " P:" << node->parent->data;
  }
  cout << endl;
  printHelper(node->left, space);
}
//prints tree diagram
void RBTree::print() {
  if (root == NULL) {
    cout << "Tree is empty.\n";
    return;
  }
  printHelper(root, 0); 
}
