#include <vector>
#include <random>

#include "cells.h"

void Cell::set_centre() noexcept
{
	for (const Node& node : nodes)
	{
		m_centre.first += node.get_pos().first / nodes.size();
		m_centre.second += node.get_pos().second / nodes.size();
	}
}

double Cell::get_area() noexcept
{
	double area = 0.0;

	std::size_t j = nodes.size() - 1;
	for (std::size_t i = 0; i < nodes.size(); i++)
	{
		area += (nodes[j].get_pos().first + nodes[i].get_pos().first) *
			(nodes[j].get_pos().second + nodes[i].get_pos().second);
		j = i;
	}

	return abs(area / 2.0);
}

void Cell::f_gravitation() noexcept
{}

void Cell::f_repulsion() noexcept
{}

void Cell::f_restoring() noexcept
{}

void Cell::f_pressure() noexcept
{
	for_each(get_nodes().begin(), get_nodes().cend(),
		[this](Node node) {
			// coefficient of compression
			double coef = get_area() / m_area;

			// current vector directed from the node
			t_point vect_end = make_vector(node.get_pos(), m_centre);

			// displacement
			vect_end.first -= vect_end.first / coef;
			vect_end.second -= vect_end.second / coef;
			node.add_displacement(vect_end);
		});
}

void StableCell::get_neighbors(const WorkingSpace& tissue) noexcept
{
	std::vector<Cell*> neighbors;
	for_each(tissue.all_stable.cbegin(), tissue.all_stable.cend(),
		[neighbors, tissue, this](Cell* cell) {
			if (distance(m_centre, cell->get_centre()) < tissue.search_radius)
				neighbors.push_back(cell);
		});
	m_neighbors = neighbors;
}

auto StableCell::next_state(const std::chrono::milliseconds& timer
	const std::vector< stable_cell_t >& neighbors) noexcept
{
	int new_state;
	State next_state;

	std::default_random_engine dre;
	std::uniform_int_distribution<int> di(1, 4);
	new_state = di(dre);
	if (m_state == State::ill)
	{
		switch (new_state)
		{
		case 1:
			return m_state;
		case 3:
			// depends on the virus traits
		case 4:
			// depends on the virus traits and time
		case 2:
			// 
		}
	}

	int counter_ill = 0;
	double concentration = 0;
	get_neighbors(tissue);
	for_each(m_neighbors.cbegin(), m_neighbors.cend(),
		[this, counter_ill](StableCell* neighbor) {
			if (neighbor->get_state() == State::ill)
				counter_ill += 1;
		});
	concentration = counter_ill / m_neighbors.size();

	if (m_state == State::healthy)
	{
		switch (new_state)
		{
		case 2:
			if (concentration > 0, 5)
				// depends on the virus traits, its concentration, number of ill
		case 3:
			return m_state;
		case 4:
			// depends on time
		case 1:
			// 
		}
	}
	if (m_state == State::healthy)
	{
		switch (new_state)
		{
		case 1:
			return m_state;
		case 2:
			// depends on the virus traits, its concentration, number of ill
		case 4:
			// depends on the virus traits and time
		case 3:
			// 
		}
	}
}

void StableCell::next_pos() const noexcept
{}

t_point Lymphocyte::set_direction(const WorkingSpace& tissue) const
{
	t_point new_direction = std::make_pair(0, 0);
	std::default_random_engine dre;
	std::uniform_int_distribution<double> di_first(tissue.left_bottom.first, tissue.right_top.first);
	std::uniform_int_distribution<double> di_second(tissue.left_bottom.second, tissue.right_top.second);
	new_direction.first = di_first(dre);
	new_direction.second = di_second(dre);

	return new_direction;
}

void Lymphocyte::update_velocity() noexcept
{}

void Lymphocyte::sensor(const WorkingSpace& tissue)
{
	for_each(tissue.all_stable.cbegin(), tissue.all_stable.cend(),
		[tissue, this](StableCell* cell) {
			if ((distance(m_centre, cell->get_centre()) < tissue.search_radius) &&
				((cell->get_state() == StableCell::State::ill) || (cell->get_state() == StableCell::State::recovered)))
				cell->set_flag(true);
		});
}

void Lymphocyte::next_pos() const noexcept
{}

t_point Fagocyte::set_direction(const WorkingSpace& tissue) const noexcept
{
	t_point center_of_mass = std::make_pair(0, 0);
	double quantity = tissue.all_fagocytes.size();
	for (Cell* fagocyte : tissue.all_fagocytes)
	{
		center_of_mass.first += fagocyte->get_centre().first / quantity;
		center_of_mass.second += fagocyte->get_centre().second / quantity;
	}
	return center_of_mass;
}

void Fagocyte::kill(const WorkingSpace& tissue) noexcept
{
	tissue.all_stable.erase(remove_if(tissue.all_stable.begin(), tissue.all_stable.end(),
		[tissue, this](StableCell* cell) {
			return ((distance(m_centre, cell->get_centre) < tissue.search_radius) &&
				(cell->get_flag() == true));
		}),
		tissue.all_stable.end());
}

void Fagocyte::next_pos() const noexcept
{}