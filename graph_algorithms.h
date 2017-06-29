#ifndef GRAPH_ALGORITHMS_H_
#define GRAPH_ALGORITHMS_H_

#include "graph.h"
#include <map>

// 因为不管T是什么类型, Graph<T>::Cost都是double类型.
// 所以为了方便, 这里直接定义无穷大为double, 
// 而不是Graph<T>::Cost, 因为这里T无法确定.
const double kInfinity = 1e300;

// 包含常见的图算法, 比如得到最小生成树
class GraphAlgorithm {
public:
    // 在Prime算法中, 用于记录每个顶点的状态的结构
    template <typename T>
    struct Statues {
        bool known = false;             // 是否已经处理过
        typename Graph<T>::Cost dv = kInfinity;  // 与已处理的点(们)的最小距离
        T pv;                           // 与已处理的点集中距离最小的点(previous vertice)
    };

    // 返回图g的最小生成树
    // Precondition: g是连通图
    template <typename T>
    static Graph<T> MinSpanningTree(Graph<T> &g);

    // 是否存在欧拉回路
    // 这里的欧拉回路, 要求起点和终点是同一个
    template <typename T>
    static bool EulerCircuitExist(Graph<T> &g);

private:
    // 返回顶点v在table中对应的pv(详情见struct Statues)
    // Precondition: table中v对应的dv不是kInfinity. 这保证了pv是有意义的.
    template <typename T>
    static T GetPv(const T &v, const std::map<T, Statues<T>> &table);

    // 用于一步一步地生成Minimun spanning tree
    // @param v     这一步加入mst的点(其实是要加这个点的某一条边)
    // @param g     需要生成最小生成树的图
    // @param mst   最小生成树, 每一步都往里面添加一条边
    // @param table 用于记录进行的状态
    template <typename T>
    static Graph<T>& UpdateOneStep(const T & v, Graph<T> &mst,
                                   std::map<T, Statues<T>> &table);

    // 选取下一个需要处理的点(下一个添加到生成树里的点)
    // 如果table中所有的点的known为false, 则必须保证所有的点的dv为kInfinity
    // Precondition: HasFinished(table) is false
    template <typename T>
    static T PickNextVertice(std::map<T, Statues<T>> &table);

    // 返回true, 如果MST已经生成完毕
    template <typename T>
    static bool HasFinished(std::map<T, Statues<T>> &table);

    // 更新table中与v直接相连的点的dv和pv(见结构struct Statues)
    template <typename T>
    static void UpdateDistance(const T &v, Graph<T> &g, std::map<T, Statues<T>> &table);
};

// 使用Mark Allen Weiss的Data Structure and Algorithm Analysis in C, 3rd Edition
// Page.314 的Prime's Algorithm(Chapter 9. Graph Algorithms)来得到最小生成树
template <typename T>
Graph<T> GraphAlgorithm::MinSpanningTree(Graph<T> &g)
{
    Graph<T> mst;
    // 创建一个vector, 包含所有图g中的顶点
    std::vector<T> vs;
    for (auto it = g.vertex_.cbegin(); it != g.vertex_.cend(); ++it)
        vs.push_back(*it);
    mst.AddVertex(vs);
    std::map<T, Statues<T>> table;
    for (auto it = g.vertex_.cbegin(); it != g.vertex_.cend(); ++it)
        table[*it] = Statues<T>{ false, kInfinity, *it };  // note, here *it is important.
                                                       // make following implementation
                                                       // convinient.
    while (!HasFinished(table)) {
        auto next_vertice = PickNextVertice(table);
        UpdateDistance(next_vertice, g, table);
        UpdateOneStep(next_vertice, mst, table);
    }
    return mst;
}

// 如果每个顶点处的度数都为偶数, 并且是连通图, 则存在欧拉回路
template<typename T>
bool GraphAlgorithm::EulerCircuitExist(Graph<T>& g)
{
    if (g.IsConnected() == false)
        return false;
    for (auto it = g.vertex_.cbegin(); it != g.vertex_.cend(); ++it)
        if (g.edges_.at(*it).size() % 2 != 0)
            return false;
    return true;
}

template <typename T>
T GraphAlgorithm::GetPv(const T &v, const std::map<T, Statues<T>> &table)
{
    return table.at(v).pv;
    // 这里如果改成下面的形式, 编译不通过, 不知为何
    // return table[v].pv;
}

template <typename T>
Graph<T>& GraphAlgorithm::UpdateOneStep(const T & v, Graph<T> &mst,
                                        std::map<T, Statues<T>> &table)
{
    mst.AddEdge(v, table[v].pv, table[v].dv);
    return mst;
}

template <typename T>
void GraphAlgorithm::UpdateDistance(const T &v, Graph<T> &g, std::map<T, Statues<T>> &table)
{
    auto it = g.edges_.at(v).cbegin();  // it points to std::pair<T, Cost>
    while (it != g.edges_.at(v).cend()) {
        if (table.at(it->first).known == true) {
            ++it;
            continue;
        }
        if (it->second < table.at(it->first).dv) {
            table[it->first].dv = it->second;
            table[it->first].pv = v;
        }
        ++it;
    }
}

template <typename T>
bool GraphAlgorithm::HasFinished(std::map<T, Statues<T>> &table)
{
    // 如果存在一个顶点的known为false, 即有未处理的点, 则返回false
    for (auto it = table.cbegin(); it != table.cend(); ++it)
        if (false == it->second.known)
            return false;
    return true;
}

template <typename T>
T GraphAlgorithm::PickNextVertice(std::map<T, Statues<T>> &table)
{
    T pos;
    Graph<T>::Cost min_value;
    auto it = table.cbegin();
    // 找到第一个known为false的点
    while (it != table.cend()) {
        if (it->second.known == false) {
            pos = it->first;
            min_value = it->second.dv;
            break;
        }
        ++it;
    }
    // 所有的点都known, 随意返回(By precondition), behaviour undefined
    if (it == table.cend())
        return T{};
    while (++it != table.cend()) {
        if (it->second.known == false && it->second.dv < min_value) {
            pos = it->first;
            min_value = it->second.dv;
        }
    }
    table[pos].known = true;
    return pos;
}

#endif