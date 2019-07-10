#pragma once
#if _WIN32
#include <Windows.h>
/***********************************
	RAII wrap  CRITICAL_SECTION
************************************/
class CriticalSectionLockGuardian
{
public:
	explicit CriticalSectionLockGuardian(CRITICAL_SECTION &Critical_Section): m_Critical_Section(Critical_Section) { ::EnterCriticalSection(&m_Critical_Section);}
	CriticalSectionLockGuardian() = delete;
	~CriticalSectionLockGuardian() { ::LeaveCriticalSection(&m_Critical_Section);}
private:
	CRITICAL_SECTION &m_Critical_Section;
};
#endif // _WIN32

