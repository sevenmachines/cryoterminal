/*
 * Main.cpp
 *
 *  Created on: 28 Mar 2011
 *      Author: "SevenMachines <SevenMachines@yahoo.co.uk>"
 */

#include "Main.h"

#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace cryo {
namespace client {

Main::Main() {
	// TODO Auto-generated constructor stub

}

Main::~Main() {
	// TODO Auto-generated destructor stub
}

}//NAMESPACE
}//NAMESPACE

int main(int argc, char* argv[]) {

	// printout args
	{
		std::stringstream ssargs;
		for (int i = 0; i < argc; i++) {
			ssargs << argv[i];
			if (i < argc) {
				ssargs << " ";
			}
		}

		std::cout << "main: " << ssargs.str() << std::endl;
	}
	// get and check commands
	if (argc < 4) {
		std::cerr << "Usage: cryoterminal <server> <port> <command>" << std::endl;
		return 1;
	}
	std::string server_str(argv[1]);
	std::string port_str(argv[2]);
	std::stringstream ss;
	for (int i = 3; i < argc; i++) {
		ss << argv[i];
		if (i < argc-1) {
			ss << " ";
		}
	}
	std::string command_str = ss.str();

	try {

		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(server_str, port_str);
		tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		tcp::resolver::iterator end;

		tcp::socket socket(io_service);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end) {
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error) {
			throw boost::system::system_error(error);
		}
		for (;;) {
			boost::array<char, 128> buf;
			boost::asio::write(socket, boost::asio::buffer(command_str));
			size_t lenin = socket.read_some(boost::asio::buffer(buf), error);
			std::cout << "CLIENT: recv: " << buf.data() << ":" << lenin << std::endl;

			if (error == boost::asio::error::eof)
				break; // Connection closed cleanly by peer.
			else if (error)
				throw boost::system::system_error(error); // Some other error.

		}

	} catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
