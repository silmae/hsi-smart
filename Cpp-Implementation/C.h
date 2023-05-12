#ifndef C_H
#define C_H

#include <iostream>
#include <string>

class C {
public:
    static const std::string RE;
    static const std::string BL;
    static const std::string R;
    static const std::string G;
    static const std::string Y;
    static const std::string B;
    static const std::string M;
    static const std::string CY;
    
    static void print(const std::string& color, const std::string& text);
};

#endif

