#pragma once
extern "C" 
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <inttypes.h>
};

/**
 * @author Simon Brisebois-Therrien
 * @date 2021-09-22 
 * 
 * 
 */

#include <iostream>
#include <vector>
#include <FormatedPrint.hpp>

namespace Codecs
{
    class VideoDecoder
    {
        private:
            /** 
             * Class members.
             **/
            
        public:
            VideoDecoder();

            void DecodeVideoData(std::string filename, const std::vector<uint8_t>* data);

        private:
    };
}