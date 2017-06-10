// 

#include <list>
#include <map>
#include <cstdlib>
#include <utility>
#include <vector>
#include <algorithm>

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

    std::size_t GetVertexNum() const { return vertex_num_; }
    std::size_t GetEdgeNum() const { return edge_num_; }

    // ��һ���Ѻõķ�ʽ�������ͼ
    void Print() const;

    ~Graph() = default;
private:
    // ���еĶ���
    std::list<T> vertex_;
    // ��ĳ����ֱ�������ĵ�(��ӷ�ӳ�˱ߵ����)
    std::map<T, std::list<std::pair<T, Cost>>> edges_;
    std::size_t vertex_num_;
    std::size_t edge_num_;
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
    // ���ﲻ��ʹ��auto it = edges_[v1].cbegin();
    // ��Ϊmap��[]���������const��, ����IsConnected��Ҫ��const
    // (Ӧ��������)
    auto &v1_list = edges_.at(v1);
    for (auto it = v1_list.cbegin(); it != v1_list.cend(); ++it)
        if (it->first == v2)
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