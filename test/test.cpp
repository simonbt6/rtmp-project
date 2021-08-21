#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "../src/core/RTMPHandshake.hpp"
#include "../src/core/RTMPParser.hpp"
#include "../src/core/rtp.hpp"
#include "../src/utils/FormatedPrint.hpp"
//#include "../include/RTMPController.hpp"

using namespace std;

vector<int> readHandshake()
{
    string line;
    vector<string> strdata;
    vector<int> data;
    ifstream file("data/handshake.bin");
    
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

vector<int> readFromIndex(vector<int> v, int index)
{
    int size = v.size();
    vector<int> data;
    for (int i = index + 1; i < size; i++)
        data.push_back(v.at(i));
    return data;
}

int main()
{
    vector<int> data = readHandshake();


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
/**
 * Message type ID:
 * 
 * User control message --> 4
 * RTMP Chunk Stream    --> 1, 2, 3, 5, 6 
 * 
 **/

