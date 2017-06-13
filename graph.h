// ������һ������ͼ���ݽṹ, ֧��ͼ�ϵĳ��ò���.
// ����ΪGraph<T>, T��ͼ�ж��������, ��������Ȼ��(�̿����ϳ�����),
// Ҳ������std::string(���������). 
// ������֮��ı���Ȩ��, ΪGraph<T>::Cost����. ���ڲ�Cost����Ϊdouble.
// ʵ������:
//  Graph<double> g;   // ������ͼ
//  g.AddVertex(3.5);  // ���һ����Ϊ3.5�Ķ���
//  g.AddVertex(4.5);  // ditto
//  g.AddEdge(3.5, 4.5, 10);  // ��3.5��4.5��������֮��ıߵ�Ȩ��Ϊ10.0
//  g.Print();         // ��ӡ�����ͼ
//  std::cout << g.IsConnected() << std::endl;  // �Ƿ�����ͨͼ

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
    // ����һ����ͼ(û�ж���, Ҳû�б�)
    Graph() : vertex_num_(0), edge_num_(0) {}
    Graph(const Graph &) = default;
    Graph(Graph &&) = default;
    Graph& operator=(const Graph &) = default;
    Graph& operator=(Graph &&) = default;

    // ����������ж�����ͬ, �����; ������ж�����ͬ, �����, �����warning
    void AddVertex(const T &);
    void AddVertex(const std::vector<T> &);
    // ���<v1, v2>����, ������.
    // ���������v1����v2, ��������, ���warning
    void AddEdge(const T &v1, const T &v2, const Cost &);
    // ��������vector, ��һ������Щ����Ҫ���, �ڶ����Ƕ�Ӧ��cost
    // Precondition: ����������Ԫ������һ��
    void AddEdge(const std::vector<std::pair<T, T>> &,
                 const std::vector<Cost> &);
    
    bool IsEmpty() const { return 0 == vertex_num_; }
    bool IsVertexExist(const T &v) const 
    {
        return std::find(vertex_.cbegin(), vertex_.cend(), v) != vertex_.cend();
    }
    // ���v1����v2����֮һ������, �򷵻�false
    bool IsConnected(const T &v1, const T &v2) const;
    // �Ƿ����������㶼��ͨ
    bool IsConnected() const;

    std::size_t GetVertexNum() const { return vertex_num_; }
    std::size_t GetEdgeNum() const { return edge_num_; }

    // ��һ���Ѻõķ�ʽ�������ͼ
    void Print() const;

    ~Graph() = default;

    // ��Ԫ
    friend class GraphAlgorithm;
private:
    // ���еĶ���
    std::list<T> vertex_;
    // ��ĳ����ֱ�������ĵ�(��ӷ�ӳ�˱ߵ����)
    std::map<T, std::list<std::pair<T, Cost>>> edges_;
    std::size_t vertex_num_;
    std::size_t edge_num_;

    // ���붥��v��ͨ�ĵ���ӵ�����s��, ��������v
    // traversed���¼��, ���traversed[v] == true, ��ʾֱ����v�����Ķ����Ѿ�������
    void GetConnectedSet(const T &v, std::unordered_set<T> &s, std::map<T, bool> &traversed) const;
};

template<typename T>
void Graph<T>::AddVertex(const T &v)
{
    // �������ӵĶ��㲻����
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
    // ��Ϊfor�ĳ�ʼ������ֻ������ͬһ�����͵�����, �ʴ˴�����ʹ��forѭ��
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
    // �������������ͨ��
    if (v1 == v2)
        return true;
    // ���ﲻ��ʹ��auto it = edges_[v1].cbegin();
    // ��Ϊmap��[]���������const��, ����IsConnected��Ҫ��const
    // (Ӧ��������)
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
    // ��������ʼ��, �����ļ�¼
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