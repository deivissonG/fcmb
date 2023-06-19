//
// Created by deivisson on 18/06/23.
//

#ifndef __Fcmb_Bozorth__
#define __Fcmb_Bozorth__

#include <iostream>
#include <sstream>

class Bozorth
{
public:
    Bozorth(const std::string& probe, const std::string& gallery);
    Bozorth(const Bozorth& bozorth) = delete;
    Bozorth& operator=(const Bozorth& bozorth) = delete;
    ~Bozorth();

    int Execute();
private:
    std::ostringstream m_Command;
    std::string m_Name;
};
#endif /* defined(__Fcmb_Bozorth__) */
