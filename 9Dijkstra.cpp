#include <iostream>
#include <queue>
#include <vector>

#define MAXN 10005
// #define int long long
// #define INF 0x7fffffffffffffff
#define INF 0x7fffffff

using namespace std;

int N, M;
vector<pair<int, int>> G[MAXN];

priority_queue<pair<int, int>,
               vector<pair<int, int>>,
               greater<pair<int, int>>> H;

int dist[MAXN];

void Dijkstra(int src) {
    for (int i = 1; i <= N; i++) {
        dist[i] = INF;
    }
    dist[src] = 0;
    H.emplace(0, src);

    while (!H.empty()) {
        pair<int, int> p = H.top();
        H.pop();
        int index = p.second;
        for (pair<int, int> &t : G[index]) {
            int adj = t.first;
            int weight = t.second;
            if (dist[adj] > dist[index] + weight) {
                dist[adj] = dist[index] + weight;
                H.emplace(dist[adj], adj);
            }
        }
    }
}

signed main(void) {
    int src;
    cin >> N >> M >> src;
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        G[u].emplace_back(v, w);
        // G[v].emplace_back(u, w);
    }

    Dijkstra(src);

    for (int i = 1; i <= N; i++) {
        cout << dist[i] << ' ';
    }

    return 0;
}

/*
5 15 5
2 2 270
1 4 89
2 1 3
5 5 261
5 2 163
5 5 275
4 5 108
4 4 231
3 4 213
3 3 119
3 1 77
3 1 6
2 4 83
5 5 196
5 5 94
*/