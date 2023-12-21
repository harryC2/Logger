 #include "logger.h"
#include <time.h>
#include<stdexcept>
#include<string.h>
#include<iostream>
#include "stdarg.h"
using namespace std;

using namespace yazi::logger;

const char* Logger::s_level[LEVEL_COUNT] = 
{
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "FATAL"
};
void Logger::log(Level level, const char *fileName, int line, const char *format, ...)
{
    if(m_level > level)
    {
        return;
    }
    if(m_fout.fail())
    {
       throw std::logic_error("open file failed" + m_fileName);
    }
    time_t ticks = time(NULL);
    struct tm * ptn = localtime(&ticks);
    char timeStamp[32];//4的倍数 内存对齐； 19-32
    memset(timeStamp,0,sizeof(timeStamp));
    strftime(timeStamp,sizeof(timeStamp),"%Y-%m-%d %H:%M:%S",ptn); // 将时间错转换成  年月日字符串保存到数组中
    //写入时间
    int len = 0;
    const char * fmt = "%s %s %s:%d ";
    len = snprintf(NULL, 0, fmt, timeStamp, s_level[level], fileName, line);
    if (len > 0)
    {
        char * buffer = new char[len + 1];
        snprintf(buffer, len + 1, fmt, timeStamp, s_level[level], fileName, line);
        m_len += len;
        buffer[len] = 0;
        m_fout << buffer;
        delete buffer;
       // m_len += len;
    }

    va_list arg_ptr;
    va_start(arg_ptr, format);
    len = vsnprintf(NULL, 0, format, arg_ptr);
    va_end(arg_ptr);
    if(len>0)
    {
        va_list arg_ptr;
        va_start(arg_ptr,format);
        //char * pStr = new char[len+1]; //大的话new 不大的话不是直接对战就好了
        char pStr[len+1];
        memset(pStr,0,sizeof(pStr));
        vsnprintf(pStr,len+1,format,arg_ptr);
         va_end(arg_ptr);
        m_len += len;
        pStr[len] =0;
        m_fout<<pStr;
       // delete pStr;
    }
      m_fout<<"\r";
      m_fout.flush();
      if(m_size != 0 && m_size < m_len)
      {
        rotate();
      }

}

void Logger::open(const std::string &fileName)
{
    //注意线程安全 操作m_fout的时候 只能由一个文件
    m_fileName = fileName;
    m_fout.open(fileName, std::ios::app);
    if (!m_fout.is_open())
    {
        throw std::logic_error("open file failed" + fileName);
    }

    //把文件的写指针移到文件结尾并往后移动0个单位
    m_fout.seekp(0, ios::end);
    /*tellg() 和tellp()
      这两个成员函数不用传入参数，返回pos_type 类型的值(根据ANSI-C++ 标准) ，
      就是一个整数，代表当前get 流 指针的位置(用tellg) 或put 流指针的位置(用
      tellp).而且不要对tellg 或tellp 的返回值进行修改。
    */
    m_len = m_fout.tellp();
}
void Logger::rotate()
{
    close(m_fileName);
    //翻页，就是另起一个文件保存当前日志

    // 另一个名字后面加一个当前时间错
    time_t ticks = time(NULL);
    struct tm * ptn = localtime(&ticks);
    char timeStamp[32];//4的倍数 内存对齐； 19-32
    memset(timeStamp,0,sizeof(timeStamp));
    strftime(timeStamp,sizeof(timeStamp),"%Y-%m-%d %H:%M:%S",ptn);
    string strNewName = m_fileName + timeStamp;
     if (rename(m_fileName.c_str(), strNewName.c_str()) != 0)
    {
        throw std::logic_error("rename log file failed: " + string(strerror(errno)));
    }
    open(m_fileName);
}
void Logger::close(const std::string &fileName)
{
    //注意线程安全
    m_fout.close();
}

Logger::Logger(/* args */)
{
}

 Logger::~Logger()
 {
 }