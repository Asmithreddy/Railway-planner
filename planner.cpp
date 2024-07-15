#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>
#include <algorithm>
#include <set>
using namespace std;

struct Train {
    string name;
    int rating;
    int departureTime;
};

struct Station {
    string name;
    vector<Train> departures;
};

unordered_map<string, Station> stations;
unordered_map<string, vector<pair<string, int>>> graph; // For graph algorithms
void sortDepartures(vector<Train>& departures) {
    sort(departures.begin(), departures.end(), [](Train& a, Train& b) {
        return a.departureTime < b.departureTime;
    });
}

void listDepartures(const string& stationName) {
    if (stations.find(stationName) != stations.end()) {
        sortDepartures(stations[stationName].departures);
        for (const auto& train : stations[stationName].departures) {
            cout << "Train: " << train.name << ", Departure: " << train.departureTime << endl;
        }
    } else {
        cout << "Station not found." << endl;
    }
}
vector<Train> getTopRatedTrains(const string& stationName, int topN) {
    priority_queue<pair<int, Train>> pq;
    for (const auto& train : stations[stationName].departures) {
        pq.push({train.rating, train});
        if (pq.size() > topN) {
            pq.pop();
        }
    }
    vector<Train> topTrains;
    while (!pq.empty()) {
        topTrains.push_back(pq.top().second);
        pq.pop();
    }
    reverse(topTrains.begin(), topTrains.end());
    return topTrains;
}
class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    TrieNode() : isEndOfWord(false) {}
};

class Trie {
public:
    TrieNode* root;
    Trie() { root = new TrieNode(); }

    void insert(const string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (!node->children[c]) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEndOfWord = true;
    }

    vector<string> search(const string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (!node->children[c]) {
                return {};
            }
            node = node->children[c];
        }
        vector<string> results;
        findWords(node, prefix, results);
        return results;
    }

private:
    void findWords(TrieNode* node, string currentWord, vector<string>& results) {
        if (node->isEndOfWord) {
            results.push_back(currentWord);
        }
        for (auto& child : node->children) {
            findWords(child.second, currentWord + child.first, results);
        }
    }
};

Trie stationTrie;
void autoComplete(const string& prefix) {
    vector<string> suggestions = stationTrie.search(prefix);
    for (const string& suggestion : suggestions) {
        cout << suggestion << endl;
    }
}
vector<int> computeLPSArray(const string& pat) {
    int M = pat.size();
    vector<int> lps(M);
    int len = 0;
    lps[0] = 0;

    int i = 1;
    while (i < M) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

void KMPSearch(const string& pat, const string& txt) {
    int M = pat.size();
    int N = txt.size();

    vector<int> lps = computeLPSArray(pat);

    int i = 0, j = 0;
    while (i < N) {
        if (pat[j] == txt[i]) {
            j++;
            i++;
        }

        if (j == M) {
            cout << "Found pattern at index " << i - j << endl;
            j = lps[j - 1];
        } else if (i < N && pat[j] != txt[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
}
void addEdge(const string& station1, const string& station2, int weight) {
    graph[station1].push_back({station2, weight});
    graph[station2].push_back({station1, weight});
}

void findConnectingTrains(const string& start, const string& end) {
    set<string> visited;
    queue<string> q;
    q.push(start);

    while (!q.empty()) {
        string station = q.front();
        q.pop();
        if (station == end) {
            cout << "Found connection to " << end << endl;
            return;
        }
        visited.insert(station);

        for (const auto& neighbor : graph[station]) {
            if (visited.find(neighbor.first) == visited.end()) {
                q.push(neighbor.first);
            }
        }
    }
    cout << "No connection found." << endl;
}
int main() {
    // Populate stations, trains, and graph
    // Example:
    stations["StationA"].departures = {{"Train1", 5, 900}, {"Train2", 4, 930}};
    stations["StationB"].departures = {{"Train3", 5, 1000}, {"Train4", 3, 1030}};
    stationTrie.insert("StationA");
    stationTrie.insert("StationB");

    addEdge("StationA", "StationB", 1);

    listDepartures("StationA");
    autoComplete("Sta");
    KMPSearch("pattern", "text containing the pattern");
    findConnectingTrains("StationA", "StationB");

    return 0;
}
