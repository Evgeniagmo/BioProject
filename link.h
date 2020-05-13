#pragma once

#include <memory>

#include "functions.h"
#include "node.h"

class Link
{
public:

	using point_t = std::pair <double, double>;
	using node_t = std::shared_ptr < Node >;

private:

	double m_length;

	node_t l_node, r_node;

public:
	Link() noexcept = default;

	explicit Link(const node_t right, const node_t left) noexcept :
		l_node(left), r_node(right), m_length(distance(l_node->get_pos(), r_node->get_pos()))
	{}

	~Link() noexcept = default;

public:

	const double count_length() const noexcept
	{
		return distance(l_node->get_pos(), r_node->get_pos());
	}

	void find_restoring() noexcept;

	void find_repulsion(const std::vector< node_t > nodes, double search_radius) noexcept;

	/*const Node get_left_node() const noexcept
	{
		return l_node;
	}

	void set_left_node(Node new_left)
	{
		l_node = new_left;
	}

	const Node get_right_node() const noexcept
	{
		return r_node;
	}

	void set_right_node(Node new_right)
	{
		r_node = new_right;
	}*/

};