
// BCSManagerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "BCSManager.h"
#include "BCSManagerDlg.h"
#include "afxdialogex.h"

#include <psapi.h>
#include <tlhelp32.h>
#include "IEnumWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT MemoryTick(LPVOID lpParam);
bool closethread = false;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBCSManagerDlg 对话框



CBCSManagerDlg::CBCSManagerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BCSMANAGER_DIALOG, pParent)
{
	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBCSManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ServerList, m_ServerList);
	DDX_Control(pDX, IDC_PluginsList, m_PluginsList);
	DDX_Control(pDX, IDC_AffinityList, m_AffinityList);
}

BEGIN_MESSAGE_MAP(CBCSManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_BN_CLICKED(IDC_BUTTON1, &CBCSManagerDlg::OnBnClickedButton1)
	//ON_BN_CLICKED(IDC_BUTTON2, &CBCSManagerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_Btn_OpenServerFloder, &CBCSManagerDlg::OnBnClickedBtnOpenserverfloder)
	ON_BN_CLICKED(IDC_Btn_Java, &CBCSManagerDlg::OnBnClickedBtnJava)
	ON_BN_CLICKED(IDC_Btn_Core, &CBCSManagerDlg::OnBnClickedBtnCore)
	ON_BN_CLICKED(IDC_Btn_OpenPluginsFloder, &CBCSManagerDlg::OnBnClickedBtnOpenpluginsfloder)
	ON_EN_CHANGE(IDC_ServerName, &CBCSManagerDlg::OnEnChangeServername)
	ON_EN_CHANGE(IDC_MemAllocate, &CBCSManagerDlg::OnEnChangeMemallocate)
	ON_EN_CHANGE(IDC_JVMExtraParam, &CBCSManagerDlg::OnEnChangeJvmextraparam)
	ON_NOTIFY(NM_RCLICK, IDC_ServerList, &CBCSManagerDlg::OnNMRClickServerlist)
	ON_NOTIFY(NM_RCLICK, IDC_PluginsList, &CBCSManagerDlg::OnNMRClickPluginslist)
	ON_COMMAND(ID_Menu_PluginsList_Refresh, &CBCSManagerDlg::OnMenuPluginslistRefresh)
	ON_NOTIFY(NM_CLICK, IDC_ServerList, &CBCSManagerDlg::OnNMClickServerlist)
	ON_NOTIFY(NM_CLICK, IDC_PluginsList, &CBCSManagerDlg::OnNMClickPluginslist)
	ON_COMMAND(ID_Menu_PluginsList_DeletePlugin, &CBCSManagerDlg::OnMenuPluginslistDeleteplugin)
	ON_COMMAND(ID_Menu_ServerList_New, &CBCSManagerDlg::OnMenuServerlistNew)
	ON_COMMAND(ID_Menu_ServerList_Delete, &CBCSManagerDlg::OnMenuServerlistDelete)
	ON_BN_CLICKED(IDC_AutoReboot, &CBCSManagerDlg::OnBnClickedAutoreboot)
	ON_BN_CLICKED(IDC_RebootAndApply, &CBCSManagerDlg::OnBnClickedRebootandapply)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_Shutdown, &CBCSManagerDlg::OnBnClickedShutdown)
	ON_BN_CLICKED(IDC_Btn_SHConhost, &CBCSManagerDlg::OnBnClickedBtnShconhost)
	ON_BN_CLICKED(IDC_Btn_Help_MemAllocate, &CBCSManagerDlg::OnBnClickedBtnHelpMemallocate)
	ON_BN_CLICKED(IDC_Btn_Help_JVMExtraParam, &CBCSManagerDlg::OnBnClickedBtnHelpJvmextraparam)
	ON_COMMAND(ID_Menu_PluginsList_AddPlugin, &CBCSManagerDlg::OnMenuPluginslistAddplugin)
END_MESSAGE_MAP()


// CBCSManagerDlg 消息处理程序

BOOL CBCSManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	setlocale(LC_CTYPE, "chs");

	// TODO: 在此添加额外的初始化代码
	CreateDirectory(L".\\BCSConfig", NULL);

	//Get CPU Cores Number
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	CPUCores = sysInfo.dwNumberOfProcessors;

	//newConfigItemsInit
	//newConfigItems.push_back(L"#Active=true");
	newConfigItems.push_back(L"#ServerName=ServerName");
	newConfigItems.push_back(L"#MemoryAllocate=2048M-4069M");
	newConfigItems.push_back(L"#JavaPath=");
	newConfigItems.push_back(L"#CorePath=");
	newConfigItems.push_back(L"#JVMExtraParam=");
	newConfigItems.push_back(L"#AutoReboot=false");
	newConfigItems.push_back(L"#LastProcessID=0");

	//List Control Init
	CRect rect;
	m_ServerList.GetClientRect(&rect);
	m_ServerList.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_ServerList.SetExtendedStyle(m_ServerList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ServerList.InsertColumn(0, _T("服务器名"), rect.Width() / 2 * 1 - 10, ListCtrlEx::Normal, LVCFMT_CENTER);
	m_ServerList.InsertColumn(1, _T("CPU"), rect.Width() / 4 + 5, ListCtrlEx::Normal, LVCFMT_CENTER);
	m_ServerList.InsertColumn(2, _T("占用"), rect.Width() / 4 + 5, ListCtrlEx::Normal, LVCFMT_CENTER);

	m_PluginsList.GetClientRect(&rect);
	m_PluginsList.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_PluginsList.SetExtendedStyle(m_PluginsList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_PluginsList.InsertColumn(0, _T("插件名"), rect.Width() - 18, ListCtrlEx::CheckBox, LVCFMT_LEFT);

	m_AffinityList.GetClientRect(&rect);
	m_AffinityList.ModifyStyle(0, LVS_SINGLESEL | LVS_REPORT | LVS_SHOWSELALWAYS);
	m_AffinityList.SetExtendedStyle(m_AffinityList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_AffinityList.InsertColumn(0, _T("相关性绑定"), rect.Width() - 18, ListCtrlEx::CheckBox, LVCFMT_LEFT);

	CString AffinitySet;
	for (int i = 0; i < CPUCores; i++)
	{
		AffinitySet.Format(L"CPU %d",i);
		m_AffinityList.InsertItem(i, AffinitySet);
		AffinitySet.Format(L"#CPU%d=true", i);
		newConfigItems.push_back(AffinitySet);
	}

	LoadConfig();
	if (configLoaded.size() != 0)
	{
		configopen = true;
		//Set Default Config Here
		currentConfig = configLoaded[0];
		serverSelected = 0;
		ShowConfig(currentConfig);

	}

	//Launch Queue
	//LaunchQueue();

	tickthread = AfxBeginThread(MemoryTick, this);


	isInit = false;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CBCSManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CBCSManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CBCSManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Create Config
int CBCSManagerDlg::CreateConfig()
{
	CString configName;
	for (int count = 0; count < 20; count++)
	{
		configName.Format(L".\\BCSConfig\\BCSConfig%02d.bcscfg", count);
		CFileFind finder;
		BOOL ifFind = finder.FindFile(configName);
		if (!ifFind)
		{
			CStdioFile config;
			config.Open(configName, CFile::modeCreate | CFile::modeReadWrite);
			for (int line = 0; line < newConfigItems.size(); line++)
			{
				config.WriteString(newConfigItems[line] + L"\n");
			}
			config.Close();
			//Refresh Config List Here
			return 0;
		}
	}
	
	AfxMessageBox(L"配置达到上限，请删除部分配置!");
	return -1;
}

//Refresh Config
void CBCSManagerDlg::LoadConfig()
{
	KillTimer(1);

	ProcessID.clear();
	configLoaded.clear();
	serverLaunch.clear();
	isServerRunning.clear();
	isAutoReboot.clear();
	m_ServerList.DeleteAllItems();
	CString configName;
	for (int count = 0; count < 20; count++)
	{
		configName.Format(L".\\BCSConfig\\BCSConfig%02d.bcscfg", count);
		CFileFind finder;
		BOOL ifFind = finder.FindFile(configName);
		if (ifFind)
		{
			configLoaded.push_back(configName);
			m_ServerList.InsertItem(count, ReadConfig(configName,L"ServerName"));
			
			//if (ReadConfig(configName, L"Active") == L"true")
			//{
			//	m_ServerList.SetCellChecked(count, 0, 2);
			//	serverLaunch.push_back(true);
			//}
			//else
			//{
			//	m_ServerList.SetCellChecked(count, 0, 0);
			//	serverLaunch.push_back(false);
			//}

			if (ReadConfig(configName, L"AutoReboot") == L"true")
			{
				isAutoReboot.push_back(true);
			}
			else
			{
				isAutoReboot.push_back(false);
			}

			//Check isRunning
			if (GetProcessName(_ttoi(ReadConfig(configName, L"LastProcessID"))) == L"java.exe")
			{
				isServerRunning.push_back(true);
				ProcessID.push_back(_ttoi(ReadConfig(configName, L"LastProcessID")));
			}
			else
			{
				isServerRunning.push_back(false);
				ProcessID.push_back(0);
				WriteConfig(configName, L"LastProcessID", L"0");
			}
		}
	}
	SetTimer(1, 5000, NULL);
}

//Read Config
CString CBCSManagerDlg::ReadConfig(CString configName, CString NodeName)
{
	CStdioFile configReader;
	CString readLine = L"";
	CString node = L"#" + NodeName + L"=";
	configReader.Open(configName, CFile::modeRead);
	while (configReader.ReadString(readLine))
	{
		if (readLine.Find(node, 0) == 0)
		{
			configReader.Close();
			return readLine.Mid(node.GetLength(), readLine.GetLength() - node.GetLength());
		}
	}
	configReader.Close();
	AfxMessageBox(L"节点不存在.");
	return L"";
}

//Write Config
int CBCSManagerDlg::WriteConfig(CString configName, CString NodeName, CString Param)
{
	std::vector<CString> readConfig;
	CStdioFile configReader;
	CString readLine = L"";

	configReader.Open(configName, CFile::modeRead);
	while (configReader.ReadString(readLine))
	{
		readConfig.push_back(readLine);
	}configReader.Close();

	bool isChange = false;
	CString node = L"#" + NodeName + L"=";
	configReader.Open(configName, CFile::modeCreate | CFile::modeReadWrite);
	for (int line = 0; line < readConfig.size(); line++)
	{
		if (readConfig[line].Find(node, 0) == 0)
		{
			readConfig[line] = node + Param;
			isChange = true;
		}
		configReader.WriteString(readConfig[line] + L"\n");
	}
	if (isChange == false)
	{
		configReader.WriteString(node + Param + L"\n");
	}
	configReader.Close();

	return 0;
}

//Show Config
int CBCSManagerDlg::ShowConfig(CString configName)
{
	CString MemAllocateSet = ReadConfig(configName, L"MemoryAllocate");
	mem_preAllocateMax = _ttoi(MemAllocateSet.Mid(MemAllocateSet.ReverseFind('-') + 1, MemAllocateSet.GetLength() - MemAllocateSet.ReverseFind('-') - 2));

	SetDlgItemText(IDC_ServerName, ReadConfig(configName, L"ServerName"));
	SetDlgItemText(IDC_MemAllocate, MemAllocateSet);
	SetDlgItemText(IDC_JavaEnviroument, ReadConfig(configName, L"JavaPath"));
	SetDlgItemText(IDC_CorePath, ReadConfig(configName, L"CorePath"));
	SetDlgItemText(IDC_JVMExtraParam, ReadConfig(configName, L"JVMExtraParam"));

	if (ReadConfig(configName, L"AutoReboot") == L"true")
	{
		((CButton*)GetDlgItem(IDC_AutoReboot))->SetCheck(1);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_AutoReboot))->SetCheck(0);
	}

	CString AffinitySet;
	for (int count = 0; count < CPUCores; count++)
	{
		AffinitySet.Format(L"CPU%d",count);
		if (ReadConfig(configName, AffinitySet) == L"true")
		{
			m_AffinityList.SetCellChecked(count, 0, 2);
		}
		else
		{
			m_AffinityList.SetCellChecked(count, 0, 0);
		}
	}

	LoadPlugins(configName);

	return 0;
}

//Load Plugins List
int CBCSManagerDlg::LoadPlugins(CString configName)
{
	pluginsName.clear();
	m_PluginsList.DeleteAllItems();
	CString CorePath, PluginPath;
	CorePath = ReadConfig(configName, L"CorePath");
	PluginPath = CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\plugins";

	CFileFind finder;
	CString jar_enable,jar_disable;
	jar_enable.Format(_T("%s\\*.jar"), PluginPath);
	jar_disable.Format(_T("%s\\*.disable"), PluginPath);
	bool bWorking = finder.FindFile(jar_enable);
	int plugins_load_count = 0;

	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory() && !finder.IsDots())
		{
			CString strRet;
			CString strFile = finder.GetFileName();
			int pos = strFile.ReverseFind('.');
			if (pos > 0)
			{
				strRet = strFile.Left(pos);
			}
			pluginsName.push_back(strRet);
			m_PluginsList.InsertItem(plugins_load_count, strRet);
			m_PluginsList.SetCellChecked(plugins_load_count, 0, 2);//Set enable
			plugins_load_count++;
		}
	}

	bWorking = finder.FindFile(jar_disable);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if (!finder.IsDirectory() && !finder.IsDots())
		{
			CString strRet;
			CString strFile = finder.GetFileName();
			int pos = strFile.ReverseFind('.');
			if (pos > 0)
			{
				strRet = strFile.Left(pos);
			}
			pluginsName.push_back(strRet);
			m_PluginsList.InsertItem(plugins_load_count, strRet);
			m_PluginsList.SetCellChecked(plugins_load_count, 0, 0);//Set disable
			plugins_load_count++;
		}
	}

	return 0;
}

/*
void CBCSManagerDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;


	memset(&sInfo, 0, sizeof(STARTUPINFO));
	memset(&pInfo, 0, sizeof(PROCESS_INFORMATION));


	sInfo.cb = sizeof(STARTUPINFO);
	sInfo.dwFlags = STARTF_USESHOWWINDOW;
	sInfo.wShowWindow = SW_SHOW;


	CreateProcess(L"E:\\minecraft-manager\\jre16\\bin\\java.exe", /* 待运行的程序 *
		L" -Xms2G -Xmx6G -XX:+UseG1GC -XX:+ParallelRefProcEnabled -XX:MaxGCPauseMillis=200 -XX:+UnlockExperimentalVMOptions -XX:+DisableExplicitGC -XX:+AlwaysPreTouch -XX:G1NewSizePercent=30 -XX:G1MaxNewSizePercent=40 -XX:G1HeapRegionSize=8M -XX:G1ReservePercent=20 -XX:G1HeapWastePercent=5 -XX:G1MixedGCCountTarget=4 -XX:InitiatingHeapOccupancyPercent=15 -XX:G1MixedGCLiveThresholdPercent=90 -XX:G1RSetUpdatingPauseTimePercent=5 -XX:SurvivorRatio=32 -XX:+PerfDisableSharedMem -XX:MaxTenuringThreshold=1 -Dusing.aikars.flags=https://mcflags.emc.gs -Daikars.new.flags=true -jar launcher-airplane.jar nogui", /* 传递给待运行程序的参数 *

		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		L"E:\\minecraft-manager\\1.16.5",
		&sInfo,
		&pInfo
	);
	
	Sleep(1000);
	CMainHWnd a;
	a.EnumWndsByPid(pInfo.dwProcessId,L"1.16.5 SC");
	//HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pInfo.dwProcessId);

	//DWORD spid = GetSubProcessID(pInfo.dwProcessId);
	//HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, spid);
	//Sleep(2000);
	

	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);
}


DWORD CBCSManagerDlg::GetSubProcessID(DWORD& dwPID)
{
	DWORD subpid = 0;
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);

	BOOL bProc = Process32First(hProcSnap, &pe);
	while (bProc)
	{
		if (pe.th32ParentProcessID == dwPID )//&& pe.szExeFile == L"conhost.exe")
		{
			subpid = pe.th32ProcessID;
		}
		bProc = Process32Next(hProcSnap, &pe);
	}
	CloseHandle(hProcSnap);
	return subpid;
}




void CBCSManagerDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	//::SetWindowText((HWND)0x00480BA6, L"1132");


	HWND n = 0;
	

}*/

//Open Server Floder
void CBCSManagerDlg::OnBnClickedBtnOpenserverfloder()
{
	if (CheckConfigOpen() == false)
		return;

	CString CorePath;
	CorePath = ReadConfig(currentConfig, L"CorePath");
	ShellExecute(NULL, NULL, CorePath.Mid(0, CorePath.ReverseFind('\\')), NULL, NULL, SW_SHOW);
}

//Java Enviroument
void CBCSManagerDlg::OnBnClickedBtnJava()
{
	if (CheckConfigOpen() == false)
		return;

	if (CheckServerRunning() == true)
	{
		return;
	}

	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, L"Java|java.exe||", NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		SetDlgItemText(IDC_JavaEnviroument, openFileDlg.GetPathName());
		WriteConfig(currentConfig, L"JavaPath", openFileDlg.GetPathName());
	}
}

//Core jar
void CBCSManagerDlg::OnBnClickedBtnCore()
{
	if (CheckConfigOpen() == false)
		return;

	if (CheckServerRunning() == true)
	{
		return;
	}

	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, L"Minecraft Core|*.jar||", NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		SetDlgItemText(IDC_CorePath, openFileDlg.GetPathName());
		configopen = false;
		WriteConfig(currentConfig, L"CorePath", openFileDlg.GetPathName());
		LoadPlugins(currentConfig);
		configopen = true;
	}
}

//Check Config Open
bool CBCSManagerDlg::CheckConfigOpen()
{
	if (configopen == false)
	{
		AfxMessageBox(L"请先添加一个服务端.");

		return false;
	}
	return true;
}

//Check Server Running
bool CBCSManagerDlg::CheckServerRunning()
{
	if (isInit == true)
	{
		return true;
	}

	if(serverSelected != -1)
		if (isServerRunning[serverSelected] == true)
		{
			AfxMessageBox(L"服务端正在运行,所做更改无效!");
			return true;
		}
	return false;
}

//Open Plugin Floder
void CBCSManagerDlg::OnBnClickedBtnOpenpluginsfloder()
{
	if (CheckConfigOpen() == false)
		return;

	CString CorePath;
	CorePath = ReadConfig(currentConfig, L"CorePath");
	ShellExecute(NULL, NULL, CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\plugins", NULL, NULL, SW_SHOW);
}

//Server Name Editor
void CBCSManagerDlg::OnEnChangeServername()
{
	if (isInit == true)
		return;
	if (configopen == false)
		return;
	CString serverName;
	GetDlgItemText(IDC_ServerName, serverName);
	WriteConfig(currentConfig, L"ServerName", serverName);

	configopen = false;
	LoadConfig();
	configopen = true;
}

//MemAllocate Editor
void CBCSManagerDlg::OnEnChangeMemallocate()
{
	if (isInit == true)
		return;
	if (configopen == false)
		return;

	if (CheckServerRunning() == true)
	{
		configopen = false;
		ShowConfig(currentConfig);
		configopen = true;
		return;
	}

	CString memAllocate;
	GetDlgItemText(IDC_MemAllocate, memAllocate);
	WriteConfig(currentConfig, L"MemoryAllocate", memAllocate);
}

//JVM ExParam Editor
void CBCSManagerDlg::OnEnChangeJvmextraparam()
{
	if (isInit == true)
		return;
	if (configopen == false)
		return;

	if (CheckServerRunning() == true)
	{
		configopen = false;
		ShowConfig(currentConfig);
		configopen = true;
		return;
	}

	CString JVMExParam;
	GetDlgItemText(IDC_JVMExtraParam, JVMExParam);
	WriteConfig(currentConfig, L"JVMExtraParam", JVMExParam);
}

//Right Click Menu ServerList
void CBCSManagerDlg::OnNMRClickServerlist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos = m_ServerList.GetFirstSelectedItemPosition();
	serverSelected = (int)m_ServerList.GetNextSelectedItem(pos);
	if (configLoaded.size() > 0)
	{
		currentConfig = configLoaded[(serverSelected == -1 ? 0 : serverSelected)];
		configopen = false;
		ShowConfig(currentConfig);
		configopen = true;
	}

	CMenu menu, * popup;
	menu.LoadMenu(IDR_Menu_ServerList);
	popup = menu.GetSubMenu(0);

	if (m_ServerList.GetSelectedCount() <= 0)
	{
		popup->EnableMenuItem(ID_Menu_ServerList_Delete, MF_GRAYED);
	}
	else
	{
		popup->EnableMenuItem(ID_Menu_ServerList_Delete, MF_ENABLED);
	}

	CPoint point;
	ClientToScreen(&point);
	GetCursorPos(&point);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x + 1, point.y + 1, this);

	*pResult = 0;
}

//Right Click Menu PluginList
void CBCSManagerDlg::OnNMRClickPluginslist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos = m_PluginsList.GetFirstSelectedItemPosition();
	int selected = (int)m_PluginsList.GetNextSelectedItem(pos);
	//currentConfig = configLoaded[(selected == -1 ? 0 : selected)];

	CMenu menu, * popup;
	menu.LoadMenu(IDR_Menu_PluginsList);
	popup = menu.GetSubMenu(0);

	if (m_PluginsList.GetSelectedCount() <= 0)
	{
		popup->EnableMenuItem(ID_Menu_PluginsList_DeletePlugin, MF_GRAYED);
	}
	else
	{
		popup->EnableMenuItem(ID_Menu_PluginsList_DeletePlugin, MF_ENABLED);
	}

	CPoint point;
	ClientToScreen(&point);
	GetCursorPos(&point);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x + 1, point.y + 1, this);

	*pResult = 0;
}

//PluginsList RC Menu Refresh
void CBCSManagerDlg::OnMenuPluginslistRefresh()
{
	// TODO: 在此添加命令处理程序代码
	LoadPlugins(currentConfig);
}

//ServerList LC
void CBCSManagerDlg::OnNMClickServerlist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos = m_ServerList.GetFirstSelectedItemPosition();
	serverSelected = (int)m_ServerList.GetNextSelectedItem(pos);

	serverSelected = (serverSelected == -1 ? 0 : serverSelected);

	if (configLoaded.size() > 0)
	{
		currentConfig = configLoaded[(serverSelected == -1 ? 0 : serverSelected)];

		//if (serverSelected > -1)
		//{
		//	WriteConfig(currentConfig, L"Active", (m_ServerList.GetCellChecked(serverSelected, 0) == 2 ? L"true" : L"false"));
		//}

		configopen = false;
		ShowConfig(currentConfig);
		configopen = true;
	}

	*pResult = 0;
}

//Plugin LC
void CBCSManagerDlg::OnNMClickPluginslist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	POSITION pos = m_PluginsList.GetFirstSelectedItemPosition();
	pluginSelected = (int)m_PluginsList.GetNextSelectedItem(pos);

	*pResult = 0;
}

//Delete Plugin
void CBCSManagerDlg::OnMenuPluginslistDeleteplugin()
{
	// TODO: 在此添加命令处理程序代码
	CString CorePath, PluginPath;
	CorePath = ReadConfig(currentConfig, L"CorePath");
	PluginPath = CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\plugins\\" + pluginsName[pluginSelected];

	if (m_PluginsList.GetCellChecked(pluginSelected, 0) == 2)
	{
		PluginPath += L".jar";
	}
	else
	{
		PluginPath += L".disable";
	}

	if (MessageBox(L"将会彻底删除该插件(配置文件将会被保留)\r\n未禁用的插件在服务器运行时无法删除!\r\n\r\n插件名: " + pluginsName[pluginSelected], L"是否删除?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		DeleteFile(PluginPath);
		LoadPlugins(currentConfig);
	}
}

//New Server Config
void CBCSManagerDlg::OnMenuServerlistNew()
{
	// TODO: 在此添加命令处理程序代码
	configopen = false;
	CreateConfig();
	LoadConfig();
	configopen = true;
}

//Delete Server Config
void CBCSManagerDlg::OnMenuServerlistDelete()
{
	// TODO: 在此添加命令处理程序代码

	//closethread = true;

	if (MessageBox(L"将会彻底删除该服务端配置\r\n将会关闭当前删除的服务端!", L"是否删除?", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		if (isServerRunning[serverSelected] == true)
		{
			AfxMessageBox(L"请先关闭服务端!");
			return;
		}

		configopen = false;
		DeleteFile(configLoaded[serverSelected]);
	}

	
	LoadConfig();
	if (configLoaded.size() != 0)
	{
		configopen = true;
		//Set Default Config Here
		currentConfig = configLoaded[0];
		serverSelected = 0;
		ShowConfig(currentConfig);
	}
	configopen = true;

	//closethread = false;
	//tickthread = AfxBeginThread(MemoryTick, this);
}

//CheckBox AutoReboot
void CBCSManagerDlg::OnBnClickedAutoreboot()
{
	if (configopen == false)
		return;


	if (((CButton*)GetDlgItem(IDC_AutoReboot))->GetCheck() == 1)
	{
		WriteConfig(currentConfig, L"AutoReboot", L"true");
		isAutoReboot[serverSelected] = true;
	}
	else
	{
		WriteConfig(currentConfig, L"AutoReboot", L"false"); 
		isAutoReboot[serverSelected] = false;
	}
}

//Apply Plugin Change
void CBCSManagerDlg::ApplyPluginChange()
{
	CString CorePath, PluginPath_Enable, PluginPath_Disable;
	CorePath = ReadConfig(currentConfig, L"CorePath");
	CFileFind finder;

	for (int count = 0; count < pluginsName.size(); count++)
	{
		PluginPath_Enable = CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\plugins\\" + pluginsName[count] + L".jar";
		PluginPath_Disable = CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\plugins\\" + pluginsName[count] + L".disable";

		if (m_PluginsList.GetCellChecked(count, 0) == 2 && finder.FindFile(PluginPath_Disable))
		{
			CopyFile(PluginPath_Disable, PluginPath_Enable, FALSE);
			DeleteFile(PluginPath_Disable);
		}
		else if(m_PluginsList.GetCellChecked(count, 0) == 0 && finder.FindFile(PluginPath_Enable))
		{
			CopyFile(PluginPath_Enable, PluginPath_Disable, FALSE);
			DeleteFile(PluginPath_Enable);
		}
	}

	LoadPlugins(currentConfig);
}

//Apply Affinity Change
void CBCSManagerDlg::ApplyAffinityChange()
{
	CString AffinitySet;
	for (int count = 0; count < CPUCores; count++)
	{
		AffinitySet.Format(L"CPU%d", count);
		if (m_AffinityList.GetCellChecked(count, 0) == 2)
		{
			WriteConfig(currentConfig, AffinitySet, L"true");
		}
		else
		{
			WriteConfig(currentConfig, AffinitySet, L"false");
		}
	}
}

//Reboot & Apply
void CBCSManagerDlg::OnBnClickedRebootandapply()
{
	// TODO: 在此添加控件通知处理程序代码
 
	CString PropertiesPath, PropertiesFile;
	PropertiesPath = ReadConfig(currentConfig, L"CorePath");
	PropertiesFile = PropertiesPath.Mid(0, PropertiesPath.ReverseFind('\\')) + L"\\server.properties";

	CFileFind finder;
	BOOL ifFind = finder.FindFile(PropertiesFile);

	CString PID;
	//Close Server
	if (isServerRunning[serverSelected] == true)
	{
		if (!ifFind)
		{
			KillProcess(ProcessID[serverSelected]);
		}
		else
		{
			SendInstruction(L"stop");
		}
		isServerRunning[serverSelected] = false;

		while (GetProcessName(ProcessID[serverSelected]) == L"java.exe")
		{
			Sleep(1000);
		}
	}

	ApplyPluginChange();
	ApplyAffinityChange();

	//Launch Server
	int pid = LaunchServerFromConfig(currentConfig);
	PID.Format(L"%d", pid);
	WriteConfig(currentConfig, L"LastProcessID", PID);
	ProcessID[serverSelected] = pid;
	Sleep(500);
	SetConhostTitle(pid, ReadConfig(currentConfig, L"ServerName"),currentConfig);
	isServerRunning[serverSelected] = true;

	//SetAffinity(pid, GetAffinityMask(currentConfig));
}

//Convert time
static __int64 FileTimeToInt64(const FILETIME& time)
{
	ULARGE_INTEGER tt;
	tt.LowPart = time.dwLowDateTime;
	tt.HighPart = time.dwHighDateTime;
	return(tt.QuadPart);
}

//GetProcessCPU
static double GetProcessCPU(DWORD pid, int Affinity)
{
	static __int64 last_time_ = 0;
	static __int64 last_system_time_ = 0;

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	__int64 system_time;
	__int64 time;

	double cpu = 0;

	GetSystemTimeAsFileTime(&now);

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, FALSE, pid);
	if (!hProcess)
	{
		return -1;
	}
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		return -1;
	}
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / Affinity;
	time = FileTimeToInt64(now);

	last_system_time_ = system_time;
	last_time_ = time;
	CloseHandle(hProcess);

	Sleep(50);

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION/*PROCESS_ALL_ACCESS*/, FALSE, pid);
	if (!hProcess)
	{
		return -1;
	}
	if (!GetProcessTimes(hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		return -1;
	}
	GetSystemTimeAsFileTime(&now);
	system_time = (FileTimeToInt64(kernel_time) + FileTimeToInt64(user_time)) / Affinity;
	time = FileTimeToInt64(now);

	CloseHandle(hProcess);

	cpu = ((double)(system_time - last_system_time_) / (double)(time - last_time_)) * 100;

	return cpu;
}

//GetProcessMemory(MB)
float GetProcessMemory(DWORD pid)
{
	SYSTEM_INFO si;
	GetSystemInfo(&si);

	HANDLE handle;
	handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

	float usedMemory = 0;

	PSAPI_WORKING_SET_INFORMATION workSet;
	memset(&workSet, 0, sizeof(workSet));
	BOOL bOk = QueryWorkingSet(handle, &workSet, sizeof(workSet));
	if (bOk || (!bOk && GetLastError() == ERROR_BAD_LENGTH))
	{
		int nSize = sizeof(workSet.NumberOfEntries) + workSet.NumberOfEntries * sizeof(workSet.WorkingSetInfo);
		char* pBuf = new char[nSize];
		if (pBuf)
		{
			QueryWorkingSet(handle, pBuf, nSize);
			PSAPI_WORKING_SET_BLOCK* pFirst = (PSAPI_WORKING_SET_BLOCK*)(pBuf + sizeof(workSet.NumberOfEntries));
			double dwMem = 0;
			for (ULONG_PTR nMemEntryCnt = 0; nMemEntryCnt < workSet.NumberOfEntries; nMemEntryCnt++, pFirst++)
			{
				if (pFirst->Shared == 0)
				{
					dwMem += si.dwPageSize;
				}
			}
			delete pBuf;
			pBuf = NULL;
			if (workSet.NumberOfEntries > 0)
			{
				usedMemory = dwMem / (1024.0 * 1024.0);
			}
		}
	}
	CloseHandle(handle);

	return usedMemory;
}

//MemoryTick
UINT MemoryTick(LPVOID lpParam)
{
	CBCSManagerDlg* obj = (CBCSManagerDlg*)lpParam;
	
	CString strShow;
	int PID = 0;

	while (closethread == false)
	{
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);

		DWORDLONG physical_memory = statex.ullTotalPhys / (1024 * 1024);
		DWORDLONG usePhys = physical_memory - statex.ullAvailPhys / (1024 * 1024);

		strShow.Format(L"内存(总占用): %.2f%% [%d MB]", ((float)usePhys / physical_memory) * 100, usePhys);
		obj->SetDlgItemText(IDC_MemUsageTotal, strShow);

		if (obj->configopen == true)
		{
			PID = obj->ProcessID[(obj->serverSelected == -1 ? 0 : obj->serverSelected)];
			if (PID != 0)
			{
				float AppUsage = GetProcessMemory(PID);

				strShow.Format(L"CPU: %.2f%%", GetProcessCPU(PID, 12));
				obj->SetDlgItemText(IDC_CPUUsage, strShow);

				strShow.Format(L"内存(服务端占用): %.2f%% [%.1f MB]", (AppUsage / physical_memory) * 100, AppUsage);
				obj->SetDlgItemText(IDC_MemUsageAppTotal, strShow);

				strShow.Format(L"内存(预分配占用): %.2f%% [%.1f MB]", (AppUsage / obj->mem_preAllocateMax) * 100, AppUsage);
				obj->SetDlgItemText(IDC_MemUsagePreAllocate, strShow);
			}

			for (int count = 0; count < obj->isServerRunning.size(); count++)
			{
				if (obj->isServerRunning[count] == false)
				{
					obj->m_ServerList.SetItemText(count, 1, L"0.00%");
					obj->m_ServerList.SetItemText(count, 2, L"0.00%");
					continue;
				}

				PID = obj->ProcessID[count];
				if (PID != 0)
				{
					float AppUsage = GetProcessMemory(PID);
					strShow.Format(L"%.2f%%", (AppUsage / physical_memory) * 100);
					obj->m_ServerList.SetItemText(count, 2, strShow);
					strShow.Format(L"%.2f%%", GetProcessCPU(PID, 12));
					obj->m_ServerList.SetItemText(count, 1, strShow);
				}
			}
		}
		Sleep(500);
	}

	return 0;
}

//Timer
void CBCSManagerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case 1:
		if (configopen == true)
		{
			AutoRebootTick();
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CBCSManagerDlg::KillProcess(DWORD PID)
{
	BOOL BKilling = FALSE;
	HANDLE hSnapShot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot == 0) 
	{
		return FALSE;
	}

	PROCESSENTRY32 thePE;
	thePE.dwSize = sizeof(PROCESSENTRY32);

	bool Status = Process32First(hSnapShot, &thePE);
	CString pathtmp, loginfo;
	while (Status)
	{
		Status = Process32Next(hSnapShot, &thePE);
		if (thePE.th32ProcessID == PID)
		{
			HANDLE han = OpenProcess(PROCESS_TERMINATE || PROCESS_QUERY_INFORMATION, false, PID);
			if (!TerminateProcess(han, 0))
			{
				break;
			}
			else
			{
				BKilling = TRUE;
				break;
			}
		}
	}
	CloseHandle(hSnapShot);
	return BKilling;
}

//Shutdown Server
void CBCSManagerDlg::OnBnClickedShutdown()
{
	if (CheckConfigOpen() == false)
		return;

	CString PropertiesPath,PropertiesFile;
	PropertiesPath = ReadConfig(currentConfig, L"CorePath"); 
	PropertiesFile = PropertiesPath.Mid(0, PropertiesPath.ReverseFind('\\')) + L"\\server.properties";

	CFileFind finder;
	BOOL ifFind = finder.FindFile(PropertiesFile);

	if (isServerRunning[serverSelected] == true)
	{
		if (!ifFind)
		{
			KillProcess(ProcessID[serverSelected]);
		}
		else
		{
			SendInstruction(L"stop");
		}

		while (GetProcessName(ProcessID[serverSelected]) == L"java.exe")
		{
			Sleep(1000);
		}

		isServerRunning[serverSelected] = false;
		ProcessID[serverSelected] = 0;

		configopen = false;
		((CButton*)GetDlgItem(IDC_AutoReboot))->SetCheck(0);
		isAutoReboot[serverSelected] = false;
		WriteConfig(currentConfig, L"AutoReboot",L"false");
		configopen = false;
	}
}

//Get ProcessName by PID
CString CBCSManagerDlg::GetProcessName(DWORD PID)
{
	PROCESSENTRY32 pe32; 
	pe32.dwSize = sizeof(pe32);

	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return L"";
	}
 
	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (pe32.th32ProcessID == PID)
		{
			break;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	} 
	CloseHandle(hProcessSnap);
	return pe32.szExeFile;
}

//LaunchServerFromConfig
DWORD CBCSManagerDlg::LaunchServerFromConfig(CString configName)
{
	STARTUPINFO sInfo;
	PROCESS_INFORMATION pInfo;

	memset(&sInfo, 0, sizeof(STARTUPINFO));
	memset(&pInfo, 0, sizeof(PROCESS_INFORMATION));

	sInfo.cb = sizeof(STARTUPINFO);
	sInfo.dwFlags = STARTF_USESHOWWINDOW;
	sInfo.wShowWindow = SW_SHOW;

	CString CorePath, Dict;
	CorePath = ReadConfig(configName, L"CorePath");
	Dict = CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\";

	CString MemAllocateSet = ReadConfig(configName, L"MemoryAllocate"),
		JVMExtraParam = ReadConfig(configName, L"JVMExtraParam"),
		launchParam,
		preAlocMemMin = L"-Xms" + MemAllocateSet.Mid(0, MemAllocateSet.ReverseFind('-')),
	    preAlocMemMax = L"-Xmx" + MemAllocateSet.Mid(MemAllocateSet.ReverseFind('-') + 1, MemAllocateSet.GetLength() - MemAllocateSet.ReverseFind('-') - 1);

	launchParam = 
		L" " + preAlocMemMin +
		L" " + preAlocMemMax +
		(JVMExtraParam == L"" ? L"" : (L" " + JVMExtraParam)) +
		L" -jar " + CorePath.Mid(CorePath.ReverseFind('\\') + 1, CorePath.GetLength() - CorePath.ReverseFind('\\')) +
		L" nogui";

	CreateProcess(ReadConfig(configName,L"JavaPath"), /* 待运行的程序 */
		launchParam.GetBuffer(), /* 传递给待运行程序的参数 */

		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		Dict,
		&sInfo,
		&pInfo
	);
	launchParam.ReleaseBuffer();

	SetProcessAffinityMask(pInfo.hProcess, wcstol(GetAffinityMask(configName), NULL, 2));

	CloseHandle(pInfo.hProcess);
	CloseHandle(pInfo.hThread);

	return pInfo.dwProcessId;
}

//Launch Queue
void CBCSManagerDlg::LaunchQueue()
{
	CString PID;
	int pid = 0;
	for (int count = 0; count < serverLaunch.size(); count++)
	{
		if (serverLaunch[count] == true)
		{
			if (isServerRunning[count] == false)
			{
				pid = LaunchServerFromConfig(configLoaded[count]);
				PID.Format(L"%d", pid);
				isServerRunning[count] = true;
				WriteConfig(configLoaded[count], L"", PID);
				Sleep(500);
				SetConhostTitle(pid, ReadConfig(configLoaded[count], L"ServerName"), configLoaded[count]);
			}
		}
	}
}

//SetConhostTitle by PID
void CBCSManagerDlg::SetConhostTitle(DWORD PID, CString title, CString configName)
{
	CMainHWnd a;
	a.EnumWndsByPid(PID);
	a.SetWindowTitle(title); 
	CString winhwnd;
	winhwnd.Format(L"%d", a.GetWinHWND());
	WriteConfig(configName, L"LastWinHWND",winhwnd);
}

//GetHwnd by PID
HWND CBCSManagerDlg::GetWindowHwnd(DWORD PID)
{
	CMainHWnd a;
	a.EnumWndsByPid(PID);
	return a.GetWinHWND();
}

//Show/Hide Conhost Window
void CBCSManagerDlg::OnBnClickedBtnShconhost()
{
	if (CheckConfigOpen() == false)
		return;
	if (isServerRunning[serverSelected] == false)
		return;

	int pid = _ttoi(ReadConfig(currentConfig, L"LastWinHWND"));
	HWND conhostHwnd = (HWND)pid;

	if (isconhostshow == false)
	{
		::ShowWindow(conhostHwnd, SW_SHOWNORMAL);
		isconhostshow = true;
	}
	else
	{
		::ShowWindow(conhostHwnd, SW_HIDE);
		isconhostshow = false;
	}
}

//SendIns
void CBCSManagerDlg::SendInstruction(CString ins)
{
	int pid = _ttoi(ReadConfig(currentConfig, L"LastWinHWND"));
	HWND conhostHwnd = (HWND)pid;

	for (int i = 0; i < ins.GetLength(); i++)
	{
		wchar_t psText_Unicode[1] = { 's' };
		wcscpy(psText_Unicode, (LPCWSTR)ins.Mid(i, 1));
		::SendMessage(conhostHwnd, WM_IME_CHAR, psText_Unicode[0], 0);
	}
	Sleep(500);
	::PostMessage(conhostHwnd, WM_KEYDOWN, 13, 0);
	::PostMessage(conhostHwnd, WM_KEYUP, 13, 0);
}

//Affinity Mask Generator
CString CBCSManagerDlg::GetAffinityMask(CString configName)
{
	CString Mask = L"",str;

	for (int count = CPUCores - 1; count >= 0; count--)
	{
		str.Format(L"CPU%d", count);
		if (ReadConfig(configName, str) == L"true")
		{
			Mask += L"1";
		}
		else
		{
			Mask += L"0";
		}
	}

	return Mask;
}

//Auto Reboot Tick
void CBCSManagerDlg::AutoRebootTick()
{
	CString PID;
	for (int count = 0; count < isServerRunning.size(); count++)
	{
		if (GetProcessName(ProcessID[count]) != L"java.exe" && isAutoReboot[count] == true)
		{
			configopen = false;
			ProcessID[count] = LaunchServerFromConfig(configLoaded[count]);
			isServerRunning[count] = true;
			PID.Format(L"%d", ProcessID[count]);
			WriteConfig(configLoaded[count], L"LastProcessID", PID);
			Sleep(500);
			SetConhostTitle(ProcessID[count], ReadConfig(configLoaded[count], L"ServerName"), configLoaded[count]);
			configopen = true;
		}
		else if (GetProcessName(ProcessID[count]) != L"java.exe" && isAutoReboot[count] == false)
		{
			isServerRunning[count] = false;
			WriteConfig(configLoaded[count], L"LastProcessID", L"");
		}
	}
}

//Help Info MemAllocate
void CBCSManagerDlg::OnBnClickedBtnHelpMemallocate()
{
	MessageBox(L"格式: 最小值-最大值(目前仅支持M)\r\n例: 128M-4096M");
}


void CBCSManagerDlg::OnBnClickedBtnHelpJvmextraparam()
{
	MessageBox(L"每个参数之间用空格隔开.\r\n例: -server -XX:+UseG1GC");
}

//Add Plugin
void CBCSManagerDlg::OnMenuPluginslistAddplugin()
{
	if (CheckConfigOpen() == false)
		return;

	if (serverLaunch[serverSelected] == true)
		return;

	CString CorePath;
	CorePath = ReadConfig(currentConfig, L"CorePath");
	ShellExecute(NULL, NULL, CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\plugins", NULL, NULL, SW_SHOW);

	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_READONLY, L"Java Plugin|*.jre||", NULL);
	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK)
	{
		openFileDlg.GetPathName();
		CopyFile(openFileDlg.GetPathName(), CorePath.Mid(0, CorePath.ReverseFind('\\')) + L"\\plugins\\" + openFileDlg.GetFileTitle(), FALSE);
		LoadPlugins(currentConfig);
	}
}
