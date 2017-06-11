#define BOOST_TEST_MODULE have_a_try

#include "graph.h"
#include "operations_on_graph.h"
#include <boost/test/included/unit_test.hpp>

//BOOST_AUTO_TEST_CASE(example) {
//    Graph<int> g;
//    g.AddVertex({ 1, 2, 3 });
//    g.AddEdge(1, 2, 5.6);
//    g.AddEdge(2, 3, 6.4);
//    BOOST_TEST(g.IsConnected() == true);
//    g.Print();
//}

BOOST_AUTO_TEST_CASE(connectivity) {
    Graph<int> g;
    ReadGraph(g);
    g.Print();
    BOOST_TEST(g.IsConnected());
}