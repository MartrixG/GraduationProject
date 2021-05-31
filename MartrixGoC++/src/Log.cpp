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

inline std::string MiniLog::getTimeStamp()
{
    std::stringstream ss;
    auto timeStamp = std::chrono::system_clock::now().time_since_epoch();
    auto millisecond = std::chrono::duration_cast<std::chrono::milliseconds>(timeStamp).count();
    std::time_t t(std::chrono::duration_cast<std::chrono::seconds>(timeStamp).count());
    ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
    ss << "," << std::setw(3) << std::setfill('0') << millisecond % 1000;
    return ss.str();
}
