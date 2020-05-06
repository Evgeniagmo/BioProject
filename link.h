#pragma once

#include "functions.h"
#include "node.h"

class Link
{
private:

	double m_length;

	Node l_node, r_node;

public:
	Link() noexcept = default;

	explicit Link(const Node& left, const Node& right) noexcept :
		l_node(left), r_node(right)
	{
		this->set_length();
	}

	~Link() noexcept = default;

public: 

	const double get_length() const noexcept
	{
		return m_length;
	}

	void set_length()
	{
		m_length = distance(l_node.get_pos(), r_node.get_pos());
	}

	const Node get_left_node() const noexcept
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
	}

};