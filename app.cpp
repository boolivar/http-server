#include "app.h"

#include <iostream>

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


    return 0;
}

