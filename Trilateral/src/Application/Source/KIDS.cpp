#include "../Include/KIDS.h"
#include "../Include/DominantSymmetry.h"
#include "../Include/DvorakEstimatingApprox.h"
#include "../Include/TrilateralMap.h"
#include "../Include/NLateralDescriptor.h"
#include "../Include/TrilateralMap.h"
#include "../Include/HeatKernelSignature.h"
std::vector<TrilateralMesh> Kids_dataset;
std::vector<std::vector<TrilateralDescriptor>> Kids_dataset_desc_pos;
std::vector<std::vector<TrilateralDescriptor>> Kids_dataset_desc_neg;
std::vector<std::vector<unsigned int>>  KIDS_dataset_gaussian_indices;



void KIDS_dom_sym_generate_or_read_planes(float convergence_ratio)
{
	for (size_t i = 0; i < Kids_dataset.size(); i++)
	{
		TrilateralMesh* m = &Kids_dataset[i];
		snprintf(NULL, 0, "%f", convergence_ratio);
		std::string name = m->file_name + std::to_string(convergence_ratio) + ".pln";
		std::string path("../../Trilateral/Mesh/off/DomSym/");
		path = path + name;
		Plane plane;
		// try to read the plane
		bool is_already = dom_sym_read_plane(m, plane, path);
		if (!is_already )
		{
			plane = generate_dominant_symmetry_plane(m, convergence_ratio);
			dom_sym_write_plane(m, plane, path);
		}
	}
}

void KIDS_generate_gaussians(int no_of_gaussian , float sweep_distance )
{
	for (size_t i = 0; i < Kids_dataset.size(); i++)
	{
		TrilateralMesh* m = &Kids_dataset[i];
		std::vector<DvorakPairs> dvorak_pairs = dvorak_extraction_of_significant_points( m , no_of_gaussian);
		dvorak_pairs = dvorak_distance_sweep(m, dvorak_pairs, sweep_distance);
		
		std::vector<unsigned int> gaussian_indices_i;
		KIDS_dataset_gaussian_indices.push_back(gaussian_indices_i);
		
		for (size_t j = 0; j < dvorak_pairs.size(); j++)
		{
			int index = dvorak_pairs[j].p_index;
			KIDS_dataset_gaussian_indices[i].push_back(index);
		}
	}
}


void KIDS_select_mesh(TrilateralMesh& m, int meshNo)
{
	std::string skel_path("../../Trilateral/Mesh/off/KIDS_skeleton/");
	std::string skel_file_name = m.file_name.substr(0, m.file_name.size() - 3) + "swc";
	Skeleton skeleton = skeleton_read_swc_file(&m, skel_path + skel_file_name);
	std::string hks_path = "../../Trilateral/Mesh/off/HKS/";
	HKS_read_kernel_signature(&m, hks_path);
	m = Kids_dataset[meshNo];
}

void KIDS_endpoint_matching_w_gaussian( int gaussian_endpoint , float convergence_ratio)
{
	std::vector<Plane> planes;
	for (size_t i = 0; i < Kids_dataset.size(); i++)
	{
		TrilateralMesh* m = &Kids_dataset[i];
		snprintf(NULL, 0, "%f", convergence_ratio);
		std::string name = m->file_name + std::to_string(convergence_ratio) + ".pln";
		std::string path("../../Trilateral/Mesh/off/DomSym/");
		path = path + name;
		Plane plane;
		// try to read the plane
		bool is_already = dom_sym_read_plane(m, plane, path);
		if (!is_already)
		{
			plane = generate_dominant_symmetry_plane(m, convergence_ratio);
			dom_sym_write_plane(m, plane, path);
		}
		planes.push_back(plane);
	}
	for (size_t i = 0; i < Kids_dataset.size(); i++)
	{
		std::vector<TrilateralDescriptor> desc; 
 		Kids_dataset_desc_pos.push_back(desc);
		Kids_dataset_desc_neg.push_back(desc);
		trilateral_point_matching_with_dvorak_endpoints(&Kids_dataset[i], Kids_dataset_desc_pos[i],
			Kids_dataset_desc_neg[i], planes[i], gaussian_endpoint, convergence_ratio);
		KIDS_save_descriptors(i, convergence_ratio);
	}
}

void KIDS_save_descriptors(int index, float convergence_ratio)
{
	TrilateralMesh* m = &Kids_dataset[index];
	std::string name = m->file_name + std::to_string(convergence_ratio) + ".dsc";
	std::string path("../../Trilateral/Mesh/off/Descriptor/");
	path = path + name; 
	TrilateralDescriptor_write(path, Kids_dataset_desc_pos[index], Kids_dataset_desc_neg[index]);
}


void KIDS_endpoint_matching_w_NLateral(int gaussian_end_point_no, float sweep_distance, int N)
{
	for (size_t i = 0; i < Kids_dataset.size(); i++)
	{
		TrilateralMesh* m = &Kids_dataset[i];
		std::string file_name = "";
		// 1- get the skeleton
		std::string skel_path("../../Trilateral/Mesh/off/KIDS_skeleton/");
		std::string skel_file_name = m->file_name.substr(0, m->file_name.size() - 3) + "swc";
		Skeleton skeleton = skeleton_read_swc_file(m, skel_path + skel_file_name);
		std::string hks_path = "../../Trilateral/Mesh/off/HKS/";
		HKS_read_kernel_signature(m , hks_path);
		//std::vector<NLateralDescriptor> descs=  NlateralMap_point_matching_with_skeleton_endpoints_and_OT_without_sym_plane(m, skeleton,gaussian_end_point_no, 0, N);
		//std::string desc_path("../../Trilateral/Mesh/off/Descriptor/");
		//NLateralDescriptor_write(desc_path + m->file_name, m, descs);
	}
}