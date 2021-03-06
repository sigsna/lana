#pragma once
#ifndef NODE_STRENGTH_HPP_YWA85RA2
#define NODE_STRENGTH_HPP_YWA85RA2

#include <boost/graph/distributed/concepts.hpp>

using namespace boost;

template <typename Graph>
struct node_strength_measure {
    typedef typename graph_traits<Graph>::degree_size_type degree_type;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_type;
};

template <typename Graph, typename EdgeWeightMap>
struct out_strength_measure : public node_strength_measure<Graph> {
    typedef node_strength_measure<Graph> base_type;
    typedef typename base_type::degree_type degree_type;
    typedef typename base_type::vertex_type vertex_type;
    typedef typename graph_traits<Graph>::out_edge_iterator out_edge_iterator;

    inline degree_type operator()(vertex_type v, const Graph& g, EdgeWeightMap w) {
        function_requires< IncidenceGraphConcept<Graph> >();
        out_edge_iterator i, end;
        degree_type sum = 0;

        for (tie(i, end) = out_edges(v, g); i != end; ++i) {
            sum += w[*i];
        }

        return sum;
    }
};

template <typename Graph, typename EdgeWeightMap>
inline out_strength_measure<Graph, EdgeWeightMap>
measure_out_strength(const Graph&, EdgeWeightMap)
{
    return out_strength_measure<Graph, EdgeWeightMap>();
}

template <typename Graph, typename EdgeWeightMap>
struct in_strength_measure : public node_strength_measure<Graph> {
    typedef node_strength_measure<Graph> base_type;
    typedef typename base_type::degree_type degree_type;
    typedef typename base_type::vertex_type vertex_type;
    typedef typename graph_traits<Graph>::in_edge_iterator in_edge_iterator;

    inline degree_type operator()(vertex_type v, const Graph& g, EdgeWeightMap w) {
        function_requires< BidirectionalGraphConcept<Graph> >();
        in_edge_iterator i, end;
        degree_type sum = 0;

        for (tie(i, end) = in_edges(v, g); i != end; ++i) {
            sum += w[*i];
        }

        return sum;
    }
};

template <typename Graph, typename EdgeWeightMap>
inline in_strength_measure<Graph, EdgeWeightMap>
measure_in_strength(const Graph&, EdgeWeightMap)
{
    return in_strength_measure<Graph, EdgeWeightMap>();
}

template <typename Graph, typename Vertex, typename Measure, typename EdgeWeightMap>
inline typename Measure::degree_type
node_strength(const Graph& g, Vertex v, Measure measure, EdgeWeightMap w)
{
    // function_requires< StrengthMeasureConcept<Measure, Graph> >(); TODO: write this new concept
    return measure(v, g, w);
}

template <typename Graph, typename Vertex>
inline typename graph_traits<Graph>::degree_size_type
node_strength(const Graph& g, Vertex v)
{
    return node_strength(g, v, measure_gregariousness(g));
}


// These are alias functions, intended to provide a more expressive interface.

template <typename Graph, typename Vertex>
inline typename graph_traits<Graph>::degree_size_type
out_strength(const Graph& g , Vertex v)
{
    return node_strength(g, v, measure_out_strength(g));
}

template <typename Graph, typename Vertex>
inline typename graph_traits<Graph>::degree_size_type
in_strength(const Graph& g , Vertex v)
{
    return node_strength(g, v, measure_in_strength(g));
}


template <typename Graph, typename CentralityMap, typename EdgeWeightMap, typename Measure>
inline void
all_node_strengths(const Graph& g, CentralityMap cent, EdgeWeightMap weight, Measure measure)
{
    // function_requires< VertexListGraphConcept<Graph> >(); TODO: find a suitable concept
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::vertex_iterator VertexIterator;
    function_requires< WritablePropertyMapConcept<CentralityMap, Vertex> >();
    typedef typename property_traits<CentralityMap>::value_type Centrality;

    VertexIterator i, end;

    for (tie(i, end) = vertices(g); i != end; ++i) {
        Centrality c = node_strength(g, *i, measure, weight);
        put(cent, *i, c);
    }
}

template <typename Graph, typename CentralityMap, typename EdgeWeightMap>
inline void all_node_strengths(const Graph& g, CentralityMap cent, EdgeWeightMap weight)
{
    all_node_strengths(g, cent, weight, measure_out_strength(g, weight));
}

// More helper functions for computing out_strength and in_strength.
// I hate the names of these functions, but out_strength and in_strength
// don't pluralize too well.

template <typename Graph, typename CentralityMap, typename EdgeWeightMap>
inline void all_out_strength_values(const Graph& g, CentralityMap cent, EdgeWeightMap weight)
{
    all_node_strengths(g, cent, weight, measure_out_strength(g, weight));
}

template <typename Graph, typename CentralityMap, typename EdgeWeightMap>
inline void all_in_strength_values(const Graph& g, CentralityMap cent, EdgeWeightMap weight)
{
    all_node_strengths(g, cent, weight, measure_in_strength(g, weight));
}

#endif /* end of include guard: NODE_STRENGTH_HPP_YWA85RA2 */
