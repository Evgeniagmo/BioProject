#pragma once

#include <vector>

using point_t = std::pair<double, double>;

class Node
{
private:

	point_t m_pos;

	point_t m_displacement; // for movement on each iteraction

public:

	Node() noexcept = default;

	explicit Node(const point_t& pos) noexcept : m_pos(pos)
	{}

	explicit Node(double first, double second) noexcept :
		m_pos(std::make_pair(first, second))
	{}

	~Node() noexcept = default;

public:

	const point_t get_pos() const noexcept
	{
		return m_pos;
	}

	void set_pos(const point_t& pos)
	{
		m_pos = pos;
	}

	const point_t get_displacement() const noexcept
	{
		return m_displacement;
	}

	// displacement updates on each iteraction
	void add_displacement(const point_t& displacement) noexcept;

	void add_displacement(double x, double y) noexcept;
};