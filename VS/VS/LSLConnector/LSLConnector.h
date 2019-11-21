#pragma once

//#include <boost/signals2.hpp>
#include <fstream>
#include "../../LSLlibs&headers/lsl_cpp.h"

class LSLConnector
{
public:
	LSLConnector();
	std::vector<float> getData();
	~LSLConnector();
	//boost::signals2::signal<void()> sendData; 
private:
	std::string field = "type", value = "EEG";
	std::vector<lsl::stream_info> results = lsl::resolve_stream(field, value);
	lsl::stream_inlet inlet;
	std::vector<float> sample;
	std::ofstream out;
};

