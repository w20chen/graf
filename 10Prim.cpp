#include <iostream>
#include <queue>
#include <vector>

#define MAXN 10005
#define INF 0x7fffffff

using namespace std;

int N, M;
vector<pair<int, int>> G[MAXN];

priority_queue<pair<int, int>,
               vector<pair<int, int>>,
               greater<pair<int, int>>> H;

int dist[MAXN];
bool vis[MAXN];

int Prim(int src) {
    for (int i = 1; i <= N; i++) {
        dist[i] = INF;
        vis[i] = false;
    }
    dist[src] = 0;
    H.emplace(0, src);
    int sum = 0;

    while (!H.empty()) {
        auto p = H.top();
        H.pop();
        int index = p.second;
        if (vis[index]) {
            continue;
        }
        vis[index] = true;
        sum += dist[index];
        for (pair<int, int> &t : G[index]) {
            int adj = t.first;
            int weight = t.second;
            if (dist[adj] > weight) {
                dist[adj] = weight;
                H.emplace(dist[adj], adj);
            }
        }
    }
    return sum;
}

signed main(void) {
    cin >> N >> M;
    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        G[u].emplace_back(v, w);
        G[v].emplace_back(u, w);
    }

    int ret = Prim(1);

    for (int i = 1; i <= N; i++) {
        if (!vis[i]) {
            cout << "orz";
            return 0;
        }
    }

    cout << ret;

    return 0;
}
