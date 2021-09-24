#pragma once
/**
 * @author Simon Brisebois-Therrien 
 * @since 2021-09-15
 * 
 * @brief
 * Established server responses. 
 * 
 */

#include <vector>
#include <string>

#include "RTMPMessage.hpp"
#include "RTMPSession.hpp"

#include "Netconnection.hpp"

#include <FormatedPrint.hpp>
#include <amf0.hpp>

using namespace std;

namespace RTMP
{
    class ServerResponse
    {

        public:
            // Protocol control messages.
            static vector<char> ConnectResponse(Session&);
            static vector<char> CallResponse(Session&);
            static vector<char> CreateStreamResponse(Session&);
            static vector<char> OnStatus(Session&, int level, string code, string description = "");

            // User Control messages.
            static vector<char> StreamBegin(Session&); 
    };
}