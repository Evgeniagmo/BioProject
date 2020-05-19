#include <iostream>
#include <boost/asio.hpp>
#include "WorkingSpace.h"

std::vector <double> read_data(boost::asio::ip::tcp::socket& socket)
{
	boost::asio::streambuf buf;
	boost::asio::read(socket, buf);
	return boost::asio::buffer_cast<std::vector <double>>(buf.data());
}

int main(int argc, char** argv)
{

	system("chcp 1251");

	const std::size_t size = 30;

	auto port = 3333;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

	boost::asio::io_service io_service;

	std::vector <double> data;


	try
	{
		boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

		acceptor.bind(endpoint);

		acceptor.listen(size);

		boost::asio::ip::tcp::socket socket(io_service);

		acceptor.accept(socket);

		data = read_data(socket);

	}
	catch (boost::system::system_error & e)
	{
		std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

		system("pause");

		return e.code().value();
	}

	system("pause");

	double width = data.back();
	data.pop_back();
	double height = data.back();
	data.pop_back();

	double concentration = data.back();
	data.pop_back();
	bool virus_traits = data.back();
	data.pop_back();

	WorkingSpace working_space(
		width, height,
		concentration, virus_traits);

	working_space.calc_next();
	working_space.set_next();

	// collision

	std::vector <std::vector <double> > cells = working_space.data_to_send();

	return EXIT_SUCCESS;
}