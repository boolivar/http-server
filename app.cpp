#include "app.h"

#include <fstream>
#include <functional>
#include <memory>

#include <omp.h>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

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

#pragma omp parallel
{
    for (;;) {
        _io.run();
    }
}

    return 0;
}

void App::startAccept() {
    std::shared_ptr<ip::tcp::socket> socket(new ip::tcp::socket(_io));
    _acceptor.async_accept(*socket, boost::bind(&App::handleAccept, this, socket, boost::asio::placeholders::error));
}

void App::handleAccept(std::shared_ptr<ip::tcp::socket> socket, const boost::system::error_code& e) {
    startAccept();
    if (!e) {
        std::shared_ptr<streambuf> buf(new streambuf);
        async_read_until(*socket, *buf, _regex, boost::bind(&App::handleRead, this, socket, buf,
            boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        std::cout << "#" << omp_get_thread_num() << " accept" << std::endl;
    } else {
        std::cout << "accept error: " << e << std::endl;
    }
}

void App::handleRead(std::shared_ptr<ip::tcp::socket> socket, std::shared_ptr<streambuf> buf,
                     const boost::system::error_code& e, std::size_t bytes) {
    if (!e) {
        std::istream in(buf.get());

        std::istream_iterator<char> eos;
        std::string s(std::istream_iterator<char>(in), eos);

        std::cout << "#" << omp_get_thread_num() << " read " << bytes << ":" << s << std::endl;

        buf->consume(bytes);

        std::ostream out(buf.get());
        std::shared_ptr<std::istream> data = getResponse(s);

        out << data->rdbuf();

        async_write(*socket, *buf, boost::bind(App::handleWrite, this, socket, buf,
                    boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    } else {
        std::cout << "read error: " << e << std::endl;
    }
}

std::shared_ptr<std::istream> App::getResponse(const std::string& request) {

    boost::match_results<std::string::const_iterator> what;
    boost::regex r("^GET(.*)(?:\\?.*)?HTTP");
    if (boost::regex_search(request, what, r)) {
        std::string url = what[1];
        boost::algorithm::trim(url);
        std::cout << "parse: " << url << std::endl;
        std::shared_ptr<std::ifstream> file(new std::ifstream(_dir + url, std::ifstream::in));
        if (file->good()) {
            std::cout << "found file" << std::endl;
            return file;
        }
    }

    std::cout << "file not found" << std::endl;
    return std::make_shared<std::istringstream>("HTTP/1.0 404 NOT FOUND\r\nContent-Type: text/html\r\n\r\n");
}

void App::handleWrite(std::shared_ptr<ip::tcp::socket> socket, std::shared_ptr<asio::streambuf> buf,
                     const boost::system::error_code& e, std::size_t bytes) {
    if (!e) {
        std::cout << "#" << omp_get_thread_num() << " write " << bytes << std::endl;
    } else {
        std::cout << "write error: " << e << std::endl;
    }
}
