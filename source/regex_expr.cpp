#include <iostream>
#include <regex>
#include <cmath>
#include <numeric>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/smart_ptr.hpp>

using namespace std;
using namespace boost;
using namespace boost::asio;

typedef boost::shared_ptr<ip::tcp::socket> socket_ptr;

void client_session(socket_ptr sock);

void sync_client();
void sync_server();

void async_client();
void async_server();

int main(int argc, char** argv)
{
	return 0;
}

void connect_handler(const boost::system::error_code& ec)
{

}

void handle_accept(socket_ptr sock, const boost::system::error_code& err)
{
	if (err)
		return;
	//socket_ptr sock(new ip::tcp::socket(service));
	//start_accept(sock);
}

void start_accept(socket_ptr sock)
{

}

void async_client()
{
	io_service service;
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
	ip::tcp::socket sock(service);
	sock.async_connect(ep, connect_handler);
	service.run();
}

void async_server()
{
	io_service service;
	ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
	ip::tcp::acceptor accept(service, ep);
	socket_ptr sock(new ip::tcp::socket(service));
	//start_accept(sock);
	accept.async_accept(*sock, boost::bind(handle_accept, sock, _1));
	service.run();
}

void sync_client()
{
	io_service service;
	ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
	ip::tcp::socket sock(service);
	sock.connect(ep);
}


void sync_server()
{
	io_service service;
	ip::tcp::endpoint ep(ip::tcp::v4(), 2001);
	ip::tcp::acceptor accept(service, ep);
	while (true)
	{
		socket_ptr sock(new ip::tcp::socket(service));
		accept.accept(*sock);
		boost::thread(boost::bind(client_session, sock));
	}
}

void client_session(socket_ptr sock)
{
	while (true)
	{
		char data[512];
		boost::system::error_code error;
		size_t len = sock->read_some(buffer(data), error);
		if (error == error::eof)
			return;

		if (len > 0)
		{
			write(*sock, buffer("ok", 2));
		}
	}
}
