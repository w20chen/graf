#include <iostream>
#include <cstdio>
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <list>
#include <assert.h>

#define MAXN 105

int tarjan_curtime;
std::deque<int> stk(MAXN);
std::deque<int> tarjan_dfn(MAXN);
std::deque<int> tarjan_low(MAXN);
std::deque<bool> visited(MAXN);
std::deque<bool> isEmbedded(MAXN);

std::deque<std::deque<int>> record(MAXN * 2);
std::deque<std::deque<int>> face(MAXN * 2);

int stk_idx = 0;
int faceNum = 1;

class Graph {
public:
    std::unordered_map<int, int> head;     // head[u] 表示以 u 为起点的第一条边的编号
    std::deque<int> next;                 // next[idx] 表示编号为 idx 的边的下一条边，这条边和 idx 同一起点
    std::deque<int> to;                   // to[idx] 表示编号为 idx 的边的终点。
    std::deque<int> valid;                // valid[i] is the validity of edge with index i, 删边的懒标记
    std::deque<int> fixed;                // if this graph is an H-fragment, then fixed is the set of all fiexed vertices

    int F;                            // number of faces whose boundary includes all fixed vertices of this H-fragment
    int FPosition;                    // when F == 1, FPositon is the index of coresponding face

    Graph() {
        head.clear();
        next.resize(0);
        to.resize(0);
        valid.resize(0);
        fixed.resize(0);
        F = 0;
        FPosition = 0;
        next.emplace_back(0), to.emplace_back(0), valid.emplace_back(0);
        next.emplace_back(0), to.emplace_back(0), valid.emplace_back(0);
        // 为了方便通过边的编号 i^1 得到反向边，这里预先存入两条边
    }

    void addEdge(int x, int y) {
        // 若 x 不在 head 中，则 head[x] 为默认值 0
        // 为了避免编号为 0 的边的下一条边的编号为 0，这里提前存储 0 号边作为链表尾部的标志
        next.emplace_back(head[x]);    // 编号为 next.size()-1 的下一条边的编号为 head[x]，该边和新加入的边的起点均为 x
        to.emplace_back(y);            // 编号为 next.size()-1 的边的终点为 y
        valid.emplace_back(1);
        head[x] = next.size() - 1;  // 以 x 为起点的第一条边的编号为 next.size()-1
        assert(next.size() == to.size());
        assert(next.size() == valid.size());
    }

    void addUndirectedEdge(int u, int v) {
        addEdge(u, v);
        addEdge(v, u);
    }

    int edgeNum() const {
        // 这里没有考虑 valid 懒标记
        assert(next.size() % 2 == 0);
        return next.size() / 2 - 1;
    }

    int vertexNum() const {
        // 这里没有考虑 valid 懒标记
        return head.size();
    }

    void show() const {
        std::cout << "{ ";
        for (auto p : head) {
            if (p.second != 0) {
                std::cout << p.first << ' ';
            }
        }
        std::cout << "}" << std::endl;
    }

    void removeUndirectedEdge(int u, int v) {
        for (int l = head[u]; l; l = next[l]) {
            if (to[l] == v) {
                assert(valid[l]);
                valid[l] = 0;
                assert(valid[l ^ 1]);
                valid[l ^ 1] = 0;
                return;
            }
        }
        // std::cout << u << ',' << v << endl;
        // assert(0);
    }
};

void computeF(Graph &B) {
    B.F = 0;
    std::sort(B.fixed.begin(), B.fixed.end());
    for (int i = 1; i <= faceNum; i++) {
        if (record[i].size() < B.fixed.size()) {
            continue;
        }
        int now = 0;
        for (int j = 0; j < (int)B.fixed.size(); j++) {
            while (now + 1 < (int)record[i].size() && record[i][now] < B.fixed[j]) {
                now++;
            }
            if (record[i][now] != B.fixed[j]) {
                break;
            }
            else if (j + 1 == (int)B.fixed.size()) {
                B.F++;
                B.FPosition = i;
            }
        }
    }
}

void embed(int index = 1) {
    for (int i = 1; i <= stk_idx; i++) {
        isEmbedded[stk[i]] = true;
    }
    int l = 0;
    while (face[index][l] != stk[1] && face[index][l] != stk[stk_idx]) {
        l++;
    }
    int r = face[index].size() - 1;
    while (face[index][r] != stk[1] && face[index][r] != stk[stk_idx]) {
        r--;
    }
    std::deque<int> vec1;
    for (int i = 0; i < l; i++) {
        vec1.emplace_back(face[index][i]);
    }
    if (face[index][l] != stk[1]) {
        for (int i = stk_idx; i >= 1; i--) {
            vec1.emplace_back(stk[i]);
        }
    }
    else {
        for (int i = 1; i <= stk_idx; i++) {
            vec1.emplace_back(stk[i]);
        }
    }
    for (int i = r + 1; i < (int)face[index].size(); i++) {
        vec1.emplace_back(face[index][i]);
    }

    std::deque<int> vec2;
    for (int i = r - 1; i > l; i--) {
        vec2.emplace_back(face[index][i]);
    }
    if (face[index][l] != stk[1]) {
        for (int i = stk_idx; i >= 1; i--) {
            vec2.emplace_back(stk[i]);
        }
    }
    else {
        for (int i = 1; i <= stk_idx; i++) {
            vec2.emplace_back(stk[i]);
        }
    }

    face[index] = vec1;
    record[index] = vec1;
    std::sort(record[index].begin(), record[index].end());

    faceNum++;
    face[faceNum] = vec2;
    record[faceNum] = vec2;
    std::sort(record[faceNum].begin(), record[faceNum].end());
}

bool match(int x, int goal, Graph &G) {
    visited[x] = true;
    for (int l = G.head[x]; l; l = G.next[l]) {
        int y = G.to[l];
        if (visited[y] == true) {
            continue;
        }
        if (y == goal || (isEmbedded[y] == false && match(y, goal, G))) {
            G.valid[l] = 0;
            G.valid[l ^ 1] = 0;
            if (y == goal) {
                stk_idx++;
                stk[stk_idx] = y;
            }
            stk_idx++;
            stk[stk_idx] = x;
            return true;
        }
    }
    return false;
}

void findGraph(Graph &G, int l, Graph &ret) {
    int v = G.to[l];
    int fa = G.to[l ^ 1];

    ret.addUndirectedEdge(v, fa);

    // remove the lth edge from original graph
    G.valid[l] = 0;
    G.valid[l ^ 1] = 0;

    // for Tarjan_FindBlocks, isEmbedded is always false
    if (isEmbedded[v] == false) {
        for (int k = G.head[v]; k; k = G.next[k]) {
            if (G.valid[k]) {
                findGraph(G, k, ret);
            }
        }
    }
    else if (visited[v] == false) {
        ret.fixed.emplace_back(v);
        visited[v] = true;
    }
}

bool deal_with_fragments(std::list<Graph> &Lis) {
    if (Lis.size() == 0) {
        return true;
    }
    std::list<Graph>::iterator it = Lis.begin();
    int cnt = Lis.size() - 1;
    while (!Lis.empty()) {
        Graph &B = *it;
        computeF(B);
        cnt++;
        if (B.F == 0) {
            return false;
        }
        if (cnt == (int)Lis.size() || B.F == 1) {
            stk_idx = 0;
            std::fill(visited.begin(), visited.end(), 0);
            match(B.fixed[0], B.fixed[1], B);
            embed(B.FPosition);
            std::fill(visited.begin(), visited.end(), 0);
            for (int j = 2; j < stk_idx; j++) {
                for (int l = B.head[stk[j]]; l; l = B.next[l]) {
                    if (B.valid[l]) {
                        Graph g;
                        findGraph(B, l, g);
                        if (!visited[stk[j]]) {
                            g.fixed.emplace_back(stk[j]);
                        }
                        for (int k = 0; k < (int)g.fixed.size(); k++) {
                            visited[g.fixed[k]] = 0;
                        }
                        Lis.emplace_back(g);
                    }
                }
            }
            cnt = 0;
            it = Lis.erase(it);
            it--;
        }
        it++;
        if (it == Lis.end()) {
            it = Lis.begin();
        }
    }
    return true;
}

void Tarjan_FindBlocks(int x, int fa, std::list<Graph> &Blocks, Graph &Original) {
    tarjan_curtime++;
    tarjan_dfn[x] = tarjan_curtime;
    tarjan_low[x] = tarjan_curtime;

    for (int l = Original.head[x]; l; l = Original.next[l]) {
        // for each neighbor y of vertex x
        int y = Original.to[l];
        if (y == fa) {
            continue;
        }
        if (tarjan_dfn[y] != 0) {
            tarjan_low[x] = std::min(tarjan_low[x], tarjan_dfn[y]);
        }
        else {
            Tarjan_FindBlocks(y, x, Blocks, Original);
            tarjan_low[x] = std::min(tarjan_low[x], tarjan_low[y]);
        }
    }
    // x 无法通过回溯边到达时间戳更小的点
    // 这说明 x 和 fa 之间的边是割边
    // 由于 Tarjan_FindBlocks 函数是递归的，所以我们总是先处理更深的割边
    // 我们从 x 出发，在 DFS 树上向下遍历，能够得到一个块
    if (tarjan_dfn[x] == tarjan_low[x]) {
        // std::cout << x << endl;
        // 删去割边
        Original.removeUndirectedEdge(x, fa);
        Graph thisBlock;
        for (int l = Original.head[x]; l; l = Original.next[l]) {
            if (Original.valid[l]) {
                assert(Original.to[l] != fa);
                findGraph(Original, l, thisBlock);
                break;
            }
        }
        if (thisBlock.edgeNum() != 0) {
            // 其实不排除大小为 1 的块也没事
            Blocks.emplace_back(thisBlock);
            // thisBlock.show();
        }
    }
}

void findCycle(Graph &Block) {
    assert(stk_idx == 0);
    std::fill(visited.begin(), visited.end(), 0);

    // x is the targeted vertex of the first recorded edge in Block
    int x = Block.to[2];
    assert(Block.valid[2]);

    // for random vertex x in a block, x must be inside at least one cycle
    // 但是我们这里找到的圈并不一定包含 x
    // 比如图 {(1,2),(1,3),(2,3),(3,4),(4,5),(3,5)}
    // 找到的路径可能为 1-3-4-5-3，包含圈 3-4-5-3，应当去掉 1
    while (visited[x] == false) {
        visited[x] = true;
        for (int l = Block.head[x]; l; l = Block.next[l]) {
            // 延长这个 path
            if ((l ^ 1) != stk[stk_idx]) {
                x = Block.to[l];
                stk_idx++;
                stk[stk_idx] = l;
                assert(Block.valid[l]);
                break;
            }
        }
    }

    int k = 1, r = stk_idx;
    while (Block.to[stk[k] ^ 1] != x) {
        k++;
    }

    // delete all edges in this cycle from block
    stk_idx = 0;
    for (int i = k; i <= r; i++) {
        Block.valid[stk[i]] = 0;
        Block.valid[stk[i] ^ 1] = 0;
        stk_idx++;
        stk[stk_idx] = Block.to[stk[i] ^ 1];
    }
}

bool trivial_test(const Graph &Block) {
    if (Block.edgeNum() < 5 || Block.vertexNum() < 5) {
        return true;
    }
    if (Block.edgeNum() > 3 * Block.vertexNum() - 6) {
        return false;
    }
    return true;
}

int main() {
    Graph Original = Graph();
    int N = 0, M = 0;
    std::cin >> N >> M;
    for (int i = 0; i < M; i++) {
        int x = 0, y = 0;
        std::cin >> x >> y;
        if (x == y) {
            continue;
        }
        Original.addUndirectedEdge(x, y);
    }

    if (N < 5 || M < 5) {
        printf("1");
        return 0;
    }
    if (trivial_test(Original) == false) {
        printf("0");
        return 0;
    }

    tarjan_curtime = 0;    // set initial time for Tarjan_FindBlocks
    std::fill(visited.begin(), visited.end(), 0);
    std::fill(tarjan_low.begin(), tarjan_low.end(), 0);
    std::fill(isEmbedded.begin(), isEmbedded.end(), 0);
    std::fill(tarjan_dfn.begin(), tarjan_dfn.end(), 0);

    std::list<Graph> Blocks;
    for (int i = 1; i <= N; i++) {
        if (tarjan_dfn[i] == 0) {
            Tarjan_FindBlocks(i, i, Blocks, Original);
        }
    }

    for (Graph &Block : Blocks) {
        if (Block.edgeNum() < 5 || Block.vertexNum() < 5) {
            continue;
        }
        if (trivial_test(Block) == false) {
            printf("0");
            return 0;
        }
        stk_idx = 0;
        findCycle(Block);
        face[1].emplace_back(stk[1]);
        face[1].emplace_back(stk[stk_idx]);

        embed();

        std::fill(visited.begin(), visited.end(), 0);

        std::list<Graph> fragments;
        for (int j = 1; j <= stk_idx; j++) {
            for (int l = Block.head[stk[j]]; l; l = Block.next[l]) {
                if (Block.valid[l]) {
                    Graph frag;
                    findGraph(Block, l, frag);
                    if (visited[stk[j]] == false) {
                        frag.fixed.emplace_back(stk[j]);
                    }
                    for (int k = 0; k < (int)frag.fixed.size(); k++) {
                        visited[frag.fixed[k]] = false;
                    }
                    fragments.emplace_back(frag);
                }
            }
        }

        if (deal_with_fragments(fragments) == false) {
            printf("0");
            return 0;
        }

        for (int j = 1; j <= faceNum; j++) {
            face[j].resize(0);
            record[j].resize(0);
        }
        faceNum = 1;
    }

    printf("1");

    return 0;
}

/*
9 10
1 2
2 3
3 4
4 5
5 6
6 7
7 2
2 8
8 9
2 9
*/