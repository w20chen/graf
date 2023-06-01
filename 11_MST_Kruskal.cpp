#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

class disjoint_sets {
    int *fa;
    int siz;
public:
    disjoint_sets(int n) {
        fa = new int[n + 1];
        siz = n;
        for(int i = 1; i <= n; i++) {
            fa[i] = i;
        }
    }
    int find(int x) {
        if(fa[x] == x) return x;
        else return fa[x] = find(fa[x]);
    }
    void join(int a, int b) {
        int f_a = find(a);
        int f_b = find(b);
        fa[f_a] = f_b;
    }
};


vector<pair<pair<int, int>, int> > Edges;

// ascending order
bool cmp(const pair<pair<int, int>, int> &a, const pair<pair<int, int>, int> &b) {
    return a.second < b.second;
}

int main(void) {
    int N, M, a, b, w;
    scanf("%d%d", &N, &M);

    disjoint_sets S(N);

    for(int i = 0; i < M; i++) {
        scanf("%d%d%d", &a, &b, &w);
        Edges.push_back(make_pair(make_pair(a, b), w));
    }

    sort(Edges.begin(), Edges.end(), cmp);

    int edge_num = 0, edge_idx = 0, sum = 0;
    while(edge_num < N - 1) {
        if(edge_idx == M) {
            // cannot build a tree
            sum = -1;
            break;
        }
        if(S.find(Edges[edge_idx].first.first) != S.find(Edges[edge_idx].first.second)) {
            sum += Edges[edge_idx].second;
            S.join(Edges[edge_idx].first.first, Edges[edge_idx].first.second);
            edge_num++;
        }
        edge_idx++;
    }

    if(sum == -1) cout << "orz" << endl;
    else cout << sum << endl;

    return 0;
}
