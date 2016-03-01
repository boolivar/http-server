#ifndef APP_H
#define APP_H

#include <string>

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
};

#endif // APP_H
