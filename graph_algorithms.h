#ifndef GRAPH_ALGORITHMS_H_
#define GRAPH_ALGORITHMS_H_

#include "graph.h"
#include <map>

// ��Ϊ����T��ʲô����, Graph<T>::Cost����double����.
// ����Ϊ�˷���, ����ֱ�Ӷ��������Ϊdouble, 
// ������Graph<T>::Cost, ��Ϊ����T�޷�ȷ��.
const double kInfinity = 1e300;

// ����������ͼ�㷨, ����õ���С������
class GraphAlgorithm {
public:
    // ��Prime�㷨��, ���ڼ�¼ÿ�������״̬�Ľṹ
    template <typename T>
    struct Statues {
        bool known = false;             // �Ƿ��Ѿ������
        typename Graph<T>::Cost dv = kInfinity;  // ���Ѵ���ĵ�(��)����С����
        T pv;                           // ���Ѵ���ĵ㼯�о�����С�ĵ�(previous vertice)
    };

    // ����ͼg����С������
    // Precondition: g����ͨͼ
    template <typename T>
    static Graph<T> MinSpanningTree(Graph<T> &g);

//private:
    // ���ڵݹ�, һ��һ��������Minimun spanning tree
    // @param mst   ��С������, ÿһ�������������һ����
    // @param g     ��Ҫ������С��������ͼ
    // @param table ���ڼ�¼���е�״̬
    template <typename T>
    static Graph<T>& UpdateOneStep(Graph<T> &mst, Graph<T> &g, 
                                   std::map<T, Statues<T>> &table);

    // ѡȡ��һ����Ҫ����ĵ�(��һ����ӵ���������ĵ�)
    template <typename T>
    static T PickNextVertice(std::map<T, Statues<T>> &table);

    // ����true, ���MST�Ѿ��������
    template <typename T>
    static bool HasFinished(std::map<T, Statues<T>> &table);

    // ����table����vֱ�������ĵ��dv��pv(���ṹstruct Statues)
    template <typename T>
    static void UpdateDistance(const T &v, Graph<T> &g, std::map<T, Statues<T>> &table);
};

// ʹ��Mark Allen Weiss��Data Structure and Algorithm Analysis in C, 3rd Edition
// Page.314 ��Prime's Algorithm(Chapter 9. Graph Algorithms)���õ���С������
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
    // �������һ�������knownΪfalse, ����δ����ĵ�, �򷵻�false
    for (auto it = table.cbegin(); it != table.cend(); ++it)
        if (false == it->second.known)
            return false;
    return true;
}

#endif