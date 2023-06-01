#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>

using namespace std;

class Edge {
public:
    int first;
    int second;
    bool vis;
    Edge(int u, int v) : first(u), second(v), vis(false) {}
};

int N, M;
vector<int> deg;
vector<int> path;
vector<Edge> E;
vector<vector<int>> G;  // index of edge

void DFS(int v) {
    for (int p : G[v]) {
        if (E[p].vis == false) {
            E[p].vis = true;
            DFS(E[p].first ^ E[p].second ^ v);
        }
    }
    path.emplace_back(v);
}

int main() {
    cin >> N >> M;

    G.resize(N + 1);
    deg.resize(N + 1);

    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        E.emplace_back(u, v);
        G[u].emplace_back(E.size() - 1);
        G[v].emplace_back(E.size() - 1);
        deg[u]++;
        deg[v]++;
    }

    for (int i = 1; i <= N; i++) {
        sort(G[i].begin(), G[i].end(), 
            [=](int a, int b)->bool { 
                return (E[a].first ^ E[a].second ^ i) > (E[b].first ^ E[b].second ^ i); 
            }
        );
    }

    int cnt = 0;
    int src = 0;
    for (int i = 1; i <= N; i++) {
        if (deg[i] % 2 == 1) {
            cnt++;
            src = i;
        }
    }

    if (cnt == 0) {
        DFS(N);
    }
    else if (cnt == 2) {
        DFS(src);
    }
    else {
        cout << 0 << endl;
        return 0;
    }

    if (path.size() == M + 1) {
        cout << 1 << endl;
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << path[i] << ' ';
        }
    }
    else {
        cout << 0 << endl;
    }

    return 0;
}

/*
3 5
1 2
2 3
1 2
2 3
3 1
*/