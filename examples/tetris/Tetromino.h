//
// Created by Florian on 24.02.2021.
//

#pragma once

#include <string>

class Tetromino {
public:
    static const std::string& getIString();
    static const std::string& getJString();
    static const std::string& getLString();
    static const std::string& getOString();
    static const std::string& getSString();
    static const std::string& getTString();
    static const std::string& getZString();

    static const std::string& getIndexString(const int& index);
};
