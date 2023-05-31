#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <queue>

#define MAXN 3001
#define INF 1000000000

typedef long long ll;

using namespace std;


ll read() {
    ll x = 0, f = 1;
    char c = getchar();
    while (c > '9' || c < '0') {
        if (c == '-') f = -f;
        c = getchar();
    }
    while (c <= '9' && c >= '0') {
        x = 10 * x + c - '0';
        c = getchar();
    }
    return x * f;
}


ll N, M;
vector<pair<ll, ll>> Graph[MAXN];


ll h[MAXN];
ll cnt[MAXN];
bool vis[MAXN];
queue<ll> Q;

bool Bellman_Ford(ll src) {
    for (ll i = 0; i <= N; h[i++] = INF);
    h[src] = 0;
    cnt[src] = 0;
    vis[src] = true;
    Q.push(src);

    while (!Q.empty()) {
        ll t = Q.front();
        Q.pop();
        vis[t] = false;

        for (pair<ll, ll>& p : Graph[t]) {
            if (h[p.first] > h[t] + p.second) {
                h[p.first] = h[t] + p.second;

                cnt[p.first] = cnt[t] + 1;
                if (cnt[p.first] >= N + 1) {        // N + 1 vertices in total (0 included)
                    return false;
                }

                if (!vis[p.first]) {
                    Q.push(p.first);
                    vis[p.first] = true;
                }
            }
        }
    }
    return true;
}


ll dist[MAXN];
priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<pair<ll, ll>>> P;

void Dijkstra(ll src) {
    for (ll i = 1; i <= N; i++) {
        dist[i] = INF;
        vis[i] = false;
    }

    dist[src] = 0;
    P.push(make_pair(0, src));

    while (!P.empty()) {
        ll from = P.top().second;
        P.pop();

        if (vis[from] == true) continue;
        vis[from] = true;

        for (pair<ll, ll>& p : Graph[from]) {
            ll to = p.first, weight = p.second;
            if (vis[to] == true) continue;
            if (dist[to] > dist[from] + weight + h[from] - h[to]) {
                dist[to] = dist[from] + weight + h[from] - h[to];
                P.push(make_pair(dist[to], to));
            }
        }
    }
}


signed main(void) {
    N = read();
    M = read();

    for (ll i = 0; i < M; i++) {
        ll u, v, w;
        u = read();
        v = read();
        w = read();
        Graph[u].emplace_back(make_pair(v, w));
    }

    // 这样也是可以的，只要右边即可
    /*for (ll i = 1; i <= N/2; i++) {
        Graph[0].emplace_back(make_pair(i, 0));
    }
    for (ll i = N / 2 + 1; i <= N; i++) {
        Graph[0].emplace_back(make_pair(i, 110));
    }*/

    for (ll i = 1; i <= N / 2; i++) {
        Graph[0].emplace_back(make_pair(i, 0));
    }

    // 超结点到各点的有向边不一定非要是 0，事实上只要有边即可

    if (!Bellman_Ford(0)) {
        printf("-1\n");
        return 0;
    }

    for (ll src = 1; src <= N; src++) {
        Dijkstra(src);
        ll ans = 0;
        for (ll des = 1; des <= N; des++) {
            if (dist[des] == INF) ans += des * INF;
            else ans += des * (dist[des] - h[src] + h[des]);
        }
        printf("%lld\n", ans);
    }

    return 0;
}

/*
5 7
1 2 4
1 4 10
2 3 7
4 5 3
4 2 -2
3 4 -3
5 3 4

5 5
1 2 4
3 4 9
3 4 -3
4 5 3
5 3 -2
*/