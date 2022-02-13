#include <iostream>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <regex>
#include <algorithm>
#include <logger/logger.h>

#include "string/string.h"

namespace base {

std::shared_ptr<base::Logger> String::m_logger = std::make_shared<base::Logger>();

template<typename T1, typename T2>
std::vector<std::string> 
String::SplitString(T1 input, T2 delimiter) {
    std::vector<std::string> res;
    if ((std::is_same<T1, std::string>::value || std::is_same<T1, const char*>::value) &&
        (std::is_same<T2, std::string>::value || std::is_same<T2, const char*>::value)) {

        std::string inputStr(input);
        std::string delimiterStr(delimiter);

        size_t pos_start = 0, pos_end, delim_len = std::strlen(delimiterStr.c_str());
        std::string token;
        
        while ((pos_end = inputStr.find(delimiterStr, pos_start)) != std::string::npos) {
            token = inputStr.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(inputStr.substr(pos_start));
    }
    else {
        m_logger << MESSAGE("Input arguments must be either type of std::string or const char*", base::Logger::Severity::Error);
    }
    return res;
}

template<typename T1, typename T2>
bool
String::SubstringExists(T1 input, T2 substring, size_t& pos) {
    if ((std::is_same<T1, std::string>::value || std::is_same<T1, const char*>::value) &&
        (std::is_same<T2, std::string>::value || std::is_same<T2, const char*>::value)) {
        std::string inputStr(input);
        std::string substringStr(substring);
        pos = inputStr.find(substringStr);
        if (pos != std::string::npos)
            return true;
        else
            return false;
    }
    else {
        m_logger << MESSAGE("Input arguments must be either type of std::string or const char*", base::Logger::Severity::Error);
        return false;
    }
}

template<typename T>
std::string 
String::CombineStrings(T t) {
    try {
        return std::string(t);
    }
    catch (std::exception& e) {
        auto msg = base::String::CombineStrings("Exception thrown: ", e.what());
        m_logger << MESSAGE(msg.c_str(), base::Logger::Severity::Error);
        return msg;
    }
}

template<typename T, typename... Args>
std::string 
String::CombineStrings(T t, Args... args) {
    try {
        std::string result = "";
        result += std::string(t);
        result += base::String::CombineStrings(args...);
        return result;
    }
    catch (std::exception& e) {
        auto msg = base::String::CombineStrings("Exception thrown: ", e.what());
        m_logger << MESSAGE(msg.c_str(), base::Logger::Severity::Error);
        return msg;
    }
}

template <typename T>
std::string 
String::CombineStrings(std::initializer_list<T> list) {
    try {
        std::string result = "";
        for (auto& elem : list) {
            result += std::string(elem);
        }
        return result;
    }
    catch (std::exception& e) {
        auto msg = base::String::CombineStrings("Exception thrown: ", e.what());
        m_logger << MESSAGE(msg.c_str(), base::Logger::Severity::Error);
        return msg;
    }
}

template<typename T>
T
String::ConvertStringToNumerical(const char* input) {
    auto CastToTemplateType = [](auto numeric) {
        return static_cast<T>(numeric);
    };
    try {
        if (std::is_same<T, int>::value)
            return CastToTemplateType(std::stoi(input));
        else if (std::is_same<T, double>::value)
            return CastToTemplateType(std::stod(input));
        else if (std::is_same<T, float>::value)
            return CastToTemplateType(std::stof(input));
        else if (std::is_same<T, long>::value)
            return CastToTemplateType(std::stol(input));
        else if (std::is_same<T, long double>::value)
            return CastToTemplateType(std::stold(input));
        else if (std::is_same<T, long long>::value)
            return CastToTemplateType(std::stoll(input));
        else if (std::is_same<T, unsigned long>::value)
            return CastToTemplateType(std::stoul(input));
        else if (std::is_same<T, unsigned long long>::value)
            return CastToTemplateType(std::stoull(input));
        else
            return std::numeric_limits<T>::min();
    }
    catch (const std::out_of_range& e) {
        auto msg = base::String::CombineStrings("Out of range exception is thrown: ", e.what());
        m_logger << MESSAGE(msg.c_str(), base::Logger::Severity::Error);
        return std::numeric_limits<T>::min();
    }
    catch (const std::invalid_argument& e) {
        auto msg = base::String::CombineStrings("Invalid argument exception is thrown: ", e.what());
        m_logger << MESSAGE(msg.c_str(), base::Logger::Severity::Error);
        return std::numeric_limits<T>::min();
    }
}

template<typename T>
std::string
String::ConvertNumericalToString(const T data, int precision) {
    auto ToStringWithPrecision = []<typename T>(T str, int precision) {
        std::ostringstream out;
        out.precision(precision);
        out << std::fixed << str;
        return out.str();
    };
    try {
        return ToStringWithPrecision(data, precision);
    }
    catch (const std::bad_alloc& e) {
        auto msg = base::String::CombineStrings("Bad allocation exception thrown: ", e.what());
        m_logger << MESSAGE(msg.c_str(), base::Logger::Severity::Error);
        return base::String::ConvertNumericalToString(std::numeric_limits<T>::min());
    }
}

template<typename T>
std::string
String::ToLowerCase(T input) {
    if (std::is_same<T, std::string>::value || std::is_same<T, const char*>::value) {
        std::string result(input);
        std::for_each(result.begin(), result.end(), [](char& c) {
            c = std::tolower(c);
            });
        return result;
    }
    else {
        m_logger << MESSAGE("Input arguments must be either type of std::string or const char*", base::Logger::Severity::Error);
        return "";
    }
}

template<typename T>
std::string
String::ToUpperCase(T input) {
    if (std::is_same<T, std::string>::value || std::is_same<T, const char*>::value) {
        std::string result(input);
        std::for_each(result.begin(), result.end(), [](char& c) {
            c = std::toupper(c);
            });
        return result;
    }
    else {
        m_logger << MESSAGE("Input arguments must be either type of std::string or const char*", base::Logger::Severity::Error);
        return "";
    }
}

template<typename T1, typename T2, typename T3>
std::string
String::ReplaceAll(T1 input, T2 replaceOld, T3 replaceNew) {
    if ((std::is_same<T1, std::string>::value || std::is_same<T1, const char*>::value) &&
        (std::is_same<T2, std::string>::value || std::is_same<T2, const char*>::value) &&
        (std::is_same<T3, std::string>::value || std::is_same<T3, const char*>::value)) {
        
        std::string inputStr(input);
        std::string replaceOldStr(replaceOld);
        std::string replaceNewStr(replaceNew);
        
        std::string rgx = "";
        // check for backslashes
        std::for_each(replaceOldStr.begin(), replaceOldStr.end(), [&rgx](char& c) {
            rgx += c;
            if (c == '\\')
                rgx += "\\";
        });

        std::regex r(rgx.c_str(), rgx.length());
        std::string result;

        regex_replace(back_inserter(result), inputStr.begin(), inputStr.end(), r, replaceNewStr);
        return result;
    }
    else {
        m_logger << MESSAGE("Input arguments must be either type of std::string or const char*", base::Logger::Severity::Error);
        return "";
    }
}

void TempFunc() {
    std::string str1 = "";
    const char* str2 = "";
    size_t pos = 0;

    base::String::SplitString<std::string, std::string>(str1, str1);
    base::String::SplitString<const char*, const char*>(str2, str2);
    base::String::SplitString<std::string, const char*>(str1, str2);
    base::String::SplitString<const char*, std::string>(str2, str1);

    base::String::SubstringExists<std::string, std::string>(str1, str1, pos);
    base::String::SubstringExists<const char*, const char*>(str2, str2, pos);
    base::String::SubstringExists<std::string, const char*>(str1, str2, pos);
    base::String::SubstringExists<const char*, std::string>(str2, str1, pos);

    base::String::ToLowerCase<std::string>(str1);
    base::String::ToLowerCase<const char*>(str2);

    base::String::ToUpperCase<std::string>(str1);
    base::String::ToUpperCase<const char*>(str2);

    base::String::ReplaceAll<std::string, std::string, std::string>(str1, str1, str1);
    base::String::ReplaceAll<std::string, std::string, const char*>(str1, str1, str2);
    base::String::ReplaceAll<std::string, const char*, std::string>(str1, str2, str1);
    base::String::ReplaceAll<std::string, const char*, const char*>(str1, str2, str2);
    base::String::ReplaceAll<const char*, std::string, std::string>(str2, str1, str1);
    base::String::ReplaceAll<const char*, std::string, const char*>(str2, str1, str2);
    base::String::ReplaceAll<const char*, const char*, std::string>(str2, str2, str1);
    base::String::ReplaceAll<const char*, const char*, const char*>(str2, str2, str2);

    base::String::CombineStrings<std::string>(str1);
    base::String::CombineStrings<const char*>(str2);

    base::String::CombineStrings<std::string, std::string>(str1, str1);
    base::String::CombineStrings<std::string, const char*>(str1, str2);
    base::String::CombineStrings<const char*, std::string>(str2, str1);
    base::String::CombineStrings<const char*, const char*>(str2, str2);

    base::String::CombineStrings<std::string, std::string, std::string>(str1, str1, str1);
    base::String::CombineStrings<std::string, std::string, const char*>(str1, str1, str2);
    base::String::CombineStrings<std::string, const char*, std::string>(str1, str2, str1);
    base::String::CombineStrings<std::string, const char*, const char*>(str1, str2, str2);
    base::String::CombineStrings<const char*, std::string, std::string>(str2, str1, str1);
    base::String::CombineStrings<const char*, std::string, const char*>(str2, str1, str2);
    base::String::CombineStrings<const char*, const char*, std::string>(str2, str2, str1);
    base::String::CombineStrings<const char*, const char*, const char*>(str2, str2, str2);

    base::String::CombineStrings<std::string, std::string, std::string, std::string>(str1, str1, str1, str1);
    base::String::CombineStrings<std::string, std::string, std::string, const char*>(str1, str1, str1, str2);
    base::String::CombineStrings<std::string, std::string, const char*, std::string>(str1, str1, str2, str1);
    base::String::CombineStrings<std::string, std::string, const char*, const char*>(str1, str1, str2, str2);
    base::String::CombineStrings<std::string, const char*, std::string, std::string>(str1, str2, str1, str1);
    base::String::CombineStrings<std::string, const char*, std::string, const char*>(str1, str2, str1, str2);
    base::String::CombineStrings<std::string, const char*, const char*, std::string>(str1, str2, str2, str1);
    base::String::CombineStrings<std::string, const char*, const char*, const char*>(str1, str2, str2, str2);
    base::String::CombineStrings<const char*, std::string, std::string, std::string>(str2, str1, str1, str1);
    base::String::CombineStrings<const char*, std::string, std::string, const char*>(str2, str1, str1, str2);
    base::String::CombineStrings<const char*, std::string, const char*, std::string>(str2, str1, str2, str1);
    base::String::CombineStrings<const char*, std::string, const char*, const char*>(str2, str1, str2, str2);
    base::String::CombineStrings<const char*, const char*, std::string, std::string>(str2, str2, str1, str1);
    base::String::CombineStrings<const char*, const char*, std::string, const char*>(str2, str2, str1, str2);
    base::String::CombineStrings<const char*, const char*, const char*, std::string>(str2, str2, str2, str1);
    base::String::CombineStrings<const char*, const char*, const char*, const char*>(str2, str2, str2, str2);

    base::String::CombineStrings<std::string, std::string, std::string, std::string, std::string>(str1, str1, str1, str1, str1);
    base::String::CombineStrings<std::string, std::string, std::string, std::string, const char*>(str1, str1, str1, str1, str2);
    base::String::CombineStrings<std::string, std::string, std::string, const char*, std::string>(str1, str1, str1, str2, str1);
    base::String::CombineStrings<std::string, std::string, std::string, const char*, const char*>(str1, str1, str1, str2, str2);
    base::String::CombineStrings<std::string, std::string, const char*, std::string, std::string>(str1, str1, str2, str1, str1);
    base::String::CombineStrings<std::string, std::string, const char*, std::string, const char*>(str1, str1, str2, str1, str2);
    base::String::CombineStrings<std::string, std::string, const char*, const char*, std::string>(str1, str1, str2, str2, str1);
    base::String::CombineStrings<std::string, std::string, const char*, const char*, const char*>(str1, str1, str2, str2, str2);
    base::String::CombineStrings<std::string, const char*, std::string, std::string, std::string>(str1, str2, str1, str1, str1);
    base::String::CombineStrings<std::string, const char*, std::string, std::string, const char*>(str1, str2, str1, str1, str2);
    base::String::CombineStrings<std::string, const char*, std::string, const char*, std::string>(str1, str2, str1, str2, str1);
    base::String::CombineStrings<std::string, const char*, std::string, const char*, const char*>(str1, str2, str1, str2, str2);
    base::String::CombineStrings<std::string, const char*, const char*, std::string, std::string>(str1, str2, str2, str1, str1);
    base::String::CombineStrings<std::string, const char*, const char*, std::string, const char*>(str1, str2, str2, str1, str2);
    base::String::CombineStrings<std::string, const char*, const char*, const char*, std::string>(str1, str2, str2, str2, str1);
    base::String::CombineStrings<std::string, const char*, const char*, const char*, const char*>(str1, str2, str2, str2, str2);
    base::String::CombineStrings<const char*, std::string, std::string, std::string, std::string>(str2, str1, str1, str1, str1);
    base::String::CombineStrings<const char*, std::string, std::string, std::string, const char*>(str2, str1, str1, str1, str2);
    base::String::CombineStrings<const char*, std::string, std::string, const char*, std::string>(str2, str1, str1, str2, str1);
    base::String::CombineStrings<const char*, std::string, std::string, const char*, const char*>(str2, str1, str1, str2, str2);
    base::String::CombineStrings<const char*, std::string, const char*, std::string, std::string>(str2, str1, str2, str1, str1);
    base::String::CombineStrings<const char*, std::string, const char*, std::string, const char*>(str2, str1, str2, str1, str2);
    base::String::CombineStrings<const char*, std::string, const char*, const char*, std::string>(str2, str1, str2, str2, str1);
    base::String::CombineStrings<const char*, std::string, const char*, const char*, const char*>(str2, str1, str2, str2, str2);
    base::String::CombineStrings<const char*, const char*, std::string, std::string, std::string>(str2, str2, str1, str1, str1);
    base::String::CombineStrings<const char*, const char*, std::string, std::string, const char*>(str2, str2, str1, str1, str2);
    base::String::CombineStrings<const char*, const char*, std::string, const char*, std::string>(str2, str2, str1, str2, str1);
    base::String::CombineStrings<const char*, const char*, std::string, const char*, const char*>(str2, str2, str1, str2, str2);
    base::String::CombineStrings<const char*, const char*, const char*, std::string, std::string>(str2, str2, str2, str1, str1);
    base::String::CombineStrings<const char*, const char*, const char*, std::string, const char*>(str2, str2, str2, str1, str2);
    base::String::CombineStrings<const char*, const char*, const char*, const char*, std::string>(str2, str2, str2, str2, str1);
    base::String::CombineStrings<const char*, const char*, const char*, const char*, const char*>(str2, str2, str2, str2, str2);

    base::String::CombineStrings<std::string, std::string, std::string, std::string, std::string, std::string>(str1, str1, str1, str1, str1, str1);
    base::String::CombineStrings<std::string, std::string, std::string, std::string, std::string, const char*>(str1, str1, str1, str1, str1, str2);
    base::String::CombineStrings<std::string, std::string, std::string, std::string, const char*, std::string>(str1, str1, str1, str1, str2, str1);
    base::String::CombineStrings<std::string, std::string, std::string, std::string, const char*, const char*>(str1, str1, str1, str1, str2, str2);
    base::String::CombineStrings<std::string, std::string, std::string, const char*, std::string, std::string>(str1, str1, str1, str2, str1, str1);
    base::String::CombineStrings<std::string, std::string, std::string, const char*, std::string, const char*>(str1, str1, str1, str2, str1, str2);
    base::String::CombineStrings<std::string, std::string, std::string, const char*, const char*, std::string>(str1, str1, str1, str2, str2, str1);
    base::String::CombineStrings<std::string, std::string, std::string, const char*, const char*, const char*>(str1, str1, str1, str2, str2, str2);
    base::String::CombineStrings<std::string, std::string, const char*, std::string, std::string, std::string>(str1, str1, str2, str1, str1, str1);
    base::String::CombineStrings<std::string, std::string, const char*, std::string, std::string, const char*>(str1, str1, str2, str1, str1, str2);
    base::String::CombineStrings<std::string, std::string, const char*, std::string, const char*, std::string>(str1, str1, str2, str1, str2, str1);
    base::String::CombineStrings<std::string, std::string, const char*, std::string, const char*, const char*>(str1, str1, str2, str1, str2, str2);
    base::String::CombineStrings<std::string, std::string, const char*, const char*, std::string, std::string>(str1, str1, str2, str2, str1, str1);
    base::String::CombineStrings<std::string, std::string, const char*, const char*, std::string, const char*>(str1, str1, str2, str2, str1, str2);
    base::String::CombineStrings<std::string, std::string, const char*, const char*, const char*, std::string>(str1, str1, str2, str2, str2, str1);
    base::String::CombineStrings<std::string, std::string, const char*, const char*, const char*, const char*>(str1, str1, str2, str2, str2, str2);
    base::String::CombineStrings<std::string, const char*, std::string, std::string, std::string, std::string>(str1, str2, str1, str1, str1, str1);
    base::String::CombineStrings<std::string, const char*, std::string, std::string, std::string, const char*>(str1, str2, str1, str1, str1, str2);
    base::String::CombineStrings<std::string, const char*, std::string, std::string, const char*, std::string>(str1, str2, str1, str1, str2, str1);
    base::String::CombineStrings<std::string, const char*, std::string, std::string, const char*, const char*>(str1, str2, str1, str1, str2, str2);
    base::String::CombineStrings<std::string, const char*, std::string, const char*, std::string, std::string>(str1, str2, str1, str2, str1, str1);
    base::String::CombineStrings<std::string, const char*, std::string, const char*, std::string, const char*>(str1, str2, str1, str2, str1, str2);
    base::String::CombineStrings<std::string, const char*, std::string, const char*, const char*, std::string>(str1, str2, str1, str2, str2, str1);
    base::String::CombineStrings<std::string, const char*, std::string, const char*, const char*, const char*>(str1, str2, str1, str2, str2, str2);
    base::String::CombineStrings<std::string, const char*, const char*, std::string, std::string, std::string>(str1, str2, str2, str1, str1, str1);
    base::String::CombineStrings<std::string, const char*, const char*, std::string, std::string, const char*>(str1, str2, str2, str1, str1, str2);
    base::String::CombineStrings<std::string, const char*, const char*, std::string, const char*, std::string>(str1, str2, str2, str1, str2, str1);
    base::String::CombineStrings<std::string, const char*, const char*, std::string, const char*, const char*>(str1, str2, str2, str1, str2, str2);
    base::String::CombineStrings<std::string, const char*, const char*, const char*, std::string, std::string>(str1, str2, str2, str2, str1, str1);
    base::String::CombineStrings<std::string, const char*, const char*, const char*, std::string, const char*>(str1, str2, str2, str2, str1, str2);
    base::String::CombineStrings<std::string, const char*, const char*, const char*, const char*, std::string>(str1, str2, str2, str2, str2, str1);
    base::String::CombineStrings<std::string, const char*, const char*, const char*, const char*, const char*>(str1, str2, str2, str2, str2, str2);
    base::String::CombineStrings<const char*, std::string, std::string, std::string, std::string, std::string>(str2, str1, str1, str1, str1, str1);
    base::String::CombineStrings<const char*, std::string, std::string, std::string, std::string, const char*>(str2, str1, str1, str1, str1, str2);
    base::String::CombineStrings<const char*, std::string, std::string, std::string, const char*, std::string>(str2, str1, str1, str1, str2, str1);
    base::String::CombineStrings<const char*, std::string, std::string, std::string, const char*, const char*>(str2, str1, str1, str1, str2, str2);
    base::String::CombineStrings<const char*, std::string, std::string, const char*, std::string, std::string>(str2, str1, str1, str2, str1, str1);
    base::String::CombineStrings<const char*, std::string, std::string, const char*, std::string, const char*>(str2, str1, str1, str2, str1, str2);
    base::String::CombineStrings<const char*, std::string, std::string, const char*, const char*, std::string>(str2, str1, str1, str2, str2, str1);
    base::String::CombineStrings<const char*, std::string, std::string, const char*, const char*, const char*>(str2, str1, str1, str2, str2, str2);
    base::String::CombineStrings<const char*, std::string, const char*, std::string, std::string, std::string>(str2, str1, str2, str1, str1, str1);
    base::String::CombineStrings<const char*, std::string, const char*, std::string, std::string, const char*>(str2, str1, str2, str1, str1, str2);
    base::String::CombineStrings<const char*, std::string, const char*, std::string, const char*, std::string>(str2, str1, str2, str1, str2, str1);
    base::String::CombineStrings<const char*, std::string, const char*, std::string, const char*, const char*>(str2, str1, str2, str1, str2, str2);
    base::String::CombineStrings<const char*, std::string, const char*, const char*, std::string, std::string>(str2, str1, str2, str2, str1, str1);
    base::String::CombineStrings<const char*, std::string, const char*, const char*, std::string, const char*>(str2, str1, str2, str2, str1, str2);
    base::String::CombineStrings<const char*, std::string, const char*, const char*, const char*, std::string>(str2, str1, str2, str2, str2, str1);
    base::String::CombineStrings<const char*, std::string, const char*, const char*, const char*, const char*>(str2, str1, str2, str2, str2, str2);
    base::String::CombineStrings<const char*, const char*, std::string, std::string, std::string, std::string>(str2, str2, str1, str1, str1, str1);
    base::String::CombineStrings<const char*, const char*, std::string, std::string, std::string, const char*>(str2, str2, str1, str1, str1, str2);
    base::String::CombineStrings<const char*, const char*, std::string, std::string, const char*, std::string>(str2, str2, str1, str1, str2, str1);
    base::String::CombineStrings<const char*, const char*, std::string, std::string, const char*, const char*>(str2, str2, str1, str1, str2, str2);
    base::String::CombineStrings<const char*, const char*, std::string, const char*, std::string, std::string>(str2, str2, str1, str2, str1, str1);
    base::String::CombineStrings<const char*, const char*, std::string, const char*, std::string, const char*>(str2, str2, str1, str2, str1, str2);
    base::String::CombineStrings<const char*, const char*, std::string, const char*, const char*, std::string>(str2, str2, str1, str2, str2, str1);
    base::String::CombineStrings<const char*, const char*, std::string, const char*, const char*, const char*>(str2, str2, str1, str2, str2, str2);
    base::String::CombineStrings<const char*, const char*, const char*, std::string, std::string, std::string>(str2, str2, str2, str1, str1, str1);
    base::String::CombineStrings<const char*, const char*, const char*, std::string, std::string, const char*>(str2, str2, str2, str1, str1, str2);
    base::String::CombineStrings<const char*, const char*, const char*, std::string, const char*, std::string>(str2, str2, str2, str1, str2, str1);
    base::String::CombineStrings<const char*, const char*, const char*, std::string, const char*, const char*>(str2, str2, str2, str1, str2, str2);
    base::String::CombineStrings<const char*, const char*, const char*, const char*, std::string, std::string>(str2, str2, str2, str2, str1, str1);
    base::String::CombineStrings<const char*, const char*, const char*, const char*, std::string, const char*>(str2, str2, str2, str2, str1, str2);
    base::String::CombineStrings<const char*, const char*, const char*, const char*, const char*, std::string>(str2, str2, str2, str2, str2, str1);
    base::String::CombineStrings<const char*, const char*, const char*, const char*, const char*, const char*>(str2, str2, str2, str2, str2, str2);

    base::String::CombineStrings<std::string>({ str1 });
    base::String::CombineStrings<std::string>({ str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1, str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1, str1, str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1, str1, str1, str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1, str1, str1, str1, str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1, str1, str1, str1, str1, str1, str1 });
    base::String::CombineStrings<std::string>({ str1, str1, str1, str1, str1, str1, str1, str1, str1, str1 });

    base::String::CombineStrings<const char*>({ str2 });
    base::String::CombineStrings<const char*>({ str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2, str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2, str2, str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2, str2, str2, str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2, str2, str2, str2, str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2, str2, str2, str2, str2, str2, str2 });
    base::String::CombineStrings<const char*>({ str2, str2, str2, str2, str2, str2, str2, str2, str2, str2 });

    base::String::ConvertStringToNumerical<int>(str2);
    base::String::ConvertStringToNumerical<double>(str2);
    base::String::ConvertStringToNumerical<float>(str2);
    base::String::ConvertStringToNumerical<long>(str2);
    base::String::ConvertStringToNumerical<long double>(str2);
    base::String::ConvertStringToNumerical<long long>(str2);
    base::String::ConvertStringToNumerical<unsigned long>(str2);
    base::String::ConvertStringToNumerical<unsigned long long>(str2);

    int n1 = 0;
    double n2 = 0.0;
    float n3 = 0.0f;
    long n4 = 0;
    long double n5 = 0.0;
    long long n6 = 0;
    unsigned long n7 = 0;
    unsigned long long n8 = 0;

    base::String::ConvertNumericalToString<int>(n1);
    base::String::ConvertNumericalToString<double>(n2);
    base::String::ConvertNumericalToString<float>(n3);
    base::String::ConvertNumericalToString<long>(n4);
    base::String::ConvertNumericalToString<long double>(n5);
    base::String::ConvertNumericalToString<long long>(n6);
    base::String::ConvertNumericalToString<unsigned long>(n7);
    base::String::ConvertNumericalToString<unsigned long long>(n8);
}

}