//
// Created by 11409 on 2021/5/19.
//

#ifndef MARTRIXGOC_LOG_HPP
#define MARTRIXGOC_LOG_HPP

#include <fstream>
#include <cstring>
#include <ctime>
#include <sys/time.h>

class MiniLog
{
private:
    std::string logFileName;
    std::ofstream logger;
    bool isDebug = false;

    static const std::string& getTimeStamp();

public:
    MiniLog() = default;

    bool init(const std::string &fileName, bool debug);

    void debug(const std::string &msg);

    void info(const std::string &msg);

    void warning(const std::string &msg);

    void fatal(const std::string &msg);
};
#endif //MARTRIXGOC_LOG_HPP
