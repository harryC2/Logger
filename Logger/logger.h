#pragma once
#include<string>
#include<fstream>


namespace yazi{
namespace logger
{
#define debug(format, ...) \
Logger::GetInstace()->log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define info(format, ...) \
Logger::GetInstace()->log(Logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define warning(format, ...) \
Logger::GetInstace()->log(Logger::WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define error(format, ...) \
Logger::GetInstace()->log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define fatal(format, ...) \
Logger::GetInstace()->log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

    class Logger
    {
    public:
        // 定义日志的级别】
        enum Level
        {
            DEBUG = 0,
            INFO,
            WARNING,
            ERROR,
            FATAL,
            LEVEL_COUNT
        };
        void log(Level level, const char *fileName, int line, const char *format, ...);
        void open(const std::string &fileName);
        void close(const std::string &fileName);
        static Logger *GetInstace()
        {
            static Logger logger;
            return &logger;
        }
        void SetLogLevel(Level level)
        {
            m_level = level;
        }

    private:
        unsigned int m_size{0};
        unsigned int  m_len{0};
        std::string m_fileName;                  // 文件名
        std::ofstream m_fout;                    // 文件流输出
        static const char *s_level[LEVEL_COUNT]; // 源文件定义  头文件声明静态变量
        Level m_level{Level::DEBUG};
    private:
        void rotate();
        Logger(/* args */);
        Logger(Logger &other) = delete;
        ~Logger();
    };

}
}