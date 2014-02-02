#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <bitset>


class Solution {


public:
  //this is a powerset function
  std::vector<std::vector<int> > subsets(std::vector<int> &S) {
    
    std::vector<int> item;
    std::vector<std::vector<int> > results;
    //std::bitset<size> perms; //permutations of S.size()
    std::cout<<pow(2,S.size())<<std::endl;
    
    
    for (unsigned int i = 0; i<pow(2,S.size()); i++){
      item.clear();
      for (int x = S.size(); x>=0; x--){
	//for 1 leftshifted by 0,1,2,3...
	//and it with i
	//for every occurrence=a of the &, add S[a] to item
	if (i & (1<<x)){
	  std::cout<<"adding S["<<x<<"]="<<S[x]<<std::endl;
	  item.push_back(S[x]);
	}
      }

      std::cout<<std::endl;
      //sort with lambda
      std::sort(item.begin(), item.end(),
		[] (int x, int y) { return (x<y); });
      results.push_back(item);
    }
					       
    return results;
    
  }
  
};

int main(){

  std::vector<std::vector<int> > results;
  std::cout<<"powerzetz"<<std::endl;
  std::vector<int> S;
  
  for(int i=0; i<3; i++)
    S.push_back(i+1);
  for(int i=0; i<3; i++)
    std::cout<<S[i]<<" ";
  std::cout<<std::endl;

  Solution sol;
  results=sol.subsets(S);
  
  for (std::vector<std::vector<int> >::iterator it=results.begin();
       it!=results.end(); it++){
    for (std::vector<int>::iterator it2=it->begin(); it2!=it->end();
	 it2++)
      std::cout << *it2 << " ";
    std::cout<<std::endl;
  }

  return 0;
}
