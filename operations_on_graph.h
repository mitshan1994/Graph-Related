#ifndef OPERATIONS_ON_GRAPH_H_
#define OPERATIONS_ON_GRAPH_H_
// 常用的对Graph数据结构的操作函数

#include "graph.h"
#include <iostream>
#include <string>

// 以一种格式化的方式读取图g, 返回true如果读取成功
// 输入格式: (字母解释: av(add vertex) ae(add edge))
// av 1 2 3 4 5              // 添加5个点
// ae 1 3 10.5               // 1与3之间的边cost为10.5
// ae 2 3 4                  // 2与3之间的边cost为4
// q                         // 读取结束
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