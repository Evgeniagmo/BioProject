#pragma once

#include "cells.h"

class WorkingSpace
{
public:

	WorkingSpace() noexcept = default;

	~WorkingSpace() noexcept = default;

	std::vector<Cell*> all_stable, all_lymphocytes, all_fagocytes;

	/*std::vector< std::vector< Cell* > > all_types = { epithelium, basale,
					fibroblasts, lymphocytes, fagocytes };*/

	const double search_radius = 0; // 1.5 link length

	double epithelial_cell_size; // <L> = 12,7 microns

	t_point left_bottom, right_top;


	// time control

};