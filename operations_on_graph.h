#ifndef OPERATIONS_ON_GRAPH_H_
#define OPERATIONS_ON_GRAPH_H_
// ���õĶ�Graph���ݽṹ�Ĳ�������

#include "graph.h"
#include <iostream>
#include <string>

// ��һ�ָ�ʽ���ķ�ʽ��ȡͼg, ����true�����ȡ�ɹ�
// �����ʽ: (��ĸ����: av(add vertex) ae(add edge))
// av 1 2 3 4 5              // ���5����
// ae 1 3 10.5               // 1��3֮��ı�costΪ10.5
// ae 2 3 4                  // 2��3֮��ı�costΪ4
// q                         // ��ȡ����
// Precondition: g.empty() == true
template <typename T>
bool ReadGraph(Graph<T>& g)
{
    std::string command;
    T v1, v2;
    typename Graph<T>::Cost c;
    while (std::cin >> command) {
        if (command == "q")
            return true;
        else if (command == "av") {
            while (std::cin >> v1)
                g.AddVertex(v1);
            std::cin.clear();
        }
        else if (command == "ae") {
            std::cin >> v1 >> v2 >> c;
            g.AddEdge(v1, v2, c);
        }
        else
            return false;
    }
    return false;
}

#endif