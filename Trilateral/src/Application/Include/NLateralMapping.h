#include "../Include/NLateralDescriptor.h"
#include "../Include/DominantSymmetry.h"
#include "../Include/Skeleton.h"
std::pair<std::vector<NLateralDescriptor>, std::vector<NLateralDescriptor>> NlateralMap_point_matching_copy_symmetric_points(TrilateralMesh* m, Skeleton& skeleton, Plane& plane,
	int dvorak_enpoint_no, float sweep_distance, float hks_dif_param, float curv_param, float norm_angle_param, float skel_dist_param, float n_ring_param,
	float area_dif_param, float skel_point_dist_param, int N);

std::vector<NLateralDescriptor> NlateralMap_point_matching_w_average_geodesic(TrilateralMesh* m, Skeleton& skeleton,
	int dvorak_enpoint_no, float sweep_distance, float hks_dif_param, float curv_param, float norm_angle_param, float ratio_dif_param,
	float area_dif_param, float paths_dif_param,float min_geo_tau, int avg_n_ring,
	float tri_hist_param , float distance_to_mid_param , float sdf_param ,int N, std::vector<unsigned int>& agd_point_indices);


std::vector<NLateralDescriptor> NlateralMap_descriptor_generation(TrilateralMesh* m);