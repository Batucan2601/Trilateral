#include <vector>
#include "glm/glm.hpp"
#include "TrilateralMesh.h"
struct CurvePoints
{
	std::pair<unsigned int, unsigned int> correspondence;
	unsigned int mid_point;
	float quality;
};
struct Curvature
{
	std::vector<CurvePoints> curve_points; 
	std::vector<std::vector<unsigned int>> paths; 
};

struct Curve
{
	std::vector<unsigned int> curve_path; 
	float length; 
};

Curvature CurvatureGeneration_generate(TrilateralMesh* m, std::vector<unsigned int>& agd_indices,
	float hks_param , float quality_param);
void CurvatureGeneration_update(TrilateralMesh* m, Curvature& c, std::vector<unsigned int>& agd_indices,
	float hks_param, float quality_param);
void CurvatureGeneration_curvature_quality(TrilateralMesh* m, Curvature& curv);
std::vector<Curve> CurvatureGeneration_generate_curve_paths(TrilateralMesh* m);
float  CurvatureGeneration_get_curve_length(TrilateralMesh* m, Curve& curv  );
void CurvatureGeneration_mid_point_w_AGD(TrilateralMesh* m, unsigned int& p1, unsigned int& p2);

