#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "../src/core/RTMPHandshake.hpp"
#include "../src/core/RTMPChunk.hpp"
#include "../src/core/RTMPParser.hpp"
#include "../src/core/rtp.hpp"
#include "../src/utils/FormatedPrint.hpp"
//#include "../include/RTMPController.hpp"

using namespace std;

vector<int> read(string path)
{
    string line;
    vector<string> strdata;
    vector<int> data;
    ifstream file(path);
    
    while(getline(file, line))
    {
        string buffer;
        stringstream ss;
        ss << line;
        
        while (getline(ss, buffer, ' '))
        {
            data.push_back(stoul(buffer, nullptr, 16));
        }
    }
    file.close();
    cout << "Data size: " << data.size() << endl;
    return data;
}

void readHandshakeTest()
{
    vector<int> data = read("data/handshake.bin");


    // Parse handshake
    Handshake::Handshake handshake;
    RTMP::Parser::ParseHandshake(data, handshake);
    
    // Validate RTMP packet version.
    if (!(handshake.C0.version == RTP_VERSION)) 
        printf("RTMP packet not valid.");

    printf("\nRTMP Version: %i", handshake.C0.version);
    printf("\nC1 Time: %i %i %i %i", 
            handshake.C1.time[0], 
            handshake.C1.time[1], 
            handshake.C1.time[2], 
            handshake.C1.time[3]
    );
    printf("\nC1 random bytes: ");
    Utils::FormatedPrint::PrintBytes((int*)handshake.C1.randomBytes, RANDOM_BYTES_COUNT);
}

void readCommandTest()
{
    vector<int> data = read("data/setchunksize.bin");

    Utils::FormatedPrint::PrintBytes(data.data(), data.size());

    RTMP::Chunk chunk;

    RTMP::Parser::ParseChunk(data, chunk);
}

int main()
{
    //readHandshakeTest();
    readCommandTest();
}
/**
 * Message type ID:
 * 
 * User control message --> 4
 * RTMP Chunk Stream    --> 1, 2, 3, 5, 6 
 * 
 **/

