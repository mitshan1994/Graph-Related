// 定义了一个无向图数据结构, 支持图上的常用操作.
// 类型为Graph<T>, T是图中顶点的类型, 可以是自然数(教科书上常见的),
// 也可以是std::string(比如城市名). 
// 两个点之间的边有权重, 为Graph<T>::Cost类型. 在内部Cost定义为double.
// 实例代码:
//  Graph<double> g;   // 创建空图
//  g.AddVertex(3.5);  // 添加一个名为3.5的顶点
//  g.AddVertex(4.5);  // ditto
//  g.AddEdge(3.5, 4.5, 10);  // 在3.5和4.5两个顶点之间的边的权重为10.0
//  g.Print();         // 打印出这个图
//  std::cout << g.IsConnected() << std::endl;  // 是否是连通图

#ifndef GRAPH_H_
#define GRAPH_H_

#include <list>
#include <map>
#include <cstdlib>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_set>

template <typename T>
class Graph {
public:
    using Cost = double;
    // 创建一个空图(没有顶点, 也没有边)
    Graph() : vertex_num_(0), edge_num_(0) {}
    Graph(const Graph &) = default;
    Graph(Graph &&) = default;
    Graph& operator=(const Graph &) = default;
    Graph& operator=(Graph &&) = default;

    // 如果不与已有顶点相同, 则添加; 如果已有顶点相同, 则不添加, 并输出warning
    void AddVertex(const T &);
    void AddVertex(const std::vector<T> &);
    // 如果<v1, v2>存在, 则跳过.
    // 如果不存在v1或者v2, 则不做操作, 输出warning
    void AddEdge(const T &v1, const T &v2, const Cost &);
    // 输入两个vector, 第一个是哪些边需要添加, 第二个是对应的cost
    // Precondition: 两个向量的元素数量一致
    void AddEdge(const std::vector<std::pair<T, T>> &,
                 const std::vector<Cost> &);
    
    bool IsEmpty() const { return 0 == vertex_num_; }
    bool IsVertexExist(const T &v) const 
    {
        return std::find(vertex_.cbegin(), vertex_.cend(), v) != vertex_.cend();
    }
    // 如果v1或者v2其中之一不存在, 则返回false
    bool IsConnected(const T &v1, const T &v2) const;
    // 是否任意两个点都连通
    bool IsConnected() const;

    std::size_t GetVertexNum() const { return vertex_num_; }
    std::size_t GetEdgeNum() const { return edge_num_; }

    // 以一种友好的方式输出整个图
    void Print() const;

    ~Graph() = default;

    // 友元
    friend class GraphAlgorithm;
private:
    // 所有的顶点
    std::list<T> vertex_;
    // 与某顶点直接相连的点(间接反映了边的情况)
    std::map<T, std::list<std::pair<T, Cost>>> edges_;
    std::size_t vertex_num_;
    std::size_t edge_num_;

    // 将与顶点v连通的点添加到集合s中, 包括顶点v
    // traversed里记录了, 如果traversed[v] == true, 表示直接与v相连的顶点已经检查过了
    void GetConnectedSet(const T &v, std::unordered_set<T> &s, std::map<T, bool> &traversed) const;
};

template<typename T>
void Graph<T>::AddVertex(const T &v)
{
    // 如果所添加的顶点不存在
    if (!IsVertexExist(v)) {
        vertex_.push_back(v);
        edges_[v] = std::list<std::pair<T, Cost>>{};
        ++vertex_num_;
    }
    else {
        std::cerr << "Warning: vertex " << v << " has existed.\n";
    }
}

template<typename T>
void Graph<T>::AddVertex(const std::vector<T> &vs)
{
    for (auto &v : vs) {
        AddVertex(v);
    }
}

template<typename T>
void Graph<T>::AddEdge(const T & v1, const T & v2, const Cost & c)
{
    if (!IsVertexExist(v1) || !IsVertexExist(v2)) {
        std::cerr << "At least one of the vertex does not exist.\n";
            return;
    }
    if (IsConnected(v1, v2))
        return;
    edges_[v1].push_back({ v2, c });
    edges_[v2].push_back({ v1, c });
    ++edge_num_;
}

template<typename T>
void Graph<T>::AddEdge(const std::vector<std::pair<T, T>> &vs, const std::vector<Cost> &c)
{
    // 因为for的初始化区域只允许是同一种类型的区域, 故此处不可使用for循环
    auto it1 = vs.cbegin();
    auto it2 = c.cbegin();
    while (it1 != vs.cend()) {
        AddEdge(it1->first, it1->second, *it2);
        ++it1;
        ++it2;
    }
}

template<typename T>
bool Graph<T>::IsConnected(const T & v1, const T & v2) const
{
    if (!IsVertexExist(v1)) {
        std::cerr << "IsConnected: " << v1 << " does not exist.\n";
        return false;
    }
    // 自身和自身是连通的
    if (v1 == v2)
        return true;
    // 这里不能使用auto it = edges_[v1].cbegin();
    // 因为map的[]运算符不是const的, 但是IsConnected需要是const
    // (应该是这样)
    auto &v1_list = edges_.at(v1);
    for (auto it = v1_list.cbegin(); it != v1_list.cend(); ++it)
        if (it->first == v2)
            return true;
    return false;
}

template<typename T>
bool Graph<T>::IsConnected() const
{
    if (IsEmpty())
        return true;
    std::map<T, bool> is_checked;
    for (auto it = vertex_.cbegin(); it != vertex_.cend(); ++it) {
        is_checked.insert({ *it, false });
    }
    std::unordered_set<T> connect_v1;
    // 创建并初始化, 遍历的记录
    std::map<T, bool> traversed;
    for (auto it = vertex_.cbegin(); it != vertex_.cend(); ++it)
        traversed[*it] = false;
    GetConnectedSet(vertex_.front(), connect_v1, traversed);
    if (connect_v1.size() == vertex_.size())
        return true;
    return false;
}

template <typename T>
void Graph<T>::Print() const
{
    if (IsEmpty()) {
        std::cout << "Empty Graph.\n\n";
        return;
    }
    std::cout << "Vertex\tConnected Vertex(Cost)\n";
    for (auto it = edges_.cbegin(); it != edges_.cend(); ++it) {
        std::cout << it->first << "\t";
        for (auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2) {
            std::cout << it2->first << "(" << it2->second << ")\t";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

template<typename T>
void Graph<T>::GetConnectedSet(const T &v, std::unordered_set<T> &s, std::map<T, bool> &traversed) const
{
    if (traversed[v] == true)
        return;
    traversed[v] = true;
    s.insert(v);
    for (auto it = edges_.at(v).cbegin(); it != edges_.at(v).cend(); ++it)
        GetConnectedSet(it->first, s, traversed);
}

#endif