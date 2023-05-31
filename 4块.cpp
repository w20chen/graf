#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <stack>

using namespace std;

int N, M;
int curtime;
int dfn[10005];
int low[10005];
stack<pair<int, int>> S;
vector<int> G[10005];
vector<int> blk;

void DFSBlk(int u, int fa) {
	dfn[u] = ++curtime;
	low[u] = dfn[u];
	int childnum = 0;				// child num of v

	for (int adj : G[u]) {
		if (dfn[adj] == 0) {		// unvisited
			S.emplace(min(adj, u), max(adj, u));
			childnum++;
			DFSBlk(adj, u);
			low[u] = min(low[u], low[adj]);

			if ((u == fa && childnum >= 2) || (u != fa && low[adj] >= dfn[u])) {
				int cnt = 0;
				pair<int, int> p;
				pair<int, int> tmp = make_pair(min(adj, u), max(adj, u));
				do {
					cnt++;
					p = S.top();
					S.pop();
				} while (p != tmp);
				blk.emplace_back(cnt);
			}
		}
		else if (adj != fa) {
			if (dfn[u] > dfn[adj]) {
				S.emplace(min(adj, u), max(adj, u));
			}
			low[u] = min(low[u], dfn[adj]);
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

	for (int i = 1; i <= N; i++) {
		if (dfn[i] == 0) {
			int prev = blk.size();
			DFSBlk(i, i);
			if (!S.empty()) {
				blk.emplace_back(S.size());
			}
			while (!S.empty()) {
				S.pop();
			}
			// this block only has one single vertex
			if (prev == blk.size()) {
				blk.emplace_back(0);
			}
		}
	}

	cout << blk.size() << endl;
	sort(blk.begin(), blk.end());

	for (int b : blk) {
		cout << b << ' ';
	}
	cout << endl;
	return 0;
}

/*
9 10
1 2
1 3
2 4
2 5
4 6
5 6
3 7
3 8
7 9
8 9

7 7
5 6
5 2
2 3
2 1
2 4
1 4
3 4
*/