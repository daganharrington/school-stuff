/*
  BST (add balancing for fun)
  lambdas so compile with -std=c++0x
  -dgn
 */

#include <iostream>
#include <time.h>
using namespace std;

#include "tree.h"

int main(){
    
  BST<int> x;
  
  srand(time(NULL));
  for (int i=0; i<10; i++)
    //x.insert(rand() % 100);
    x.insert(i);

  x.print_inorder();

  x.print_preorder();
  
  x.flatten_inplace();

  x.print_preorder();
  
  x.print_inorder();
  return 0;
}


template <class T>
bool BST<T>::isLeaf(Node* n){
  return (n->l == NULL && n->r == NULL);
}


template <class T> 
void BST<T>::insert(T x){
  //insert x into tree
  //preserve BST invariant
  cout << " :: x = " << x << endl;
  Node* parent;
  Node* nn = new Node; 
  nn->l = NULL;
  nn->r = NULL;
  nn->parent = NULL;
  nn->key = x;

  if (isEmpty()) 
    root = nn;
  else{
    //start at rooot and walk bst style
    Node* current = root;
    cout << "Inserting " << nn->key << endl;
    while (current)
      {
	parent = current;
	if (nn->key < current->key)
	  current = current->l;
	else
	  current = current->r;
      }//upon exiting ... current will be null
    if (nn->key < parent->key)
      parent->l = nn;
    else
      parent->r = nn;
    
  }
}

template <class T>
template<typename Func>
void BST<T>::traverse_inorder(Node* n, Func f)
{
  if (n){
    if (n->l)
      traverse_inorder(n->l, f);
    //cout <<" " << n->key;
    f(n->key);
    if (n->r)
      traverse_inorder(n->r, f);
  }
}

template <class T>
template<typename Func>
void BST<T>::traverse_preorder(Node* n, Func f){
  if (n){
    f(n->key);
    if (n->l)
      traverse_preorder(n->l, f);
    if (n->r)
      traverse_preorder(n->r, f);
  }
}

template <class T>
void BST<T>::print_inorder(){
  //lambda prints
  cout << "Printing inorder : ";
  auto func = [] (T x) {  cout << " " << x;   };
  traverse_inorder(root, func);
  cout << endl;
  
}

template <class T>
void BST<T>::print_preorder(){
  cout << "Printing preorder : ";
  auto func = [] (T x) {  cout << " " << x;  };
  traverse_preorder(root, func);
  cout << endl;

}

template <class T>
void BST<T>::flatten_inplace(Node *n){
  //recursive would be nice
  cout << "flattening\n";
  cout << n->key << endl;
}
