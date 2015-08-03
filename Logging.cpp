#include <iostream>
#include <ostream>
#include <fstream>
#include "Logging.h"
#include <ctime>

const char* Logger::STDOUT = "stdout";
std::ofstream Logger::NullOStream;

Logger::Logger(const std::string &s, enum LogLevel level):
    m_level(level)
{
    if (s != STDOUT)
        m_OStream.reset(new std::ofstream(s.c_str(), std::ofstream::app));
}


#define LOG_METHOD(shortname, name) \
    std::ostream& Logger::shortname() \
    { \
        if (GetInstance().m_level < LOGLEVEL_ ## name) \
            return NullOStream; \
        char t[100]; \
        time_t now = time(nullptr); \
        tm* ltm = localtime(&now); \
        strftime(t, 100, "%d/%m/%Y %T", std::localtime(&now)); \
        std::string time_str(t); \
        try { \
            return GetOutStream() << #name ":" <<"("<<time_str<<") "; \
        } catch (...) { return GetOutStream(); } \
    }

LOG_METHOD(D, DEBUG)
LOG_METHOD(L, LOG)
LOG_METHOD(I, INFO)
LOG_METHOD(N, NOTICE)
LOG_METHOD(W, WARNING)
LOG_METHOD(C, CONDITION_DEBUG)

#undef LOG_METHOD

std::ostream& Logger::E()
{
    if (GetInstance().m_level < LOGLEVEL_ERROR)
        return NullOStream;
    return GetErrStream()<<"ERROR:";
}

std::ostream& Logger::GetOutStream()
{
    if (GetInstance().m_OStream.get())
        return *GetInstance().m_OStream.get();
    return std::cout;
}

std::ostream& Logger::GetErrStream()
{
    if (GetInstance().m_OStream.get())
        return *GetInstance().m_OStream.get();
    return std::cerr;
}

void Logger::ResetInstanceToStdout(enum LogLevel level)
{
    if (m_pInstance)
        delete m_pInstance;
    m_pInstance = new Logger(STDOUT, level);
}

void Logger::ResetInstance(const std::string& logfilename, enum LogLevel level)
{
    if (m_pInstance)
        delete m_pInstance;
    m_pInstance = new Logger(logfilename, level);
    LOG_N << "Log reseted to " << logfilename << std::endl;
}

void Logger::PY(const char* pylog, bool isErr)
{
    static bool pyNewLine = true;
    std::ostream& out = isErr ? GetErrStream() : GetOutStream();
    const size_t pLen = 2000;
    char p[pLen];
    size_t pos = 0;
    size_t startPos = 0;
    while (pylog[pos] != '\0' && pos < pLen)
    {
        if (pyNewLine)
        {
            if (isErr)
                out << "PYERR: ";
            else
                out << "PY: ";
            pyNewLine = false;
        }
        if (pylog[pos++] == '\n' || (pos-startPos) >= (pLen - 1))
        {
            strcpy(p, pylog+startPos);
            p[pos-startPos] = '\0';

            out << p;

            pyNewLine = true;
            startPos = pos;
        }
    }
    if (startPos != pos)
    {
        strcpy(p, pylog+startPos);
        p[pos+1] = '\0';

        out << p;
    }
    out.flush();
}
