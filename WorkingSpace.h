#pragma once

#include <chrono>
#include <memory>
#include <thread>
#include <vector>

#include "cells.h"

class WorkingSpace
{

public:

	using stable_cell_t = std::shared_ptr < StableCell >;
	using point_t = std::pair <double, double>;

private:

	std::vector < stable_cell_t > all_stable, all_lymphocytes, all_fagocytes;

	const double epithelial_cell_size = 12.7; // <L> = 12,7 microns

	const double intercellular_space_size = 0.03; // <L> =  30 nm

	const double search_radius = 0.75 * epithelial_cell_size; // 1.5 link length

	point_t m_left_top = std::make_pair(0.0, 0.0);
	point_t m_right_bottom;

	double m_virus_concentration;
	double m_virus_traits;

public:

	WorkingSpace() noexcept = default;

	// width and height in microns
	explicit WorkingSpace(
		double width, double height,
		double concentration, double virus_traits) noexcept :
		m_right_bottom(std::make_pair(width, height)),
		m_virus_concentration(concentration), m_virus_traits(virus_traits)
	{
		initialize();
	}

	~WorkingSpace() noexcept = default;

private:

	void initialize();

public:

	auto get_neighbors(stable_cell_t cell) const noexcept;

	void calc_next_state(const std::chrono::milliseconds& timer) noexcept;

};