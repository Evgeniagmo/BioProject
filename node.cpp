#include "node.h"

void Node::add_displacement(const t_point& displacement) noexcept
{
	m_displacement.first += displacement.first;
	m_displacement.second += displacement.second;
}