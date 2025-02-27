#pragma once 
#include <vector>
#include "Histogram.h"
#include "Skeleton.h"

enum PointStatus
{
	INSIDE,
	OUTSIDE,
	EDGE,
	MIDPOINT,
};
enum TrilateralGeometry
{
	ENCLOSED, // expected case
	LINEAR, // no closed space
	ENCLOSED_TWO // two points enclose 
};
struct TrilateralDescriptor
{
	double area; // ROI
	double total_length;
	double geodesic_lenght_1_2;//  geodesic length between 1 - 2
	double geodesic_lenght_1_3;//  length between 1 - 3
	double geodesic_lenght_2_3;//  length between 2 - 3
	double euclidian_lenght_1_2;//  euclidian length between 1 - 2
	double euclidian_lenght_1_3;//  euclidian length between 1 - 2
	double euclidian_lenght_2_3;//  euclidian length between 1 - 2
	double curvature_1_2;
	double curvature_1_3;
	double curvature_2_3;
	unsigned int p1; //point indices
	unsigned int p2;
	unsigned int p3;
	// extras
	float n_ring_area_p1;
	float n_ring_area_p2;
	float n_ring_area_p3;
	// histogram
	Histogram histogram;
	std::vector<unsigned int> visited_indices;
	std::vector<int> path_1_2;
	std::vector<int> path_1_3;
	std::vector<int> path_2_3;
	TrilateralMesh m_inside;
	TrilateralGeometry geo;
	Histogram hist_path_1_2;
	Histogram hist_path_1_3;
	Histogram hist_path_2_3;
	Eigen::VectorXd weight; 
	TrilateralDescriptor();
	bool check_colinearity();
};

void TrilateralDescriptor_generate_mesh_inside(TrilateralMesh* m, TrilateralDescriptor& desc);
void TrilateralDescriptor_generate_descriptor_with_resolution(TrilateralMesh* m_inside, TrilateralDescriptor& desc);
void TrilateralDescriptor_generate_mesh_with_resolution(TrilateralMesh* m, TrilateralDescriptor& desc, int res);
TrilateralDescriptor  TrilateralDescriptor_create(TrilateralMesh* m, int point_index1, int point_index2, int point_index3, bool is_simplified);
void TrilateralDescriptor_write(std::string filename, std::vector<TrilateralDescriptor>& positive_desc, std::vector<TrilateralDescriptor>& negative_desc);
void TrilateralDescriptor_read(std::string filename, std::vector<TrilateralDescriptor>& positive_desc, std::vector<TrilateralDescriptor>& negative_desc);
Histogram TrilateralDescriptor_generate_cdf_of_areas(TrilateralMesh* m, TrilateralDescriptor& desc1, int division_no);
void  TrilateralDescriptor_get_trilateral_with_closest_points(TrilateralMesh* m , Skeleton& skel, std::vector<unsigned int>& mesh_indices,SkeletonTree& skelTree,
std::vector<TrilateralDescriptor>& descriptors);
