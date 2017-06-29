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

    // �Ƿ����ŷ����·
    // �����ŷ����·, Ҫ�������յ���ͬһ��
    template <typename T>
    static bool EulerCircuitExist(Graph<T> &g);

private:
    // ���ض���v��table�ж�Ӧ��pv(�����struct Statues)
    // Precondition: table��v��Ӧ��dv����kInfinity. �Ᵽ֤��pv���������.
    template <typename T>
    static T GetPv(const T &v, const std::map<T, Statues<T>> &table);

    // ����һ��һ��������Minimun spanning tree
    // @param v     ��һ������mst�ĵ�(��ʵ��Ҫ��������ĳһ����)
    // @param g     ��Ҫ������С��������ͼ
    // @param mst   ��С������, ÿһ�������������һ����
    // @param table ���ڼ�¼���е�״̬
    template <typename T>
    static Graph<T>& UpdateOneStep(const T & v, Graph<T> &mst,
                                   std::map<T, Statues<T>> &table);

    // ѡȡ��һ����Ҫ����ĵ�(��һ����ӵ���������ĵ�)
    // ���table�����еĵ��knownΪfalse, ����뱣֤���еĵ��dvΪkInfinity
    // Precondition: HasFinished(table) is false
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
    Graph<T> mst;
    // ����һ��vector, ��������ͼg�еĶ���
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

// ���ÿ�����㴦�Ķ�����Ϊż��, ��������ͨͼ, �����ŷ����·
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
    // ��������ĳ��������ʽ, ���벻ͨ��, ��֪Ϊ��
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
    // �������һ�������knownΪfalse, ����δ����ĵ�, �򷵻�false
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
    // �ҵ���һ��knownΪfalse�ĵ�
    while (it != table.cend()) {
        if (it->second.known == false) {
            pos = it->first;
            min_value = it->second.dv;
            break;
        }
        ++it;
    }
    // ���еĵ㶼known, ���ⷵ��(By precondition), behaviour undefined
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