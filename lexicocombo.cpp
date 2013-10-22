//Dagan Harrington
//CS 5100 / Strzemecki / HW : 'Generating Combinations'
/*
 * Generate the r-combinations of an ordered, ordinal array [a1,a2,..,an]
 * in lexicograpical order.
 * 
 * user specifies n value and r value and combinations are printed
 * 
 
 */


#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <cmath>


using namespace std;

void generateCombinations(int narray[], int n, int r);
int factorial(int n);


int main(int argc, char** argv) {
    int n, r;
    
    cout << "Generate r-combinations of of size n and display in lexicographical order" << endl;
    cout << "n = "; cin >> n;
    cout << "r = "; cin >> r;
    
    int narray[n];
    for (int i = 1; i <= n; i++)    //populate array : [1,2,..,n]
        narray[i] = i;
    
    generateCombinations(narray, n, r);
    
    
    
    
    return 0;
}
void generateCombinations(int narray[], int n, int r)
{
    int numofcombos = factorial(n)/(factorial(r)*factorial(n-r));
    int rarray[r];
    
    for (int i = 1; i <= r; i++)//prints the first, trivial combination and populates rarray
    {
        rarray[i] = narray[i];
        cout << rarray[i] << " ";
    }
    cout << endl;
    
;
    
    
    for (int elem = 2; elem <= numofcombos; elem++)
    {
        int i = r;
        for (i=r; i >= 1; i--)
        {
            if (rarray[i]!=(n - r + i))
                {
                rarray[i] = ++rarray[i];
                break;
                }
            
            
            
        }
        
        for (int j = i + 1; j <= r; j++)
            {
                rarray[j] = rarray[j-1]+1;
                //rarray[j+1] = rarray[j] + 1; //doesn't really work here
            }
        
        
                       
        for (int m = 1; m <= r; m++)
                cout << rarray[m] << " ";
        cout << endl;
    }
    
 }
int factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}