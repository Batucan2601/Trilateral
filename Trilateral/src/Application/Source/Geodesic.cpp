#include "../Include/Geodesic.h"
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include "../Include/CoreTypeDefs.h"
#include "raylib.h"
// Function to find n-ring neighbors for a specific point
static std::vector<unsigned int> findNRingNeighbors(TrilateralMesh* m, int startPointId, int n) {
	std::unordered_set<int> visited; // Track visited points
	std::queue<std::pair<int, int>> q; // Pair of (current point ID, depth level)
	std::unordered_set<int> result; // Store neighbors within n-rings

	q.push({ startPointId, 0 });
	visited.insert(startPointId);

	while (!q.empty()) {
		std::pair<int,int>p = q.front();
		int depth = p.second;
		q.pop();

		if (depth > 0) {
			result.insert(p.first);
		}

		if (depth < n) {

			for (int i = 0; i < m->adjacenies[p.first].size(); i++ ) {
				int neighbour = m->adjacenies[p.first][i].first;
				if (visited.find(neighbour) == visited.end()) {
					visited.insert(neighbour);
					q.push({ neighbour, depth + 1 });
				}
			}
		}
	}
	std::vector<unsigned int> return_vec; 
	for (auto& num : result) {
		return_vec.push_back(num);
	}
	return return_vec;
}

std::vector<float> Geodesic_dijkstra(TrilateralMesh& m, int point_index)
{
	// return for prdecessors
	std::vector<float> matrix_vec;
	//init fibonacci heap
	FibonacciHeap<std::pair<float, int >> min_fib_heap;
	//init array 
	float* matrix = new float[m.vertices.size()];
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		if (point_index == i)
		{
			matrix[i] = 0.0f;
		}
		else
		{
			//INFINITY
			matrix[i] = (float)INFINITY;
		}
	}
	//init extra stuff
	int vertex_visited = 0; //total umber of visits 
	unsigned int* is_vertex_visited = new unsigned int[m.vertices.size()];
	int* predecessor = new int[m.vertices.size()]; //predecessor vertices for all 
	//float* will_be_visited_list = new float[m.vertices.size()];
	// std::vector<std::pair<int , float>> shortest_path_set; //not yet included but calculated 
	//fill array 
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		is_vertex_visited[i] = 0; // not visited
	}
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		predecessor[i] = -1; // not visited
	}

	//dijkstra
	std::pair<float, int > first_index;
	first_index.first = 0.0f; //distance 
	first_index.second = point_index;

	std::pair<float, int > current_pair;
	min_fib_heap.insert(first_index);
	while (!min_fib_heap.isEmpty())
	{
		current_pair = min_fib_heap.removeMinimum();
		// get the adjacent vertices
		int current_index = current_pair.second;
		// if it is already visited pass
		if (is_vertex_visited[current_index] == 1)
		{
			continue;
		}
		is_vertex_visited[current_index] = 1;
		float distance = current_pair.first;
		for (size_t i = 0; i < m.adjacenies[current_index].size(); i++)
		{
			int adjacent_index = m.adjacenies[current_index][i].first;
			float adjacent_distance = m.adjacenies[current_index][i].second;
			if (is_vertex_visited[adjacent_index] == 0)
			{
				//relaxation
				if (matrix[adjacent_index] > distance + m.adjacenies[current_index][i].second)
				{
					//update
					matrix[adjacent_index] = distance + m.adjacenies[current_index][i].second;
					predecessor[adjacent_index] = current_index;
					//push 
					std::pair<float, int> new_vertex;
					new_vertex.first = matrix[adjacent_index];
					new_vertex.second = adjacent_index;
					min_fib_heap.insert(new_vertex);
				}
			}
		}
	}
	// copy the predecessor 
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		matrix_vec.push_back(matrix[i]);
	}
	delete[] matrix;
	delete[] is_vertex_visited;
	delete[] predecessor;
	return matrix_vec;

}
std::vector<int> Geodesic_dijkstra_predecessors(TrilateralMesh& m, int point_index)
{
	// return for prdecessors
	std::vector<int> predecessors;
	//init fibonacci heap
	FibonacciHeap<std::pair<float, int >> min_fib_heap;
	//init array 
	float* matrix = new float[m.vertices.size()];
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		if (point_index == i)
		{
			matrix[i] = 0.0f;
		}
		else
		{
			//INFINITY
			matrix[i] = (float)INFINITY;
		}
	}
	//init extra stuff
	int vertex_visited = 0; //total umber of visits 
	unsigned int* is_vertex_visited = new unsigned int[m.vertices.size()];
	int* predecessor = new int[m.vertices.size()]; //predecessor vertices for all 
	//float* will_be_visited_list = new float[m.vertices.size()];
	// std::vector<std::pair<int , float>> shortest_path_set; //not yet included but calculated 
	//fill array 
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		is_vertex_visited[i] = 0; // not visited
	}
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		predecessor[i] = -1; // not visited
	}

	//dijkstra
	std::pair<float, int > first_index;
	first_index.first = 0.0f; //distance 
	first_index.second = point_index;

	std::pair<float, int > current_pair;
	min_fib_heap.insert(first_index);
	while (!min_fib_heap.isEmpty())
	{
		current_pair = min_fib_heap.removeMinimum();
		// get the adjacent vertices
		int current_index = current_pair.second;
		// if it is already visited pass
		if (is_vertex_visited[current_index] == 1)
		{
			continue;
		}
		is_vertex_visited[current_index] = 1;
		float distance = current_pair.first;
		for (size_t i = 0; i < m.adjacenies[current_index].size(); i++)
		{
			int adjacent_index = m.adjacenies[current_index][i].first;
			float adjacent_distance = m.adjacenies[current_index][i].second;
			if (is_vertex_visited[adjacent_index] == 0)
			{
				//relaxation
				if (matrix[adjacent_index] > distance + m.adjacenies[current_index][i].second)
				{
					//update
					matrix[adjacent_index] = distance + m.adjacenies[current_index][i].second;
					predecessor[adjacent_index] = current_index;
					//push 
					std::pair<float, int> new_vertex;
					new_vertex.first = matrix[adjacent_index];
					new_vertex.second = adjacent_index;
					min_fib_heap.insert(new_vertex);
				}
			}
		}
	}
	// copy the predecessor 
	for (size_t i = 0; i < m.vertices.size(); i++)
	{
		predecessors.push_back(predecessor[i]);
	}
	delete[] matrix;
	delete[] is_vertex_visited;
	delete[] predecessor;
	return predecessors;

}
std::vector<int> Geodesic_between_two_points(TrilateralMesh& m, int p1_index, int p2_index)
{
	std::vector<int> predecessors = Geodesic_dijkstra_predecessors(m, p1_index);
	std::vector<int> consec_indices;
	int pred = p2_index;
	while (pred != p1_index)
	{
		consec_indices.push_back(pred);
		pred = predecessors[pred];
	}
	consec_indices.insert(consec_indices.end(), p1_index);
	return consec_indices;
}

bool Geodesic_proximity(TrilateralMesh& m , NLateralDescriptor& desc1, NLateralDescriptor& desc2, float proximity)
{
	std::vector<float> distances = Geodesic_dijkstra(m, desc1.indices[0]);
	return distances[desc2.indices[0]] < proximity;
}

std::vector<unsigned int> Geodesic_avg_dijkstra_modified(TrilateralMesh* m, float sweep_percentage, int N_ring, bool is_color)
{
	std::vector<std::pair<float, unsigned int>> gradient_indices;
	int vertex_size = m->vertices.size();
	std::vector<float> avg_geodesic_distances(vertex_size);
	float biggest = -INFINITY;
	float biggest_dijkstra = -INFINITY;
	for (size_t i = 0; i < vertex_size; i++)
	{
		std::vector<float> distances_i = Geodesic_dijkstra(*m, i);
		//sum the distances
		float sum = 0;
		for (size_t j = 0; j < vertex_size; j++)
		{
			sum += distances_i[j];
			if (biggest_dijkstra < distances_i[j])
			{
				biggest_dijkstra = distances_i[j];
			}
		}
		avg_geodesic_distances[i] = sum;
		if (sum > biggest)
		{
			biggest = sum;
		}
	}
	//average the one ring
	std::vector<float> temp_avg_values = avg_geodesic_distances;
	for (size_t i = 0; i < vertex_size; i++)
	{
		float avg = temp_avg_values[i];
		std::vector<unsigned int> neighbours = findNRingNeighbors(m, i, N_ring);
		for (size_t j = 0; j < neighbours.size(); j++)
		{
			int neighbour_index = neighbours[j];
			avg += temp_avg_values[neighbour_index];
		}
		avg = avg / (neighbours.size() + 1);
		avg_geodesic_distances[i] = avg;
	}
	//sample where gradient is 0
	for (size_t i = 0; i < vertex_size; i++)
	{
		bool is_maxima = true;
		bool is_minima = true;

		std::vector<unsigned int> neighbours;
		neighbours = findNRingNeighbors(m, i, N_ring);
		for (size_t j = 0; j < neighbours.size(); j++)
		{
			int index = neighbours[j];
			if (index == i)
			{
				continue;
			}
			if (avg_geodesic_distances[index] > avg_geodesic_distances[i])
			{
				is_maxima = false;
			}
			if (avg_geodesic_distances[index] < avg_geodesic_distances[i])
			{
				is_minima = false;
			}
		}
		if (is_maxima || is_minima)
		{
			gradient_indices.push_back(std::make_pair(avg_geodesic_distances[i], i));
		}
	}
	std::vector< unsigned int> extremums;
	for (size_t i = 0; i < gradient_indices.size(); i++)
	{
		int index_front = gradient_indices[i].second;
		bool is_index_front = true;
		for (size_t j = 0; j < extremums.size(); j++)
		{
			int ext_index = extremums[j];
			std::vector<float> distances_ext = Geodesic_dijkstra(*m, ext_index);
			if (distances_ext[index_front] < (biggest_dijkstra * sweep_percentage))
			{
				is_index_front = false;
			}

		}
		if (is_index_front)
		{
			extremums.push_back(gradient_indices[i].second);
		}
	}
	std::vector<unsigned int> extremum_simplified; 
	for (size_t i = 0; i < extremums.size(); i++)
	{
		int index_i = extremums[i];
		float val_i = avg_geodesic_distances[index_i];
		float percentage = 5;
		bool is_close_found = false; 
		for (size_t j = 0; j < extremums.size(); j++)
		{
			if (i == j)
			{
				continue;
			}
			int index_j = extremums[j];
			float val_j = avg_geodesic_distances[index_j];
			float dif = std::abs(val_i - val_j);
			if (dif < (val_i * percentage / 100 ))
			{
				is_close_found = true; 
				break; 
			}
		}
		if (is_close_found)
		{
			extremum_simplified.push_back(index_i);
		}
	}
	extremums = extremum_simplified;
	if (is_color)
	{
		m->color_all(BLACK);
		//min max float 
		auto min = std::min_element(avg_geodesic_distances.begin() , avg_geodesic_distances.end());
		auto max = std::max_element(avg_geodesic_distances.begin() , avg_geodesic_distances.end());
		for (size_t i = 0; i < vertex_size; i++)
		{
			glm::vec3 color = CoreType_getColor(avg_geodesic_distances[i], *min, *max);
			m->raylib_mesh.colors[i * 4] = color[0] * 255;
			m->raylib_mesh.colors[i * 4 + 1] = color[1] * 255;
			m->raylib_mesh.colors[i * 4 + 2] = color[2] * 255;
			m->raylib_mesh.colors[i * 4 + 3] = 255;
		}
		/*for (size_t i = 0; i < extremums.size(); i++)
		{
			int index = extremums[i];
			m->raylib_mesh.colors[index * 4] = 255;
			m->raylib_mesh.colors[index * 4 + 1] = 255;
			m->raylib_mesh.colors[index * 4 + 2] = 255;
			m->raylib_mesh.colors[index * 4 + 3] = 255;
		}*/
		m->update_raylib_mesh();
	}

	return extremums;

}
std::vector<unsigned int> Geodesic_avg_dijkstra_modified_to_points(TrilateralMesh* m, std::vector<unsigned int> points, int& no_of_points, 
float sweep_percentage, int N_ring, bool is_color)
{
	std::vector<std::pair<float, unsigned int>> gradient_indices;
	int vertex_size = m->vertices.size();
	std::vector<float> avg_geodesic_distances(vertex_size);
	float biggest = -INFINITY;
	float biggest_dijkstra = -INFINITY;
	for (size_t i = 0; i < vertex_size; i++)
	{
		std::vector<float> distances_i = Geodesic_dijkstra(*m, i);

		//sum the distances
		float sum = 0;
		for (size_t j = 0; j < points.size(); j++)
		{
			int index = points[j];
			sum += distances_i[index];
			if (biggest_dijkstra < distances_i[index])
			{
				biggest_dijkstra = distances_i[index];
			}
		}
		avg_geodesic_distances[i] = sum;
		if (sum > biggest)
		{
			biggest = sum;
		}

	}

	//average the one ring
	std::vector<float> temp_avg_values = avg_geodesic_distances;
	for (size_t i = 0; i < vertex_size; i++)
	{
		float avg = temp_avg_values[i];
		std::vector<unsigned int> neighbours = findNRingNeighbors(m, i, N_ring);
		for (size_t j = 0; j < neighbours.size(); j++)
		{
			int neighbour_index = neighbours[j];
			avg += temp_avg_values[neighbour_index];
		}
		avg = avg / (neighbours.size() + 1);
		avg_geodesic_distances[i] = avg;
	}
	//sample where gradient is 0
	for (size_t i = 0; i < vertex_size; i++)
	{
		bool is_maxima = true;
		bool is_minima = true;

		std::vector<unsigned int> neighbours;
		neighbours = findNRingNeighbors(m, i, N_ring);
		for (size_t j = 0; j < neighbours.size(); j++)
		{
			int index = neighbours[j];
			if (index == i)
			{
				continue;
			}
			if (avg_geodesic_distances[index] > avg_geodesic_distances[i])
			{
				is_maxima = false;
			}
			if (avg_geodesic_distances[index] < avg_geodesic_distances[i])
			{
				is_minima = false;
			}

		}
		/*int neighbour_size = m->adjacenies[i].size();
		for (size_t j = 0; j < neighbour_size; j++)
		{
			int neighbour_index = m->adjacenies[i][j].first;
			if (avg_geodesic_distances[neighbour_index] > avg_geodesic_distances[i])
			{
				is_maxima = false;
			}
			if (avg_geodesic_distances[neighbour_index] < avg_geodesic_distances[i])
			{
				is_minima = false;
			}
		}*/
		if (is_maxima || is_minima)
		{
			gradient_indices.push_back(std::make_pair(avg_geodesic_distances[i], i));
		}
	}
	std::vector< unsigned int> extremums;
	for (size_t i = 0; i < gradient_indices.size(); i++)
	{
		int index_front = gradient_indices[i].second;
		bool is_index_front = true;
		for (size_t j = 0; j < extremums.size(); j++)
		{
			int ext_index = extremums[j];
			std::vector<float> distances_ext = Geodesic_dijkstra(*m, ext_index);
			if (distances_ext[index_front] < (biggest_dijkstra * sweep_percentage))
			{
				is_index_front = false;
			}

		}
		if (is_index_front)
		{
			extremums.push_back(gradient_indices[i].second);
		}
	}
	std::sort(gradient_indices.begin(), gradient_indices.end());

	extremums.insert(extremums.end(), points.begin(), points.end());
	if (is_color)
	{
		for (size_t i = 0; i < vertex_size; i++)
		{
			m->raylib_mesh.colors[i * 4] = avg_geodesic_distances[i] / biggest * 255;
			m->raylib_mesh.colors[i * 4 + 1] = 0;
			m->raylib_mesh.colors[i * 4 + 2] = 0;
			m->raylib_mesh.colors[i * 4 + 3] = 255;
		}
		for (size_t i = 0; i < extremums.size(); i++)
		{
			int index = extremums[i];
			m->raylib_mesh.colors[index * 4] = 255;
			m->raylib_mesh.colors[index * 4 + 1] = 255;
			m->raylib_mesh.colors[index * 4 + 2] = 255;
			m->raylib_mesh.colors[index * 4 + 3] = 255;
		}
		m->update_raylib_mesh();
	}

	return extremums;
}
std::vector<unsigned int> Geodesic_avg_dijkstra(TrilateralMesh* m, int& c, float sweep_percentage, int N_ring,  bool is_color)
{
	std::vector<std::pair<float , unsigned int>> gradient_indices;
	int vertex_size = m->vertices.size();
	std::vector<float> avg_geodesic_distances( vertex_size);
	float biggest = -INFINITY;
	float biggest_dijkstra = -INFINITY; 
	for (size_t i = 0; i < vertex_size; i++)
	{
		std::vector<float> distances_i = Geodesic_dijkstra(*m, i);

		//sum the distances
		float sum = 0;
		for (size_t j = 0; j < vertex_size; j++)
		{
			sum += distances_i[j];
			if (biggest_dijkstra < distances_i[j])
			{
				biggest_dijkstra = distances_i[j];
			}
		}
		
		sum = sum * m->areas[i] / 3.0;
		avg_geodesic_distances[i] = sum;
		if (sum > biggest)
		{
			biggest = sum;
		}

	}
	
	//average the one ring
	std::vector<float> temp_avg_values = avg_geodesic_distances;
	for (size_t i = 0; i < vertex_size; i++)
	{
		float avg = temp_avg_values[i]; 
		std::vector<unsigned int> neighbours = findNRingNeighbors(m, i, N_ring);
		for (size_t j = 0; j < neighbours.size(); j++)
		{
			int neighbour_index = neighbours[j];
	
			avg += temp_avg_values[neighbour_index];
		}
		avg = avg / (neighbours.size() + 1);
		avg_geodesic_distances[i] = avg; 
	}
	int n_ring = 3;
	//sample where gradient is 0
	for (size_t i = 0; i < vertex_size; i++)
	{
		bool is_maxima = true; 
		bool is_minima = true; 
		
		std::vector<unsigned int> neighbours;
		neighbours = findNRingNeighbors(m, i, N_ring);
		for (size_t j = 0; j < neighbours.size(); j++)
		{
			int index = neighbours[j];
			if (index == i)
			{
				continue;
			}
			if (avg_geodesic_distances[index] > avg_geodesic_distances[i])
			{
				is_maxima = false;
			}
			if (avg_geodesic_distances[index] < avg_geodesic_distances[i])
			{
				is_minima = false;
			}

		}
		/*int neighbour_size = m->adjacenies[i].size();
		for (size_t j = 0; j < neighbour_size; j++)
		{
			int neighbour_index = m->adjacenies[i][j].first;
			if (avg_geodesic_distances[neighbour_index] > avg_geodesic_distances[i])
			{
				is_maxima = false; 
			}
			if (avg_geodesic_distances[neighbour_index] < avg_geodesic_distances[i])
			{
				is_minima = false; 
			}
		}*/
		if (is_maxima || is_minima )
		{
			gradient_indices.push_back(std::make_pair(avg_geodesic_distances[i], i));
		}
	}

	std::sort(gradient_indices.begin(), gradient_indices.end());
	


	std::vector< unsigned int> extremums;
	for (size_t i = 0; i < gradient_indices.size(); i++)
	{
		extremums.push_back(gradient_indices[i].second);
	}
	

	if (is_color)
	{
		for (size_t i = 0; i < vertex_size; i++)
		{
			m->raylib_mesh.colors[i * 4] = avg_geodesic_distances[i] / biggest * 255;
			m->raylib_mesh.colors[i * 4 + 1] = 0;
			m->raylib_mesh.colors[i * 4 + 2] = 0;
			m->raylib_mesh.colors[i * 4 + 3] = 255;
		}
		for (size_t i = 0; i < extremums.size(); i++)
		{
			int index = extremums[i];
			m->raylib_mesh.colors[index * 4] = 255;
			m->raylib_mesh.colors[index * 4 + 1] = 255;
			m->raylib_mesh.colors[index * 4 + 2] = 255;
			m->raylib_mesh.colors[index * 4 + 3] = 255;
		}
		m->update_raylib_mesh();
	}
	
	return extremums;
	
}
std::vector<unsigned int> Geodesic_min_dijkstra(TrilateralMesh*m , 
std::vector< unsigned int> agd_extremums, float sweep_percentage, float tau, bool is_color)
{
	int vertex_size = m->vertices.size();
	std::vector<std::pair<float, unsigned int>> min_geo_distances(vertex_size); 
	std::vector<std::pair<float, unsigned int>> gradient_indices;
	float maximum_length = -INFINITY; 
	float maximum_min_geodesic = -INFINITY; 
	for (size_t i = 0; i < vertex_size; i++)
	{
		std::vector<float> distances_i = Geodesic_dijkstra(*m, i);
		for (size_t j = 0; j < distances_i.size(); j++)
		{
			if (maximum_length < distances_i[j])
			{
				maximum_length = distances_i[j];
			}
		}
		float smallest = INFINITY; 
		for (size_t j = 0; j < agd_extremums.size(); j++)
		{
			int ext_index = agd_extremums[j];
			if (distances_i[ext_index] < smallest)
			{
				smallest = distances_i[ext_index];
			}
		}
		min_geo_distances[i] = std::make_pair(smallest , i); 
	}

	//for tau
	for (size_t i = 0; i < min_geo_distances.size(); i++)
	{
		if (maximum_min_geodesic < min_geo_distances[i].first)
		{
			maximum_min_geodesic = min_geo_distances[i].first;
		}
	}
	//sample where gradient is 0
	for (size_t i = 0; i < vertex_size; i++)
	{
		bool is_maxima = true;
		int neighbour_size = m->adjacenies[i].size();
		for (size_t j = 0; j < neighbour_size; j++)
		{
			int neighbour_index = m->adjacenies[i][j].first;
			if (min_geo_distances[neighbour_index] > min_geo_distances[i])
			{
				is_maxima = false;
			}
		}
		if (is_maxima && min_geo_distances[i].first > (tau * maximum_min_geodesic) )
		{
			gradient_indices.push_back(min_geo_distances[i]);
			std::cout << " min geo percentage " << min_geo_distances[i].first<< "  " << maximum_min_geodesic << " "
			<< min_geo_distances[i].first / maximum_min_geodesic << std::endl;
		}
	}
	std::sort(gradient_indices.begin(), gradient_indices.end());

	std::vector< unsigned int> extremums;
	for (size_t i = 0; i < gradient_indices.size() ; i++)
	{
		int index_front = gradient_indices[i].second;
		bool is_index_front = true;
		for (size_t j = 0; j < extremums.size(); j++)
		{
			int ext_index = extremums[j];
			std::vector<float> distances_ext = Geodesic_dijkstra(*m, ext_index);
			if (distances_ext[index_front] < (maximum_length * sweep_percentage))
			{
				is_index_front = false;
			}
		}
		if (is_index_front)
		{
			extremums.push_back(gradient_indices[i].second);
		}
	}

	if (is_color)
	{
		for (size_t i = 0; i < extremums.size(); i++)
		{
			int index = extremums[i];
			m->raylib_mesh.colors[index * 4] = 0;
			m->raylib_mesh.colors[index * 4 + 1] = 0;
			m->raylib_mesh.colors[index * 4 + 2] = 255;
			m->raylib_mesh.colors[index * 4 + 3] = 255;
		}
		m->update_raylib_mesh();
	}
	
	extremums.insert(extremums.end(), agd_extremums.begin(), agd_extremums.end());
	return extremums;
}


void Geodesic_write_sampled_points(TrilateralMesh* m, std::vector<unsigned int>& agd_points)
{
	std::ofstream file("sampled_points.txt");
	for (size_t i = 0; i < agd_points.size(); i++)
	{
		file << " " << agd_points[i] << " " << "\n";
	}
	file.close();

}
void Geodesic_read_sampled_points(TrilateralMesh* m, std::vector<unsigned int>& sampled_points)
{
	std::ifstream file("sampled_points.txt");
	// Read the file line by line
	std::string line;
	while (std::getline(file, line)) {
		std::stringstream ss(line); // Use stringstream to parse the line
		int number;
		std::vector<int> nums;
		// Extract numbers from the line
		while (ss >> number) {
			nums.push_back(number);
		}
		sampled_points.push_back(nums[0]);
	}
}


std::vector<unsigned int> Geodesic_find_biggest_AGD(TrilateralMesh* m, float sweep_percentage , int sample_size )
{
	std::vector<unsigned int> biggest_agd_indices; 
	int vertex_size = m->vertices.size();
	std::vector<float> avg_geodesic_distances(vertex_size);
	float biggest = -INFINITY;
	float biggest_dijkstra = -INFINITY;
	for (size_t i = 0; i < vertex_size; i++)
	{
		std::vector<float> distances_i = Geodesic_dijkstra(*m, i);
		//sum the distances
		float sum = 0;
		for (size_t j = 0; j < vertex_size; j++)
		{
			sum += distances_i[j];
			if (biggest_dijkstra < distances_i[j])
			{
				biggest_dijkstra = distances_i[j];
			}
		}
		sum = sum * m->areas[i] / 3.0;
		avg_geodesic_distances[i] = sum;
		if (sum > biggest)
		{
			biggest = sum;
		}
	}

	for (size_t i = 0; i < sample_size; i++)
	{
		auto max_auto= std::max_element(avg_geodesic_distances.begin(), avg_geodesic_distances.end());
		int max_index = std::distance(avg_geodesic_distances.begin() , max_auto);
		float max_val = *max_auto;

		std::vector<float> distances = Geodesic_dijkstra(*m , max_index);
		bool is_new_point_added = true; 
		for (size_t j = 0; j < biggest_agd_indices.size(); j++)
		{
			if (distances[biggest_agd_indices[j]] <  biggest_dijkstra *  sweep_percentage )
			{
				is_new_point_added = false;
				break; 
			}
		}
		if (is_new_point_added)
		{
			biggest_agd_indices.push_back(max_index);
		}
		avg_geodesic_distances.erase(avg_geodesic_distances.begin() + max_index);
	}


	return biggest_agd_indices;


}

unsigned int Geodesic_find_midpoint(TrilateralMesh* m, unsigned int index1, unsigned int index2)
{
	std::vector<int> paths =Geodesic_between_two_points(*m, index1, index2);
	float dist = 0;
	for (size_t i = 0; i < paths.size()-1; i++)
	{
		dist = dist + glm::distance(m->vertices[paths[i]], m->vertices[paths[i + 1]]);
	}
	float half_dist = 0; 
	for (size_t i = 0; i < paths.size()-1; i++)
	{
		half_dist = half_dist + glm::distance(m->vertices[paths[i]], m->vertices[paths[i + 1]]);
		if (half_dist >= dist / 2)
		{
			return paths[i];
		}
	}
}