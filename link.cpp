#include "link.h"

void Link::find_restoring() noexcept
{
	// delta x for Hooke's law (stiffness = 0)
	double deformation = count_length() - m_length;

	// for right node
	point_t vect_end = make_vector(l_node->get_pos(), r_node->get_pos());
	vect_end.first = norm(vect_end).first * deformation * 0.5;
	vect_end.second = norm(vect_end).second * deformation * 0.5;

	r_node->add_displacement(vect_end);

	// for left node
	l_node->add_displacement(vect_end.first * (-1), vect_end.second * (-1));
}

void Link::find_repulsion(const std::vector< node_t > nodes, double repulsion_radius) noexcept
{
	// making vector from link and normal vector to the link directed out of the cell
	point_t vect = make_vector(l_node->get_pos(), r_node->get_pos());

	point_t normal = norm(std::make_pair(vect.second, vect.first * (-1)));

	for (node_t node : nodes)
	{
		// vector from one end of link to other cell's node
		// and projection on the normal vector
		point_t dist = make_vector(l_node->get_pos(), node->get_pos());

		point_t projection = std::make_pair(dist.first * normal.first, dist.second * normal.second);

		double projection_length = length(projection);

		if (projection_length < repulsion_radius && projection_length != 0)
		{
			// gravitation -1/x^2 (to the link)
			// repulsion 1/x^4 (from the link)
			double gravitation = -0.001 / length(projection);

			double repulsion = gravitation * gravitation;

			point_t displacement = std::make_pair(
				normal.first * (repulsion + gravitation), normal.second * (repulsion + gravitation));
			node->add_displacement(displacement);
		}
	}
}