#include "node.h"

void Node::add_displacement(const point_t& displacement) noexcept
{
	m_displacement.first += displacement.first;
	m_displacement.second += displacement.second;
}

void Node::add_displacement(double x, double y) noexcept
{
	m_displacement.first += x;
	m_displacement.second += y;
}