#pragma once

#include <boost/asio.hpp>

#include <string>  
#include <iostream> 
#include <sstream> 
#include <cmath>
#include <vector>
#include <iomanip>
#include <chrono>
#include <cstdlib>
#include <ctime>

using point_t = std::pair <double, double>;



// distance between two points
inline double distance(const point_t& left, const point_t& right) noexcept
{
	return std::hypot((left.first - right.first), (left.second - right.second));
}

// length of given vector
inline double length(const point_t& vector) noexcept
{
	return std::hypot(vector.first, vector.second);
}

//creating vector directed from initial point to final point
inline point_t make_vector(const point_t& init, const point_t & final) noexcept
{
	return std::make_pair((final.first - init.first), (final.second - init.second));
}

// vector with length = 1
inline point_t norm(const point_t& vector) noexcept
{
	const auto l = length(vector);

	if (l) { return std::make_pair(vector.first / l, vector.second / l); }
	else { return std::make_pair(0, 0); }

}

// center of given vector
inline point_t find_center(const point_t& left, const point_t& right) noexcept
{
	return std::make_pair((left.first + right.first) * 0.5, (left.second + right.second) * 0.5);
}


// time in milliseconds to format hh:mm:ss.zzz
inline std::string time_to_send(std::chrono::milliseconds ms) noexcept
{
	std::chrono::hours hh = std::chrono::duration_cast<std::chrono::hours>(ms);

	std::chrono::minutes mm = std::chrono::duration_cast<std::chrono::minutes>(ms % std::chrono::hours(1));

	std::chrono::seconds ss = std::chrono::duration_cast<std::chrono::seconds>(ms % std::chrono::minutes(1));

	std::chrono::milliseconds zzz = std::chrono::duration_cast<std::chrono::milliseconds>(ms % std::chrono::seconds(1));

	std::stringstream buffer;

	buffer << std::setfill('0') << std::setw(2) << hh.count() << ":"
		<< std::setw(2) << mm.count() << ":"
		<< std::setw(2) << ss.count() << "."
		<< std::setw(3) << zzz.count();

	return 	buffer.str();
}


inline std::vector<double> read_data_until(boost::asio::ip::tcp::socket& socket)
{
	boost::asio::streambuf buffer;

	boost::asio::read_until(socket, buffer, '\n');

	std::string message;

	std::istream input_stream(&buffer);
	std::getline(input_stream, message, '\n');

	std::stringstream ss(message);

	std::vector<double> values;
	double value;
	while (ss >> value)
	{
		values.push_back(value);

		if (ss.peek() == ' ')
			ss.ignore();
	}

	return values;
}

//get the last element of the vector
inline double read_data_from(std::vector <double>& data) noexcept
{
	if (!data.empty())
	{
		double value = data.back();
		data.pop_back();

		return value;
	}
}

inline void write_data(boost::asio::ip::tcp::socket& socket, double data)
{
	int size_to_send = sizeof(data);

	auto size_as_string = (std::to_string(size_to_send) + '\n');

	boost::asio::write(socket, boost::asio::buffer(size_as_string));

	auto data_as_string = (std::to_string(data) + '\n');

	boost::asio::write(socket, boost::asio::buffer(data_as_string));
}