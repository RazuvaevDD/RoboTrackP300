#pragma once

#include <fstream>
#include "../../LSLlibs&headers/lsl_cpp.h"

class LSLConnector
{
public:
	LSLConnector();
	std::vector<float> getData();
	~LSLConnector();

private:
	std::string field = "type", value = "EEG";
	std::vector<lsl::stream_info> results = lsl::resolve_stream(field, value);
	lsl::stream_inlet inlet;
	std::vector<float> sample;
	std::ofstream out;
};

