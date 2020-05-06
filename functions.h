#pragma once

#include <cmath>

using t_point = std::pair <double, double>;

double distance(t_point left, t_point right)
{
	return std::hypot((left.first - right.first), (left.second - right.second));
}

t_point make_vector(t_point init, t_point final) //creating vector directed from initial point to final point
{
	return std::make_pair((final.first - init.first), (final.second - init.second));
}
