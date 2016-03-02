#ifndef APP_H
#define APP_H

#include <string>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/error_code.hpp>
#include <boost/regex.hpp>

using namespace boost;

class App
{
public:
    App(const std::string& ip, int port, const std::string& dir);
    ~App();

    int run();

private:
    std::string _ip;
    int _port;
    std::string _dir;
    boost::regex _regex;

    void handleAccept(std::shared_ptr<asio::ip::tcp::socket> socket, const boost::system::error_code& e);
    void handleRead(std::shared_ptr<asio::ip::tcp::socket> socket, std::shared_ptr<asio::streambuf> buf, const boost::system::error_code& e, std::size_t bytes);
};

#endif // APP_H
