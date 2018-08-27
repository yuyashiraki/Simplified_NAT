#include <iostream>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <string>
#include "NAT.h"

using namespace std;

void usage(string exec_command)
{
    cout << "Usage: " + exec_command + " <NATfile> (<FLOWfile>) (<Outputfile>)" << endl;
    cout << "\tIf Outputfile is not specified, output to stdout." << endl;
    cout << "\tIf FLOWfile is not specified, read FLOW from stdin." << endl;
}

int main(int argc, char * argv[])
{
    string exec_command(argv[0]);
    if (argc < 2 || argc > 4) {
        cout << "ERROR: Invalid exec format" << endl;
        usage(exec_command);
        return 1;
    }
    string nat_filename(argv[1]);
    NAT nat(nat_filename);

    ifstream flow_f;
    istream *flow_ptr;
    ofstream out_f;
    ostream *out_ptr;
    if (argc > 2) {
        /*
         * If FLOWfile is specified, use that file as an input.
         * Otherwise, use stdin as an input.
         */
        string flow_filename(argv[2]);
        flow_f.open(flow_filename);
        if (!flow_f) {
            cout << "ERROR: Unable to open file \"" + flow_filename + "\"" << endl;
            return 1;
        }
        flow_ptr = &flow_f;
    } else {
        flow_ptr = &cin;
    }
    if (argc > 3) {
        /*
         * If Outputfile is specified, use that file as an output.
         * Otherwise, use stdout as an output.
         */
        string out_filename(argv[3]);
        out_f.open(out_filename);
        if (!out_f) {
            cout << "ERROR: Unable to open file \"" + out_filename + "\"" << endl;
            return 1;
        }
        out_ptr = &out_f;
    } else {
        out_ptr = &cout;
    }

    /*
     * Read each line from FLOWfile, split line by ',' to obtain IP and Port.
     * Translate the IP:Port by using NAT class.
     */
    string line, result;
    while (getline(*flow_ptr, line)) {
        int cur = 0;
        string ip, port;
        stringstream ss(line);
        getline(ss, ip, ':');
        getline(ss, port);
        if (0 == nat.translate(ip, port, result)) {
            *out_ptr << line << " -> " << result << endl;
        } else {
            *out_ptr << "No nat match for " << line << endl;
        }
    }

    if (out_f) out_f.close();
    if (flow_f) flow_f.close();
    return 0;
}
