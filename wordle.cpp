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
set<string> generateWords(string in, const string floating, int pos);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
  set<string> words = generateWords(in, floating, 0);

  set<string> answer;
  for (set<string>::iterator it = words.begin(); it != words.end(); ++it) {
    // if the word exists in the set
    if (dict.find(*it) != dict.end()) answer.insert(*it);
  }
  return answer;
}

// Define any helper functions here
set<string> generateWords(string in, const string floating, int pos) {
    set<string> words = {};

    if (!in.length()) return words;

    unsigned int count = 0;
    for (unsigned int i = 0; i < in.length(); i++) {
      if (in[i] == '-') ++count;

    }
    if (count == 0) {
      words.insert(in);
      return words;
    }
    
    if (in[pos] == '-') {
      for (unsigned int i = 0; i < floating.length(); i++) {
        in[pos] = floating[i];
        string temp = floating.substr(0, i) + floating.substr(i+1);
        set<string> wordsTemp = generateWords(in, temp, pos+1);
        words.insert(wordsTemp.begin(), wordsTemp.end());
        in[pos] = '-';
      }
    
      if (floating.length() < count) {
        for (char c = 'a'; c <= 'z'; ++c) {
            in[pos] = c;
            set<string>wordsTemp = generateWords(in, floating, pos+1);
            words.insert(wordsTemp.begin(), wordsTemp.end());
            in[pos] = '-';
        }
      }
    }
    else {
      set<string> wordsTemp = generateWords(in, floating, pos+1);
      words.insert(wordsTemp.begin(), wordsTemp.end());
    }
  return words;
}