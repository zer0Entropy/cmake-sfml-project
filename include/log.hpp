#pragma once

#include <fstream>
#include <string>
#include <vector>

struct LogMessage {
    std::string     header;
    std::string     body;
    bool            errorFlag;
};

class LogMgr {
public:
    LogMgr() = delete;
    LogMgr(std::string path);
    LogMgr(const LogMgr& copy) = delete;
    LogMgr(LogMgr& copy) = delete;
    LogMgr(LogMgr&& move) = delete;
    ~LogMgr();

    enum class ErrorCode {
        Success,
        ReadFailure,
        WriteFailure
    };

    ErrorCode                   LoadHistory();
    ErrorCode                   InitOutput();

    void                        CreateMessage(std::string_view header, std::string_view body, bool errorFlag = false);
    void                        WriteMessage(const LogMessage& message);
private:
    std::string                 path;
    std::ofstream               outputFile;
    std::vector<LogMessage>     history;
};