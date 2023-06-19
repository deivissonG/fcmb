//
// Created by deivisson on 18/06/23.
//

#include "Bozorth.h"
#include "Utils.h"
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

Bozorth::Bozorth(const std::string& probe, const std::string& gallery)
{
    m_Command << "exec" << SEPARATOR << "bozorth3 " << probe << ".xyt " << gallery << ".xyt";
}

Bozorth::~Bozorth()
{
}

int Bozorth::Execute()
{
    LOG("Executing Bozorth")
    std::array<char, 128> buffer;
    std::string result;
    char *cmd = new char[m_Command.str().length() + 1];
    strcpy(cmd, m_Command.str().c_str());
    std::unique_ptr<FILE, decltype(&pclose) > pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    std::cout << result << std::endl;
//    int result = system(m_Command.str().c_str());
//    if (result == 0) {
//        std::cout << "Failed to execute Bozorth!" << std::endl;
//    } else {
        LOG("Done!")
//    }
    char *c_result = new char[m_Name.length() + 1];
    strcpy(c_result, result.c_str());
    return atoi(c_result);
}

