#include "RunTimeMeausre.h"


CRunTimeMeausre::CRunTimeMeausre()
{

}


CRunTimeMeausre::~CRunTimeMeausre()
{

}

bool CRunTimeMeausre::PrintTimePassed(TIMEUNIT timecalcunit,std::string PrintPrexInfo)
{

	auto time_now = system_clock::now();
	if (timecalcunit == 0)
	{
		std::cout << PrintPrexInfo << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - m_start).count() << std::endl;
		return true;
	}
	else if (timecalcunit == 1)
	{
		std::cout << PrintPrexInfo << std::chrono::duration_cast<std::chrono::microseconds>(time_now - m_start).count() << std::endl;
		return true;
	}
	else if (timecalcunit == 2)
	{
		std::cout << PrintPrexInfo << std::chrono::duration_cast<std::chrono::nanoseconds>(time_now - m_start).count() << std::endl;
		return true;
	}
	return false;
}

long long CRunTimeMeausre::GetTimePassed(TIMEUNIT timecalcunit)
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
