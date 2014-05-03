/* LL class as a springboard for list algorithms */

template <class T>
class LL {
  
 private:
  struct Node
  {
    Node* next;
    Node* parent;
    T value;
  };
  Node* root;
  
 public:
  //LL() {root->parent = NULL; root->next = NULL;}
  void insert(T);
  void remove(T);
  void print();
  T search(T);
  
  
  template<typename Func>
    void mapLL(Node*, Func);
  template<typename Func>
    T reduceLL(Node*, Func);

  //some manipulation functions:
  
  //sorting:
  

  //cycle detection


  //two-sum


  //three-sum


  //n-sum

  //what's that rivest algorithm ?




  bool isEmpty() const {return root == NULL;}

};
