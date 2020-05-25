#include <iostream>
#include <boost/asio.hpp>
#include <type_traits>

#include "WorkingSpace.h"
#include "functions.h"

int main(int argc, char** argv)
{
	const std::size_t size = 25;

	auto port = 8000;

	// 8000
	// 178.215.110.34
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

	boost::asio::io_service io_service;

	// initial conditions from client
	std::vector<double> initial_cond;

	std::vector <std::vector <double> > cells;

	double ill_counter;

	std::string current_time;

	std::chrono::milliseconds timer(0);

	std::chrono::milliseconds test(3000);

	//WorkingSpace working_space(
	//	200, 20,
	//	0.5, 0);

	//std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();

	//while (timer < test)
	//{
	//	timer += std::chrono::duration_cast <std::chrono::milliseconds> (
	//		std::chrono::steady_clock::now() - time_point);

	//	time_point = std::chrono::steady_clock::now();

	//	working_space.calc_next();
	//	working_space.collision_update();
	//	working_space.set_next();

	//	cells = working_space.data_to_send();

	//	ill_counter = working_space.statistics_to_send();

	//	current_time = time_to_send(timer);

	//	std::cout << ill_counter << std::endl;

	//	std::cout << current_time << std::endl;

	//}

	try
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

		acceptor.bind(endpoint);

		acceptor.listen(size);

		boost::asio::ip::tcp::socket socket(io_service);

		acceptor.accept(socket);

		std::cout << "connection" << std::endl;

		initial_cond = read_data_until(socket);

		std::cout << "data received" << std::endl;

		double width = read_data_from(initial_cond);
		double height = read_data_from(initial_cond);
		double concentration = read_data_from(initial_cond);
		bool virus_traits = read_data_from(initial_cond);

		std::cout << "data is ready" << std::endl;

		//system("pause");

		WorkingSpace working_space(
			width, height,
			concentration, virus_traits);

		std::chrono::steady_clock::time_point time_point = std::chrono::steady_clock::now();

		while (timer < test)
		{
			timer += std::chrono::duration_cast <std::chrono::milliseconds> (
				std::chrono::steady_clock::now() - time_point);

			time_point = std::chrono::steady_clock::now();

			working_space.calc_next();
			working_space.collision_update();
			working_space.set_next();

			cells = working_space.data_to_send();

			ill_counter = working_space.statistics_to_send();

			current_time = time_to_send(timer);

			//write_data(socket, cells);

			write_data(socket, ill_counter);

			//write_data(socket, current_time);

		}

	}
	catch (boost::system::system_error & e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		return e.code().value();
	}



	system("pause");

	return EXIT_SUCCESS;
}