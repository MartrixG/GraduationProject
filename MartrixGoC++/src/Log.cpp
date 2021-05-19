//
// Created by 11409 on 2021/5/19.
//

#include "Log.hpp"

bool MiniLog::init(const std::string &fileName, bool debug)
{
    this->isDebug = debug;
    this->logFileName = fileName;
    logger.open(this->logFileName, std::ios::out);
    if(!logger.is_open())
    {
        return false;
    }
    logger << "=====================================================" << std::endl;
    logger << "Log start." << std::endl;
    logger << "=====================================================" << std::endl;
    return true;
}

void MiniLog::debug(const std::string &msg)
{
    if(this->isDebug)
    {
        logger << MiniLog::getTimeStamp() << " [DEBUG] " << msg << std::endl;
    }
}

void MiniLog::info(const std::string &msg)
{
    logger << MiniLog::getTimeStamp() << " [INFO] " << msg << std::endl;
}

void MiniLog::warning(const std::string &msg)
{
    logger << MiniLog::getTimeStamp() << " [WARNING] " << msg << std::endl;
}

void MiniLog::fatal(const std::string &msg)
{
    logger << MiniLog::getTimeStamp() << " [FATAL] " << msg << std::endl;
}

inline const std::string & MiniLog::getTimeStamp()
{
    static std::string timeStamp;
    time_t tt = time(nullptr);
    struct tm *stm = localtime(&tt);

    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    int ms = tv.tv_usec / 1000;

    char tmp[27];
    sprintf(tmp, "[%04d-%02d-%02d] [%02d:%02d:%02d,%03d]", 1900 + stm->tm_year, 1 + stm->tm_mon, stm->tm_mday, stm->tm_hour,
            stm->tm_min, stm->tm_sec, ms);

    timeStamp = std::string(tmp);
    return timeStamp;
}
