#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>
#include <cstdio>
using namespace std;
enum color {gray = -2, white = -1};
class Graph{
    int V;
    vector<int> *adj;
public:
    Graph(int V);  // Constructor
    void edge(int u, int v);// add edge <u,v> to graph
    void bfs(int s, int d);// find the shortest path from s to d
};

Graph *graph;

Graph::Graph(int V){
    this->V = V;
    adj = new vector<int>[V];
}

void Graph::edge(int u, int v){
    if(find(adj[u].begin(), adj[u].end(), v) == adj[u].end()) {
        adj[u].push_back(v); // Add v to u’s list.
    }
    if(find(adj[v].begin(), adj[v].end(), u) == adj[v].end()) {
        adj[v].push_back(u); // Add u to v’s list.
    }
}

void Graph::bfs(int s, int d){
    vector<int> node_color;
    vector<int> pi;
    vector<int> inversed_path;
    for (int j = 0; j < V; ++j) {
        node_color.push_back(white); // Label all the nodes as white
        pi.push_back(-5); //set the predecessor of each element as "-5", means no predecessor.
    }

    queue<int> q; // Create a queue for bfs
    node_color[s] = gray; // Label the current node as gray and enqueue it
    q.push(s);

    while(!q.empty()){
        int u = q.front();// Dequeue a node from the queue
        q.pop();
        for (unsigned int i = 0; i < adj[u].size(); ++i){
            if (node_color[adj[u][i]] == white){
                node_color[adj[u][i]] = gray;
                pi[adj[u][i]] = u;
                q.push(adj[u][i]);
            }
        }
    }
    if (d == s){
        cout << s << "-" << s << endl;
    }
    if (d >= V or d < 0){
        cerr << "Error: The destination does not exist. " << endl;
    }
    if (d >=0 and d < V and pi[d] == -5 and d != s){
        cerr << "Error: A path does not exist between them. " << endl;
    }
    if (d >= 0 and d < V and pi[d] != -5 and d != s){
        while (pi[d] != -5){
            inversed_path.push_back(d);
            d = pi[d];
        }
        inversed_path.push_back(s);
        for (int i = inversed_path.size()-1; i > 0; --i) {
            cout << inversed_path[i] << "-";
        }
        cout << inversed_path[0] << endl;
    }

}



int main(){
    while(!cin.eof()){
        string input;
        getline(cin, input);
//        cout << input <<"\n";
        int v;
        if (input.size() == 0){
            continue;
        }
        if (input.find("V") == 0) {
            v = stoi(input.substr(2));
            cout << input <<endl;
            delete graph;
            graph = new Graph(v);
        }
        if (input.find("E") == 0) {
            size_t find0 = input.find("<");
            cout << input <<endl;
            int flag = 0;
            while (find0 != string::npos) {
                size_t find1 = input.find(">", find0);
                string edge_temp = input.substr(find0 + 1, find1 - find0 - 1);
                size_t m = edge_temp.find(",");
                int e0 = stoi(edge_temp.substr(0, m));
                int e1 = stoi(edge_temp.substr(m + 1));
                if (e0 >= v or e1 >= v or e0 < 0 or e1 < 0) {
                    cerr << "Error: edges include vertices not exist." << endl;
                    flag = 1;
                    break;
                }
                find0 = input.find("<", find0 + 1);
            }
            if (flag == 0){
                size_t found0 = input.find("<");
                while (found0 != string::npos) {
                    size_t found1 = input.find(">", found0);
                    string edge_temp = input.substr(found0 + 1, found1 - found0 - 1);
                    size_t m = edge_temp.find(",");
                    int e0 = stoi(edge_temp.substr(0, m));
                    int e1 = stoi(edge_temp.substr(m + 1));
                    graph->edge(e0, e1);
                    found0 = input.find("<", found0 + 1);
                }
            }
        }
        if (input.find("s") == 0){
            size_t find2 = input.find(" ");
            size_t find3 = input.find(" ", find2 + 1);
            int source = stoi(input.substr(2, find3 - 1));
            int desti = stoi(input.substr(find3 + 1));
            graph->bfs(source, desti);
        }
    }
    return 0;
}

//V 5
//E {<0,2>,<2,1>,<2,3>,<3,4>,<4,1>
//s 4 0