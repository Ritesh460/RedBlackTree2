#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
//btw I used some question marks in my code, I was seeing if I could conserve lines and I learned that question mark can be useful for condensing if else statements
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
  void fixDelete(Node* node, Node* parent);
public:
  RBTree();
  void add(int val);
  void readFromFile(char filename[]);
  void print();
  Node* searchNode(int val);
  bool search(int val);
  Node* minimum(Node* node);
  void transplant(Node* oldNode, Node* newNode);
  Color getColor(Node* node);
  void remove(int val);
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
    cout << "4. Search" << endl;
    cout << "5. Delete" << endl;
    cout << "6. Exit " << endl;
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
       cout << "Enter number to search: ";
       cin >> value;
       if (tree.search(value)) {
         cout << value << " is in the tree.\n";
       }
       else {
         cout << value << " is not in the tree.\n";
       }
    }
    else if (choice == 5) {
     cout << "What value do you want to delete: ";
     cin >> value;
     tree.remove(value);
    }
    else if (choice == 6) {
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
//using Color enum to get color of certain nodes
Color RBTree::getColor(Node* node) {
  if (node == NULL) {
    return BLACK;
  }
  return node->color;
}
//searchnode helper to ease code in search, lowk could combine but it's feel more simpler doing like this, also used to search for a specific value
Node* RBTree::searchNode(int val) {
  Node* current = root;
  while (current != NULL) {
    if (val == current->data) {
      return current;
    }
    else if (val < current->data) {
      current = current->left;
    }
    else {
      current = current->right;
    }
  }
  return NULL;
}
//main search function, also returns true if value exists in tree
bool RBTree::search(int val) {
  Node* found = searchNode(val);
  if (found == NULL) {
    return false;
  }
  return true;
}
//find minimum node, primarily used for finding successor during deletion
Node* RBTree::minimum(Node* node) {
  while (node->left != NULL) {
    node = node->left;
  }
  return node;
}
//changing and replace parts of the big tree with other parts, done during deletion for reconfiguring
void RBTree::transplant(Node* oldNode, Node* newNode) {
  if (oldNode->parent == NULL) {
    root = newNode;
  }
  else if (oldNode == oldNode->parent->left) {
    oldNode->parent->left = newNode;
  }
  else {
    oldNode->parent->right = newNode;
  }
  if (newNode != NULL) {
    newNode->parent = oldNode->parent;
  }
}
//remove node from RB tree
void RBTree::remove(int val) {
  //find node to delete
  Node* z = searchNode(val);
  if (z == NULL) {
    cout << "Number not found.\n";
    return;
  }

  Node* y = z;
  Node* x = NULL;
  Node* xParent = NULL;
  Color originalColor = y->color;
  //case 1
  if (z->left == NULL) {
    x = z->right;
    xParent = z->parent;
    transplant(z, z->right);
  }
  //case 2
  else if (z->right == NULL) {
    x = z->left;
    xParent = z->parent;
    transplant(z, z->left);
  }
  //case 3
  else {
    y = minimum(z->right);
    originalColor = y->color;
    x = y->right;
    if (y->parent == z) {
      xParent = y;
      if (x != NULL) {
        x->parent = y;
      }
    }
    else {
      xParent = y->parent;
      transplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }

    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  delete z;

  if (originalColor == BLACK) {
    fixDelete(x, xParent); 
  }

  if (root != NULL) {
    root->color = BLACK;
  }
}
//main function used for all cases for fixing insertion after deletion (this was the hardest and most tedious function I ever did)
void RBTree::fixDelete(Node* node, Node* parent) {
  while (node != root && getColor(node) == BLACK) {
    if (parent == NULL) {
      break;
    }

    if (node == parent->left) {
      Node* sibling = parent->right;
      //chunks of case 2 where sibling is red
      if (getColor(sibling) == RED) {
        sibling->color = BLACK;
        parent->color = RED;
        rotateLeft(parent);
        sibling = parent->right;
      }
      //case 3 and 4
      if (getColor(sibling == NULL ? NULL : sibling->left) == BLACK && getColor(sibling == NULL ? NULL : sibling->right) == BLACK) {
        if (sibling != NULL) {
          sibling->color = RED;
        }
        node = parent;
        parent = node->parent;
      }
      else {
	//case 5
        if (getColor(sibling == NULL ? NULL : sibling->right) == BLACK) {
          if (sibling != NULL && sibling->left != NULL) {
            sibling->left->color = BLACK;
          }
          if (sibling != NULL) {
            sibling->color = RED;
            rotateRight(sibling);
          }
          sibling = parent->right;
        }
        //case 6 with outer child being red
        if (sibling != NULL) {
          sibling->color = parent->color;
        }
        parent->color = BLACK;
        if (sibling != NULL && sibling->right != NULL) {
          sibling->right->color = BLACK;
        }
        rotateLeft(parent);
        node = root;
      }
    }
    //i realize that I need more versions for the cases when node was right child so more in here
    else {
      Node* sibling = parent->left;
      //case 2
      if (getColor(sibling) == RED) {
        sibling->color = BLACK;
        parent->color = RED;
        rotateRight(parent);
        sibling = parent->left;
      }
      //case 3 and 4
      if (getColor(sibling == NULL ? NULL : sibling->right) == BLACK && getColor(sibling == NULL ? NULL : sibling->left) == BLACK) {
        if (sibling != NULL) {
          sibling->color = RED;
        }
        node = parent;
        parent = node->parent;
      }
      else {
	//case 5
        if (getColor(sibling == NULL ? NULL : sibling->left) == BLACK) {
          if (sibling != NULL && sibling->right != NULL) {
            sibling->right->color = BLACK;
          }          
          if (sibling != NULL) {
            sibling->color = RED;
            rotateLeft(sibling);
          }
          sibling = parent->left;
        }
        //case 6
        if (sibling != NULL) {
          sibling->color = parent->color;
        }
        parent->color = BLACK;

        if (sibling != NULL && sibling->left != NULL) {
          sibling->left->color = BLACK;
        }

        rotateRight(parent);
        node = root;
      }
    }
  }

  if (node != NULL) {
    node->color = BLACK;
  }
}
/*
I will use this as a guide for deletion cases when doing this:
Case 1: N is a new root
Case 2: Sibling is red -> rotate through parents, switch P and S colors, call case 3
Case 3: Sibling is black -> color S red, recursively call case 1 on P, otherwise we proceed to case 4
Case 4: P is red, and S and S's children is black -> color P black & S red, otherwise proceed to case 5
Case 5: S and S's left are black, S's right is red, and node is right, also S and S's right are black, S's left is red, and node is left -> rotate through S, change S to red and child to black, otherwise proceed to case 6
Case 6: S is black, S's left is red, and node is right OR S is black, S's right is red, and node is left //checking cases: currently around 5 work, and Case 6 is a bit glitchy
*/
