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

//private:
    // 用于递归, 一步一步地生成Minimun spanning tree
    // @param mst   最小生成树, 每一步都往里面添加一条边
    // @param g     需要生成最小生成树的图
    // @param table 用于记录进行的状态
    template <typename T>
    static Graph<T>& UpdateOneStep(Graph<T> &mst, Graph<T> &g, 
                                   std::map<T, Statues<T>> &table);

    // 选取下一个需要处理的点(下一个添加到生成树里的点)
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
    std::map<T, Statues<T>> table;
    for (auto it = g.vertex_.cbegin(); it != g.vertex_.cend(); ++it)
        table[*it] = Status{};

}

template <typename T>
Graph<T>& GraphAlgorithm::UpdateOneStep(Graph<T> &mst, Graph<T> &g,
                                        std::map<T, Statues<T>> &table)
{

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

#endif