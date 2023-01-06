// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
using namespace std;


//DO: if there are no floating characters
//instead of inserting floating characters maybe just produce
//all combinations then delete the ones that dont have floaitng characters

// Add prototypes of helper functions here
const size_t search (const set<string>& dict, string word);
void insertFloating (string in, string floating, int x, set<string>& results);
void alphabetCombos (string characters, string prefix, int k, set<string>& combos);
void insertAlphabet (set<string> combos, set<string>& results, int wordSize, int empty);

//string to hold alphabet
const string alphabet = "abcdefghijklmnopqrstuvwxyz";

//to store and return set of results
std::set<std::string> results; //?doesnt this die at the end?

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    //string to hold in word, editable to create result
    string out = in;
    // cout << "OUT WORD: " << out << endl;

    //inserting floating characters
   if (floating != ""){
    // cout << "nonempty floating" << endl;
    int x =0;
    insertFloating (out, floating, x, results);
   }
    
    if (floating == ""){
        // cout << "empty floating" << endl;
        results.insert(out);
    }

    int count = 0; //to count '-'
    for (int i=0; i<in.size(); i++){
        if (in[i] == '-'){
            count ++;
        }
    }

    //stores empty spots with floating letters included
    int empty = count - floating.size();
    
    //insert all combinations of alphabet
    std::set<std::string> combos;
    alphabetCombos(alphabet, "", empty, combos);

    insertAlphabet (combos, results, in.size(), empty);

    // for (auto s: results){
    //     cout << s << endl;
    // }

    //testing search
    // string wordz = "adz";
    // int hello = search (dict, wordz);
    // cout << "RESULT OF SEARCH: " << hello <<endl;

    // for (auto s: results){
    //     cout << "ORIGINAL: " << s << endl;
    // }

    int validity = 0;
    auto it = results.begin();
    while (it != results.end()){
        // cout << "word: " << *it << endl;
        validity = search(dict, *it);
        if (validity == -1){
            // cout << "erasing: " << *it << endl;
            it = results.erase(it);
        } else {
            ++it;
        }
    }

    // for (auto s : results){
    //     cout << "word: " << s << endl;
    //     // cout << "validity before: " << validity << endl;
    //     validity = search (dict, s);
    //     // cout << "validity after search: " << validity << endl;
    //     if (validity == -1){
    //         // cout << "ERASING!" <<endl;
    //         results.erase(s);
    //     } else {
    //         cout << "valid word: " << s << endl;
    //     }
    // }

    return results;
}

// Define any helper functions here
//to search dictionary for whether word is valid, returns position of word
const size_t search (const set<string>& dict, string word){
    auto pos = dict.find(word);

    int searchPosition = distance(dict.begin(), pos);

    if (pos != dict.end()){
        return searchPosition;
    } else {
        return -1;
    }
}

//inserts floating characters
void insertFloating (string in, string floating, int x, set<string>& results){
    if (x > floating.size() -1){ //gone through all floating characters
        return;
    }

    for (int i=0; i<in.size(); i++) {
        string next = in;
        if (in[i] == '-'){
            next[i] = floating[x];
            insertFloating(next, floating, x+1, results);
        } else {
            continue;
        }
        
        if (x == floating.size() - 1){ //inserted all floating characters
            results.insert(next);
        }
    }
}

void insertAlphabet (set<string> combos, set<string>& results, int wordSize, int empty){
    for (auto s : results){
        for (auto j : combos){
            string next = s;
            int n =0; //to go through combos characters
            for (int x =0; x<wordSize; x++){
                if (next[x] == '-'){
                    next[x] = j[n];
                    results.insert(next);
                    n++;
                }
            }
        }
    }
}

//creates all alphabet combos for given length k and stores them in combos set
void alphabetCombos (string characters, string prefix, int k, set<string>& combos){
    // cout <<"in function" << endl;
    
    if (k==0){
        combos.insert(prefix);
        return;
    }

    for (int i=0; i< characters.size(); i++){
        string newPrefix = prefix + characters[i];
        alphabetCombos (characters, newPrefix, k-1, combos);
    }
}