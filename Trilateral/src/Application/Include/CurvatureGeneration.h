#pragma once 
#include <vector>
#include "glm/glm.hpp"
#include "TrilateralMesh.h"
#include "NLateralDescriptor.h"
struct CurvePoints
{
	std::pair<unsigned int, unsigned int> correspondence;
	unsigned int mid_point;
	unsigned int strength; // how many points have been here
	float quality;
	bool is_strong;
};
struct Curvature
{
	std::vector<CurvePoints> curve_points; 
	std::vector<std::vector<unsigned int>> paths;
	std::vector<float> curve_quality; // quality of the curve not the points 
	unsigned int midpoint_index; 
	unsigned int midpoint_inv_index;
	std::vector < std::pair<unsigned int, unsigned int>> removed_pairs; // cant pair these again. 
	float get_avg_quality(TrilateralMesh* m );
	std::vector<unsigned int> get_strong_points();
	void add_strong_list(std::vector<unsigned int> strong_list);
	void generate_curve_quality(TrilateralMesh* m);

};

struct Curve
{
	std::vector<unsigned int> curve_path; 
	float length; 
};

Curvature CurvatureGeneration_generate(TrilateralMesh* m, std::vector<unsigned int>& agd_indices,
	float hks_param , float quality_param);
std::pair<Curvature, Curvature> CurvatureGeneration_generate_full_curv(TrilateralMesh* m, std::vector<unsigned int>& agd_indices,
	float hks_param, float quality_param, float distance_param, float closeness_param, float fuziness, float biggest_dijkstra, std::vector<unsigned  int>& original_agd_indices);
void CurvatureGeneration_update(TrilateralMesh* m, Curvature& c, std::vector<unsigned int>& agd_indices,
	float hks_param, float quality_param);
void CurvatureGeneration_update_w_quality(TrilateralMesh* m, Curvature& c, std::vector<unsigned int>& agd_indices,
	float hks_param, float quality_param);
void CurvatureGeneration_curvature_quality(TrilateralMesh* m, Curvature& curv);
std::vector<Curve> CurvatureGeneration_generate_curve_paths(TrilateralMesh* m);
float  CurvatureGeneration_get_curve_length(TrilateralMesh* m, Curve& curv  );

void CurvatureGeneration_laplacian_smoothing(TrilateralMesh* m, Curvature& c, float quality_param  );
bool CurvatureGeneration_curve_smoothing(TrilateralMesh* m, Curvature& c, float quality_dif_param);

bool CurvatureGeneration_add_new_matching(TrilateralMesh* m, Curvature& c,
	std::vector<unsigned int>& agd_indices, unsigned int mid_point_index, unsigned int mid_point_index_2,
	float quality_param, float hks_param, float distance_to_midpoint_param, float closeness_param,
	float fuziness, float biggest_dijkstra, std::vector<unsigned int> original_agd_vertices);

void  curve_prune(TrilateralMesh* m, Curvature& c, bool is_front, bool is_back);
bool check_if_correspondece_cuts(TrilateralMesh* m, Curvature& c, NLateralDescriptor& desc1, NLateralDescriptor& desc2);

void CurvatureGeneration_second_step(TrilateralMesh* m , Curvature& c);
bool Curvature_curve_intersection_with_two_points(TrilateralMesh* m, Curvature& c, unsigned int p1, unsigned int p2, float ratio_param);
bool Curvature_curve_intersection_with_two_points(TrilateralMesh* m, Curvature& c, unsigned int p1, unsigned int p2, int& no_of_hits);
void Curvature_extend_curvature(TrilateralMesh* m, Curvature& c);
void Curvature_color_sides(TrilateralMesh* m, Curvature& c);
bool Curvature_compare_with_new_addition(TrilateralMesh* m, Curvature c, NLateralDescriptor& desc1, NLateralDescriptor& desc2);
void build_curvature(TrilateralMesh* m, Curvature& curvature);

void CurvatureGeneration_add_new_point_to_curve_PCA(TrilateralMesh* m, Curvature& c);
Curvature Curvature_generation_connect_front_and_back(TrilateralMesh* m, Curvature& front, Curvature& back);
Curvature CurvatureGeneration_move_PCA_and_connect(TrilateralMesh* m, Curvature& front,
	Curvature& back);


Curvature CurvatureGeneration_generate_curve_w_sym_pairs(TrilateralMesh* m);