#include <iostream>
#include <algorithm>
#include <time.h>

#include "LL.h"

template <class T>
void LL<T>::insert(T x){
  Node *newnode = new Node;
  newnode->value = x;
  newnode->next = NULL;

  Node *walker = root;
  if (root){
    while (walker->next) { walker = walker->next; }
    walker->next = newnode;
  }
  else
    root = newnode; 
}

template <class T>
void LL<T>::print(){
  Node* curr = root;
  while (curr){
    std::cout<<curr->value<<" ";
    curr=curr->next;
  }
  std::cout<<std::endl;
  
}

template <class T>
void LL<T>::remove(T x){

}

template <class T>
T LL<T>::search(T x){

}

int main(int argc, char** argv){
  
  //LL<double> *list = new LL<double>();
  LL<int> *list = new LL<int>();
  //LL<double> *list;


  if (argc > 1){
    std::cout << 
      "not sure how to make object instantiation use typeof()\n";
  }
  else{
    srand(time(NULL));
    for (int i=0; i<20; i++)
      list->insert(rand() % 100 + 1);
    
    list->print();
  }

  return 42;
}
