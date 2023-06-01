#include <iostream>
#include <cstdio>

#define INF 0x7fffffff

int **R;

int main(void) {
    int n, m, s;
    int a, b, w;

    std::cin >> n >> m >> s;

    R = new int *[n + 1];
    for(int i = 1; i <= n; i++) {
        R[i] = new int [n + 1];
        for(int j = 1; j <= n; j++) {
            R[i][j] = INF;
        }
    }

    for(int i = 0; i < m; i++) {
        scanf("%d%d%d", &a, &b, &w);
        // multi-edges
        R[a][b] = (R[a][b] > w ? w : R[a][b]);
    }

    for(int i = 1; i <= n; i++) {
        R[i][i] = 0;
    }

    // Floyd

    for(int k = 1; k <= n; k++) {
        for(int i = 1; i <= n; i++) {
            for(int j = 1; j <= n; j++) {
                // INF + INF > INT_MAX
                if(R[i][k] == INF || R[k][j] == INF) continue;
                if(R[i][j] > R[i][k] + R[k][j]) {
                    R[i][j] = R[i][k] + R[k][j];
                }
            }
        }
    }

    for(int i = 1; i <= n; i++) {
        printf("%d ", R[s][i]);
    }
    return 0;
}