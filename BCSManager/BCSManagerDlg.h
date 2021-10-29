
// BCSManagerDlg.h: 头文件
//

#pragma once

#include <locale>
#include <string>
#include <vector>
#include "ListControlEx/ListCtrlEx.h"

static inline std::string format(const char* fmt, ...)
{
#define FORMAT_MSG_BUFFER_SIZE (204800)
	char szBuffer[FORMAT_MSG_BUFFER_SIZE + 1] = { 0 };
	va_list args;
	va_start(args, fmt);
	vsnprintf(szBuffer, FORMAT_MSG_BUFFER_SIZE, fmt, args);
	va_end(args);
	std::string strRet = szBuffer;
	return strRet;
}


// CBCSManagerDlg 对话框
class CBCSManagerDlg : public CDialogEx
{
// 构造
public:
	CBCSManagerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BCSMANAGER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:

	std::vector<CString> newConfigItems;
	std::vector<CString> pluginsName;

	int CPUCores = 0;

	void EnableControl();
	void DisableControl();

	bool CheckConfigOpen();
	bool CheckServerRunning();

	void AutoRebootTick();

	bool isInit = true;

	int pluginSelected = -1;

	bool isconhostshow = true;

	bool sd_reboot = false;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	//DWORD GetSubProcessID(DWORD& dwSubPID);
	CWinThread* tickthread;

	std::vector<CString> configLoaded;
	std::vector<DWORD> ProcessID;
	std::vector<bool> isServerRunning;
	std::vector<bool> serverLaunch;
	std::vector<bool> isAutoReboot;
	std::vector<int> isAutoMemClean, mem_preAllocateMax;
	std::vector<float> memRecord;
	CString currentConfig;
	//int mem_preAllocateMax = 0;
	int serverSelected = -1;

	bool configopen = false;

	int CreateConfig();
	void LoadConfig();

	CString ReadConfig(CString configName, CString NodeName);
	int WriteConfig(CString configName, CString NodeName, CString Param);
	int ShowConfig(CString configName);

	int LoadPlugins(CString configName);

	void ApplyAffinityChange();
	void ApplyPluginChange();

	CString GetProcessName(DWORD PID);
	void LaunchQueue();
	DWORD LaunchServerFromConfig(CString configName);

	void SetConhostTitle(DWORD PID, CString title, CString configName);
	HWND GetWindowHwnd(DWORD PID);

	void SendInstruction(CString ins);
	CString GetAffinityMask(CString configName);

	BOOL KillProcess(DWORD PID);

	void AddLogInfo(CString log);
	void UpdateConfig(CString configName);

	ListCtrlEx::CListCtrlEx m_ServerList;
	ListCtrlEx::CListCtrlEx m_PluginsList;
	ListCtrlEx::CListCtrlEx m_AffinityList;
	ListCtrlEx::CListCtrlEx m_ListLog;
	afx_msg void OnBnClickedBtnOpenserverfloder();
	afx_msg void OnBnClickedBtnJava();
	afx_msg void OnBnClickedBtnCore();
	afx_msg void OnBnClickedBtnOpenpluginsfloder();
	afx_msg void OnEnChangeServername();
	afx_msg void OnEnChangeMemallocate();
	afx_msg void OnEnChangeJvmextraparam();
	afx_msg void OnNMRClickServerlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClickPluginslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuPluginslistRefresh();
	afx_msg void OnNMClickServerlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickPluginslist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMenuPluginslistDeleteplugin();
	afx_msg void OnMenuServerlistNew();
	afx_msg void OnMenuServerlistDelete();
	afx_msg void OnBnClickedAutoreboot();
	afx_msg void OnBnClickedRebootandapply();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedShutdown();
	afx_msg void OnBnClickedBtnShconhost();
	afx_msg void OnBnClickedBtnHelpMemallocate();
	afx_msg void OnBnClickedBtnHelpJvmextraparam();
	afx_msg void OnMenuPluginslistAddplugin();
	afx_msg void OnEnChangeMemcleanthreshold();
};
