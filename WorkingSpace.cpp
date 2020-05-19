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

	// set the state of the border cells to initialize the contamination
	for (std::size_t it = 0; it < counter_x; ++it)
	{
		std::default_random_engine dre;
		std::uniform_int_distribution<int> uid(1, 1000);
		int random_num = uid(dre);

		if (random_num < m_virus_concentration * 1000) // in proportion to virus_concentration
		{
			all_stable[it]->set_new_state(StableCell::State::ill); // else: remain healthy
			all_stable[it]->change_state();
		}

	}
}

auto WorkingSpace::get_neighbors(stable_cell_t cell) const noexcept
{
	std::vector< stable_cell_t > neighbors;

	// distance(m_centre, cell_centre) < search_radius
	// it needs optimization (too slow, uses getters
	for (stable_cell_t stable : all_stable)
	{
		if (distance(stable->count_centre(), cell->count_centre()) < search_radius)
			neighbors.push_back(stable);
	}

	return neighbors;
}

void WorkingSpace::calc_next() noexcept
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
			// count new state, not fill
			all_stable[it]->set_new_state(all_stable[it]->next_state(neighbors, m_virus_traits));
			// count displacement
			all_stable[it]->f_pressure();
			all_stable[it]->f_restoring();
			all_stable[it]->f_repulsion(neighbors, search_radius);
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

void WorkingSpace::set_next()
{
	for (auto it = all_stable.begin(); it != all_stable.end(); )
	{
		(*it)->change_state();
		if ((*it)->get_state() != StableCell::State::dead)
		{
			(*it)->move();
			++it;
		}
		else
		{
			it = all_stable.erase(it);
		}
	}
}

std::vector <std::vector <double> > WorkingSpace::data_to_send() noexcept
{
	std::vector <std::vector <double> > numbers;
	for (stable_cell_t cell : all_stable)
	{
		numbers.push_back(cell->pos_state_to_numbers());
	}
}
