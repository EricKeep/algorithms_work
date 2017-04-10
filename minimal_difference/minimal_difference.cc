/* minimal_difference.cc
 *
 * This program uses dynamic programming to find the best way to split up
 * an even-sized group of numbers into two groups of equal size and the smallest difference in sum.
 * Written by Eric Keep - heavily modified from code by David Juedes
 * Oct. 5, 2016
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <climits>

using namespace std;

int main(int argc, char* argv[]) {

  // Read in the input weights.
  vector<int> weights;

  int sum_weight = 0;
  if (argc > 1){
    ifstream file_in;
    file_in.open(argv[1]);
    if (file_in.fail()){
      cout << "Failed to open file: " << argv[1] << endl;
      exit(-1);
    }
    while (!file_in.eof()){
      int x;
      file_in >> x;
      if (!file_in.fail()){
	weights.push_back(x);
	sum_weight += x;
      }
    }
  }
  else{
    cout << "Please enter a sequence of numbers: " << endl;
    string seq;
    getline(cin, seq);
    stringstream string_nums(seq);
    int x;
    while (string_nums >> x){
      weights.push_back(x);
      sum_weight += x;
    }
  }
  // set of indices for the split groups
  set<int> setA;
  set<int> setB;
  //best difference in weight of team 1 from the average
  int best_diff = INT_MAX;
  int sumA;
  int sumB;

  // error for odd team size
  if (weights.size()%2 !=0) {
    cout << "Error: input sequence is not of an even size" << endl;
    exit(-1);
  } else {
    set<int> sum1;
    set<int> sum2;
    int group_size = weights.size()/2; // size of split groups
    int biggest = sum_weight/2; // best case - we end up with two group with equal weights - only true if weight of team is half of sum of weights.
    // we can look for a team that can make the weight closest to this value - other group will be a little bit over this best case value (or equal).

    /* total_weights[i][j][t] is a boolean array indicating whether or not you can create a group of size j from values
     * in weights from 1 to i (weights actually starts at index 0 - we'll compensate for that in calcuations)
     * such that the sum of all the values is equal to t
     * Start with a closed world assumption, filling in true values for base cases.
     */
    bool total_weights[weights.size() + 1][group_size + 1][biggest + 1] = { false } ;
    
    // no matter how many numbers we have, we can always form a team of 0 that weighs 0 pounds
    for (int i = 1; i <= weights.size(); i++){
      total_weights[i][0][0] = true;
    }
    
    // fills up other values of the table, based on knowing that the current sum can either be or
    // not be made from the last element of the set of numbers (represented by i)
    for (int i = 1; i <= weights.size(); i++){ // i = 2 to weights.size() or last number
      for (int j = 1; j <= group_size; j++){ // j = 1 to half the size of the list of numbers
	for (int t = 1; t <= biggest; t++){ // t = 1 to best group weight (half of sum)
	  if (t - weights[i-1] < 0) // prevents negative indexing 
	    total_weights[i][j][t] = total_weights[i-1][j][t];
	  else{
	    total_weights[i][j][t] = total_weights[i-1][j][t] || total_weights[i-1][j-1][t - weights[i-1]];  				
	  }  // end else
	} // end for
      } // end for
    } // end for
    
    // look for the weight closest to half that can be built
    int best = -1; // best weight
    for (int t = 1; t <= biggest; t++){
      if (total_weights[weights.size()][group_size][t] == true && (biggest - t < best_diff)){
	best_diff = biggest - t;
	best = t;
      }
    }
    if (best == -1){
      cout << "Error finding weight" << endl;
      exit(-1);
    }

    //this loop finds the set of numbers that gets this best weight and saves them in sum1
    int b = best;
    int i = weights.size();
    int j = group_size;
    while (b >= 0 && i > 0 && j >= 0){
      // if part of list, add to sum1
      if(total_weights[i-1][j-1][b - weights[i-1]]){
	if(b != 0){
	  sum1.insert(i-1);
	}
	b -= weights[i-1]; i--; j--;
      }
      //if not, decrease i to move on and find next student
      else if(total_weights[i-1][j][b]){
	i--;
      }
      //exits loop for base case
      else if(b == 0 && j == 0)
	break;
    }
    
    setA = sum1;
    // puts every other student in teamB
    for(int i = 0; i < weights.size(); i++){
      if(setA.find(i) == setA.end())
	setB.insert(i);
    }
    sumA = best;
    sumB = sum_weight - best;
  }
  //output of the best setup
  cout << "Best team assignment (by weight) = " << endl;
  cout << "Team 1:" <<endl;
  //sums weights of each team
  int weightA = 0;
  int weightB = 0;
  for (set<int>::iterator p = setA.begin();p!=setA.end();++p) {
    cout << *p+1 << "(" << weights[*p] << ") ";
    weightA += weights[*p];
  }
  cout << endl;
  if (sumA != weightA){
    cout << "Error finding weights - team A" << endl;
    exit(-1);
  }
  cout << "Total weight = " << weightA << endl;
  cout << "Team 2:" <<endl;
  for (set<int>::iterator p = setB.begin();p!=setB.end();++p) {
    cout << *p+1 << "(" << weights[*p] << ") ";
    weightB += weights[*p];
  }
  cout << endl;
  if (sumB != weightB){
    cout << "Error finding weights - team A" << endl;
    exit(-1);
  }
  cout << "Total weight = " << weightB << endl;
}
