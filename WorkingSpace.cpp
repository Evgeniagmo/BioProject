#include "WorkingSpace.h"

void WorkingSpace::initialize()
{
	auto period = epithelial_cell_size + intercellular_space_size;

	// counting number of cells for given width and height
	auto counter_x = m_right_bottom.first / period;
	auto counter_y = m_right_bottom.second / period;

	// counting real size of working space
	// taking into account the width of the intercellular distance
	m_right_bottom.first = intercellular_space_size + counter_x * period;
	m_right_bottom.second = intercellular_space_size + counter_y * period;

	for (std::size_t i = 0; i < counter_x; ++i)
	{
		for (std::size_t j = 0; j < counter_y; ++j)
		{
			all_stable.push_back(std::make_shared < StableCell >(
				std::make_pair(intercellular_space_size + period * i,
					intercellular_space_size + period * j)));
		}
	}
}

auto WorkingSpace::get_neighbors(stable_cell_t cell) const noexcept
{
	std::vector< stable_cell_t > neighbors;

	// distance(m_centre, cell_centre) < search_radius
	// it needs optimization (too slow, uses getters)
	for (stable_cell_t stable : all_stable)
	{
		if (distance(stable->get_centre(), cell->get_centre()) < search_radius)
			neighbors.push_back(stable);
	}

	return neighbors;
}

void WorkingSpace::calc_next_state(const std::chrono::milliseconds& timer) noexcept
{
	auto threads_number = std::thread::hardware_concurrency();

	std::vector < std::thread > threads(threads_number);

	auto height = m_right_bottom.second / epithelial_cell_size;

	auto size = all_stable.size() / threads_number;
	auto tail = all_stable.size() - threads_number * size;

	auto function = [=](std::size_t id)
	{
		auto first_cell = size * id;
		auto last_cell = first_cell + size;

		last_cell = last_cell > all_stable.size() ? all_stable.size() : last_cell;

		for (std::size_t it = first_cell; it < last_cell; ++it)
		{
			std::vector< stable_cell_t > neighbors = get_neighbors(all_stable[it]);
			// optimization: count first, then fill
			all_stable[it]->set_state(all_stable[it]->next_state(timer, neighbors));
		}
	};

	for (std::size_t i = 0; i < threads_number; ++i)
	{
		threads[i] = std::thread(function, i);
	}

	if (tail > 0)
	{
		function(threads_number);
	}

	for (std::size_t i = 0; i < threads_number; ++i)
	{
		threads[i].join();
	}
}