#pragma once
/**
 * @file Object.hpp
 * 
 * @author Simon Brisebois-Therrien (simonbt6@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-09-16
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <map>
#include <string>

using namespace std;

namespace Utils
{
    enum class DataType
    {
        Number,
        Boolean,
        String,
        Object
    };

    struct Property
    {
        DataType type;
        virtual ~Property() = default;
    };

    template <typename T>
    struct Field : Property
    {
        T value;
    };

    enum PropertyType
    {
        app,
        flashver,
        swtUrl,
        tcUrl,
        fpad,
        audioCodecs,
        videoCodecs,
        videoFunction,
        pageUrl,
        objectEncoding,
        type
    };

    static inline map<std::string, PropertyType> PropertyTypeLinker = {
        {"app", PropertyType::app},
        {"flashVer", PropertyType::flashver},
        {"swtUrl", PropertyType::swtUrl},
        {"tcUrl", PropertyType::tcUrl},
        {"fpad", PropertyType::fpad},
        {"audioCodecs", PropertyType::audioCodecs},
        {"videoCodecs", PropertyType::videoCodecs},
        {"videoFunction", PropertyType::videoFunction},
        {"pageUrl", PropertyType::pageUrl},
        {"objectEncoding", PropertyType::objectEncoding},
        {"type", PropertyType::type},
    };

    static inline map<PropertyType, std::string> PropertyNameLinker = {
        {PropertyType::app, "app"},
        {PropertyType::flashver, "flashVer"},
        {PropertyType::swtUrl, "swtUrl"},
        {PropertyType::tcUrl, "tcUrl"},
        {PropertyType::fpad, "fpad"},
        {PropertyType::audioCodecs, "audioCodecs"},
        {PropertyType::videoCodecs, "videoCodecs"},
        {PropertyType::videoFunction, "videoFunction"},
        {PropertyType::pageUrl, "pageUrl"},
        {PropertyType::objectEncoding, "objectEncoding"},
        {PropertyType::type, "type"}
    };
        
    typedef map<PropertyType, Property*> Object;
}