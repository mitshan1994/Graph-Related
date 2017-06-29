#define BOOST_TEST_MODULE have_a_try

#include "graph.h"
#include "operations_on_graph.h"
#include "graph_algorithms.h"
#include <boost/test/included/unit_test.hpp>
#include <map>
#include <iostream>

//BOOST_AUTO_TEST_CASE(example) {
//    Graph<int> g;
//    g.AddVertex({ 1, 2, 3 });
//    g.AddEdge(1, 2, 5.6);
//    g.AddEdge(2, 3, 6.4);
//    BOOST_TEST(g.IsConnected() == true);
//    g.Print();
//}

//BOOST_AUTO_TEST_CASE(connectivity) {
//    Graph<int> g;
//    ReadGraph(g);
//    g.Print();
//    BOOST_TEST(g.IsConnected());
//}

//BOOST_AUTO_TEST_CASE(member_function_test) {
//    using S = typename GraphAlgorithm::Statues<int>;
//    std::map<int, S> table;
//    table[1] = S{ false };
//    table[2] = S{ true };
//    table[3] = S{ true };
//    BOOST_TEST(GraphAlgorithm::HasFinished(table) == false);
//    table[1].known = true;
//    BOOST_TEST(GraphAlgorithm::HasFinished(table) == true);
//}
//
//// test PickNextVertice function
//BOOST_AUTO_TEST_CASE(choose_vertex) {
//    // simulated table: (next to be picked is vertex 4)
//    // v   known  dv   pv
//    // 1     T     0    1
//    // 2     F     3    1
//    // 3     F    inf   -
//    // 4     F     2    1
//    // 5     T     1    1
//    // 6     F     4    5
//    std::map<int, GraphAlgorithm::Statues<int>> table;
//    table[1] = { true, 0, 1 };
//    table[2] = { false, 3, 1 };
//    table[3] = { false, kInfinity };
//    table[4] = { false, 2, 1 };
//    table[5] = { true, 1, 1 };
//    table[6] = { false, 4, 5 };
//    int next_picked = GraphAlgorithm::PickNextVertice<int>(table);
//    BOOST_TEST(next_picked == 4);
//    table[4].dv = 10;
//    next_picked = GraphAlgorithm::PickNextVertice<int>(table);
//    BOOST_TEST(2 == next_picked);
//    BOOST_TEST(GraphAlgorithm::GetPv(5, table) == 1);
//    BOOST_TEST(GraphAlgorithm::GetPv(6, table) == 5);
//}
//
//// 手工测试, 直接避免写main函数
//BOOST_AUTO_TEST_CASE(main_test) {
//    Graph<int> g;
//    ReadGraph(g);
//    g.Print();
//    GraphAlgorithm::MinSpanningTree(g).Print();
//}

BOOST_AUTO_TEST_CASE(euler_circuits) {
    Graph<int> g;
    ReadGraph(g);
    g.Print();
    std::cout << "Does euler circuit exists : "
        << GraphAlgorithm::EulerCircuitExist(g) << std::endl;
}