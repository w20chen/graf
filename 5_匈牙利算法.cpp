#include <iostream>
#include <vector>
#include <algorithm>
#include <cstring>

#define MAXN 1005

using namespace std;


vector<int> G[MAXN];
int match[MAXN];
bool vis[MAXN];

bool dfs(int u) {
    for (int &adj : G[u]) {
        if (vis[adj] == false) {
            vis[adj] = true;
            if (match[adj] == 0 || dfs(match[adj])) {
                match[adj] = u;
                return true;
            }
        }
    }
    return false;
}

int main(void) {
    int N, M, E;
    cin >> N >> M >> E;
    for (int i = 0; i < E; i++) {
        int a, b;
        cin >> a >> b;
        G[a].emplace_back(b);
    }

    int cnt = 0;
    for (int i = 1; i <= N; i++) {
        memset(vis, 0, sizeof(vis));
        if (dfs(i)) {
            cnt++;
        }
    }
    cout << cnt << endl;

    return 0;
}