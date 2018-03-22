#include <algorithm>
#include <cctype>
#include <limits>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
using namespace std;

map<string, int> c;
map<string, double> p;
map<string, double> d;

bool isAlphabet(string &word) {
    for (string::iterator it = word.begin(); it != word.end(); it++) {
        if (!isalpha(*it)) {
            return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    // Collect unigram and bigram counts from dictionary 
    map<string, int> c;
    ifstream dictFile("linux_words_for_assignment");
    string line;
    
    while (dictFile >> line) {
        if (isAlphabet(line)) {
            transform(line.begin(), line.end(), line.begin(), ::tolower);

            // Unigram
            for (int i = 0; i < line.length(); i++) {
                string unigram = line.substr(i, 1);
                map<string, int>::const_iterator it = c.find(unigram);
                if (it == c.end()) {
                    c[unigram] = 1;
                } else {
                    c[unigram] += 1;
                }
            }
            
            // Bigram
            for (int i = 0; i < line.length() - 1; i++) {
                string bigram = line.substr(i, 2);
                map<string, int>::const_iterator it = c.find(bigram);
                if (it == c.end()) {
                    c[bigram] = 1;
                } else {
                    c[bigram] += 1;
                }
            }
        }
    }

    // Calculate unigram and bigram probabilities
    // Unigram
    map<string, double> p;
    int totalUnigramCount = 0;
    
    for (int i = 0; i < 26; i++) {
        totalUnigramCount += c[string(1, 'a' + i)];
    }

    for (int i = 0; i < 26; i++) {
        string unigram(1, 'a' + i);
        p[unigram] = 1.0*c[unigram]/totalUnigramCount;
        d[unigram] = -log2(p[unigram]);
    }

    // Bigram
    map<string, int> totalBigramCount;
    for (int i = 0; i < 26; i++) {
        string firstBigram(1, 'a' + i);
        int count = 0;
        for (int j = 0; j < 26; j++) {
            string secondBigram(1, 'a' + j);
            string bigram = firstBigram + secondBigram;
            count += c[bigram];
        }
        totalBigramCount[firstBigram + "0"] = count;
    }
    
    for (int i = 0; i < 26; i++) {
        string firstBigram(1, 'a' + i);
        for (int j = 0; j < 26; j++) {
            string secondBigram(1, 'a' + j);
            string bigram = firstBigram + secondBigram;
            string condProb = secondBigram + "|" + firstBigram;
            p[condProb] = 1.0*c[bigram]/totalBigramCount[firstBigram + "0"];
            d[condProb] = -log2(p[condProb]);
        }
    }
    
    // Store probabilities in a file
    // Format: unigram/bigram, probability
    ofstream probFile;
    probFile.open("unibigram_entropies.txt");
    for (map<string, double>::iterator it = d.begin(); it != d.end(); it++) {
        ostringstream oss;
        oss << fixed;
        oss << setprecision(15);
        oss << it->first << "," << it->second << endl;
        string output = oss.str();
        probFile << output;
    }
    probFile.close();

    return 0;
}
