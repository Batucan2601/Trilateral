#define _USE_MATH_DEFINES 
#include "../Include/DvorakEstimatingApprox.h"
#include <cmath>
#include "../Include/Geodesic.h"
float gaussian_curvature(TrilateralMesh* mesh , int index )
{
	float area = mesh->areas[index];
	float A_i = area / 3; // voronoi

	float gaussian_curvature = 0;
	float phi = 0; 
	for (size_t i = 0; i < mesh->adjacenies[index].size(); i++)
	{
		glm::vec3 p1 = mesh->vertices[index]; //itself 
		glm::vec3 p2 = mesh->vertices[mesh->adjacenies[index][i].first]; // second point
		for (size_t j = 0; j < mesh->adjacenies[index].size(); j++)
		{
			for (size_t k = 0; k < mesh->adjacenies[mesh->adjacenies[index][i].first].size(); k++)
			{
				int index_j = mesh->adjacenies[index][j].first;
				int index_k = mesh->adjacenies[mesh->adjacenies[index][i].first][k].first;
				if (index_j == index_k)
				{
					//calculate the cosine
					glm::vec3 p3 = mesh->vertices[index_j];
					float cosine = glm::dot(p3-p1 , p2-p1) / (glm::length(p2 - p1) * glm::length(p3 - p1));
					float arc_cos = std::acosf(cosine);
					phi += arc_cos;
				}
			}
		}

	}
	gaussian_curvature = 1.0 / A_i * (2 * M_PI - phi/2.0);
	return gaussian_curvature;
}
std::vector<DvorakPairs> dvorak_extraction_of_significant_points(TrilateralMesh* m, int P)
{
	std::vector<DvorakPairs> dvorak_pairs;
	int N = m->vertices.size();

	//sort the indices for biggest P 
	std::vector<std::pair<float, size_t>> value_index_pairs;
	std::vector<size_t> sorted_indices;
	for (size_t i = 0; i < N; ++i) 
	{
		value_index_pairs.emplace_back(gaussian_curvature(m, i), i);
	}
	std::sort(value_index_pairs.begin(), value_index_pairs.end(),
		[](const std::pair<float, size_t>& a, const std::pair<float, size_t>& b) {
			return a.first < b.first;
		});
	for (const auto& pair : value_index_pairs) {
		sorted_indices.push_back(pair.second);
	}

	for (size_t i = 0; i < P; i++)
	{
		DvorakPairs dvorakPair_i;
		dvorakPair_i.gaussian_curv = value_index_pairs[i].first;
		dvorakPair_i.p_index= sorted_indices[i];
		dvorak_pairs.push_back(dvorakPair_i);
	}
	return dvorak_pairs;

}
std::vector<DvorakPairs> dvorak_extraction_of_significant_points(TrilateralMesh* m, std::vector<unsigned int> & indices )
{
	std::vector<DvorakPairs> dvorak_pairs;
	int N = m->vertices.size();
	int P = indices.size();
	//sort the indices for biggest P 
	std::vector<std::pair<float, size_t>> value_index_pairs;
	std::vector<size_t> sorted_indices;
	for (size_t i = 0; i < P; ++i)
	{
		value_index_pairs.emplace_back(gaussian_curvature(m, indices[i]), indices[i]);
	}
	std::sort(value_index_pairs.begin(), value_index_pairs.end(),
		[](const std::pair<float, size_t>& a, const std::pair<float, size_t>& b) {
			return a.first < b.first;
		});
	for (const auto& pair : value_index_pairs) {
		sorted_indices.push_back(pair.second);
	}

	for (size_t i = 0; i < P; i++)
	{
		DvorakPairs dvorakPair_i;
		dvorakPair_i.gaussian_curv = value_index_pairs[i].first;
		dvorakPair_i.p_index = sorted_indices[i];
		dvorak_pairs.push_back(dvorakPair_i);
	}
	return dvorak_pairs;
}

Plane dvorak_generate_plane(MeshFactory& mesh_fac, int selected_index , int P , float S , float c_min_norm)
{
	TrilateralMesh* m = &mesh_fac.mesh_vec[selected_index];
	int N = m->vertices.size();
	std::vector<DvorakPairs> significant_points = dvorak_extraction_of_significant_points(m, P);
	std::vector<std::pair<int,int>> best_pairs = dvorak_chose_criterion( m , significant_points , S , c_min_norm );
	std::vector<Plane> candidate_planes = dvorak_generate_candidate_planes(m , best_pairs);
	Plane p;
	p.normal = glm::vec3(0, 0, 0);
	return p; 
}
bool dvorak_curvature_similarity_criterion(std::vector<DvorakPairs>& best_pairs, float S, int index1, int index2)
{
	float gaussian_comparison = best_pairs[index1].gaussian_curv / best_pairs[index2].gaussian_curv;
	if( S <= gaussian_comparison && gaussian_comparison <= 1.0f/S)
	{ 
		return true;
	}
	return false;
}
bool dvorak_normal_angle_criterion(TrilateralMesh* m , std::vector<DvorakPairs>& best_pairs, int index1 , int index2 , float c_min_norm)
{
	glm::vec3 x_i_x_j = m->vertices[best_pairs[index1].p_index] - m->vertices[best_pairs[index2].p_index];
	glm::vec3 n_i_n_j = m->normals[best_pairs[index1].p_index] - m->normals[best_pairs[index2].p_index];
	
	float cos_theta = glm::dot(x_i_x_j, n_i_n_j) / (glm::length(x_i_x_j) * glm::length(n_i_n_j));
	float cosine = glm::acos(cos_theta);

	if (cosine > c_min_norm)
	{
		return true; 
	}
	return false; 
}

std::vector<std::pair<int, int>> dvorak_chose_criterion(TrilateralMesh* m , std::vector<DvorakPairs>& significant_points , float S , float c_min_norm )
{
	std::vector<std::pair<int, int>> criter_pairs;
	// check for each pair
	int significant_p_size = significant_points.size();
	bool is_criter = false;
	for (size_t i = 0; i < significant_p_size; i++)
	{
		for (size_t j = i; j < significant_p_size; j++)
		{
			if (i == j)
			{
				continue; 
			}
			
			is_criter = dvorak_curvature_similarity_criterion(significant_points, S, i, j);
			if (!is_criter)
			{
				continue; 
			}
			is_criter = dvorak_normal_angle_criterion(m , significant_points,  i, j , c_min_norm);
			if (!is_criter)
			{
				continue; 
			}
			std::pair<int, int> temp_pair;
			temp_pair.first = significant_points[i].p_index;
			temp_pair.second = significant_points[j].p_index;
			criter_pairs.push_back(temp_pair);
			
		}
	}
	return criter_pairs;
}

std::vector<Plane> dvorak_generate_candidate_planes(TrilateralMesh* m, std::vector<std::pair<int, int>>& best_pairs ) 
{
	std::vector<Plane> planes; 
	for (size_t i = 0; i < best_pairs.size(); i++)
	{
		glm::vec3 n_i_j = m->vertices[best_pairs[i].first] - m->vertices[best_pairs[i].second];
		glm::vec3 x_i_j = (m->vertices[best_pairs[i].first] + m->vertices[best_pairs[i].second]) / 2.0f;
		float D = -(n_i_j.x * x_i_j.x + n_i_j.y * x_i_j.y + n_i_j.z * x_i_j.z);
		Plane p = generate_plane_from_formula(n_i_j.x, n_i_j.y, n_i_j.z, D);
		planes.push_back(p);
	}
	return planes; 
}


void dvorak_show_signifcant_points(TrilateralMesh* m , int P )
{
	std::vector<DvorakPairs> dvorak_pairs  = dvorak_extraction_of_significant_points(m, P);
	for (size_t i = 0; i < P; i++)
	{
		int index = dvorak_pairs[i].p_index;
		m->raylib_mesh.colors[index * 4] = 0;
		m->raylib_mesh.colors[index * 4 + 1] = 0;
		m->raylib_mesh.colors[index * 4 + 2] = 255;
		m->raylib_mesh.colors[index * 4 + 3] = 255;
	}
	m->update_raylib_mesh();
}

std::vector<DvorakPairs> dvorak_distance_sweep(TrilateralMesh* m, std::vector<DvorakPairs>& dvorak_pairs, float dist )
{
	std::vector<DvorakPairs> extracted_dvorak_pairs;
	std::sort(dvorak_pairs.begin(), dvorak_pairs.end(), [](const DvorakPairs a, DvorakPairs& b) {
		return a.gaussian_curv > b.gaussian_curv;
	});
	while (dvorak_pairs.size() > 0 )
	{
		DvorakPairs p = dvorak_pairs[0];
		extracted_dvorak_pairs.push_back(p);
		dvorak_pairs.erase(dvorak_pairs.begin(), dvorak_pairs.begin() + 1);

		int index = p.p_index;
		std::vector<float> distances = Geodesic_dijkstra(*m, index);
		std::vector<unsigned int> close_vertices;
		for (size_t i = 0; i < dvorak_pairs.size(); i++)
		{
			if (distances[dvorak_pairs[i].p_index] < dist)
			{
				close_vertices.push_back(i);
			}
		}

		std::sort(close_vertices.begin(), close_vertices.end(), std::greater<unsigned int>());

		// Remove elements from the vector starting from the highest index
		for (int index : close_vertices) {
			if (index >= 0 && index < dvorak_pairs.size()) {
				dvorak_pairs.erase(dvorak_pairs.begin() + index);
			}
			else {
				std::cerr << "Invalid index: " << index << std::endl;
			}
		}

	}

	for (size_t i = 0; i < m->vertices.size(); i++)
	{
		m->raylib_mesh.colors[i * 4] = 0;
		m->raylib_mesh.colors[i * 4 + 1] = 0;
		m->raylib_mesh.colors[i * 4 + 2] = 0;
		m->raylib_mesh.colors[i * 4 + 3] = 255;
	}
	for (size_t i = 0; i < extracted_dvorak_pairs.size(); i++)
	{
		int index = extracted_dvorak_pairs[i].p_index;
		m->raylib_mesh.colors[index * 4] = 0;
		m->raylib_mesh.colors[index * 4 + 1] = 0;
		m->raylib_mesh.colors[index * 4 + 2] = 255;
		m->raylib_mesh.colors[index * 4 + 3] = 255;

	}
	m->update_raylib_mesh();
	return extracted_dvorak_pairs;
}