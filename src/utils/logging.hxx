
/*
 * Copyright (c) 2012
 * COMPUTER APPLICATIONS IN SCIENCE & ENGINEERING
 * BARCELONA SUPERCOMPUTING CENTRE - CENTRO NACIONAL DE SUPERCOMPUTACIÓN
 * http://www.bsc.es

 * This file is part of Pandora Library. This library is free software; 
 * you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation;
 * either version 3.0 of the License, or (at your option) any later version.
 * 
 * Pandora is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#ifndef __UTILS_LOGGING_HXX__
#define __UTILS_LOGGING_HXX__


#include <Exception.hxx>
#include <mpi.h>
#include <omp.h>
#include <thread>
#include <fstream>
#include <Logger.hxx>
#include <boost/format.hpp>

namespace Model
{

/**
 * We subclass the standard logging class to add some extra functionality
 * without modifying that class, and redefine the logging macros (with different names) below.
 * The Logger subclass uses a singleton and keeps track of time in order to timestamp the log messages.
 */
class Logger : Engine::Logger
{
protected:
	static Logger* _instance;
	
	double _startTime;
	
public:
	static void init(double startTime, const std::string& logDir) {
		_instance = new Logger(startTime, logDir);
	}
	
	static Logger& instance() {
		if (!_instance) throw Engine::Exception("The logger needs to be explicitly initialized before using it");
		return *_instance;
	}
	
	std::ostream& log(const std::string& level, const std::string& fileName) {
		double elapsed = MPI_Wtime() - _startTime;
		std::ostream& s = Engine::Logger::log(fileName);
		
		// Append the log level
		s  << "[" << level << "]";
		
		// Append the thread ID
// 		s  << "[" << std::this_thread::get_id() << "]";
		s  << "[#" << omp_get_thread_num() << "]";
		
		// Append the time
		s  << "[" << boost::format("%8.5f") % elapsed << "]";
		
		// Append a space for readability
		s << " ";
		return s;
	}
	
	
	Logger(double startTime, const std::string& logDir) :
		Engine::Logger(),
		_startTime(startTime)
	{
		setLogsDir(logDir);
	}
};

} // namespace Engine


// ************************
// DEBUG MACRO
// ************************
#ifdef PANDORADEBUG 
#define PDEBUG(file, message) Model::Logger::instance().log("DEBUG", file) << message << std::endl;
#else
#define PDEBUG(file, message)
#endif

// ************************
// INFO MACRO
// ************************
#define PINFO(file, message) Model::Logger::instance().log("INFO", file) << message << std::endl;


#endif

