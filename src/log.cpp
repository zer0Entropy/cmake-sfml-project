#include "../include/log.hpp"

LogMgr::LogMgr(std::string path):
    path{path} {
    LogMgr::ErrorCode inputOutcome{LoadHistory()};
    LogMgr::ErrorCode outputOutcome{InitOutput()};
}

LogMgr::~LogMgr() {

}

LogMgr::ErrorCode LogMgr::LoadHistory() {
    ErrorCode       outcome{ErrorCode::Success};
    std::ifstream   inputFile{path};
    if(!inputFile.good()) {
        outcome = ErrorCode::ReadFailure;
    }
    else {
        while(!inputFile.eof()) {
            std::string     line;
            std::getline(inputFile, line);
            LogMessage      message;
            if(line.compare("ERROR") == 0) {
                message.errorFlag = true;
            }
            else {
                message.errorFlag = false;
            }
            std::getline(inputFile, line);
            message.header = line;
            std::getline(inputFile, line);
            message.body = line;
            history.push_back(message);
        }
    }
    if(inputFile.is_open()) {
        inputFile.close();
    }
    return outcome;
}

LogMgr::ErrorCode LogMgr::InitOutput() {
    ErrorCode       outcome{ErrorCode::Success};
    outputFile.open(path, std::ios_base::app);
    if(!outputFile.good()) {
        outcome = ErrorCode::WriteFailure;
    }
    return outcome;
}

void LogMgr::CreateMessage(std::string_view header, std::string_view body, bool errorFlag) {
    LogMessage      message{(std::string)header, (std::string)body, errorFlag};
    history.push_back(message);
    WriteMessage(message);
}

void LogMgr::WriteMessage(const LogMessage& message) {
    std::string msgType{};
    if(message.errorFlag) {
        msgType = "[ERROR]";
    }
    else {
        msgType = "[INFO]";
    }
    outputFile.write(msgType.c_str(), msgType.length());
    outputFile.write(" ", 1);
    outputFile.write(message.header.c_str(), message.header.length());
    outputFile.write("\n", 1);
    outputFile.write(message.body.c_str(), message.body.length());
    if(message.body.length() > 0) {
        outputFile.write("\n", 1);
    }
    outputFile.write("\n", 1);
}