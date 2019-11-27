#include "RunTimeMeausre.h"


RunTimeMeausre::RunTimeMeausre()
{

}


RunTimeMeausre::~RunTimeMeausre()
{

}

void RunTimeMeausre::printTimePassed(TIMEUNIT timecalcunit,std::string PrintPrexInfo/*=""*/, bool isReset /*= false*/)
{

	auto time_now = system_clock::now();
	if (timecalcunit == 0)
	{
		std::cout << PrintPrexInfo << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - m_start).count() << std::endl;
	}
	else if (timecalcunit == 1)
	{
		std::cout << PrintPrexInfo << std::chrono::duration_cast<std::chrono::microseconds>(time_now - m_start).count() << std::endl;
	}
	else if (timecalcunit == 2)
	{
		std::cout << PrintPrexInfo << std::chrono::duration_cast<std::chrono::nanoseconds>(time_now - m_start).count() << std::endl;
	}
	if (isReset)
	{
		timePointReset();
	}
	return;
}

void RunTimeMeausre::timePointReset()
{
	m_start = { system_clock::now() };
}

long long RunTimeMeausre::getTimePassed(TIMEUNIT timecalcunit)
{
	auto time_now = system_clock::now();
	if (timecalcunit==0)
	{
		return std::chrono::duration_cast<std::chrono::milliseconds>(time_now - m_start).count();
	}
	else if (timecalcunit == 1)
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(time_now - m_start).count();
	}
	else if (timecalcunit == 2)
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(time_now - m_start).count();
	}
	
}
