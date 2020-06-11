#ifndef GRID_H
#define GRID_H

#include <vector>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include "../src/Utility/Collision.h"

template <typename T>
class Grid {
public:
	Grid();
	Grid(int cell_width, int cell_height, int width, int height);

	int insert(Bounding_Box position, std::shared_ptr<T> obj);
	bool insert(int z, int x, std::pair<Bounding_Box, std::shared_ptr<T>>& pair);

	bool remove(int z, int x, std::shared_ptr<T> obj);
	int remove(Bounding_Box position, std::shared_ptr<T> obj);

	void clear();

	std::vector<std::vector<std::pair<Bounding_Box, std::shared_ptr<T>>>*> get_cells(const Bounding_Box position);
	std::vector<std::pair<Bounding_Box, std::shared_ptr<T>>>* get_cell(int z, int x);

	int _cell_width;
	int _cell_height;

	int _width;
	int _height;
private:

	std::vector<std::vector<std::vector<std::pair<Bounding_Box, std::shared_ptr<T>>>>> _cells;
	std::vector<std::pair<Bounding_Box, std::shared_ptr<T>>> _outer_cell;
};

template <typename T>
Grid<T>::Grid() :
	_cell_width		( 0 ),
	_cell_height	( 0 ),
	_width			( 0 ),
	_height			( 0 )
{}

template <typename T>
Grid<T>::Grid(int cell_width, int cell_height, int width, int height) :
	_cell_width		( cell_width ),
	_cell_height    ( cell_height ),
	_width			( width ),
	_height			( height )
{
	_cells.resize(height);
	for(auto &row : _cells) {
		row.resize(width);
	}
}

template <typename T>
bool Grid<T>::insert(int z, int x, std::pair<Bounding_Box, std::shared_ptr<T>>& pair) {
	if ((z < _height && z >= 0) && (x < _width && x >= 0)) {
		_cells[z][x].push_back(pair);
		return true;
	}
	return false;
}

template <typename T>
int Grid<T>::insert(Bounding_Box position, std::shared_ptr<T> obj) {
	const int z = int(position.min.z / _cell_height);
	const int z2 = int(position.max.z / _cell_height);
	const int x = int(position.min.x / _cell_width);
	const int x2 = int(position.max.x / _cell_width);
	int inserted = 0;
	auto pair = std::make_pair(position, obj);

	if((z2 - z) >= 2 || (x2 - x) >= 2) {
		_outer_cell.push_back(pair);
		return 1;
	}

	inserted += insert(z, x, pair);
	if (x2 != x) inserted += insert(z, x2, pair);
	if (z2 != z) inserted += insert(z2, x, pair);
	if (z2 != z && x2 != x) inserted += insert(z2, x2, pair);

	return inserted;
}

template <typename T>
bool Grid<T>::remove(int z, int x, std::shared_ptr<T> obj) {
	if ((z < _height && z >= 0) && (x < _width && x >= 0)) {
		for(auto it = _cells[z][x].begin(); it != _cells[z][x].end(); ++it) {
			if (obj == it->second) {
				_cells[z][x].erase(it);
				return true;
			}
		}
	}
	return false;
}

template <typename T>
int Grid<T>::remove(Bounding_Box position, std::shared_ptr<T> obj) {
	const int z = int(position.min.z / _cell_height);
	const int z2 = int(position.max.z / _cell_height);
	const int x = int(position.min.x / _cell_width);
	const int x2 = int(position.max.x / _cell_width);
	int removed = 0;

	if ((z2 - z) >= 2 || (x2 - x) >= 2) {
		for(auto it = _outer_cell.begin(); it != _outer_cell.end(); ++it) {
			if(obj == it->second) {
				_outer_cell.erase(it);
				return 1;
			}
		}
		return 0;
	}

	removed += remove(z, x, obj);
	if (x2 != x) removed += remove(z, x2, obj);
	if (z2 != z) removed += remove(z2, x, obj);
	if ((z2 != z) && (x2 != x)) removed += remove(z2, x2, obj);

	return removed;
}

template <typename T>
std::vector<std::pair<Bounding_Box, std::shared_ptr<T>>>* Grid<T>::get_cell(int z, int x) {
	if (( z < _height && z >= 0 ) && ( x < _width && x >= 0 ) && _cells[z][x].size() > 0) {
		return &_cells[z][x];
	}
	return nullptr;
}

template <typename T>
std::vector<std::vector<std::pair<Bounding_Box, std::shared_ptr<T>>>*> Grid<T>::get_cells(const Bounding_Box position) {
	std::vector<std::vector<std::pair<Bounding_Box, std::shared_ptr<T>>>*> cells;

	cells.push_back(&_outer_cell);

	const int z = int(position.min.z / _cell_height);
	const int z2 = int(position.max.z / _cell_height);
	const int x = int(position.min.x / _cell_width);
	const int x2 = int(position.max.x / _cell_width);

	if (auto cell = get_cell(z, x)) cells.push_back(cell);
	if (x2 != x) if (auto cell = get_cell(z, x2)) cells.push_back(cell);
	if (z2 != z) if (auto cell = get_cell(z2, x)) cells.push_back(cell);
	if ((z2 != z) && (x2 != x)) if (auto cell = get_cell(z2, x2)) cells.push_back(cell);

	return cells;
}

template <typename T>
void Grid<T>::clear() {
	_cells.clear();
	_cells.resize(_height);
	for (auto& row : _cells) {
		row.resize(_width);
	}
	_outer_cell.clear();
}

#endif