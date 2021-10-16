#pragma once
/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   10:10
	file base:	CWindowIterator
	file ext:	cpp
	author:		zhangwf

	purpose:	封装枚举窗口类
*********************************************************************/
//
#include <Windows.h>

/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   10:03
	file base:	CWindowIterator
	file ext:	h
	author:		zhangwf

	purpose:	封装枚举窗口类
*********************************************************************/
#ifndef _CWINDOW_ITERATOR_ZWF_H_
#define _CWINDOW_ITERATOR_ZWF_H_
//
#include <vector>
using namespace std;
//
class CWindowIterator
{
public:
	// 构造析构函数
	CWindowIterator();
	~CWindowIterator();

public:
	// 接口函数
	void EnumWnds();                        // 枚举窗口
	unsigned long GetWndCounts();           // 获得窗口总数
	HWND First();                           // 获得第一个窗口句柄
	HWND Next();                            // 获得下一个窗口句柄
	HWND GetHWnd(unsigned long wndIndex);   // 根据索引获得窗口句柄		

protected:
	vector<HWND> m_hWndArray;               // 存放枚举得到的窗口句柄
	unsigned long m_nCurrentWndIndex;       // 当前窗口句柄

	// 枚举窗口回调函数
	static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lp);

	// virtual enumerator
	virtual BOOL AddWnd(HWND hwnd);

	// override to filter different kinds of windows
	virtual BOOL IsTargetWnd(HWND hwnd) { return TRUE; }

};
//
//
// 派生出主窗口类（只获取进程的主窗口）
class CMainHWnd : public CWindowIterator
{
public:
	// 构造析构函数
	CMainHWnd();
	CMainHWnd(unsigned long pid);
	~CMainHWnd();

public:
	// 接口函数
	void EnumWndsByPid(DWORD pid);
	void SetWindowTitle(const wchar_t* title);
	HWND GetWinHWND();

	HWND winHwnd = NULL;

protected:
	DWORD m_pid;
	const wchar_t* TitleSet;
	virtual BOOL IsTargetWnd(HWND hwnd);

};
//
#endif








//
// 构造函数
CWindowIterator::CWindowIterator()
{
	// 初始化变量
	m_hWndArray.clear();
	m_nCurrentWndIndex = 0;
}

// 析构函数
CWindowIterator::~CWindowIterator()
{

}

//
// 接口函数
// 获得窗口总数
unsigned long CWindowIterator::GetWndCounts()
{
	return m_hWndArray.size();
}

// 获得第一个窗口句柄
HWND CWindowIterator::First()
{
	if (m_hWndArray.size() != 0)
	{
		m_nCurrentWndIndex = 0;
		return m_hWndArray[m_nCurrentWndIndex];
	}

	return NULL;
}

// 获得下一个窗口句柄
HWND CWindowIterator::Next()
{
	if (m_nCurrentWndIndex + 1 < m_hWndArray.size())
	{
		m_nCurrentWndIndex += 1;
		return m_hWndArray[m_nCurrentWndIndex];
	}

	return NULL;
}

// 根据索引获得窗口句柄
HWND CWindowIterator::GetHWnd(unsigned long wndIndex)
{
	if (wndIndex < m_hWndArray.size())
	{
		return m_hWndArray[wndIndex];
	}

	return NULL;
}

void CWindowIterator::EnumWnds()
{
	// 清除上次内容
	m_hWndArray.clear();

	// 枚举所有窗口
	::EnumWindows(EnumWndProc, (LPARAM)this);
}

//
// 非接口函数
// 枚举窗口回调函数
BOOL CALLBACK CWindowIterator::EnumWndProc(HWND hwnd, LPARAM lp)
{
	return ((CWindowIterator*)lp)->AddWnd(hwnd);
}

// virtual enumerator
BOOL CWindowIterator::AddWnd(HWND hwnd)
{
	if (IsTargetWnd(hwnd) == TRUE)
	{
		m_hWndArray.push_back(hwnd);
	}

	return TRUE;
}
//
//
// 派生出主窗口句柄类
// 构造函数
CMainHWnd::CMainHWnd()
	:CWindowIterator()
{
	m_pid = 0;
}

// 带参构造函数
CMainHWnd::CMainHWnd(unsigned long pid)
	:CWindowIterator()
{
	m_pid = pid;
	EnumWnds();
}

// 析构函数
CMainHWnd::~CMainHWnd()
{

}

//
// 接口函数
void CMainHWnd::EnumWndsByPid(DWORD pid)
{
	m_pid = pid;
	EnumWnds();
}
//
// 非接口函数
BOOL CMainHWnd::IsTargetWnd(HWND hwnd)
{
	// 顶层且可见
	//if (::IsWindowVisible(hwnd) == TRUE)
	{
		DWORD pidwin;
		::GetWindowThreadProcessId(hwnd, &pidwin);
		if (pidwin == m_pid)
		{
			winHwnd = hwnd;
			return TRUE;
		}
	}

	// 不是目标窗口
	return FALSE;
}

inline void CMainHWnd::SetWindowTitle(const wchar_t* title)
{
	::SetWindowText(winHwnd, title);
}

inline HWND CMainHWnd::GetWinHWND()
{
	return (winHwnd == NULL ? NULL : winHwnd);
}