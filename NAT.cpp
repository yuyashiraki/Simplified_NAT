#include "NAT.h"

uint32_t NAT::StrIPtoUint (string ip)
{
    unsigned int a, b, c, d;
    sscanf(ip.c_str(), "%u.%u.%u.%u", &a ,&b, &c, &d);
    uint32_t rt = (a << 24) | (b << 16) | (c << 8) | d;
    return rt;
}

string NAT::UinttoStrIP (uint32_t ip)
{
    string a, b, c, d;
    a = to_string((unsigned)((ip >> 24) & 0xFF));
    b = to_string((unsigned)((ip >> 16) & 0xFF));
    c = to_string((unsigned)((ip >> 8) & 0xFF));
    d = to_string((unsigned)(ip & 0xFF));
    return a + "." + b + "." + c + "." + d;
}

uint16_t NAT::StrPorttoUint (string port)
{
    unsigned int rt;
    sscanf(port.c_str(), "%u", &rt);
    return (uint16_t) rt;
}

string NAT::UinttoStrPort (uint16_t port)
{
    return to_string((unsigned) port);
}

NAT::NAT (string filename)
{
    ifstream fin;
    fin.open(filename);
    if (!fin) {
        cout << "ERROR: Unable to open file (" + filename + ")" << endl;
        exit(1);
    }

    string line;
    while (getline(fin, line)) {
        /* Read each line, split the line by ',' and ':' to translation pair */
        string from_ip, from_port, to_ip, to_port;
        stringstream ss(line);
        getline(ss, from_ip, ':');
        getline(ss, from_port, ',');
        getline(ss, to_ip, ':');
        getline(ss, to_port);
        pair<uint32_t, uint16_t> val = make_pair(StrIPtoUint(to_ip), StrPorttoUint(to_port));
        if (from_ip == "*") {
            uint16_t key = StrPorttoUint(from_port);
            if (anyip_table.count(key)) {
                cout << "Warning: Override duplicate entry for \"" + from_ip  + ":" + from_port + "\"" << endl;
            }
            anyip_table[key] = val;
        } else if (from_port == "*") {
            uint32_t key = StrIPtoUint(from_ip);
            if (anyport_table.count(key)) {
                cout << "Warning: Override duplicate entry for \"" + from_ip  + ":" + from_port + "\"" << endl;
            }
            anyport_table[key] = val;
        } else {
            uint64_t key = (StrIPtoUint(from_ip) << 16) | StrPorttoUint(from_port);
            if (exact_table.count(key)) {
                cout << "Warning: Override duplicate entry for \"" + from_ip  + ":" + from_port + "\"" << endl;
            }
            exact_table[key] = val;
        }
    }
    fin.close();
}

int NAT::translate(string ip, string port, string &result)
{
    uint32_t anyport_key = StrIPtoUint(ip);
    uint16_t anyip_key = StrPorttoUint(port);
    uint64_t exact_key = (anyport_key << 16) | anyip_key;
    pair<uint32_t, uint16_t> val;

    if (exact_table.count(exact_key)) {
        /* Exact match found */
        val = exact_table[exact_key];
    } else if (anyport_table.count(anyport_key)) {
        /* IP match with any port */
        val = anyport_table[anyport_key];
    } else if (anyip_table.count(anyip_key)) {
        /* Port match with any IP */
        val = anyip_table[anyip_key];
    } else {
        /* No match found */
        return -1;
    }
    result = UinttoStrIP(val.first) + ":" + UinttoStrPort(val.second);
    return 0;
}
