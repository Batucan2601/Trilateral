#pragma once
#include "glm/glm.hpp"
#include <map>
#include "Skeleton.h"
#include "TrilateralMesh.h"
#include "MeshFactory.h"



struct SkeletalNLateral
{
	int N;
	std::vector<int> point_indices;
	std::vector<std::vector<float>>  geodesic_distances;
	std::vector<std::vector<int>>  predecessor_list;
	SkeletalNLateral(Skeleton& skeleton, const std::vector<int>& point_indices, int N);
	float geo_dist_to_skel_mid_point;

};

std::vector<std::pair<int, int>> SkeletalNLateral_compare_endpoints_with_SkeletalNlateral(Skeleton& skeleton,TrilateralMesh*m, int N , std::vector<float>& mesh_sdf_array);
float SkeletalNLateral_compareTwoSkeletalNLateral(SkeletalNLateral& nLateral1, SkeletalNLateral& nLateral2, int N);
void SkeletalNLateral_buffer();
void SkeletalNLateral_generate_buffer(Skeleton& skeleton, std::vector<std::pair<int, int>>& pairs);
void SkeletalNLateral_draw(MeshFactory& mesh_fac, unsigned int shader_id);
