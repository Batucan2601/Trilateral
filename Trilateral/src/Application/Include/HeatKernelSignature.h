#pragma once 
#include "../Include/Laplace-Beltrami.h"
#include "../Include/MeshFactory.h"
#include "../Include/TrilateralDescriptor.h"

void HKS_extract_kernel_signature(TrilateralMesh* m);

void HKS_read_kernel_signature(TrilateralMesh* m , std::string& path);

std::vector<std::vector<double>> HKS_compute_kernel(TrilateralMesh* m, std::pair<Eigen::VectorXd, Eigen::MatrixXd>& eigen_pairs, const std::vector<double>& timeSteps
, int time_step_no);

void HKS_hks_on_descriptor(TrilateralMesh* m, TrilateralDescriptor& desc);

std::vector<std::pair<int, float>> HKS_extraction_significant_points(TrilateralMesh* m, int P);
std::vector<std::pair<int, float>> HKS_sweep_distance(TrilateralMesh* m, std::vector<std::pair<int, float>> pair, float distance);


std::vector<DvorakPairs> HKS_to_dvorak_pairs(TrilateralMesh* m, std::vector<std::pair<int, float>>& pairs);
