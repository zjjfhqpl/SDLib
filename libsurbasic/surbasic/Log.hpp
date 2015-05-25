
#ifndef LOG_H
#define LOG_H
#include <string>

namespace SDBasic{
	namespace Log{
	
		class LogScope
		{
		public:
			typedef enum _LogLevel
			{
				LOG_DEBUG = 0,	//����
				LOG_NORMAL,		//һ��
				LOG_MEDIUM,		//��
				LOG_HIGH		//��
			} ELogLevel;

			LogScope(ELogLevel level,const std::string & file,const std::string & line,const std::string & fun);
			~LogScope();
		private:
			std::string fun_;
			std::string line_;
			std::string file_;
		};
		inline LogScope::LogScope(ELogLevel level,const std::string & file,const std::string & line,const std::string & fun)
		{
			
		}
		inline LogScope::~LogScope()
		{

		}
	}
}
#endif
