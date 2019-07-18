#pragma once
#include <chrono>
#include <iostream>
#include <string>
using namespace std::chrono;
class CRunTimeMeausre
{
public:
	typedef enum  Timeunit
	{
		milliseconds,
		micro,
		nano
	}TIMEUNIT;
public:
	CRunTimeMeausre();
	virtual ~CRunTimeMeausre();
	long long GetTimePassed(TIMEUNIT timecalcunit);
	bool PrintTimePassed(TIMEUNIT timecalcunit,std::string PrintPrexInfo="");
private:
	
private:
	time_point<system_clock> m_start = {system_clock::now()};

};

