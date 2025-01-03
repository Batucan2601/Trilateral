#pragma once 
#include <vector>
#include "../Include/TrilateralMesh.h"
#include "../Include/FibonacciHeap.h"
#include "../Include/NLateralDescriptor.h"

bool Geodesic_proximity(TrilateralMesh& m, NLateralDescriptor& desc1, NLateralDescriptor& desc2, float proximity);
std::vector<float> Geodesic_dijkstra(TrilateralMesh& m, int point_index);
std::vector<int> Geodesic_dijkstra_predecessors(TrilateralMesh& m, int point_index);
std::vector<int> Geodesic_between_two_points(TrilateralMesh& m, int p1_index, int p2_index);
std::vector<unsigned int> Geodesic_avg_dijkstra(TrilateralMesh* m, int& number_of_points,float sweep_distance,int N_ring, bool is_color);
std::vector<unsigned int> Geodesic_min_dijkstra(TrilateralMesh* m,std::vector<unsigned int> average_geodesic_function,
float sweep_percentage,float tau, bool is_color);
std::vector<unsigned int> Geodesic_avg_dijkstra_modified(TrilateralMesh* m, float sweep_percentage, int N_ring, bool is_color);
std::vector<unsigned int> Geodesic_avg_dijkstra_modified_to_points(TrilateralMesh* m, std::vector<unsigned int> points,
int& no_of_points, float sweep_percentage, int N_ring, bool is_color);

std::vector<unsigned int> Geodesic_find_biggest_AGD(TrilateralMesh* m, float sweep_percentage, int sample_size );
void Geodesic_write_sampled_points(TrilateralMesh* m, std::vector<unsigned int>& agd_points);
void Geodesic_read_sampled_points(TrilateralMesh* m, std::vector<unsigned int>& sampled_points);
unsigned int Geodesic_find_midpoint(TrilateralMesh* m, unsigned int index1, unsigned int index2);