#include "Utils.hpp"

#include <stdio.h>
#include <WinSock.h>

// https://www.codeguru.com/csharp/csharp/cs_network/article.php/c6045/Obtain-all-IP-addresses-of-local-machine.htm

std::vector<std::string> get_ip_addresses()
{
    std::vector<std::string> ip_addresses;

    WORD wVersionRequested;
    WSADATA wsaData;
    char name[255];
    PHOSTENT hostinfo;
    wVersionRequested = MAKEWORD(1, 1);
    char *ip_address;

    if (WSAStartup(wVersionRequested, &wsaData) != 0)
    {
        return ip_addresses;
    }
    if (gethostname(name, sizeof(name)) != 0)
    {
        return ip_addresses;
    }

    if ((hostinfo = gethostbyname(name)) != NULL)
    {
        int nCount = 0;
        while (hostinfo->h_addr_list[nCount])
        {
            ip_address = inet_ntoa(*(struct in_addr *)hostinfo->h_addr_list[nCount]);
            std::string ip_address_str(ip_address);
            ip_addresses.push_back(ip_address_str);
            nCount++;
        }
    }
    return ip_addresses;
};
