/* tree class and definitions
   -dgn
 */
typedef int color;

template <class T>
class BST {

 private:
  struct Node
  {
    Node* l;
    Node* r;
    Node* parent; //why not
    T key; 
    color c; //for RB
    int num;
  };
  Node* root;



 public:
  BST() {root = NULL;}
  //~BST();
  void insert(T);           //O(h)
  void remove(T);           //O(h)          
  
  bool search(T);           //O(h)
  
  template<typename Func>
    void traverse_inorder(Node*, Func);
  template<typename Func>
    void traverse_preorder(Node*, Func);

  bool isEmpty() const {return root == NULL;}           //O(1)
  bool isLeaf(Node*);
  T minimum();
  T maximum();
  T succ();
  T pred();
  
  void print_inorder();
  void print_preorder();    //Theta(n)
  void print_postorder();   //Theta(n)

};
