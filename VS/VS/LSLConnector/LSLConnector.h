#pragma once

#include <boost/signals2.hpp>

class LSLConnector
{
public:
	LSLConnector();
	void getData();
	~LSLConnector();
	boost::signals2::signal<void()> sendData; 
};

