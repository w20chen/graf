#include <iostream>
#include <cstdio>
#include <queue>


#define INF 0x7fffffff
#define MAXNUM 100001

struct node {
    int index;
    int weight;
    node *next;
} *Graph[MAXNUM];


void add(int a, int b, int weight) {
    if(Graph[a] == nullptr) {
        Graph[a] = new node;
        Graph[a]->index = b;
        Graph[a]->weight = weight;
        Graph[a]->next = nullptr;
    } else {
        node *ptr = Graph[a];
        while(ptr->next != nullptr) {
            ptr = ptr->next;
        }
        ptr->next = new node;
        ptr->next->weight = weight;
        ptr->next->next = nullptr;
        ptr->next->index = b;
    }
}


std::vector<int> dist(MAXNUM, INF);
std::vector<bool> inqueue(MAXNUM, false);


int main() {
    int n, m, s;
    int a, b, w;
    scanf("%d%d%d", &n, &m, &s);
    for(int i = 0; i < m; i++) {
        scanf("%d%d%d", &a, &b, &w);
        add(a, b, w);
        // a ---> b
    }

    std::queue<int> Q;
    dist[s] = 0;
    Q.push(s);
    inqueue[s] = true;   // Unneeded

    while(!Q.empty()) {
        int i = Q.front();
        Q.pop();
        inqueue[i] = false;
        // for each vertex adjacent to vertex i
        for(node *ptr = Graph[i]; ptr; ptr = ptr->next) {
            // always renew distance when we visit a vertex
            if(dist[ptr->index] > dist[i] + ptr->weight) {
                dist[ptr->index] = dist[i] + ptr->weight;
                // make sure a vertex is in queue as long as it is renewed
                if(!inqueue[ptr->index]) {
                    Q.push(ptr->index);
                    inqueue[ptr->index] = true;
                }
            }
        }
    }

    for(int i = 1; i <= n; i++) {
        printf("%d ", dist[i]);
    }
    return 0;
}

// if dist[u] is renewed (relaxed),
// we need push it in queue to renew the dist of its neighbors
// even if it has ever been in queue (but has been poped out).