#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "../src/core/RTMPHandshake.hpp"
#include "../src/core/RTMPChunk.hpp"
#include "../src/core/RTMPParser.hpp"
#include "../src/core/RTMPSession.hpp"

#include "../src/utils/FormatedPrint.hpp"

#define RTP_VERSION 3

using namespace std;

vector<char> read(string path)
{
    string line;
    vector<string> strdata;
    vector<char> data;
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
    vector<char> C0C1Data = read("data/handshakeC0C1.bin");
    vector<char> C2Data = read("data/handshakeC2.bin");

    vector<char> connectCmd = read("data/connect.bin");
    vector<char> createstreamCmd = read("data/createstream.bin");

    // Parse handshake
    RTMP::Session session;
    RTMP::Parser::ParseData(C0C1Data, session.handshake);
    printf("\nF0 & F1 parsed. Parsing F2...");
    RTMP::Parser::ParseData(C2Data, session.handshake);

    
    // Validate RTMP packet version.
    if (!(session.handshake.C0.version == RTP_VERSION)) 
        printf("RTMP packet not valid.");

    // Handshake done.
    RTMP::Parser::ParseData(connectCmd, session.handshake);
    RTMP::Parser::ParseData(createstreamCmd, session.handshake);

    printf("\nRTMP Version: %i", session.handshake.C0.version);
    printf("\nC1 Time: %X %X %X %X", 
            session.handshake.C1.time[0], 
            session.handshake.C1.time[1], 
            session.handshake.C1.time[2], 
            session.handshake.C1.time[3]
    );
    printf("\nC1 random bytes: ");
    //Utils::FormatedPrint::PrintBytes((int*)handshake.C1.randomBytes, RANDOM_BYTES_COUNT);
}

void readCommandTest()
{
    vector<char> data = read("data/publishtest.bin");

    Utils::FormatedPrint::PrintBytes((int*)data.data(), data.size());

    RTMP::Chunk chunk;

    RTMP::Parser::ParseChunk(data, chunk);
}

int main()
{
    readHandshakeTest();
    //readCommandTest();
}
/**
 * Message type ID:
 * 
 * User control message --> 4
 * RTMP Chunk Stream    --> 1, 2, 3, 5, 6 
 * 
 **/

