
// TextureProcessSystem.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "TextureProcessSystem.h"
#include "MainFrm.h"

#include "TextureProcessSystemDoc.h"
#include "TextureProcessSystemView.h"
#include "GLBaseView.h"

#include "FindTextureElementPosition.h"
#include "Parameterization.h"
#include "LocalParameterization.h"
#include <fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTextureProcessSystemApp

BEGIN_MESSAGE_MAP(CTextureProcessSystemApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTextureProcessSystemApp::OnAppAbout)
	ON_COMMAND(ID_BUTTON_SETVALUE_L, &CTextureProcessSystemApp::OnSetValueL)
	ON_COMMAND(ID_BUTTON_SETVALUE_C, &CTextureProcessSystemApp::OnSetValueC)
	ON_COMMAND(ID_BUTTON_SETVALUE_G, &CTextureProcessSystemApp::OnSetValueG)
	ON_COMMAND(ID_BUTTON_SETVALUE_M, &CTextureProcessSystemApp::OnSetValueM)
	ON_COMMAND(ID_BUTTON_SETVALUE_T, &CTextureProcessSystemApp::OnSetValueT)
	ON_COMMAND(ID_BUTTON_CHECK, &CTextureProcessSystemApp::Check)
	ON_COMMAND(ID_BUTTON_SELECTFACE, &CTextureProcessSystemApp::SelectFace)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTextureProcessSystemApp 构造

CTextureProcessSystemApp::CTextureProcessSystemApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("TextureProcessSystem.AppID.NoVersion"));

	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CTextureProcessSystemApp 对象

CTextureProcessSystemApp theApp;


// CTextureProcessSystemApp 初始化

BOOL CTextureProcessSystemApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO:  应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTextureProcessSystemDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CTextureProcessSystemView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->SetWindowText("纹理处理系统");
	m_pMainWnd->UpdateWindow();


	
	return TRUE;
}

int CTextureProcessSystemApp::ExitInstance()
{
	//TODO:  处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CTextureProcessSystemApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CTextureProcessSystemApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
void CTextureProcessSystemApp::OnSetValue(char rbffuntion)
{
	

	CFrameWnd* pMain=(CFrameWnd*)CWinThread::m_pMainWnd;
	CTextureProcessSystemDoc * pDoc = (CTextureProcessSystemDoc*)pMain->CFrameWnd::GetActiveDocument();

	
	pDoc->calculateValue(rbffuntion);
	CString s1;
	s1.Format("%s 插值",&rbffuntion);
	AfxMessageBox(s1);

}
void CTextureProcessSystemApp::OnSetValueL()
{
	OnSetValue('l');

}void CTextureProcessSystemApp::OnSetValueC()
{
	OnSetValue('c');
}

void CTextureProcessSystemApp::OnSetValueG()
{
	OnSetValue('g');
}
void CTextureProcessSystemApp::OnSetValueM()
{
	OnSetValue('m');
}
void CTextureProcessSystemApp::OnSetValueT()
{
	OnSetValue('t');

}

// CTextureProcessSystemApp 消息处理程序
void CTextureProcessSystemApp::SelectFace()
{
	CFrameWnd* pMain=(CFrameWnd*)CWinThread::m_pMainWnd;
	CTextureProcessSystemDoc * pDoc = (CTextureProcessSystemDoc*)pMain->CFrameWnd::GetActiveDocument();
	//pDoc->markConnectedFace(0,0 ,1000);
	/*点选
	pDoc->markConnectedFace(0.5,0.9659258262890683 ,10000);
	//cos5=0.9961946980917455
	//cos10=0.984807753012208 
	//cos15=0.9659258262890683
	//cos20=0.9396926207859084
	//cos25=0.9063077870366499
	//cos30=0.8660254037844387
	//cos45=0.7071067811865476
	//cos60=0.5000000000000000
	
	int count=0;
	for(int i=0;i<pDoc->plyLoader.faceArry.size();i++)
	{
		if(pDoc->plyLoader.faceArry.at(i).isShowColorIn3D)
		{
			count++;
		}	
	}

	CString s1;
	s1.Format("Select %d face",count);
	AfxMessageBox(s1);
	*/
	//加载样本
	FindTextureElementPosition *_ftep=new FindTextureElementPosition();
	pDoc->_ftep = _ftep;
	_ftep->init(pDoc);
	CString s1;
	s1.Format("%d ",_ftep->m_targetTexture->tes.size());
	AfxMessageBox(s1);

}
void CTextureProcessSystemApp::Check()
{
	CFrameWnd* pMain=(CFrameWnd*)CWinThread::m_pMainWnd;
	CTextureProcessSystemDoc * pDoc = (CTextureProcessSystemDoc*)pMain->CFrameWnd::GetActiveDocument();
	
	pDoc->selectfaceLsit.clear();

	int size=pDoc->plyLoader.faceArry.size();
	pDoc->count=0;
	int times=0;//展开次数
	int lastCount=0;
	int tempCenter=0;
	int newCenter=0;
	double minCos=1;	
	//为每个基元贴图
	/*ofstream f("triangleIndex.txt");
	for (int i = 0; i < pDoc->_ftep->m_targetTexture->tes.size(); i++)
	{
		//pDoc->texGenTime++;
		if (!pDoc->_ftep->m_targetTexture->tes[i]->isShow) continue;
		pDoc->calVertex2D(pDoc->_ftep->m_targetTexture->tes[i]->pos, pDoc->_ftep->m_targetTexture->tes[i]->face->facenum);
		f << pDoc->_ftep->m_targetTexture->tes[i]->face->facenum << endl;
		pDoc->calTexCorByIndex(pDoc->_ftep->m_targetTexture->tes[i]->face->facenum, 8);
	}
	f.close();
	*/
	/*
	
	pDoc->calTexCorByIndex(1585, 8);
	pDoc->calTexCorByIndex(1587, 8);
	pDoc->calTexCorByIndex(3700, 8);
	pDoc->calTexCorByIndex(3786, 8);
	pDoc->calTexCorByIndex(1690, 8);
	pDoc->calTexCorByIndex(3701, 8);
	*/
	//pDoc->buildTexCoordByIndex(0, 5,20);
	//pDoc->calTexCorByIndex(1585, 8);
	pDoc->buildTexCoordByIndex(1585, 50,50,0.04);
	/*pDoc->buildTexCoordByIndex(1587, 4);
	pDoc->buildTexCoordByIndex(3700, 4);
	pDoc->buildTexCoordByIndex(3786, 4);
	pDoc->buildTexCoordByIndex(1690, 4);
	pDoc->buildTexCoordByIndex(3701, 4);*/
	pDoc->logTex = true;
	pDoc->show_ftep = false; 
}


//CString s1;
//s1.Format("新添加:  %d,times: %d", pDoc->count - lastCount, times);
//lastCount = pDoc->count;
//AfxMessageBox(s1);
