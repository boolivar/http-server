#include "app.h"

#include <iostream>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/bind.hpp>

using namespace boost::asio;

App::App(const std::string& ip, int port, const std::string& dir) :
    _ip(ip), _port(port), _dir(dir)
{
    std::cout << "ip:" << _ip << std::endl;
    std::cout << "port:" << _port << std::endl;
    std::cout << "dir:" << _dir << std::endl;
}

App::~App()
{
}

int App::run()
{
    io_service io;
    ip::tcp::acceptor acceptor(io, ip::tcp::endpoint(ip::address::from_string(_ip), _port), true);

    std::shared_ptr<ip::tcp::socket> socket(new ip::tcp::socket(io));
    acceptor.async_accept(*socket, boost::bind(&App::handleAccept, this, socket, boost::asio::placeholders::error));

    io.run();

    return 0;
}

void App::handleAccept(std::shared_ptr<ip::tcp::socket> socket, const boost::system::error_code& e) {
    if (!e) {
        std::shared_ptr<streambuf> buf(new streambuf);
        async_read_until(*socket, *buf, '\n', boost::bind(&App::handleRead, this, socket, buf,
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred()));
        std::cout << "accept" << std::endl;
    } else {
        std::cout << "accept error: " << e << std::endl;
    }

}

void App::handleRead(std::shared_ptr<ip::tcp::socket> socket, std::shared_ptr<streambuf> buf,
                     const boost::system::error_code& e, std::size_t bytes) {
    if (!e) {
        //std::istream in(buf);
        std::cout << "read: " << bytes << std::endl;
    } else {
        std::cout << "read error: " << e << std::endl;
    }
}

