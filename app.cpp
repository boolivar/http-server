#include "app.h"

#include <iostream>
#include <memory>
#include <functional>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>

using namespace boost::asio;

App::App(const std::string& ip, int port, const std::string& dir):
    _acceptor(_io, ip::tcp::endpoint(ip::address::from_string(ip), port)),
    _dir(dir), _regex("\\n\\r??\\n")
{
    std::cout << "ip:" << ip << std::endl;
    std::cout << "port:" << port << std::endl;
    std::cout << "dir:" << dir << std::endl;
}

App::~App()
{
}

int App::run()
{
    startAccept();
    _io.run();

    return 0;
}

void App::startAccept() {
    std::shared_ptr<ip::tcp::socket> socket(new ip::tcp::socket(_io));
    _acceptor.async_accept(*socket, boost::bind(&App::handleAccept, this, socket, boost::asio::placeholders::error));
}

void App::handleAccept(std::shared_ptr<ip::tcp::socket> socket, const boost::system::error_code& e) {
    //startAccept();
    if (!e) {
        std::shared_ptr<streambuf> buf(new streambuf);
        async_read_until(*socket, *buf, _regex, boost::bind(&App::handleRead, this, socket, buf,
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        std::cout << "accept" << std::endl;
    } else {
        std::cout << "accept error: " << e << std::endl;
    }
}

void App::handleRead(std::shared_ptr<ip::tcp::socket> socket, std::shared_ptr<streambuf> buf,
                     const boost::system::error_code& e, std::size_t bytes) {
    if (!e) {
        std::istream in(buf.get());
        std::string s;
        in >> s;
        std::cout << "read " << bytes << ":" << s << std::endl;
        buf->consume(bytes);

        std::ostream out(buf.get());
        out << "Hello" << std::endl;
        async_write(*socket, *buf, boost::bind(App::handleWrite, this, socket, buf,
                    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    } else {
        std::cout << "read error: " << e << std::endl;
    }
}

void App::handleWrite(std::shared_ptr<ip::tcp::socket> socket, std::shared_ptr<streambuf> buf,
                     const boost::system::error_code& e, std::size_t bytes) {
    if (!e) {
        std::cout << "write " << bytes << std::endl;
    } else {
        std::cout << "write error: " << e << std::endl;
    }
}
