#include <iostream>
#include <algorithm>
#include <string>
#include <cctype>

using namespace std;

bool _alpha(char c){  return ((!isalpha(c) && !isdigit(c))); }

struct _alpha {
     bool operator() (const char& c) 
       { return ((!isalpha(c) && !isdigit(c))); }    
};

bool isPal (string s){
  cout << s << endl;
  //map lowercase over string and keep only alphas & nums
  s.erase(remove_if(s.begin(), s.end(), _alpha),s.end());	//using funct as pred
  //s.erase(remove_if(s.begin(), s.end(), _alpha()), s.end());  //using struct as pred

  cout << s << endl;
  for (int i=0; i<s.length(); i++)
    if (isalpha(s[i]))
      s[i]=tolower(s[i]);
  return (equal (s.begin(), 
		 s.begin() + s.size()/2, s.rbegin()));
}

int main (int argc, char **argv){
  if (argc != 2)
    return 42;

  cout << isalpha(' ') << endl;
  
  string ss = argv[1];
  if (isPal(ss))
    cout << "isPal\n";
  else
    cout << "nope\n";
  
  return 0;
}
