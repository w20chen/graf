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


/* Tarjan */
int dfn[MAXN];
int low[MAXN];
int curtime = 0;

stack<int> stk;
bool instack[MAXN];


/* scc index starts at 0 */
int scc_num;
int scc_sum[MAXN];
int belong[MAXN];

void Tarjan(int cur) {
    dfn[cur] = low[cur] = ++curtime;
    stk.push(cur);
    instack[cur] = true;

    for(auto adj : Graph[cur]) {
        if(dfn[adj] == 0) {         // unvisited
            Tarjan(adj);
            low[cur] = min(low[cur], low[adj]);
        } else if(instack[adj]) {   // visited and already in stack
            low[cur] = min(low[cur], dfn[adj]);
        }
    }
    if(dfn[cur] == low[cur]) {      // pop until find current vertex
        while(stk.empty() == false && stk.top() != cur) {
            instack[stk.top()] = false;
            scc_sum[scc_num] += weight[stk.top()];
            belong[stk.top()] = scc_num;
            stk.pop();
        }
        if (stk.empty() == false && stk.top() == cur) {
            instack[cur] = false;
            scc_sum[scc_num] += weight[stk.top()];
            belong[stk.top()] = scc_num;
            stk.pop();
        }
        scc_num++;
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
    }

    for (int i = 1; i <= N; i++) {
        if(dfn[i] == 0) {   // unvisited
            Tarjan(i);
        }
    }

    build();

    for (int i = 0; i < scc_num; i++) {
        if (degin[i] == 0) {
            Toposort(i);
        }
    }

    int ans = 0;
    for (int i = 0; i < scc_num; i++) {
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

*/