//
//  graph.h
//  pool2graph
//
//  Created by Nicholas Solem on 4/3/23.
//  Copyright © 2023 Nicholas Solem. All rights reserved.
//

#ifndef graph_h
#define graph_h
#include <cstring>
#include <iostream>
#include <fstream>
#include <array>
#include <limits>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/core/span.hpp>

#include "Random.h"
#include "Analysis.h"

namespace nvs {
namespace sgt {
struct connectionHeuristics;
}
}
#include "Distance.h"

namespace nvs{
namespace sgt {	// stochastic graph traversal
typedef std::vector<std::vector<essentia::Real>> multiDimRealVec_t;

struct vertex_property{
	double timestamp;
	unsigned int cluster;
	float const *fundamentalFreq;
	float const *voicedness;
	float const *loudness;
	std::vector<float> const *PCAvec;
};
struct edge_property{
    float timbral_distance;
    float temporal_distance;
	float pitch_distance;
	float loudness_distance;
};
typedef boost::adjacency_list < boost::vecS, // out-edges, a Sequence or an AssociativeContainer
						boost::vecS, 		// vertices, a Sequence or a RandomAccessContainer
						boost::bidirectionalS,	// was using directedS, but bidirectionalS allows access to IN edges
						vertex_property,	// VertexProperty
						edge_property	 	// EdgeProperty
						> DirectedGraph_t;
typedef boost::graph_traits<DirectedGraph_t>::edge_iterator edge_iterator_t;
typedef DirectedGraph_t::adjacency_iterator adjacency_iter_t;
typedef DirectedGraph_t::vertex_descriptor vertex_descriptor_t;
typedef DirectedGraph_t::vertex_iterator vertex_iterator_t;
typedef DirectedGraph_t::edge_descriptor edge_descriptor_t;

struct connectionHeuristics{
	float timbralPercentile {0.12f};
	size_t timbralNumSearch {100UL};
	float loudnessPercentile {0.15f};
	size_t loudnessNumSearch {100UL};
	float maxPitchDeviationRatio {5.f / 4.f};	// over 500 Hz
	float maxPitchDeviationAbsolute {100.f};	// under 500 Hz
	
	size_t minimumConnections {6};
	size_t minimumConnectionCandidates {18};
};



float getMaxTimbralDistanceForConnection(multiDimRealVec_t const &pca_mat, float percentile = 0.15f, size_t nSearch = 100UL);
float getMaxTimbralDistanceForConnectionFromVertex(const vertex_descriptor_t v, multiDimRealVec_t const &pca_mat, float percentile = 0.15f, size_t nSearch = 100UL);

float getMaxLoudnessDistanceForConnection(std::vector<float> const &loudnessVec, float percentile = 0.15f, size_t nSearch = 100UL);

// create directed graph based on numFrames
DirectedGraph_t createGraphFromAnalysisData(AnalysisData const &data, connectionHeuristics const settings = connectionHeuristics());

void printGraph(DirectedGraph_t const &dg);
void printEdges(DirectedGraph_t &dg);
void printProbs(std::vector<double> const &probs);

double getOutEdgeWeightPercentile(DirectedGraph_t &dg, vertex_iterator_t vit, double percentile = 0.5);

vertex_iterator_t vertexDescriptorToIterator(DirectedGraph_t const &dg, vertex_descriptor_t const v_dscr);
vertex_descriptor_t traverseToNearestVertex(DirectedGraph_t &dg, vertex_iterator_t vit);

inline std::vector<double> getProbabilitiesFromCurrentNode(DirectedGraph_t const &dg, vertex_descriptor_t current_vertex, const float C_kernelScaling);
inline void exaggerateProbabilities(std::vector<double> &probs, double power);
inline void normalizeProbabilities(std::vector<double> &probs);


vertex_descriptor_t traverseToRandomVertex(DirectedGraph_t &dg, vertex_descriptor_t current_vertex, const float C_kernelScaling = .01f, double probPower = 1.0);

void reduceEdges(DirectedGraph_t &dg, double percentile = 0.5);
void removeProportionOfVertices(DirectedGraph_t &dg, double proportion);
  
void exportGraphAsDot(DirectedGraph_t &dg, std::string const &filename);

// this is to replace use of raw DirectedGraph_t
// it will store probabilities, current node/iterator
class StatefulDirectedGraph;

} // end namespace sgt
} // end namespace nvs

#endif /* graph_h */
