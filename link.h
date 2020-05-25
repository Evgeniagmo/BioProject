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
		l_node(left), r_node(right), m_length(distance(left->get_pos(), right->get_pos()))
	{}

	~Link() noexcept = default;

public:

	const double count_length() const noexcept
	{
		return distance(l_node->get_pos(), r_node->get_pos());
	}

	//forces on each iteraction
	void find_restoring() noexcept;
	void find_repulsion(const std::vector< node_t > nodes, double search_radius) noexcept;

};