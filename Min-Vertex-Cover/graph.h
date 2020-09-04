#include<vector>
#include<queue>
#include<set>
#include<random>
#include<algorithm>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

using std::default_random_engine;
using std::uniform_int_distribution; 

std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
/*
The "Graph" class is an abstract for an input graph. The main structure is described as follow:
Attributes:
    vert_num: Store the total number of vertices;
    adjacency_list: Store the input graph with a form of adjancy list;
    edges: Store the edge information for computing minimum vertex cover.
Functions:
    Generate(): Generate the adjacency list with an input vector which stores vertices as pairs;
    CNF_SAT_VC(): Compute the min-cover problem by CNF-SAT;
    APPROX_VC_1: Compute the min-cover problem by APPROX_VC_1;
    APPROX_VC_2: Compute the min-cover problem by APPROX_VC_2;
*/
template <class T>
class Graph {
    public:
        Graph();
        int vert_num;
        std::vector<std::vector<T>> adjacency_list;
        std::vector<std::vector<T>> edges;
        ~Graph() { adjacency_list.clear(); }
        void Generate(std::vector<std::vector<T>> vertex);
        std::vector<int> CNF_SAT_VC();
        std::vector<int> APPROX_VC_1();
        std::vector<int> APPROX_VC_2();
};

template <class T>
Graph<T>::Graph() {
    vert_num = 0;
    adjacency_list.resize(1);
    edges.resize(1);
}

template <class T>
void Graph<T>::Generate(std::vector<std::vector<T>> vertex) {
    // If the list is not empty, then clear the list to regenerate the graph.
    if(adjacency_list.size() != 0) {
        adjacency_list.clear();
        edges.clear();
    }
    adjacency_list.resize(vert_num);
    edges = vertex;
    for(unsigned i = 0; i < vertex.size(); ++i) {
        T v1 = vertex[i][0];
        T v2 = vertex[i][1];
        if(v1 >= vert_num || v2 >= vert_num) {
            std::cerr << "Error: Vertex in E doesn't exist in V!\n";
            adjacency_list.clear();
            break;
        }
        adjacency_list[v1].push_back(v2);
        adjacency_list[v2].push_back(v1);
    }
}

template <class T>
std::vector<int> Graph<T>::CNF_SAT_VC() {
    std::vector<int> cover;
    for(int k = 1; k <= vert_num; ++k) {
        solver.reset (new Minisat::Solver());;
        Minisat::Lit prop[vert_num][k];
        Minisat::vec<Minisat::Lit> clause;

        // Initialize n x k atomic propositions.
        for(int i = 0; i < vert_num; ++i) {
            for(int j = 0; j < k; ++j) {
                pthread_testcancel();
                prop[i][j] = Minisat::mkLit(solver->newVar());
            }
        }
        // At least one vertex is the ith vertex in the vertex cover.
        for(int j = 0; j < k; ++j) {   
            for(int i = 0; i < vert_num; ++i) {
                pthread_testcancel();
                clause.push(prop[i][j]);
            }
            solver->addClause(clause);
            clause.clear();
        }
        // No one vertex can appear twice in a vertex cover.
        for(int m = 0; m < vert_num; ++m) {
            for(int p = 0; p < k - 1; ++p) {
                for(int q = p + 1; q < k; ++q) {
                    pthread_testcancel();
                    solver->addClause(~prop[m][p], ~prop[m][q]);
                }
            }
        }
        // No more than one vertex appears in the mth position of the vertex cover.
        for(int m = 0; m < k; ++m) {
            for(int p = 0; p < vert_num - 1; ++p) {
                for(int q = p + 1; q < vert_num; ++q) {
                    pthread_testcancel();
                    solver->addClause(~prop[p][m], ~prop[q][m]);
                }
            }
        }
        // Every edge is incident to at least one vertex in the vertex cover.
        for(unsigned e = 0; e < edges.size(); ++e) {
            int i = edges[e][0];
            int j = edges[e][1];
            for(int x = 0; x < k; ++x) {
                pthread_testcancel();
                clause.push(prop[i][x]);
                clause.push(prop[j][x]);
            }
            solver->addClause(clause);
            clause.clear();
        }
        // If there is a solution then store the result and terminate.
        if(solver->solve()) {
            for(int i = 0; i < vert_num; ++i) {
                for(int j = 0; j < k; ++j) {
                    if(solver->modelValue(prop[i][j]) == Minisat::l_True) {
                        cover.push_back(i);
                    }
                }
            }
            break;
        }
    }
    pthread_testcancel();
    return cover;
}

template <class T>
std::vector<int> Graph<T>::APPROX_VC_1() {
    std::vector<std::vector<T>> S(adjacency_list);
    std::vector<int> cover;
    while(true) {
        int max_degree = 0;
        int cand = 0;
        for(int i = 0; i < S.size(); ++i) {
            if(S[i].size() > max_degree) {
                max_degree = S[i].size();
                cand = i;
            }
        }
        if(S[cand].empty()) {
            break;
        }
        cover.push_back(cand);
        S[cand].clear();
        for(int j = 0; j < S.size(); ++j) {
            S[j].erase(std::remove(std::begin(S[j]), std::end(S[j]), cand), std::end(S[j]));
        }
    }
    std::sort(cover.begin(), cover.end());
    return cover;
}

template <class T>
std::vector<int> Graph<T>::APPROX_VC_2() {
    default_random_engine e;
    std::vector<std::vector<T>> E(edges);
    std::vector<int> cover;
    while(!E.empty()) {
        uniform_int_distribution<int> u(0, E.size() - 1);
        int cand = u(e);
        int x = int(E[cand][0]);
        int y = int(E[cand][1]);
        cover.push_back(x);
        cover.push_back(y);
        int j = 0;
        while(j < E.size()) {
            if(E[j][0] == x || E[j][0] == y || E[j][1] == x || E[j][1] == y) {
                E.erase(std::remove(std::begin(E), std::end(E), E[j]), std::end(E));
            }
            else {
                ++j;
            }
        }
    }
    std::set<int> s(cover.begin(), cover.end());
    cover.assign(s.begin(), s.end());
    std::sort(cover.begin(), cover.end());
    return cover;
}
