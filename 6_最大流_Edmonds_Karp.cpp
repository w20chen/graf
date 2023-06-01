#include <iostream>
#include <cstdio>
#include <algorithm>
#include <cstring>
#include <queue>

#define MAXN 205
#define INF 0x7fffffffffffffff
#define int long long

using namespace std;

int N, M;
int G[MAXN][MAXN];


bool inqueue[MAXN];
int path[MAXN];

// BFS 找到一条从 start 到 end 的路径，并维护 path
bool EK_BFS(int start, int end) {
    queue<int> Q;
    memset(inqueue, 0, sizeof(inqueue));
    path[start] = -1;
    Q.push(start);
    inqueue[start] = true;

    while (!Q.empty()) {
        int t = Q.front();
        Q.pop();
        if (t == end) {
            return true;
        }

        for (int i = 1; i <= N; i++) {
            if (G[t][i] != 0 && inqueue[i] == false) {
                inqueue[i] = true;
                path[i] = t;
                Q.push(i);
            }
        }
    }
    return false;
}



int Edmonds_Karp(int start, int end) {
    int mn;
    int flow_ans = 0;

    while (EK_BFS(start, end)) {
        mn = INF;                       // shortest capacity on path
        int u = end;                        // from end to start
        while (path[u] != -1) {
            mn = min(mn, G[path[u]][u]);    // edge from path[u] to u
            u = path[u];
        }

        flow_ans += mn;

        u = end;
        while (path[u] != -1) {
            G[path[u]][u] -= mn;
            G[u][path[u]] += mn;
            u = path[u];
        }
    }
    return flow_ans;
}


signed main(void) {
    int src, tar;
    cin >> N >> M >> src >> tar;

    for (int i = 0; i < M; i++) {
        int u, v, w;
        cin >> u >> v;
        cin >> w;
        G[u][v] += w;   // 重边
    }

    cout << Edmonds_Karp(src, tar) << endl;

    return 0;
}