#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <string>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

double d[702];
unordered_map<string, bool> seen;

typedef pair<string, double> Node;
typedef vector<Node> Anavec;

Anavec getTopAnagrams(const string &word) {
    Anavec anagrams;
    anagrams.reserve(5);

    // Dijkstra
    struct NodeGreater {
        bool operator()(const Node& lhs, const Node& rhs) const {
            return lhs.second > rhs.second;
        }
    };
    priority_queue<Node, Anavec, NodeGreater> pq;

    for (char letter : word) {
        string sLetter = string(1, letter);
        pq.push(Node(sLetter, d[letter - 'a']));
    }

    while (anagrams.size() < 5 && !pq.empty()) {
        Node minNode = pq.top();
        pq.pop();

        if (seen[minNode.first]) {
            continue;
        }

        string subword = word;
        for (char letter : minNode.first) {
            string sLetter = string(1, letter);
            for (size_t i = 0, length = subword.length(); i < length; ++i) {
                if (subword.substr(i, 1) == sLetter) {
                    subword.erase(i, 1);
                    break;
                }
            }
        }

        if (subword == "") {
            anagrams.push_back(Node(minNode.first, minNode.second));
        } else {
            for (char letter : subword) {
                string newName = minNode.first + letter;

                // Goodness = d(e0) + d(e1|e0) + d(e2|e1) + ... + d(eT-1|eT-2)
                int length = newName.length();
                pq.push(Node(newName, minNode.second + d[(newName[length - 2] - 'a' + 1)*26 + (newName[length - 1] - 'a')]));
            }
        }

        seen[minNode.first] = true;
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

    return 0;
}
