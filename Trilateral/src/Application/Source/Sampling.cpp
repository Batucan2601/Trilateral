#include "../Include/Sampling.h"
#include "../Include/Geodesic.h"
#include <stdlib.h>     /* srand, rand */
#include <random>
std::vector<unsigned int>  furthest_point_sampling(TrilateralMesh* m, int no_of_samples, bool is_points_colored )
{
	int N = m->vertices.size();
	float* distance = new float[N];
	int* sampled = new int[no_of_samples];
	//initialize distance
	for (size_t i = 0; i < N; i++)
	{
		distance[i] = 1e6; 
	}
	for (size_t i = 0; i < no_of_samples; i++)
	{
		sampled[i] = -1;
	}
	srand(time(NULL));
	//sampling starts
	int sample_idx =  0;
	sampled[0] = sample_idx;
	for (size_t i = 1; i < no_of_samples; i++)
	{
		//update distances
		std::vector<float> distance_matrix_p1 = Geodesic_dijkstra(*m, sample_idx);
		for (size_t j = 0; j < N; j++)
		{
			if (distance_matrix_p1[j] < distance[j])
			{
				distance[j] = distance_matrix_p1[j];
			}
		}
		//sample next
		// get max
		float maxValue = -1;
		int maxIndex = -1;
		for (size_t j = 0; j < N; j++)
		{
			if (maxValue < distance[j])
			{
				maxValue = distance[j];
				maxIndex = j; 
			}
		}
		sample_idx = maxIndex;
		sampled[i] = sample_idx;

	}
	std::vector<unsigned int> sampled_id_vector; 
	for (size_t i = 0; i < no_of_samples; i++)
	{
		sampled_id_vector.push_back(sampled[i]);
	}
	delete[] distance;
	delete[] sampled;


	// redrawing part
	if (is_points_colored)
	{
		for (size_t i = 0; i < sampled_id_vector.size(); i++)
		{
			int index = sampled_id_vector[i];
			m->raylib_mesh.colors[index * 4] = 255;
			m->raylib_mesh.colors[index * 4 + 1] = 0;
			m->raylib_mesh.colors[index * 4 + 2] = 0;
			m->raylib_mesh.colors[index * 4 + 3] = 255;
		}
	}
	m->update_raylib_mesh();

	return sampled_id_vector;
}

std::vector<unsigned int>  random_symmetry_indices_sampling(TrilateralMesh* m, int no_of_samples)
{
	srand(time(NULL));
	std::vector<unsigned int> random_sym_pairs;
	std::vector<bool> is_sample_exists(m->vertices.size(), 0);
	for (size_t i = 0; i < no_of_samples / 2 ; i++)
	{

		int random_no;
		while (true)
		{
			random_no = rand() % (m->vertices.size());
			if (!is_sample_exists[random_no])
			{
				is_sample_exists[random_no] = true; 
				break; 
			}
		}
		for (size_t j = 0; j < m->symmetry_pairs.size(); j++)
		{
			if (m->symmetry_pairs[j].first == random_no )
			{
				random_sym_pairs.push_back(m->symmetry_pairs[j].first);
				random_sym_pairs.push_back(m->symmetry_pairs[j].second);
				is_sample_exists[m->symmetry_pairs[j].second] = true; 
			}
		}
	}

	m->colors.clear();
	// color
	for (size_t i = 0; i < m->vertices.size(); i++)
	{
		if (is_sample_exists[i])
		{
			m->colors.push_back(glm::vec3(1.0f, 1.0f, 0.0f));
		}
		else
		{
			m->colors.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
		}
	}

	return random_sym_pairs; 
}

//furthest points sampling in specific part of a mesh decided by vector partial points 
std::vector<unsigned int>  furthest_point_sampling_on_partial_points(TrilateralMesh* m, int no_of_samples, std::vector<unsigned int>& map_partial_to_original)
{
	// generate a new mesh from existing mesh
	TrilateralMesh partial_mesh = *m;
	// create a boolean vector in order to select partial indices for O(1) selection
	std::vector<bool> is_point_on_partial_mesh(m->vertices.size() , false);
	for (size_t i = 0; i < map_partial_to_original.size(); i++)
	{
		is_point_on_partial_mesh[map_partial_to_original[i]] = true;
	}
	std::vector<unsigned int> map_original_to_partial(m->vertices.size(), -1);
	for (size_t i = 0; i < map_partial_to_original.size(); i++)
	{
		map_original_to_partial[map_partial_to_original[i]] = i;
	}

	partial_mesh.vertices.clear();
	partial_mesh.adjacenies.clear();
	partial_mesh.colors.clear();
	for (size_t i = 0; i < map_partial_to_original.size(); i++)
	{
		partial_mesh.vertices.push_back(m->vertices[map_partial_to_original[i]]);
	}
	for (size_t i = 0; i < map_partial_to_original.size(); i++)
	{
		partial_mesh.adjacenies.push_back(std::vector<std::pair<int, float>>());
		for (size_t j = 0; j < m->adjacenies[map_partial_to_original[i]].size(); j++)
		{
			if (is_point_on_partial_mesh[m->adjacenies[map_partial_to_original[i]][j].first ])
			{
				std::pair<int, float> adjacency;
				adjacency.first = map_original_to_partial[m->adjacenies[map_partial_to_original[i]][j].first];
				adjacency.second = m->adjacenies[map_partial_to_original[i]][j].second;
				partial_mesh.adjacenies[i].push_back(adjacency);
			}
		}
		partial_mesh.colors.push_back(glm::vec3(0, 0, 0));
	}

	//now with the new mesh, do furthespoint search 
	std::vector<unsigned int>  partial_mesh_fps_points =  furthest_point_sampling(&partial_mesh, no_of_samples , false);
	//convert the points back 
	std::vector<unsigned int> fps_points_corrected;
	for (size_t i = 0; i < partial_mesh_fps_points.size(); i++)
	{
		unsigned int fps_index_for_partial_point = map_partial_to_original[partial_mesh_fps_points[i]];
		
		fps_points_corrected.push_back(fps_index_for_partial_point);
	}

	for (size_t i = 0; i < fps_points_corrected.size(); i++)
	{
		int index = fps_points_corrected[i];
		m->raylib_mesh.colors[index * 4] = 255;
		m->raylib_mesh.colors[index * 4 + 1] = 0;
		m->raylib_mesh.colors[index * 4 + 2] = 0;
		m->raylib_mesh.colors[index * 4 + 3] = 255;
	}
	return fps_points_corrected;
}


std::vector<unsigned int>  uniform_point_sampling(TrilateralMesh* m, int no_of_samples, bool is_points_colored )
{
	float total_area = 0;
	float cum_sum = 0;
	int N = m->vertices.size();
	for (size_t i = 0; i < N; i++)
	{
		total_area += m->areas[i];
	}
	std::vector<float> cumulative_distribution;
	for (size_t i = 0; i < N; i++)
	{
		cum_sum += m->areas[i] / total_area;
		cumulative_distribution.push_back(cum_sum);
	}

	// Random sampling
	std::vector<unsigned int> samples;
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<double> dist(0.0, 1.0);
	
	for (int i = 0; i < no_of_samples; ++i) {
		double r = dist(rng);

		// Find the triangle to sample using binary search
		auto it = std::lower_bound(cumulative_distribution.begin(), cumulative_distribution.end(), r);
		int triIndex = std::distance(cumulative_distribution.begin(), it);

		// Sample a point within the selected triangle
		//Vec3 sampledPoint = samplePointInTriangle(mesh[triIndex], rng);
		samples.push_back(triIndex);
	}

	for (size_t i = 0; i < no_of_samples; i++)
	{
		int index = samples[i];
		m->raylib_mesh.colors[index * 4] = 255;
		m->raylib_mesh.colors[index * 4 + 1] = 0;
		m->raylib_mesh.colors[index * 4 + 2] = 0;
		m->raylib_mesh.colors[index * 4 + 3] = 255;
	}
	m->update_raylib_mesh();
	
	return samples;
}
std::vector<unsigned int>  midpoint_sampling(TrilateralMesh* m, float no_of_samples_percentage , float& biggest,
unsigned int mid1 , unsigned int mid2) //sampling from midpoint
{
	//get the mid points
	unsigned int no_of_samples = m->vertices.size() * no_of_samples_percentage; 
	std::vector<unsigned int> sampled_points = { mid1 , mid2  };
	for (size_t i = 0; i < no_of_samples ; i++)
	{
		float max_distance = 0; 
		int index = -1;
		std::vector < std::vector<float>> distances_of_distances; 
		for (size_t j = 0; j < sampled_points.size(); j++)
		{
			std::vector<float> distances = Geodesic_dijkstra(*m, sampled_points[j]);
			distances_of_distances.push_back(distances);
		}
		std::vector<float> total_distances(m->vertices.size(), 0);
		for (size_t j = 0; j < sampled_points.size(); j++)
		{
			for (size_t k = 0; k < m->vertices.size(); k++)
			{
				total_distances[k] += distances_of_distances[j][k];
			}
		}
		unsigned int index_max_elem = sampled_points[0];
		while (std::find(sampled_points.begin(), sampled_points.end(), index_max_elem) != sampled_points.end())
		{
			auto max_elem = std::max_element(total_distances.begin(), total_distances.end());
			index_max_elem = std::distance(total_distances.begin(), max_elem);
			total_distances.erase(total_distances.begin() + index_max_elem);
		}

		sampled_points.push_back(index_max_elem);
	}

	/*for (size_t i = 0; i < no_of_samples * 3; i++)
	{
		float max_distance = 0;
		int index = -1;
		std::vector < std::vector<float>> distances_of_distances;
		for (size_t j = 0; j < sampled_points.size(); j++)
		{
			std::vector<float> distances = Geodesic_dijkstra(*m, sampled_points[j]);
			distances_of_distances.push_back(distances);
		}
		std::vector<float> total_distances(m->vertices.size(), 0);
		for (size_t j = 0; j < sampled_points.size(); j++)
		{
			for (size_t k = 0; k < m->vertices.size(); k++)
			{
				total_distances[k] += distances_of_distances[j][k];
			}
		}

		unsigned int index_min_elem = sampled_points[0];
		while (std::find(sampled_points.begin(), sampled_points.end(), index_min_elem) != sampled_points.end())
		{
			auto min_elem = std::max_element(total_distances.begin(), total_distances.end());
			index_min_elem = std::distance(total_distances.begin(), min_elem);
			total_distances.erase(total_distances.begin() + index_min_elem);
		}
		sampled_points.push_back(index_min_elem);
	}*/

	//get another for minimum
	m->color_points(sampled_points, GREEN);
	return sampled_points;

}