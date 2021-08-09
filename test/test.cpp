#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

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

int main()
{
    vector<int> data = readHandshake();
    
    // index of C0
    int iC0;

    for (int i = 0; i < data.size(); i++)
    {
        int x = data.at(i);

        // Find 0x03/C0 index.
        if (x == RTP_VERSION) iC0 = i;
    }

    // Handshake C1.
    vector<int> C1 = readFromIndex(data, iC0);
}

