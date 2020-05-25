#pragma once

#include <vector>

#include "functions.h"

using point_t = std::pair<double, double>;

class Node
{
private:

	point_t m_pos = std::make_pair(0, 0);

	// for movement on each iteraction and fixig collisions
	point_t m_displacement = std::make_pair(0, 0);

public:

	Node() noexcept = default;

	// via position
	explicit Node(const point_t pos) noexcept : m_pos(pos)
	{}

	// via x and y
	explicit Node(double first, double second) noexcept :
		m_pos(std::make_pair(first, second))
	{}

	~Node() noexcept = default;

public:

	const point_t get_pos() const noexcept
	{
		return m_pos;
	}

	// set new position after calculating forces and before fixing intersections
	// update displacement before next iteraction
	void set_pos()
	{
		m_pos = m_displacement;
	}

	const point_t get_displacement() const noexcept
	{
		return m_displacement;
	}

	// displacement updates on each iteraction after each function calculating forces
	void add_displacement(const point_t displacement) noexcept;

	void add_displacement(double x, double y) noexcept;

	// after calculating all forces it fixes intersection between the displacement vector and other cells
	void check_displacement(point_t a, point_t b) noexcept;
};