#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/serialization/serialization.hpp>

#include "../src/core/rtp.hpp"
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

struct C1
{
    int* time;
    int* zeros;
    int* randomBytes;
};

C1 readC1(vector<int> v)
{
    v.erase(v.begin());   
    if (v.size() != 1536) return;
    
    /**
     * time:            4 bytes
     * zero:            4 bytes
     * random bytes:    1528 bytes
     **/
    int time[4], 
        zeros[4], 
        randomBytes[1528];

    for (int i = 0; i < v.size(); i++)
    {
        if (i < 4) time[i] = v.at(i);
        else if (i >= 4 && i < 8) zeros[i] = v.at(i);
        else if (i >= 8) randomBytes[i] = v.at(i); 
    }

    return C1 {time, zeros, randomBytes};
}

int main()
{
    vector<int> data = readHandshake();

    // Validate RTMP packet version.
    if (!(data.at(0) == RTP_VERSION)) 
        printf("RTMP packet not valid.");

    C1 c1 = readC1(data);
    
    for (int i = 0; i < 4; i++)
    {
        cout << c1.time[i] << endl;
    }
    

}
/**
 * Message type ID:
 * 
 * User control message --> 4
 * RTMP Chunk Stream    --> 1, 2, 3, 5, 6 
 * 
 **/

