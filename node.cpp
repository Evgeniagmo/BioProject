#include "node.h"

void Node::add_displacement(const point_t displacement) noexcept
{
	m_displacement.first += displacement.first;
	m_displacement.second += displacement.second;
}

void Node::add_displacement(double x, double y) noexcept
{
	m_displacement.first += x;
	m_displacement.second += y;
}


// a, b - nodes of cell (counterclock-wise), r - vector from a to b
// d - previous node position (m_pos), c - current node position, s - reverse displacement
// b = a + r, d = c + s
// system:		b = a + t*r, d = c + u*s
// scalar values t and u:	t = ((c - a) x s)/(r x s),	 u = ((c - a) x r)/(r x s)
// if t and u are between 0 and 1: intersection point is a + u*s

void Node::check_displacement(point_t a, point_t b) noexcept
{
	point_t r = make_vector(a, b);
	double divisor = r.first * m_displacement.second - r.second * m_displacement.first;
	double u = ((m_pos.first - a.first) * r.second -
		(m_pos.second - a.second) * r.first) / divisor;
	double t = ((m_pos.first - a.first) * m_displacement.second -
		(m_pos.second - a.second) * m_displacement.first) / divisor;

	if (0.0 <= u && 0.0 <= t && u <= 1.0 && t <= 1.0)
	{
		// if there is intersection, set displacement near intersection point
		m_displacement.first *= (u * 1.01);
		m_displacement.second *= (u * 1.01);
		set_pos();
	}
}