#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void recursiveWordFind(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict,
    std::set<std::string>& wordList);

bool floatingConstraint(const std::string& in, const std::string& floating, int blanks);


// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    //int index = in.find_first_of('-');
    //int length = in.size();
    std::string word = in;
    std::set<std::string> list;

    //std::cout << "index of -: " << index << std::endl;
    //std::cout << "length of in: " << length << std::endl;
    //std::cout << "size of wordList: " << wordList.size() << std::endl;
    
    if (in == "") {
      return list;
    }

    recursiveWordFind(in, floating, dict, list);
    return list;

}

// Define any helper functions here
void recursiveWordFind(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict, 
    std::set<std::string>& wordList)
{
    int length = in.size();
    
    int index = in.find_first_of('-');
    std::string word = in;

    //BASE CASE: if the word is "complete" (has no -) then 
    if (index == -1) {
        //std::cout << "end of recursion" << std::endl;
        string tempWord = word;
        //check if word satisfies floating
        for (unsigned int j = 0; j < floating.size(); j++) {
            char c = floating[j];
            //if char c is NOT in word then break
            if (tempWord.find(c) == std::string::npos) {
                //std::cout << "word does not satisfy floating" << std::endl;
                return;
            }
            else {
              tempWord.erase(tempWord.find_first_of(c), 1);
            }
            
        }
        //if word is NOT in dict then break
            if (dict.count(word) == 0) {
                //std::cout << "word not in dict" << std::endl;
                return;
            }

        //a valid word satisfying all conditions is found
        wordList.insert(word);
        //std::cout << "word found: " << word << std::endl;
        return;
    }

    //RECURSIVE CASE: generates all possible word values for each - 

    int blanks = 0;
    for (int i = 0; i < length; i++) {
      if (in[i] == '-') {
        blanks++;
      }
    }
    //std::cout << "blanks: " << blanks << std::endl;

    for (int i = 0; i < 26; i++) {
        char c = ('a' + i);
        string s(1, c);
        word = word.replace(index, 1, s);
        //std::cout << c << std::endl;
        //std::cout << word << std::endl;

        //only run to next iteration if there exists a potential solution
        if (floatingConstraint(word, floating, blanks)) {
          recursiveWordFind(word, floating, dict, wordList);
        }
    }

}

bool floatingConstraint(const std::string& in, const std::string& floating, int blank) {
  unsigned int found = 0;

  for (unsigned int i = 0; i < floating.size(); i++) {
    //if char i is in in string
    if (in.find(floating[i]) != std::string::npos) {
      //std::cout << "found char " << in[i] << std::endl; 
      found++;
    }
  }

  //if size of letters needed is greater than blanks + found
  if (blank + found < floating.size()) {
    return false;
  }

  //if number of blanks is less than letters needed
  return true;
}
