#pragma once

#include <vector>
#include <string>
#include <initializer_list>
#include <memory>

namespace base {

class Logger;

class String {
public:

    template<typename T1, typename T2>
    static std::vector<std::string> 
    SplitString(T1 input, T2 delimiter);
    
    template<typename T1, typename T2>
    static bool
    SubstringExists(T1 input, T2 substring, size_t& pos);

    template<typename T>
    static std::string
    CombineStrings(T t);
    
    template<typename T, typename... Args>
    static std::string 
    CombineStrings(T t, Args... args);

    template <typename T>
    static std::string
    CombineStrings(std::initializer_list<T> list);

    template<typename T>
    static T
    ConvertStringToNumerical(const char* input);

    template<typename T>
    static std::string
    ConvertNumericalToString(const T data, int precision = 2);

    template<typename T>
    static std::string
    ToLowerCase(T input);

    template<typename T>
    static std::string
    ToUpperCase(T input);

    template<typename T1, typename T2, typename T3>
    static std::string
    ReplaceAll(T1 input, T2 replaceOld, T3 replaceNew);

private:
    static std::shared_ptr<base::Logger> m_logger;
};

} // namespace base