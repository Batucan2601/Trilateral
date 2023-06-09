#pragma once
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
using std::ofstream;
#define INFINITE 10000000

struct Edge
{
	glm::vec3 p1;
	glm::vec3 p2;
	float distance;
};
class Mesh
{
public:
	std::vector<glm::vec3> vertices; //fill with constructor  
	std::vector<glm::vec3> colors; //fill with constructor  

	std::vector<Edge> edges; //fill with constructor 
	std::vector<int> vertex_indices; //fill with constructor 
	std::vector<std::vector<std::pair<int, float>>> adjacenies; // all adjacensies for all vertices
	std::vector<unsigned int > triangles;

	//assignment 2 
	std::vector<float > quad_vertices;
	std::vector<unsigned int > quad_indices; // first four 
	std::vector<float> original_vertices;

	std::vector<glm::vec3> original_vertices_for_sqrt3;

	glm::mat4 model_mat;
	//project 
	bool off_format = false;
	bool ply_format = false;
	//MVP
	glm::mat4 MVP;
	Mesh(char* filename);
	Mesh(glm::vec3 *p1 , glm::vec3* p2 , glm::vec3* p3 , glm::vec3* p4);
	glm::mat4 move_mesh(glm::vec3 direction);
	glm::mat4 scale_mesh(glm::vec3 scale);

	private:
	void read_ply_format(char* filename);
	void read_off_format(char* filename);
};


