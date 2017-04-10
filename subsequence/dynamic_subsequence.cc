/* Program: dynamic_subsequence.cc
 * Author: Eric Keep
 *
 * This program uses a tabular dynamic programming method to find the maximum possible
 * contiguous subsequence of a sequence of numbers
 *
 * This is accomplished in something between O(n) and O(n^2) complexity by using two tables of values
 * of best possible subsequences that are based on previous sums
 * 
 * No_i is the best subsequence from 0 to i that doesn't include i
 * Contains_i is the best subsequence from 0 to i that does contain i
 * The best sum is the higher value of the ends of these vectors 
 *
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <limits.h>
#include <cstdlib>
#include <algorithm>
using namespace std;

int main(int argc, char* argv[]){
  
  vector <int> inp_numbers;
  // highest sum of a subsequence that is between 0 and i and includes i
  vector <int> Contains_i;
  // highest sum of a subsequence that is between 0 and i and doesn't include i
  vector <int> No_i;

  // indices of this 'best' subsequence
  int best_i, best_j;

  // file input
  if (argc > 1){
    ifstream file_in;
    file_in.open(argv[1]);
    if (file_in.fail()){
      cout << "Failed to open file: " << argv[1] << endl;
      exit(1);
    }
    while (!file_in.eof()){
      int x;
      file_in >> x;
      if (!file_in.fail()) {
	inp_numbers.push_back(x);
      }
    }
    file_in.close();    
  }

  else{
    cout << "Please enter a sequence of numbers: " << endl;
    string seq;
    getline(cin, seq);
    stringstream string_nums(seq);
    int x;
    while (string_nums >> x){
      inp_numbers.push_back(x);
    }
  }
  No_i.resize(inp_numbers.size());
  Contains_i.resize(inp_numbers.size());

  // initialize the first values of both vectors
  // No_i is 0 since it can't hold any values
  // Contains_i is the first value of the input sequence
  No_i[0] = 0;
  Contains_i[0] = inp_numbers[0];

  // base case - initial number is best
  best_i = 0; best_j = 0;

  for (int i = 1; i < inp_numbers.size(); i++){
    // the best subsequence not including i either contains i - 1 or does not
    No_i[i] = max(No_i[i-1], Contains_i[i-1]);
    // if i is a part of the subsequence, it either contains i - 1 subseq or just itself
    // note that it should contain the highest value sum up to i - 1 if it includes i - 1
    Contains_i[i] = max(Contains_i[i-1] + inp_numbers[i], inp_numbers[i]);
    // if the current value is higher than any subsequence thus far, it becomes our new k
    if (Contains_i[i] == inp_numbers[i] && Contains_i[i] > No_i[i]){
      best_i = i;
      best_j = i;
    }
    // if the current i makes the subsequence bigger, mark it as new j
    if (Contains_i[i] > No_i[i])
      best_j = i;
  }

  // output
  cout << "Best sum: ";
  cout << max(Contains_i[inp_numbers.size() - 1], No_i[inp_numbers.size() - 1]) << endl;

  cout << "From " << best_i + 1 << " to " << best_j + 1 << endl;
  cout << "Sequence: " << endl;
  for (int i = best_i; i <= best_j; i++){
    cout << inp_numbers[i] << " ";
  }
  cout << endl;

  return 0;
}
