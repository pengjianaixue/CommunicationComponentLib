#pragma once
#include <chrono>
#include <iostream>
#include <string>
using namespace std::chrono;
class RunTimeMeasure
{
public:
	typedef enum  Timeunit
	{
		milliseconds,
		microseconds,
		nanoseconds
	}TIMEUNIT;
public:
	RunTimeMeasure();
	virtual ~RunTimeMeasure();
	long long getTimePassed(TIMEUNIT timecalcunit);
	void printTimePassed(TIMEUNIT timecalcunit,std::string PrintPrexInfo="",bool isReset = false);
	void timePointReset();
private:
	
private:
	time_point<system_clock> m_start = {system_clock::now()};

};

