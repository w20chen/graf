#include <iostream>
#include <cstdio>
#include <algorithm>
#include <deque>
#include <unordered_map>
#include <list>
#include <assert.h>

#define MAXN 105

signed tarjan_curtime;
std::deque<signed> stk(MAXN);
std::deque<signed> tarjan_dfn(MAXN);
std::deque<signed> tarjan_low(MAXN);
std::deque<bool> visited(MAXN);
std::deque<bool> isEmbedded(MAXN);

std::deque<std::deque<signed>> record(MAXN * 2);
std::deque<std::deque<signed>> face(MAXN * 2);

signed stk_idx = 0;
signed faceNum = 1;

class Graph {
public:
    std::unordered_map<signed, signed> head;     // head[u] 表示以 u 为起点的第一条边的编号
    std::deque<signed> next;                 // next.at(idx) 表示编号为 idx 的边的下一条边，这条边和 idx 同一起点
    std::deque<signed> to;                   // to.at(idx) 表示编号为 idx 的边的终点。
    std::deque<signed> valid;                // valid.at(i) is the validity of edge with index i, 删边的懒标记
    std::deque<signed> fixed;                // if this graph is an H-fragment, then fixed is the set of all fiexed vertices

    signed F;                            // number of faces whose boundary includes all fixed vertices of this H-fragment
    signed FPosition;                    // when F == 1, FPositon is the index of coresponding face

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

    void addEdge(signed x, signed y) {
        // 若 x 不在 head 中，则 head[x] 为默认值 0
        // 为了避免编号为 0 的边的下一条边的编号为 0，这里提前存储 0 号边作为链表尾部的标志
        next.emplace_back(head[x]);    // 编号为 next.size()-1 的下一条边的编号为 head[x]，该边和新加入的边的起点均为 x
        to.emplace_back(y);            // 编号为 next.size()-1 的边的终点为 y
        valid.emplace_back(1);
        head[x] = next.size() - 1;  // 以 x 为起点的第一条边的编号为 next.size()-1
        assert(next.size() == to.size());
        assert(next.size() == valid.size());
    }

    void addUndirectedEdge(signed u, signed v) {
        addEdge(u, v);
        addEdge(v, u);
    }

    signed edgeNum() const {
        // 这里没有考虑 valid 懒标记
        assert(next.size() % 2 == 0);
        return next.size() / 2 - 1;
    }

    signed vertexNum() const {
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

    void removeUndirectedEdge(signed u, signed v) {
        for (signed l = head[u]; l; l = next.at(l)) {
            if (to.at(l) == v) {
                assert(valid.at(l));
                valid.at(l) = 0;
                assert(valid.at(l ^ 1));
                valid.at(l ^ 1) = 0;
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
    for (signed i = 1; i <= faceNum; i++) {
        if (record.at(i).size() < B.fixed.size()) {
            continue;
        }
        signed now = 0;
        for (signed j = 0; j < (signed)B.fixed.size(); j++) {
            while (now + 1 < (signed)record.at(i).size() && record.at(i).at(now) < B.fixed.at(j)) {
                now++;
            }
            if (record.at(i).at(now) != B.fixed.at(j)) {
                break;
            }
            else if (j + 1 == (signed)B.fixed.size()) {
                B.F++;
                B.FPosition = i;
            }
        }
    }
}

void embed(signed index = 1) {
    for (signed i = 1; i <= stk_idx; i++) {
        isEmbedded.at(stk.at(i)) = true;
    }
    signed l = 0;
    while (face.at(index).at(l) != stk.at(1) && face.at(index).at(l) != stk.at(stk_idx)) {
        l++;
    }
    signed r = face.at(index).size() - 1;
    while (face.at(index).at(r) != stk.at(1) && face.at(index).at(r) != stk.at(stk_idx)) {
        r--;
    }
    std::deque<signed> vec1;
    for (signed i = 0; i < l; i++) {
        vec1.emplace_back(face.at(index).at(i));
    }
    if (face.at(index).at(l) != stk.at(1)) {
        for (signed i = stk_idx; i >= 1; i--) {
            vec1.emplace_back(stk.at(i));
        }
    }
    else {
        for (signed i = 1; i <= stk_idx; i++) {
            vec1.emplace_back(stk.at(i));
        }
    }
    for (signed i = r + 1; i < (signed)face.at(index).size(); i++) {
        vec1.emplace_back(face.at(index).at(i));
    }

    std::deque<signed> vec2;
    for (signed i = r - 1; i > l; i--) {
        vec2.emplace_back(face.at(index).at(i));
    }
    if (face.at(index).at(l) != stk.at(1)) {
        for (signed i = stk_idx; i >= 1; i--) {
            vec2.emplace_back(stk.at(i));
        }
    }
    else {
        for (signed i = 1; i <= stk_idx; i++) {
            vec2.emplace_back(stk.at(i));
        }
    }

    face.at(index) = vec1;
    record.at(index) = vec1;
    std::sort(record.at(index).begin(), record.at(index).end());

    faceNum++;
    face.at(faceNum) = vec2;
    record.at(faceNum) = vec2;
    std::sort(record.at(faceNum).begin(), record.at(faceNum).end());
}

bool match(signed x, signed target, Graph &G) {
    visited.at(x) = true;
    for (signed l = G.head[x]; l; l = G.next.at(l)) {
        signed y = G.to.at(l);
        if (visited.at(y) == true) {
            continue;
        }
        if (target == y || (isEmbedded.at(y) == false && match(y, target, G))) {
            G.valid.at(l) = 0;
            G.valid.at(l ^ 1) = 0;
            if (target == y) {
                stk_idx++;
                stk.at(stk_idx) = y;
            }
            stk_idx++;
            stk.at(stk_idx) = x;
            return true;
        }
    }
    return false;
}

void findGraph(Graph &G, signed l, Graph &ret) {
    signed v = G.to.at(l);
    signed fa = G.to.at(l ^ 1);

    ret.addUndirectedEdge(v, fa);

    // remove the lth edge from original graph
    G.valid.at(l) = 0;
    G.valid.at(l ^ 1) = 0;

    // for Tarjan_FindBlocks, isEmbedded is always false
    if (isEmbedded.at(v) == false) {
        for (signed k = G.head[v]; k; k = G.next.at(k)) {
            if (G.valid.at(k)) {
                findGraph(G, k, ret);
            }
        }
    }
    else if (visited.at(v) == false) {
        ret.fixed.emplace_back(v);
        visited.at(v) = true;
    }
}

bool deal_with_fragments(std::list<Graph> &fragments_list) {
    if (fragments_list.size() == 0) {
        return true;
    }
    std::list<Graph>::iterator it = fragments_list.begin();
    signed cnt = fragments_list.size() - 1;
    while (!fragments_list.empty()) {
        Graph &B = *it;
        computeF(B);
        cnt++;
        if (B.F == 0) {
            return false;
        }
        if (cnt == (signed)fragments_list.size() || B.F == 1) {
            stk_idx = 0;
            std::fill(visited.begin(), visited.end(), 0);
            if (B.fixed.size() == 1) {
                match(B.fixed.front(), B.fixed.front(), B);
            }
            else if (B.fixed.size() >= 2) {
                match(B.fixed.front(), B.fixed.at(1), B);
            }
            else {
                assert(0);
            }
            embed(B.FPosition);
            std::fill(visited.begin(), visited.end(), 0);
            for (signed j = 2; j < stk_idx; j++) {
                for (signed l = B.head[stk.at(j)]; l; l = B.next.at(l)) {
                    if (B.valid.at(l)) {
                        Graph g;
                        findGraph(B, l, g);
                        if (!visited.at(stk.at(j))) {
                            g.fixed.emplace_back(stk.at(j));
                        }
                        for (signed k = 0; k < (signed)g.fixed.size(); k++) {
                            visited.at(g.fixed.at(k)) = 0;
                        }
                        fragments_list.emplace_back(g);
                    }
                }
            }
            cnt = 0;
            it = fragments_list.erase(it);
            it--;
        }
        it++;
        if (it == fragments_list.end()) {
            it = fragments_list.begin();
        }
    }
    return true;
}

void Tarjan_FindBlocks(signed x, signed fa, std::list<Graph> &Blocks, Graph &Original) {
    tarjan_curtime++;
    tarjan_dfn.at(x) = tarjan_curtime;
    tarjan_low.at(x) = tarjan_curtime;

    for (signed l = Original.head[x]; l; l = Original.next.at(l)) {
        // for each neighbor y of vertex x
        signed y = Original.to.at(l);
        if (y == fa) {
            continue;
        }
        if (tarjan_dfn.at(y) != 0) {
            tarjan_low.at(x) = std::min(tarjan_low.at(x), tarjan_dfn.at(y));
        }
        else {
            Tarjan_FindBlocks(y, x, Blocks, Original);
            tarjan_low.at(x) = std::min(tarjan_low.at(x), tarjan_low.at(y));
        }
    }
    // x 无法通过回溯边到达时间戳更小的点
    // 这说明 x 和 fa 之间的边是割边
    // 由于 Tarjan_FindBlocks 函数是递归的，所以我们总是先处理更深的割边
    // 我们从 x 出发，在 DFS 树上向下遍历，能够得到一个块
    if (tarjan_dfn.at(x) == tarjan_low.at(x)) {
        // std::cout << x << endl;
        // 删去割边
        Original.removeUndirectedEdge(x, fa);
        Graph thisBlock;
        for (signed l = Original.head[x]; l; l = Original.next.at(l)) {
            if (Original.valid.at(l)) {
                assert(Original.to.at(l) != fa);
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
    signed x = Block.to.at(2);
    assert(Block.valid.at(2));

    // for random vertex x in a block, x must be inside at least one cycle
    // 但是我们这里找到的圈并不一定包含 x
    // 比如图 {(1,2),(1,3),(2,3),(3,4),(4,5),(3,5)}
    // 找到的路径可能为 1-3-4-5-3，包含圈 3-4-5-3，应当去掉 1
    while (visited.at(x) == false) {
        visited.at(x) = true;
        for (signed l = Block.head[x]; l; l = Block.next.at(l)) {
            // 延长这个 path
            if ((l ^ 1) != stk.at(stk_idx)) {
                x = Block.to.at(l);
                stk_idx++;
                stk.at(stk_idx) = l;
                assert(Block.valid.at(l));
                break;
            }
        }
    }

    signed k = 1, r = stk_idx;
    while (Block.to.at(stk.at(k) ^ 1) != x) {
        k++;
    }

    // delete all edges in this cycle from block
    stk_idx = 0;
    for (signed i = k; i <= r; i++) {
        Block.valid.at(stk.at(i)) = 0;
        Block.valid.at(stk.at(i) ^ 1) = 0;
        stk_idx++;
        stk.at(stk_idx) = Block.to.at(stk.at(i) ^ 1);
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

signed main() {
    Graph Original = Graph();
    signed N = 0, M = 0;
    std::cin >> N >> M;
    for (signed i = 0; i < M; i++) {
        signed x = 0, y = 0;
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
    for (signed i = 1; i <= N; i++) {
        if (tarjan_dfn.at(i) == 0) {
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
        face.at(1).emplace_back(stk.at(1));
        face.at(1).emplace_back(stk.at(stk_idx));

        embed();

        std::fill(visited.begin(), visited.end(), 0);

        std::list<Graph> fragments;
        for (signed j = 1; j <= stk_idx; j++) {
            for (signed l = Block.head[stk.at(j)]; l; l = Block.next.at(l)) {
                if (Block.valid.at(l)) {
                    Graph frag;
                    findGraph(Block, l, frag);
                    if (visited.at(stk.at(j)) == false) {
                        frag.fixed.emplace_back(stk.at(j));
                    }
                    for (signed k = 0; k < (signed)frag.fixed.size(); k++) {
                        visited.at(frag.fixed.at(k)) = false;
                    }
                    fragments.emplace_back(frag);
                }
            }
        }

        if (deal_with_fragments(fragments) == false) {
            printf("0");
            return 0;
        }

        for (signed j = 1; j <= faceNum; j++) {
            face.at(j).resize(0);
            record.at(j).resize(0);
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

/*
9 12
1 2
1 3
2 3
2 9
2 8
3 4
4 5
5 6
5 7
7 8
7 9
6 8
*/

/*
14 14
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
1 10
1 11
10 11
11 12
*/
