#include "server.h"
#include "getopt.h"
#include "wrap/wrap.h"
using namespace std;

extern char *optarg;
extern int optind, opterr, optopt;

int main(int argc, char** argv)
{
    int c;
    string ip = "127.0.0.1";
    uint port = 9999;
    long long timeout = -1;
    while (-1 != (c = getopt(argc, argv, "p:i:t:h")))
    {
        switch (c)
        {
        case 'p':
            port = atoi(optarg);
            if (0 == port)
                Printf("Option p parameter is incorrect\n");
            break;
        case 'i':
            while (' ' == *optarg)
                ++optarg;
            ip = string(optarg);
            break;
        case 't':
            timeout = atoll(optarg);
            if (timeout <= 0 || timeout > 60*60*24)
            {
                Printf("Option t parameter is incorrect, should be 1~60*60*24\n");
                return 1;
            }
            break;
        case 'h':
            Printf("usage: command\n\t[-p Listening port]\n\t"
                "[-i Listening address]\n\t[-t The read timeout for a bufferevent]\n\t[-h help]\n");
            return 0;
        default:
            Printf("unknow option:%c\n",optopt);
            break;
        }
    }

    if (-1 == timeout)
        run(ip, port);
    else 
        run(ip, port, {timeout, 0});
    return 0;
}
