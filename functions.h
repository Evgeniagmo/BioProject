#pragma once

#include <cmath>;

using point_t = std::pair <double, double>;

double distance(const point_t& left, const point_t& right) noexcept
{
	return std::hypot((left.first - right.first), (left.second - right.second));
}

double length(point_t vector) noexcept
{
	return std::hypot(vector.first, vector.second);
}

point_t make_vector(const point_t& init, const point_t & final) noexcept //creating vector directed from initial point to final point
{
	return std::make_pair((final.first - init.first), (final.second - init.second));
}

point_t norm(const point_t vector) noexcept
{
	const auto l = length(vector);

	return std::make_pair(vector.first / l, vector.second / l);
}

point_t find_center(const point_t& left, const point_t& right) noexcept
{
	return std::make_pair((left.first + right.first) * 0.5, (left.second + right.second) * 0.5);
}
