#pragma once

#include <chrono>
#include <memory>
#include <thread>
#include <random>
#include <vector>

#include "cells.h"

class WorkingSpace
{

public:

	using stable_cell_t = std::shared_ptr < StableCell >;
	using point_t = std::pair <double, double>;

private:

	const double epithelial_cell_size = 12.7; // <L> = 12,7 microns

	const double intercellular_space_size = 0.03; // <L> =  30 nm

	const double neighbour_radius = 1.5 * epithelial_cell_size; // 1.5 link length

	const double repulsion_radius = 0.1 * intercellular_space_size;

	std::vector < stable_cell_t > all_stable; // all_lymphocytes, all_fagocytes;

	point_t m_left_top = std::make_pair(0.0, 0.0);
	point_t m_right_bottom;

	std::vector < point_t > m_borders;

	double m_virus_concentration = 0; // to initialize contamination (from 0 to 1)
	bool m_virus_traits = 0; // cell can recover = true (else = false)

	double m_ill_counter = 0;

public:

	WorkingSpace() noexcept = default;

	// width and height in microns
	explicit WorkingSpace(
		double width, double height,
		double concentration, bool virus_traits) noexcept :
		m_right_bottom(std::make_pair(width, height)),
		m_virus_concentration(concentration), m_virus_traits(virus_traits)
	{
		initialize();
	}

	~WorkingSpace() noexcept = default;

private:

	void initialize();

	// checking the distance between two centers
	auto get_neighbors(const stable_cell_t cell) const noexcept;

	// compare two cells: whether the first is higher or not
	/*bool compare(const stable_cell_t left, const stable_cell_t right) const noexcept
	{
		return (left->count_centre().second < right->count_centre().second);
	}*/

public:

	// calculating next state and next position
	void calc_next() noexcept;

	// set next state and next position, remove dead, count ill
	void set_next() noexcept;

	// check_collisions
	void collision_update() noexcept;

	// vector from all positions of nodes and states
	std::vector <std::vector <double> > data_to_send() noexcept;

	double statistics_to_send() const noexcept
	{
		return m_ill_counter;
	}

};