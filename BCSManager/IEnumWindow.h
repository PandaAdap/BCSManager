#pragma once
/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   10:10
	file base:	CWindowIterator
	file ext:	cpp
	author:		zhangwf

	purpose:	��װö�ٴ�����
*********************************************************************/
//
#include <Windows.h>

/********************************************************************
	created:	2010/03/09
	created:	9:3:2010   10:03
	file base:	CWindowIterator
	file ext:	h
	author:		zhangwf

	purpose:	��װö�ٴ�����
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
	// ������������
	CWindowIterator();
	~CWindowIterator();

public:
	// �ӿں���
	void EnumWnds();                        // ö�ٴ���
	unsigned long GetWndCounts();           // ��ô�������
	HWND First();                           // ��õ�һ�����ھ��
	HWND Next();                            // �����һ�����ھ��
	HWND GetHWnd(unsigned long wndIndex);   // ����������ô��ھ��		

protected:
	vector<HWND> m_hWndArray;               // ���ö�ٵõ��Ĵ��ھ��
	unsigned long m_nCurrentWndIndex;       // ��ǰ���ھ��

	// ö�ٴ��ڻص�����
	static BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lp);

	// virtual enumerator
	virtual BOOL AddWnd(HWND hwnd);

	// override to filter different kinds of windows
	virtual BOOL IsTargetWnd(HWND hwnd) { return TRUE; }

};
//
//
// �������������ֻࣨ��ȡ���̵������ڣ�
class CMainHWnd : public CWindowIterator
{
public:
	// ������������
	CMainHWnd();
	CMainHWnd(unsigned long pid);
	~CMainHWnd();

public:
	// �ӿں���
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
// ���캯��
CWindowIterator::CWindowIterator()
{
	// ��ʼ������
	m_hWndArray.clear();
	m_nCurrentWndIndex = 0;
}

// ��������
CWindowIterator::~CWindowIterator()
{

}

//
// �ӿں���
// ��ô�������
unsigned long CWindowIterator::GetWndCounts()
{
	return m_hWndArray.size();
}

// ��õ�һ�����ھ��
HWND CWindowIterator::First()
{
	if (m_hWndArray.size() != 0)
	{
		m_nCurrentWndIndex = 0;
		return m_hWndArray[m_nCurrentWndIndex];
	}

	return NULL;
}

// �����һ�����ھ��
HWND CWindowIterator::Next()
{
	if (m_nCurrentWndIndex + 1 < m_hWndArray.size())
	{
		m_nCurrentWndIndex += 1;
		return m_hWndArray[m_nCurrentWndIndex];
	}

	return NULL;
}

// ����������ô��ھ��
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
	// ����ϴ�����
	m_hWndArray.clear();

	// ö�����д���
	::EnumWindows(EnumWndProc, (LPARAM)this);
}

//
// �ǽӿں���
// ö�ٴ��ڻص�����
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
// �����������ھ����
// ���캯��
CMainHWnd::CMainHWnd()
	:CWindowIterator()
{
	m_pid = 0;
}

// ���ι��캯��
CMainHWnd::CMainHWnd(unsigned long pid)
	:CWindowIterator()
{
	m_pid = pid;
	EnumWnds();
}

// ��������
CMainHWnd::~CMainHWnd()
{

}

//
// �ӿں���
void CMainHWnd::EnumWndsByPid(DWORD pid)
{
	m_pid = pid;
	EnumWnds();
}
//
// �ǽӿں���
BOOL CMainHWnd::IsTargetWnd(HWND hwnd)
{
	// �����ҿɼ�
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

	// ����Ŀ�괰��
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