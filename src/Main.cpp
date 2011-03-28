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
	try {
		if (argc != 2) {
			std::cerr << "Usage: client <host>" << std::endl;
			return 1;
		}
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query("localhost", "10666");
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
			std::string bufout = "client sends a fuck you!";
			boost::asio::write(socket, boost::asio::buffer(bufout));
			size_t lenin = socket.read_some(boost::asio::buffer(buf), error);
			std::cout <<  "CLIENT: recv: " << buf.data() << ":" << lenin << std::endl;

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
