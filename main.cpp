#include <iostream>
using namespace std;
#include "Logger\logger.h"

using namespace yazi::logger;

int main()
{
    Logger::GetInstace()->open("D:\\chStudly\\vsCodeProject\\Logger\\test.log");
    Logger::GetInstace()->SetLogLevel(Logger::Level::INFO);
    Logger::GetInstace()->log(Logger::DEBUG,__FILE__,__LINE__,"hello wrold");
    Logger::GetInstace()->log(Logger::DEBUG,__FILE__,__LINE__,"tips:请求日志 type:%d parm1:%s parm2:%s",2018,"requestName","""lili");
    debug("tips:请求日志 type:%d parm1:%s parm2:%s",2018,"requestName","""lili");
    info("tips:请求日志 type:%d parm1:%s parm2:%s",2018,"requestName","""lili");
    std::cout<< "hello world"<<std::endl;
    return 0;
}