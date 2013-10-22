//DAGAN HARRINGTON - DR. LYONS

/* 
 Give a detailed problem formulation for the following problem:\
 You have 3 jugs, measuring 12 gallons, 8 gallons and 3 gallons and a water faucet. 
 * You can fill jugs, empty them to other jugs or empty them out. 
 * You need to measure exactly one gallon.
 * 
 * (b) Use the problem formulation to write a program that 
 * generates the full search space, labeling
 *  any goal states.
 */

#include <cstdlib>
#include <iomanip>
#include <iostream>

#include <deque>
#include <list>
//#include <tuple>  ??????????

#define AMAX 12
#define BMAX 8
#define CMAX 3
#define RAWSPACE 468


using namespace std;

struct Node{
	int state[3]; //3slot-array- 0 is 12 gallon, 1 is 8 gallon, 2 is 3 gallon.
	Node *parent; 
	string action; //action applied to parent to generate current node object.	
	Node (){
		//null constructor
		for (int i = 0; i < 3; i++){
			state[i] = 0;
		}
        parent = NULL;
        action = "Nowhere";
	};
	
	Node (Node *nodetocopy, string act){//copy-ish constructor.
		state[0] = nodetocopy->state[0];
		state[1] = nodetocopy->state[1];
		state[2] = nodetocopy->state[2];
		parent = nodetocopy;
		action = act; //Action which produced the currently constructed Node.
	}
};

deque<Node> exploredset; //take out of global namespace?
deque<Node> fringe;

bool isExplored (Node &a)
{
	deque<Node>::iterator it; 
	bool b = 0;
	for (it = exploredset.begin(); it != exploredset.end(); it++)
	{
	  if ((a.state[0] == (*it).state[0]) &&
	      (a.state[1] == (*it).state[1]) &&
	      (a.state[2] ==(*it).state[2]))		
	    b = 1;
	}
	return b;
}

bool isFringe (Node &a)
{
  deque<Node>::iterator it;
  bool b = 0;
  for (it = fringe.begin(); it != fringe.end(); it++)
    {
      if ((a.state[0] == (*it).state[0]) &&
	  (a.state[1] == (*it).state[1]) &&
	  (a.state[2] ==(*it).state[2]))
	
	/*then*/		b = 1;
    }
  return b;
}

void isGoal(Node *n)
{
  cout << "Goal State : (" << n->state[0] << "-" << n->state[1] << "-" << n->state[2] << ") -- ARRIVED AT BY : " << endl;   
  while (n->parent != NULL)
    {
      cout << "(" << n->parent->state[0] << "-" << n->parent->state[1] << "-" << n->parent->state[2] << ") " << endl;
      n = n->parent;
    }
  cout << endl;
}

void FillA(Node *n)//Fill A from well
{
  Node a(n, "Fill A");	
  a.state[0] = AMAX;
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
  
}
void FillB(Node *n)//Fill B from well
{
  Node a(n, "Fill B");
  a.state[1] = BMAX;
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void FillC(Node *n)//Fill C from well
{
  Node a(n, "Fill C");
  a.state[2] = CMAX;
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void EmptyA(Node *n)//Empty A
{
  Node a(n, "Empty A");
  a.state[0] = 0;
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void EmptyB(Node *n)//Empty B
{
  Node a(n, "Empty B");
  a.state[1] = 0;
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void EmptyC(Node *n)//Empty C
{
  Node a(n, "Empty C");
  a.state[2] = 0;
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void TransferAtoB(Node *n)
{
  Node a(n, "Fill A to B");
  while (a.state[0]>0 && a.state[1]<BMAX) 
    { --a.state[0]; /*-----INTO----->*/ ++a.state[1];} //extremely awkward method
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void TransferAtoC(Node *n)
{
  Node a(n, "Fill A to C");
  while (a.state[0]>0 && a.state[2]<CMAX) 
    {  --a.state[0]; /*-----INTO----->*/ ++a.state[2];}
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void TransferBtoA(Node *n)
{
  Node a(n, "Fill B to A");
  while (a.state[1]>0 && a.state[0]<AMAX) 
    {--a.state[1]; /*-----INTO----->*/ ++a.state[0];}
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void TransferBtoC(Node *n)
{
  Node a(n, "Fill B to C");
  while (a.state[1]>0 && a.state[2]<CMAX) 
    { --a.state[1]; /*-----INTO----->*/ ++a.state[2];}
  if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}
void TransferCtoA(Node *n)
{
  Node a(n, "Fill C to A");
  while (a.state[2]>0 && a.state[0]<AMAX) 
    {  --a.state[2]; /*-----INTO----->*/ ++a.state[0];
      if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
    }
}
void TransferCtoB(Node *n)
{
    Node a(n, "Fill C to B");
    while (a.state[2]>0 && a.state[1]<BMAX) 
      {   --a.state[2]; /*-----INTO----->*/ ++a.state[1];}
    if (!isExplored(a) && !isFringe(a)) fringe.push_back (a);
}

int main() {
  
  Node root;//initialize with null constructor
  
  int r = 0;//temporary debuggish counter .. remove later
  
  fringe.push_back (root); //initialize the fringe with root node
  
    //?	; //something was here and now no compile .
  
	
  while (!fringe.empty())
    
    
    {
      cout << fringe.front().state[0] << "-" << fringe.front().state[1] << "-" << fringe.front().state[2] << " |||| "
	   << "[fringe size : <" << fringe.size() << ">---explored set size : <" << exploredset.size() << ">" << endl;
      
      bool f = isExplored(fringe.front());
      
      
      if (((fringe.front().state[0] == 1) ||
	   (fringe.front().state[1] == 1)||
	   (fringe.front().state[2] == 1))/* && (f = 0)*/) // AKA : is a goal state.
	{  
	  exploredset.push_back(fringe.front());
	  fringe.pop_front();
	  isGoal(&exploredset.back()); 	
	}//endif
      else 
	{ 
	  exploredset.push_back(fringe.front()); 
	  fringe.pop_front();
	  
	  //important to call functions with object in the explored set
	  //was calling fcns from fringe set...which removes and adds objects
	  //thus losing addresses.
          
	  //also important to use a deque or list for explored set
	  //as vector STL container moves things around (thus losing addresses)
	  //as it auto resizes things.
	  
	  
	  //do these if flags really save time by limiting function calls?  they could at least be internal.
	  if (exploredset.back().state[0] != 12) FillA(&exploredset.back());
	  if (exploredset.back().state[1] != 8 ) FillB(&exploredset.back());
	  if (exploredset.back().state[2] != 3 ) FillC(&exploredset.back());
	  if (exploredset.back().state[0]>0) EmptyA(&exploredset.back());
	  if (exploredset.back().state[1]>0) EmptyB(&exploredset.back());
	  if (exploredset.back().state[2]>0) EmptyC(&exploredset.back());
	  if (exploredset.back().state[0]>0 && exploredset.back().state[1]<BMAX) TransferAtoB(&exploredset.back());
	  if (exploredset.back().state[0]>0 && exploredset.back().state[2]<CMAX) TransferAtoC(&exploredset.back());
	  if (exploredset.back().state[1]>0 && exploredset.back().state[0]<AMAX) TransferBtoA(&exploredset.back());
	  if (exploredset.back().state[1]>0 && exploredset.back().state[2]<CMAX) TransferBtoC(&exploredset.back());
	  if (exploredset.back().state[2]>0 && exploredset.back().state[0]<AMAX) TransferCtoA(&exploredset.back());
	  if (exploredset.back().state[2]>0 && exploredset.back().state[1]<BMAX) TransferCtoB(&exploredset.back());
	  
	}
      
    }
  
  return 0;
}
