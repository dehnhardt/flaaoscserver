/*
 * MyLogger.cpp
 *
 *  Created on: 28.11.2015
 *      Author: dehnhardt
 */

#include "MyLogger.h"
#include "../spdlog/spdlog.h"

#include <iostream>

namespace flaarlib
{

MyLogger::~MyLogger()
{
	// TODO Auto-generated destructor stub
}

MyLogger::MyLogger()
{
	try
	{
		spdlog::set_async_mode(1048576); //queue size must be power of 2
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_st>());
		sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_st>("/tmp/FlaarLib", true));
		auto logger = std::make_shared<spdlog::logger>("Flaarlib", begin(sinks), end(sinks));
		spdlog::register_logger(logger);
		spdlog::set_level(spdlog::level::debug);
	}
	catch (const spdlog::spdlog_ex &ex)
	{
		std::cout << "Log failed: " << ex.what() << std::endl;
	}

}

void MyLogger::debug( const std::string &message )
{
	auto l = spdlog::get("Flaarlib");
	l->debug( message );
}

void MyLogger::error(const std::string &message )
{
	auto l = spdlog::get("Flaarlib");
	l->error( message );
}

void MyLogger::info(const std::string &message )
{
	auto l = spdlog::get("Flaarlib");
	l->info( message );
}

void MyLogger::log(const std::string &message )
{
	auto l = spdlog::get("Flaarlib");
	l->info( message);
}


} /* namespace flaarlib */
