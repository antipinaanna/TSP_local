#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>
#include <fstream>
#include <filesystem>
#define INT_MAX 2147483647

namespace fs = std::filesystem;

std::vector<unsigned> get_solution(const std::vector<std::vector<unsigned>> &distance_matrix, unsigned current_city) {
    unsigned n = distance_matrix.size();
    std::vector<unsigned> tour(n);
    std::vector<bool> visited(n, false);

    visited[current_city] = true;
    tour[0] = current_city;

    for (unsigned i = 1; i < n; i++) {
        if (i % 100 == 0) {
            std::cout << "iter " << i << std::endl;
        }
        int nearest_city = -1;
        unsigned min_distance = INT_MAX;

        for (unsigned j = 0; j < n; j++) {
            if (visited[j]) continue;
            unsigned distance = distance_matrix[current_city][j];
            if (distance < min_distance) {
                min_distance = distance;
                nearest_city = j;
   	     }
        }
        visited[nearest_city] = true;
        current_city = nearest_city;
        tour[i] = current_city;
    }

    return tour;
}

unsigned get_tour_cost(const std::vector<unsigned> &tour, const std::vector<std::vector<unsigned>> &distance_matrix) {
    unsigned cost = 0;
    for (size_t i = 0; i < tour.size() - 1; i++) {
        cost += distance_matrix[tour[i]][tour[i + 1]];
    }
    cost += distance_matrix[tour.back()][tour.front()];
    return cost;
}

std::vector<std::vector<unsigned>> generate_distance_matrix(std::vector<std::pair<float, float>> &distance) {
    unsigned n = distance.size();
    std::vector<std::vector<unsigned>> distance_matrix(n, std::vector<unsigned>(n));
    for (unsigned i = 0; i < n; i++) {
        for (unsigned j = 0; j < n; j++) {
            float deltaX = distance[i].first - distance[j].first;
            float deltaY = distance[i].second - distance[j].second;
            distance_matrix[i][j] = sqrt(deltaX * deltaX + deltaY * deltaY);
        }
    }
    return distance_matrix;
}


void swap_edges(std::vector<unsigned> &tour, unsigned i, unsigned j) {
    while (i < j) {
        std::swap(tour[i], tour[j]);
        i++;
        j--;
    }
}

void local_s(std::vector<unsigned> &tour, const std::vector<std::vector<unsigned>> &distanceMatrix) {
    bool improved = true;
    unsigned n = tour.size();
    unsigned iter = 0;
    while (improved && (iter < 100)) {
        improved = false;
        iter++;

        for (unsigned i = 0; i < n - 1; i++) {
            for (unsigned j = i + 2; j < n - 1; j++) {
                unsigned old_cost = distanceMatrix[tour[i]][tour[i + 1]] + distanceMatrix[tour[j]][tour[j + 1]];
                unsigned new_cost = distanceMatrix[tour[i]][tour[j]] + distanceMatrix[tour[i + 1]][tour[j + 1]];
                if (new_cost < old_cost) {
                    swap_edges(tour, i + 1, j);
                    improved = true;
                }
            }
        }
    }
}


int main() {
    std::string folderPath = "./data";
    std::ofstream file_out("output_dataframe2");
	if (file_out.is_open())
	{
		for (const auto &entry : fs::directory_iterator(folderPath)) 
		{
			if (entry.is_regular_file())
			{
				std::string filePath = entry.path().string();
				std::ifstream file(filePath);
				if (file.is_open())
				{
                    unsigned n;
					file >> n;
					std::vector<std::pair<float, float>> distance (n); 
					for (int i = 0; i < n; i++)
					{
						file >> distance[i].first >> distance[i].second;
					}
                    std::vector<std::vector<unsigned>> distance_matrix = generate_distance_matrix(distance);
                    std::vector<unsigned> solution = get_solution(distance_matrix, 0);
                    std::cout << "COST FIRST " << get_tour_cost(solution, distance_matrix) << std::endl;
                    local_s(solution, distance_matrix);
                    unsigned cost = get_tour_cost(solution, distance_matrix);
                    std::cout << "COST SECOND " << cost << std::endl;
                    std::cout << std::endl;
					file_out << filePath << ", "<< cost << std::endl;
					file.close();
				}
				else
					std::cout << "Не удалось открыть файл" << std::endl;
			}
		}
		std::cout << "End" << std::endl;
		file_out.close();
	}
}
