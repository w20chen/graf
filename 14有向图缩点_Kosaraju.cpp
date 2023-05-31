#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <stack>
#include <deque>

using namespace std;


const int MAXN = 10002;
int N, M;
int weight[MAXN];


/* Graph */
deque<int> Graph[MAXN];
deque<int> GR[MAXN];


bool vis[MAXN];
vector<int> seq;

// vis[x] == false
void DFS_GR(int x) {
    vis[x] = true;
    for(auto y : GR[x]) {
        if(!vis[y]) {
            DFS_GR(y);
        }
    }
    seq.push_back(x);
}


/* scc index starts at 1 */
int scc_num;
int scc_sum[MAXN];
int belong[MAXN];

void DFS(int x) {
    belong[x] = scc_num;
    scc_sum[scc_num] += weight[x];
    for(auto y : Graph[x]) {
        if(belong[y] == 0) {
            DFS(y);
        }
    }
}


deque<int> newGraph[MAXN];
int degin[MAXN];

void build(void) {
    for(int i = 1; i <= N; i++) {
        for(auto j : Graph[i]) {
            if(belong[i] != belong[j]) {
                newGraph[belong[i]].push_back(belong[j]);
                degin[belong[j]]++;
            }
        }
    }
}


/* Toposort */

// 到达该点时已付出的代价
int len[MAXN];

// degin[i] == 0 deleted
void Toposort(int i) {
    for(auto j : newGraph[i]) {
        if(degin[j] != 0) {     // undeleted
            degin[j]--;
            len[j] = max(len[j], len[i] + scc_sum[i]);
            if(degin[j] == 0) {
                Toposort(j);
            }
        }
    }
}


int main(void) {
    cin >> N >> M;
    for (int i = 1; i <= N; i++) {
        cin >> weight[i];
    }

    int u, v;
    for (int i = 0; i < M; i++) {
        cin >> u >> v;
        Graph[u].push_back(v);
        GR[v].push_back(u);
    }

    for (int i = 1; i <= N; i++) {
        if(!vis[i]) {
            DFS_GR(i);
        }
    }

    // 将反向图的后序序列反转
    for (int i = seq.size() - 1; i >= 0; i--) {
        if(belong[seq[i]] == 0) {
            scc_num++;
            DFS(seq[i]);
        }
    }

    build();

    for (int i = 1; i <= scc_num; i++) {
        if (degin[i] == 0) {
            Toposort(i);
        }
    }

    int ans = 0;
    for (int i = 1; i <= scc_num; i++) {
        ans = max(ans, len[i] + scc_sum[i]);
    }
    cout << ans << endl;

    return 0;
}

/*

10 20
970 369 910 889 470 106 658 659 916 964
3 2
3 6
3 4
9 5
8 3
5 8
9 1
9 7
9 8
7 5
3 7
7 8
1 7
10 2
1 10
4 8
2 6
3 1
3 5
8 5



10 20
986 587 671 143 521 985 204 36 921 990
4 2
2 8
7 8
2 4
1 7
8 7
10 2
5 9
4 5
5 8
4 9
2 7
5 6
9 1
9 6
4 10
9 7
9 2
1 2
5 7




*/