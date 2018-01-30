/*
 * MyLogger.h
 *
 *  Created on: 28.11.2015
 *      Author: dehnhardt
 */

#ifndef SRC_LOGGING_MYLOGGER_H_
#define SRC_LOGGING_MYLOGGER_H_

#include <logging/FLLogger.h>
#include <logging/FLLog.h>

namespace flaarlib {


class MyLogger: public FLLogger {
public:
	MyLogger();
	virtual ~MyLogger();
	void debug( const std::string & message );
	void error(const std::string & message );
	void info(const std::string & message );
	void log(const std::string & message );
};

} /* namespace flaarlib */

#endif /* SRC_LOGGING_MYLOGGER_H_ */
