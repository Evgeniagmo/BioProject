#include "WorkingSpace.h"

void WorkingSpace::initialize()
{
	auto period = epithelial_cell_size + intercellular_space_size;

	// counting number of cells for given width and height
	std::size_t counter_x = m_right_bottom.first / period;
	std::size_t counter_y = m_right_bottom.second / period;

	// counting real size of working space
	// taking into account the width of the intercellular distance
	m_right_bottom.first = intercellular_space_size + counter_x * period;
	m_right_bottom.second = intercellular_space_size + counter_y * period;

	m_borders.push_back(m_left_top);
	m_borders.push_back(std::make_pair((0.0), m_right_bottom.second));
	m_borders.push_back(m_right_bottom);
	m_borders.push_back(std::make_pair(m_right_bottom.first, (0.0)));

	for (std::size_t i = 0; i < counter_x; ++i)
	{
		for (std::size_t j = 0; j < counter_y; ++j)
		{
			all_stable.push_back(std::make_shared < StableCell >(epithelial_cell_size,
				std::make_pair(intercellular_space_size + period * i,
					intercellular_space_size + period * j)));
		}
	}

	// set the state of the border cells to initialize the contamination
	for (std::size_t it = 0; it < counter_x; ++it)
	{
		int random_num = rand() % 1000;

		if (random_num < m_virus_concentration * 1000) // in proportion to virus_concentration
		{
			all_stable[it]->set_new_state(StableCell::State::ill); // else: remain healthy
			all_stable[it]->change_state();

			std::cout << 0 << std::endl;
		}
		else
		{
			std::cout << 1 << std::endl;
		}


	}
}

auto WorkingSpace::get_neighbors(const stable_cell_t cell) const noexcept
{
	std::vector< stable_cell_t > neighbors;

	const point_t cell_centre = cell->count_centre();

	// distance(m_centre, cell_centre) < neighbour_radius
	for (stable_cell_t stable : all_stable)
	{
		const double dist = distance(stable->count_centre(), cell_centre);

		if (dist < neighbour_radius && dist != 0.0)
			neighbors.push_back(stable);
	}

	return neighbors;
}

void WorkingSpace::calc_next() noexcept
{
	//for (std::size_t i = 0; i < all_stable.size(); ++i)
	//{

	//	std::vector< stable_cell_t > neighbors = get_neighbors(all_stable[i]);
	//	// count new state, not fill
	//	if (neighbors.size() > 0)
	//	{
	//		all_stable[i]->set_new_state(all_stable[i]->next_state(neighbors, m_virus_traits));
	//		// count displacement
	//		all_stable[i]->f_pressure();
	//		all_stable[i]->f_restoring();
	//		all_stable[i]->f_repulsion(neighbors, repulsion_radius);
	//	}

	//}

	auto threads_number = std::thread::hardware_concurrency();

	std::vector < std::thread > threads(threads_number);


	auto size = all_stable.size() / threads_number;
	auto body = size * threads_number;
	auto tail = all_stable.size() - body;

	auto function = [=](std::size_t id)
	{
		auto first_cell = size * id;
		auto last_cell = first_cell + size;

		last_cell = last_cell > all_stable.size() ? all_stable.size() : last_cell;

		for (std::size_t i = first_cell; i < last_cell; ++i)
		{
			std::vector< stable_cell_t > neighbors = get_neighbors(all_stable[i]);
			// count new state, not fill
			all_stable[i]->set_new_state(all_stable[i]->next_state(neighbors, m_virus_traits));
			// count displacement
			all_stable[i]->f_pressure();
			all_stable[i]->f_restoring();
			all_stable[i]->f_repulsion(neighbors, neighbour_radius);
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

// set next state and next position, remove dead, count ill
void WorkingSpace::set_next() noexcept
{
	m_ill_counter = 0;

	for (auto it = all_stable.begin(); it != all_stable.end(); )
	{
		(*it)->change_state();
		if ((*it)->get_state() != StableCell::State::dead)
		{
			(*it)->move();

			if ((*it)->get_state() == StableCell::State::ill)
			{
				m_ill_counter++;
			}

			++it;
		}
		else
		{
			it = all_stable.erase(it);
		}
	}
}

// check_collisions
void WorkingSpace::collision_update() noexcept
{
	std::sort(all_stable.begin(), all_stable.end(), [](const stable_cell_t a, const stable_cell_t b) {
		return a->count_centre().second < b->count_centre().second;
		});

	for (std::size_t i = 0; i < all_stable.size(); ++i)
	{
		all_stable[i]->check_borders(m_borders);

		for (std::size_t j = i + 1; j < all_stable.size() &&
			all_stable[i]->count_centre().second + epithelial_cell_size >
			all_stable[j]->count_centre().second - epithelial_cell_size; ++j)
		{
			all_stable[j]->check_intersection(all_stable[i]->get_nodes());
		}
	}
}

// all positions of nodes and states
std::vector <std::vector <double> > WorkingSpace::data_to_send() noexcept
{
	std::vector <std::vector <double> > values;
	for (stable_cell_t cell : all_stable)
	{
		values.push_back(cell->pos_state_to_numbers());
	}

	return values;
};