#define BOOST_TEST_MODULE have_a_try

#include "graph.h"
#include "operations_on_graph.h"
#include "graph_algorithms.h"
#include <boost/test/included/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE(member_function_test) {
    using S = typename GraphAlgorithm::Statues<int>;
    std::map<int, S> table;
    table[1] = S{ false };
    table[2] = S{ true };
    table[3] = S{ true };
    BOOST_TEST(GraphAlgorithm::HasFinished(table) == false);
    table[1].known = true;
    BOOST_TEST(GraphAlgorithm::HasFinished(table) == true);
}