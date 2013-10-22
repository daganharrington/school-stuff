//Dagan Harrington
//CS 5100

//Program to compute cos(x) using the Maclaurin polynomial expansion.
//Terminates when 'infinite sum' and cmath's cos(x) fcn are 'equal'

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <float.h>

using namespace std;

int main(int argc, char** argv) {
  
    double x, term = 1.0, oldsum = 0.0, newsum = term;
    int i = 1;    
    cout << endl << "Maclaurin approximation of cos(x)";
    cout << endl << "x = ? : ";
    cin >> x;
    cout << setprecision(20) << setiosflags << (ios::fixed|ios::showpoint);
    cout << endl;
    int alternatingcount=1;
    
    while (oldsum!=newsum)
      {
      oldsum = newsum;
        term = (term * (x*x))/((i*2)*((i*2)-1));
        
        if (alternatingcount % 2 == 0)                                  
	  newsum = newsum + term;
        else
	  newsum = newsum - term;
        
        cout << setw(3) << i << "  TERM =  " << 
	  term << " SUM = " << newsum << endl;
        ++i;
        ++alternatingcount;
	
        
    }
    
    cout << endl << endl << " cos(x) = " << cos(x) << endl;

    return 0;    
}

