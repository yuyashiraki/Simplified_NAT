Simplified NAT
===
## Description
- `main.cpp` : Main function to run the program
- `NAT.h` : Header where NAT class is defined
- `NAT.cpp` : Functions in NAT class are defined 

## Functionality
- Translate a received IP:Port pair into a new IP:Port pair based on the configuration file
- Either IP or Port, but not both, can be '\*' to match any IP or Port
- If translation is not found, return 'No match'
- It first look for exact match, second look for IP match, third look for Port match

## Usage
```
$ g++ -std=gnu++14 main.cpp NAT.cpp -o exec
$ ./exec <NATfile> (<FLOWfile>) (<Outputfile>)
```
** If Outputfile is not specified, output to stdout.
If FLOWfile is not specified, read FLOW from stdin.**

## Test
- Made sure when FLOWfile or Outputfile or both are not specified, it still works correctly with stdin and stdout
- Made sure output is correct with random cases (given as sample from Illumio), edge cases (e.g. IP=0.0.0.0, IP=255.255.255.255, Port=0, Port=65535), and duplicate case by examining the output of all the tests file in the `tests` directory

## Performance
- The implementation focus on reducing the space by keeping both IP and Port in binary
- Lookup time would be O(1) by using Hash Table
- Due to the 90 minutes time constraint, actual performance is not measured

## Observation
1. **Test** - In the test, since IP range is 0.0.0.0~255.255.255.255 and Port range is 0~65535, edge cases are defined as the case when IP or Port is in the corner of its range (file: `tests/NAT1.txt` ~ `tests/NAT6.txt`). To make sure a lookup order is preserved, duplicate matching cases are tested (file: `tests/NAT7.txt`).
2. **Design** - The important part of NAT implementation is how to keep the NAT entries and how to look up them. As defined in `NAT.h`, I keep three hash tables (unordered_map) for when exact IP:Port is defined, when IP is \*, when Port is \*. To make query faster, the code stores key in binary rather than string since hashing function for string consumes extra overhead. To reduce space, everything is stored in binary, but as a trade off, it can cause overhead of converting into binary and vice versa. Also, to make the code readable, I used class and separated into multiple files.
3. **Future Optimization** - If there are no 90 minutes constraint, I have developed the code for performance measurement to actually check the time per query and space consumption. Also, the structure of directory is not well organized. It should consists of `Makefile`, `src/`, `header/`, and `test.sh` to automate compile and test and to clarify the code organization.
4. **Well-define challenge** - To modify the challenge well-defined, I would make the challenge closer to actual NAT. For example, instead of '\*', introduce subnet for NAT entries. In addition to convert to static entries, let it convert to IP or Port randomly selected from given range. Have inbound and outbound transaction.
5. **Any other comment** - Thank you for taking time.
