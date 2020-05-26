#include <vector>
#include <random>

#include "cells.h"


// count center of current cell as center of mass
// needed for sorting, updating neighbors
point_t Cell::count_centre() const noexcept
{
	point_t centre = std::make_pair(0.0, 0.0);

	for (const node_t node : m_nodes)
	{
		centre.first += node->get_pos().first / m_nodes.size();
		centre.second += node->get_pos().second / m_nodes.size();
	}

	return centre;
}

// count area of current cell
// needed for F_pressure()
double Cell::count_area() const noexcept
{
	double area = 0.0;

	std::size_t j = m_nodes.size() - 1;
	for (std::size_t i = 0; i < m_nodes.size(); i++)
	{
		area += (m_nodes[j]->get_pos().first * m_nodes[i]->get_pos().second) -
			(m_nodes[j]->get_pos().second * m_nodes[i]->get_pos().first);
		j = i;
	}

	return abs(area / 2.0);
}

void Cell::f_repulsion(const std::vector< stable_cell_t >& neighbors, double repulsion_radius) noexcept
{
	for (Link link : m_links)
	{
		for (stable_cell_t neighbour : neighbors)
		{
			link.find_repulsion(neighbour->get_nodes(), repulsion_radius);
		}
	}
}

void Cell::f_restoring() noexcept
{
	for (Link link : m_links)
	{
		link.find_restoring();
	}
}

void Cell::f_pressure() noexcept
{
	// coefficient of compression
	double coef = m_area / count_area();

	for (node_t node : m_nodes)
	{
		// vector directed from the node
		point_t vect_end = make_vector(node->get_pos(), count_centre());

		// calculating displacement
		// 0.25 - for each of four nodes
		vect_end.first *= (1 - coef) * 0.25;
		vect_end.second *= (1 - coef) * 0.25;
		node->add_displacement(vect_end);
	}
}


void Cell::move() noexcept
{
	for (node_t node : m_nodes)
	{
		node->set_pos();
		// reverse displacement before fixing collisions
		node->add_displacement(node->get_displacement().first * (-2),
			node->get_displacement().second * (-2));
	}
}

// check if any nodes of current cell are inside the given cell
void Cell::check_intersection(const std::vector< node_t >& nodes) noexcept
{
	for (const node_t node : m_nodes)
	{

		const std::size_t size = nodes.size();

		for (std::size_t i = 0; i < size; ++i)
		{
			node->check_displacement(nodes[i]->get_pos(), nodes[(i + 1) % size]->get_pos());
		}

		// displacement = 0 before next iteraction
		node->add_displacement(node->get_displacement().first * (-1),
			node->get_displacement().second * (-1));
	}
}

//check if any nodes of current cell are outside the working space
void Cell::check_borders(const std::vector <point_t>& borders) noexcept
{
	// for each node of the cell
	for (const node_t node : m_nodes)
	{
		const std::size_t size = borders.size();

		for (std::size_t i = 0; i < size; ++i)
		{
			node->check_displacement(borders[i], borders[(i + 1) % size]);
		}
	}
}

void StableCell::initialize(const point_t left_top)
{
	// setting nodes, counterclock-wise
	m_nodes.push_back(std::make_shared < Node >(Node(left_top)));
	m_nodes.push_back(std::make_shared < Node >(Node(left_top.first, left_top.second + m_cell_size)));
	m_nodes.push_back(std::make_shared < Node >(Node(left_top.first + m_cell_size, left_top.second + m_cell_size)));
	m_nodes.push_back(std::make_shared < Node >(Node(left_top.first + m_cell_size, left_top.second)));

	// setting links
	const auto size = m_nodes.size();

	for (std::size_t i = 0; i < size; ++i)
	{
		m_links.push_back(Link(m_nodes[i], m_nodes[(i + 1) % size]));
	}
	// normal area
	m_area = m_cell_size * m_cell_size;
}


StableCell::State StableCell::next_state(
	const std::vector< stable_cell_t > neighbors, bool virus_traits) const noexcept
{
	int random_num = rand() % 1000;

	// counting ill neighbors
	int counter_ill = 0;
	for (const stable_cell_t neighbor : neighbors)
	{
		if (neighbor->get_state() == State::ill)
			counter_ill += 1;
	}

	double ill_concentration = 1000 * counter_ill / neighbors.size();

	if (m_state == State::healthy)
	{
		if (random_num > ill_concentration) {
			std::cout << 0;
			return State::healthy;					//-----------------------------
		}
		else {
			std::cout << 1;
			return State::ill;						//--------------------------------
		} // in proportion to ill_concentration
	}

	if (m_state == State::ill)
	{
		if (!virus_traits) //can not recover
		{
			if (random_num > ill_concentration) {
				std::cout << 1;
				return State::ill;
			}
			else {
				std::cout << 1;
				return State::ill;
			} // in proportion to ill_concentration				-------------------------------------ill to dead
		}
		else // can recover
		{
			if (random_num > ill_concentration) { return State::ill; }
			else if (random_num < ill_concentration / 2) { return State::recovered; }
			else { return State::dead; }	// equally likely to die or recover
		}
	}

	if (m_state == State::recovered)
	{
		if (random_num > ill_concentration) { return State::recovered; }
		else { return State::ill; } // in proportion to ill_concentration
	}
}


std::vector <double> StableCell::pos_state_to_numbers() const noexcept
{
	// coordinates of nodes + state
	std::vector <double> numbers;
	for (node_t node : m_nodes)
	{
		numbers.push_back(node->get_pos().first);
		numbers.push_back(node->get_pos().second);
	}

	// State to double
	double state_num;
	switch (m_state)
	{
	case State::healthy:
	{
		state_num = 0.0;
		break;
	}
	case State::ill:
	{
		state_num = 1.0;
		break;
	}
	case State::recovered:
	{
		state_num = 2.0;
		break;
	}
	}
	numbers.push_back(state_num);
	std::cout << numbers[4] << " " << numbers[5] << " ";
	std::cout << state_num << std::endl;						//--------------------------------------------------------
	return numbers;
}

//point_t Lymphocyte::set_direction(const WorkingSpace& tissue) const
//{
//	point_t new_direction = std::make_pair(0, 0);
//	std::default_random_engine dre;
//	std::uniform_int_distribution<double> di_first(tissue.left_bottom.first, tissue.right_top.first);
//	std::uniform_int_distribution<double> di_second(tissue.left_bottom.second, tissue.right_top.second);
//	new_direction.first = di_first(dre);
//	new_direction.second = di_second(dre);
//
//	return new_direction;
//}
//
//void Lymphocyte::update_velocity() noexcept
//{}
//
//void Lymphocyte::sensor(const WorkingSpace& tissue)
//{
//	for_each(tissue.all_stable.cbegin(), tissue.all_stable.cend(),
//		[tissue, this](StableCell* cell) {
//			if ((distance(m_centre, cell->get_centre()) < tissue.search_radius) &&
//				((cell->get_state() == StableCell::State::ill) || (cell->get_state() == StableCell::State::recovered)))
//				cell->set_flag(true);
//		});
//}
//
//void Lymphocyte::next_pos() const noexcept
//{}
//
//point_t Fagocyte::set_direction(const WorkingSpace& tissue) const noexcept
//{
//	point_t center_of_mass = std::make_pair(0, 0);
//	double quantity = tissue.all_fagocytes.size();
//	for (Cell* fagocyte : tissue.all_fagocytes)
//	{
//		center_of_mass.first += fagocyte->get_centre().first / quantity;
//		center_of_mass.second += fagocyte->get_centre().second / quantity;
//	}
//	return center_of_mass;
//}
//
//void Fagocyte::kill(const WorkingSpace& tissue) noexcept
//{
//	tissue.all_stable.erase(remove_if(tissue.all_stable.begin(), tissue.all_stable.end(),
//		[tissue, this](StableCell* cell) {
//			return ((distance(m_centre, cell->get_centre) < tissue.search_radius) &&
//				(cell->get_flag() == true));
//		}),
//		tissue.all_stable.end());
//}
//
//void Fagocyte::next_pos() const noexcept
//{}