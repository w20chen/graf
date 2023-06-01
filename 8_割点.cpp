#include <iostream>
#include <vector>
#include <algorithm>

#define MAXN 20000

using namespace std;

int N, M;
vector<int> G[MAXN];

int Root = 1;   // DFS tree root
int dfn[MAXN];
int low[MAXN];
int curtime = 0;

vector<int> cut;

void Tarjan(int v, int fa) {   // visit v, the father of v is fa
	int childnum = 0;
	dfn[v] = ++curtime;
	low[v] = dfn[v];

	for (int adj : G[v]) {
		if (dfn[adj] == 0) {   // unvisited
			childnum++;
			Tarjan(adj, v);    // visit adj, the father of adj is v
			low[v] = min(low[v], low[adj]);
		}
		else if (adj != fa) {  // visited
			low[v] = min(low[v], dfn[adj]);
		}
	}

	if (v == Root && childnum >= 2) {
		cut.emplace_back(v);
	}
	else if (fa != Root && low[v] >= dfn[fa]) {
		cut.emplace_back(fa);
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

	for (int i = 1; i <= N; i++) {
		if (dfn[i] == 0) {   // unvisited
			Root = i;
			Tarjan(Root, Root);
		}
	}

	// remove duplicate
	sort(cut.begin(), cut.end());
	cut.erase(unique(cut.begin(), cut.end()), cut.end());
	sort(cut.begin(), cut.end());

	cout << cut.size() << endl;
	for (int c : cut) {
		cout << c << endl;
	}

	return 0;
}