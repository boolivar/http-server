#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

#include <unistd.h>

using namespace std;

static std::string ip = "127.0.0.1";
static int port = 8080;
static std::string dir = "./";

static int parseOpts(int argc, char** argv);

int main(int argc, char** argv)
{
    if (parseOpts(argc, argv) == 0) {
        return 0;
    }
    return 1;
}

// -h <ip> -p <port> -d <directory>
static int parseOpts(int argc, char** argv) {
    int c;

    opterr = 0;

    while ((c = getopt (argc, argv, "h:p:d:")) != -1)
    {
        switch (c)
        {
        case 'h':
            ip = optarg;
            break;
        case 'p':
            port = atoi(optarg);
            break;
        case 'd':
            dir = optarg;
            break;
        case '?':
            if (strchr("hpd", optopt) != NULL)
                fprintf (stderr, "Option -%c requires an argument.\n", optopt);
            else if (isprint (optopt))
                fprintf (stderr, "Unknown option `-%c'.\n", optopt);
            else
                fprintf (stderr,
                         "Unknown option character `\\x%x'.\n",
                         optopt);
            return -1;
        default:
            return -2;
        }
    }
    return 0;
}

