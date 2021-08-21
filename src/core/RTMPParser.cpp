/**
 * Author: Simon Brisebois-Therrien (simonbt6)
 * Date: 2021-08-14
 * 
 * Implementation RTMP packet parser.
 **/

#include "RTMPParser.hpp"

using namespace std;

namespace RTMP {

    void print(int* arr, int length)
    {
        for (int i = 0; i < length; i++)
        {
            printf("\n%i", arr[i]);
        }
    }

    void Parser::ParseF0(vector<int>& data, Handshake::Handshake& handshake)
    {
        handshake.C0.version = (unsigned short int) data.at(0);        
    }

    /**
     * Time: 4 bytes.
     * Zeros: 4 bytes.
     * Random bytes: 1528 bytes.
     **/
    void Parser::ParseF1(vector<int>& data, Handshake::Handshake& handshake)
    {
        // Time
        for (int i = 0; i < TIME_BYTES_COUNT + 1; i++)
            handshake.C1.time[i] = data.at(i + 1);

        // Random bytes
        for (int i = 0; i < RANDOM_BYTES_COUNT; i++)
            handshake.C1.randomBytes[i] = data.at(i + 9);
    }

    void Parser::ParseF2(vector<int>& data, Handshake::Handshake& handshake)
    {
        for (int i = 0; i < TIME_BYTES_COUNT; i++) 
            handshake.C2.time[i] = data.at(i);
        for (int i = 0; i < TIME_BYTES_COUNT; i++)
            handshake.C2.time2[i] = data.at(i + TIME_BYTES_COUNT);
        for (int i = 0; i < RANDOM_BYTES_COUNT; i++)
            handshake.C2.randomBytes[i] = data.at(i + (2*TIME_BYTES_COUNT));
    }

    void Parser::ParseHandshake(vector<int>& data, Handshake::Handshake& handshake)
    {
        int size = data.size();
        
        // F0 & F1
        if (size == 1537)
        {
            Parser::ParseF0(data, handshake);
            Parser::ParseF1(data, handshake);
            handshake.state = Handshake::State::VersionSent;
        }
        // F1
        else if (size == 1536)
        {
            Parser::ParseF1(data, handshake);
            handshake.state = Handshake::State::AcknowledgeSent;
        }
        // Probably F2
        else
        {
            Parser::ParseF2(data, handshake);
            handshake.state = Handshake::State::DoneSent;
        }
    }
}