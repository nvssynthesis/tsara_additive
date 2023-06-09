//
//  Distance.h
//  probability_graph_testing
//
//  Created by Nicholas Solem on 5/15/23.
//  Copyright © 2023 Nicholas Solem. All rights reserved.
//

#ifndef Distance_h
#define Distance_h
#include "graph.h"

namespace nvs {
namespace dst {

std::optional<float> euc_distance(std::vector<float> const &x0, std::vector<float> const &x1);
std::optional<float> kullback_leibler_divergence(std::vector<float> const &x0, std::vector<float> const &x1);

// this will not be normalized!
template<typename float_t>
inline float_t distance2prob(float_t distance, float_t C_kernelScaling){
	auto num = -(distance*distance);
	auto den = (float_t)2.0 * (C_kernelScaling*C_kernelScaling);
	return std::exp(num / den);
}

float loudnessDistance(float src_loud, float dst_loud);

float pitchDistanceRatio(float src_freq, float dst_freq);
bool pitchDistanceInRange(float src_freq, float dst_freq, nvs::sgt::connectionHeuristics const& settings);

}	// namespace dst
}	// namespace nvs

#endif /* Distance_h */
