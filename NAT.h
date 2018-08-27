#ifndef __NAT_H__
#define __NAT_H__

#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <string>

using namespace std;

class NAT
{
private:
    /* table for exact match of received ip:port to new ip:port translation */
    unordered_map<uint64_t, pair<uint32_t, uint16_t>> exact_table;

    /* table for wild card IP */
    unordered_map<uint16_t, pair<uint32_t, uint16_t>> anyip_table;

    /* table for wild card Port */
    unordered_map<uint32_t, pair<uint32_t, uint16_t>> anyport_table;

    /* Convert IP in string to IP in uint32_t */
    uint32_t StrIPtoUint (string ip);

    /* Conver IP in uint32_t to IP in string */
    string UinttoStrIP(uint32_t ip);

    /* Convert Port in string to Port in uint16_t */    uint16_t StrPorttoUint (string port);

    /* Convert Port in uint16_t to Port in string */
    string UinttoStrPort(uint16_t port);

public:
    /* Constructor reads file and construct NAT table */
    NAT(string filename);

    /*
     * Translate received IP:Port to new IP:Port pair by looking up the table.
     * The translation result would be added in &result, return -1 if not match, return 0 if match.
     */
    int translate(string ip, string port, string &result);
};

#endif
