#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>

#define MAXN 50001

using namespace std;


int N, M;
vector<int> Graph[MAXN];
int cnt;

int read() {
    int x = 0;
    char c = getchar();
    while (c > '9' || c < '0') {
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    return x;
}


int curtime;
int low[MAXN];
int dfn[MAXN];

void Tarjan(int x, int fa) {
    low[x] = dfn[x] = ++curtime;

    for (int &y : Graph[x]) {
        if (y == fa) {
            continue;
        }
        if (dfn[y] == 0) {
            Tarjan(y, x);
            low[x] = min(low[x], low[y]);

            if (low[y] > dfn[x]) {
                cnt++;
            }

        }
        else {
            low[x] = min(low[x], dfn[y]);
        }
    }
    // if(low[x] > dfn[fa]) cnt++;
}

int main(void) {
    N = read();
    M = read();

    for (int i = 0; i < M; i++) {
        int u, v;
        u = read();
        v = read();
        Graph[u].emplace_back(v);
        Graph[v].emplace_back(u);
    }

    for (int i = 1; i <= N; i++) {
        if (dfn[i] == 0) {
            Tarjan(i, i);
        }
    }
    // output number of bridges
    cout << cnt << endl;

    return 0;
}