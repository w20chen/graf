#include <iostream>
#include <vector>

#define MAXN 20000

using namespace std;

int N, M;
vector<int> G[MAXN];
bool vis[MAXN];

void DFS(int v) {
	vis[v] = true;
	for (int adj : G[v]) {
		if (vis[adj] == false) {
			DFS(adj);
		}
	}
}

int main() {
	cin >> N >> M;
	for (int i = 0; i < M; i++) {
		int u, v;
		cin >> u >> v;
		G[u].emplace_back(v);
		G[v].emplace_back(u);
	}

	int cnt = 0;

	for (int i = 1; i <= N; i++) {
		if (vis[i] == false) {
			cnt++;
			DFS(i);
		}
	}

	cout << cnt << endl;

	return 0;
}