#include <algorithm>
#include <array>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

double d[702];
array<char, 26> letterCounts;

typedef pair<string, double> Node;
typedef vector<Node> Anavec;

Anavec getTopAnagrams(const string &word) {
    Anavec anagrams;
    anagrams.reserve(5);
    int anagramSize = 0;
    const int wordLength = word.length();

    // Dijkstra
    struct NodeGreater {
        bool operator()(const Node& lhs, const Node& rhs) const {
            return lhs.second > rhs.second;
        }
    };
    priority_queue<Node, Anavec, NodeGreater> pq;

    for (char letter : word) {
        ++letterCounts[letter - 'a'];
    }

    for (int i = 0; i < 26; ++i) { 
        if (letterCounts[i] > 0) {
            Node tempNode(string(1, (char)(i + 'a')), d[i]);
            pq.push(move(tempNode));
        }
    }

    while (anagrams.size() < 5 && !pq.empty()) {
        Node minNode = pq.top();
        pq.pop();

        if (minNode.first.length() == wordLength) {
            anagrams.push_back(move(Node(minNode.first, minNode.second)));
        } else {
            array<char, 26> tempLetterCounts = letterCounts;

            for (char letter : minNode.first) {
                --tempLetterCounts[letter - 'a'];
            }

            for (int i = 0; i < 26; ++i) {
                if (tempLetterCounts[i] > 0) {
                    string newName = minNode.first + char(i + 'a');
                    int length = newName.length();

                    // Goodness = d(e0) + d(e1|e0) + d(e2|e1) + ... + d(eT-1|eT-2)
                    Node tempNode(newName, minNode.second + d[(newName[length - 2] - 'a' + 1)*26 + (newName[length - 1] - 'a')]);
                    pq.push(move(tempNode));
                }
            }
        }
    }

    sort(anagrams.begin(), anagrams.end(), [&](const Node &a, const Node &b)->bool{ return a.second < b.second; });
    return anagrams;
}

int main(int argc, char** argv) {
    // Get command line argument
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " WORD" << endl;
        return -1;
    }
    string input = argv[1];
    cout << fixed;
    cout << setprecision(15);

    // Collect entropies
    ifstream file("unibigram_entropies.txt");
    string line;
    while (file >> line) {
        for (size_t i = 0, length = line.length(); i < length; ++i) {
            if (line.substr(i, 1) == ",") {
                double prob = stod(line.substr(i + 1, length - i));

                if (i == 1) {
                    // Unigram
                    d[line[0] - 'a'] = prob;
                } else {
                    // Bigram
                    d[(line[0] - 'a' + 1)*26 + (line[1] - 'a')] = prob;
                }

                break;
            }
        }
    }

    // Get results
    Anavec results = getTopAnagrams(input);
    for (Node result : results) {
        cout << result.first << " " << result.second << endl;
    }

    quick_exit(0);
}
