#ifndef __LOG_H__
#define __LOG_H__

// Adapted from Dr Dobb's article "Logging In C++" - Petru Marginean, September 05, 2007

#include <sstream>
#include <string>
#include <stdio.h>
#include <sys/time.h>

inline std::string GetCurrentDateAndTime();

enum TLogLevel 
{
	ERROR, 
	WARNING, 
	INFO, 
	DEBUG
};

//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
template <typename T>
class CLog
{
protected:
    std::ostringstream os;

public:
    CLog();
    virtual ~CLog();
    std::ostringstream& Get(TLogLevel level = INFO);

    static TLogLevel& ReportingLevel();
    static std::string ToString(TLogLevel level);
    static TLogLevel FromString(const std::string& level);   
    
private:
    CLog(const CLog&);
    CLog& operator =(const CLog&);
};

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
template <typename T>
CLog<T>::CLog()
{
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
template <typename T>
std::ostringstream& CLog<T>::Get(TLogLevel level)
{
    os  << GetCurrentDateAndTime() << " - " <<  ToString(level) << " - ";

    return os;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
template <typename T>
CLog<T>::~CLog()
{
    os << std::endl;
    T::Output(os.str());
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
template <typename T>
TLogLevel& CLog<T>::ReportingLevel()
{
    static TLogLevel reportingLevel = DEBUG;
    return reportingLevel;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
template <typename T>
std::string CLog<T>::ToString(TLogLevel level)
{
	static const char* const buffer[] = 
	{
		"[ERROR]", 
		"[WARN ]", 
		"[INFO ]", 
		"[DEBUG]"
	};
	return buffer[level];
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
template <typename T>
TLogLevel CLog<T>::FromString(const std::string& level)
{
    if (level == "[DEBUG]")
        return DEBUG;
    if (level == "[INFO ]")
        return INFO;
    if (level == "[WARN ]")
        return WARNING;
    if (level == "[ERROR]")
        return ERROR;
        
    CLog<T>().Get(WARNING) << "Unknown logging level '" << level << "'. Using INFO level as default.";
    
    return INFO;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************

class COutputToFile
{
public:
    static FILE*& Stream();
    static void Output(const std::string& msg);
};

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
inline FILE*& COutputToFile::Stream()
{
    static FILE* pStream = stderr;
    return pStream;
}

//**********************************************************************
//
//
//
//----------------------------------------------------------------------
inline void COutputToFile::Output(const std::string& msg)
{   
    FILE* pStream = Stream();
    if (!pStream)
    {
        return;
	}
    fprintf(pStream, "%s", msg.c_str());
    fflush(pStream);
}

#define FILELOG_DECLSPEC


//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************

class FILELOG_DECLSPEC CFileLog : public CLog<COutputToFile> {};

//typedef Log<OutputToFile> FILELog;

#ifndef FILELOG_MAX_LEVEL
#define FILELOG_MAX_LEVEL DEBUG
#endif

#define LOG(level) \
    if (level > FILELOG_MAX_LEVEL) ;\
    else if (level > CFileLog::ReportingLevel() || !COutputToFile::Stream()) ; \
    else CFileLog().Get(level)


//**********************************************************************
//
//
//
//----------------------------------------------------------------------
inline std::string GetCurrentDateAndTime() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    return buf;
}

#endif //__LOG_H__
