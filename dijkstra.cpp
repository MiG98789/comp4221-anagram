#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <queue>
#include <utility>
#include <vector>

using namespace std;

map<string, double> d;

typedef pair<string, double> Node;
typedef vector<Node> Anavec;

Anavec getTopAnagrams(const string &word) {
    Anavec anagrams;

    // Dijkstra
    struct NodeGreater {
        bool operator()(const Node& lhs, const Node& rhs) const {
            return lhs.second > rhs.second;
        }
    };
    priority_queue<Node, Anavec, NodeGreater> pq;

    for (char letter : word) {    
        string sLetter = string(1, letter);
        pq.push(Node(sLetter, d[sLetter]));
    }

    while (anagrams.size() < 5 && !pq.empty()) {
        Node minNode = pq.top();
        pq.pop();

        string subword = word;
        for (char letter : minNode.first) {
            string sLetter = string(1, letter);
            for (size_t i = 0, length = subword.length(); i < length; i++) {
                if (subword.substr(i, 1) == sLetter) {
                    subword.erase(i, 1);
                    break;
                }
            }
        }

        if (subword == "") {
            Node toInsert(minNode.first, minNode.second);
            if (find(anagrams.begin(), anagrams.end(), toInsert) == anagrams.end()) {
                anagrams.push_back(toInsert);
            }
        } else {
            for (char letter : subword) {
                string newName = minNode.first + letter;

                // Goodness = d(e0) + d(e1|e0) + d(e2|e1) + ... + d(eT-1|eT-2)
                int length = newName.length();
                pq.push(Node(newName, minNode.second + d[newName.substr(length - 1, 1) + "|" + newName.substr(length - 2, 1)]));
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
        for (size_t i = 0, length = line.length(); i < length; i++) {
            if (line.substr(i, 1) == ",") {
                d[line.substr(0,i)] = stod(line.substr(i + 1, length - i));
                break;
            }
        }
    }

    // Get results
    Anavec results = getTopAnagrams(input);
    cout << results[0].first << " " << results[0].second << endl;
    cout << results[1].first << " " << results[1].second << endl;
    cout << results[2].first << " " << results[2].second << endl;
    cout << results[3].first << " " << results[3].second << endl;
    cout << results[4].first << " " << results[4].second << endl;

    return 0;
}
