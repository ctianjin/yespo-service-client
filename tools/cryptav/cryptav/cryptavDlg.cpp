cryptavDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "cryptav.h"
#include "cryptavDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CRYPT_LENGTH 1024 * 1000

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CcryptavDlg 对话框




CcryptavDlg::CcryptavDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CcryptavDlg::IDD, pParent)
	, m_strDirectory(_T(""))
	, m_strProgress(_T(""))
	, m_strPassword(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcryptavDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATICDR, m_strDirectory);
	DDX_Text(pDX, IDC_STATICPG, m_strProgress);
	DDX_Text(pDX, IDC_EDITPS, m_strPassword);
}

BEGIN_MESSAGE_MAP(CcryptavDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTONFD, &CcryptavDlg::OnBnClickedButtonfd)
	ON_BN_CLICKED(IDC_BUTTONCR, &CcryptavDlg::OnBnClickedButtoncr)
	ON_BN_CLICKED(IDC_BUTTONDECR, &CcryptavDlg::OnBnClickedButtondecr)
END_MESSAGE_MAP()


// CcryptavDlg 消息处理程序

BOOL CcryptavDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_strPassword = _T("shitfromMOUTH");
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CcryptavDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcryptavDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcryptavDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CcryptavDlg::CryptFile(HCRYPTKEY hKey, LPCTSTR fn)
{
	CFile f;
	f.Open(fn,CFile::modeReadWrite);

	DWORD dwBufferLen, dwBlockSize , dwRemain, dwReadCnt;

	dwBufferLen = CRYPT_LENGTH ;
	BYTE * p = new BYTE[dwBufferLen + 1024] ;

	BYTE* lp = p;

	f.SeekToBegin();
	dwReadCnt = f.Read(p, CRYPT_LENGTH);

    CryptEncrypt(hKey, NULL, TRUE, 0, NULL, &dwBufferLen, dwBufferLen) ;

	dwBlockSize = dwBufferLen ;
            
	dwRemain = dwReadCnt;
    // 分段加密
    while ( dwRemain > 0)
	{		
		if(dwRemain >  dwBlockSize ) {
			CryptEncrypt( hKey, 0, FALSE, 0, (BYTE*)lp, &dwBlockSize, dwBufferLen ) ;
		} else {
			CryptEncrypt( hKey, 0, TRUE, 0, (BYTE*)lp, &dwRemain, dwBufferLen ) ;
		}
		lp += dwBlockSize;
		dwRemain -= dwBlockSize ;
	}

	f.SeekToBegin();
	f.Write(p, dwReadCnt);

	f.Close();

	delete [] p ;
}
void CcryptavDlg::DecryptFile(HCRYPTKEY hKey, LPCTSTR fn)
{
	CFile f;
	f.Open(fn,CFile::modeReadWrite);

	DWORD dwBufferLen, dwBlockSize , dwRemain, dwReadCnt;
	
	dwBufferLen = CRYPT_LENGTH ;
	BYTE * p = new BYTE[dwBufferLen + 1024] ;

	BYTE* lp = p;

	f.SeekToBegin();
	dwReadCnt = f.Read(p, CRYPT_LENGTH);

    CryptEncrypt(hKey, NULL, TRUE, 0, NULL, &dwBufferLen, dwBufferLen) ;

	dwBlockSize = dwBufferLen ;
            
	dwRemain = dwReadCnt;
    // 分段解密
    while ( dwRemain > 0)
	{		
		if(dwRemain >  dwBlockSize ) {
			CryptDecrypt( hKey, 0, FALSE, 0, (BYTE*)lp, &dwReadCnt ) ;
		} else {
			CryptDecrypt( hKey, 0, TRUE, 0, (BYTE*)lp, &dwReadCnt ) ;
		}
		lp += dwBlockSize;
		dwRemain -= dwBlockSize ;
	}

	f.SeekToBegin();
	f.Write(p, dwReadCnt);

	f.Close();

	delete [] p ;
}

void CcryptavDlg::OnBnClickedButtonfd()
{
	// TODO: 在此添加控件通知处理程序代码
    WCHAR szPath[MAX_PATH];     //存放选择的目录路径 
    CString str;

    ZeroMemory(szPath, sizeof(szPath));   

    BROWSEINFO bi;   
    bi.hwndOwner = m_hWnd;   
    bi.pidlRoot = NULL;   
    bi.pszDisplayName = (LPWSTR)szPath;   
    bi.lpszTitle = _T("请选择加密文件所在的目录：");   
    bi.ulFlags = 0;   
    bi.lpfn = NULL;   
    bi.lParam = 0;   
    bi.iImage = 0;   
    //弹出选择目录对话框
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

    if(lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))   
    {
		m_strDirectory = szPath;
		UpdateData(FALSE);
    }
    else   
        AfxMessageBox(_T("无效的目录，请重新选择")); 
}

void CcryptavDlg::OnBnClickedButtoncr()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData() ;

	if(m_strDirectory.IsEmpty()){
        AfxMessageBox(_T("无效的目录，请重新选择")); 
		return;
	}

	if(m_strPassword.IsEmpty()){
        AfxMessageBox(_T("无效的密码，请重新输入")); 
		return;
	}


    BOOL bResult ;

    HCRYPTPROV hCryptoProv ;
	TCHAR UserName[] = TEXT("sych888") ;

	if( !CryptAcquireContext( // 不存在！
		&hCryptoProv,         //返回的CSP句柄
		UserName,             //密码容器名,为NULL时表示使用默认密钥容器，
		                      // 默认密钥容器名为用户登录名
		//NULL,
		NULL,                 //NULL时使用默认CSP名（微软RSA Base Provider）
		PROV_RSA_FULL,        //CSP类型
		0) )                   //Flag values
	{
		CryptAcquireContext(
			&hCryptoProv,
			UserName, 
			//NULL,
			NULL,
			PROV_RSA_FULL,
			CRYPT_NEWKEYSET) ;//创建以UserName为名的密钥容器
	}

	//创建密钥容器成功，并得到CSP句柄
    HCRYPTHASH hHash ;
	//DWORD dwDataLen ;
	HCRYPTKEY hKey ;
	// Get hash object!
    bResult = CryptCreateHash(hCryptoProv, CALG_MD5, 0, 0, &hHash) ;
    //bResult = CryptCreateHash(hCryptoProv, CALG_SHA1, 0, 0, &hHash) ;

	// 散列口令
	CryptHashData(hHash, (unsigned char *)m_strPassword.GetBuffer(), m_strPassword.GetLength(), 0);

	// 从散列表中派生密钥
    CryptDeriveKey(hCryptoProv, CALG_RC4, hHash, 0, &hKey);
	//CryptDeriveKey(hCryptoProv, ENCRYPT_ALGORITHM, hHash, 0, &hKey);

	// 删除散列表
	CryptDestroyHash(hHash);

	CString ff = m_strDirectory + _T("\\*.mp3");
	CFileFind cf;
	BOOL bWorking = cf.FindFile(ff);
	while(bWorking) {
	    bWorking = cf.FindNextFile();
		
		m_strProgress = cf.GetFileName() + _T(" 加密处理中......");
		UpdateData(FALSE);
		CryptFile(hKey, cf.GetFilePath());
	}

	m_strProgress = _T("加密处理结束。");
	UpdateData(FALSE);
    // 销毁密钥
	CryptDestroyKey(hKey) ;

	CryptReleaseContext( hCryptoProv, 0 ) ;

}


void CcryptavDlg::OnBnClickedButtondecr()
{
	UpdateData() ;

	if(m_strDirectory.IsEmpty()){
        AfxMessageBox(_T("无效的目录，请重新选择")); 
		return;
	}

	if(m_strPassword.IsEmpty()){
        AfxMessageBox(_T("无效的密码，请重新输入")); 
		return;
	}


	// TODO: 在此添加控件通知处理程序代码
	BOOL bResult;

	HCRYPTPROV hCryptoProv;
	TCHAR UserName[] = TEXT("sych888") ;
	if( !CryptAcquireContext( // 不存在！
		&hCryptoProv,         //返回的CSP句柄
		UserName,             //密码容器名,为NULL时表示使用默认密钥容器，
		                      // 默认密钥容器名为用户登录名
		//NULL,
		NULL,                 //NULL时使用默认CSP名（微软RSA Base Provider）
		PROV_RSA_FULL,        //CSP类型
		0) )                   //Flag values
	{
		CryptAcquireContext(
			&hCryptoProv,
			UserName, 
			//NULL,
			NULL,
			PROV_RSA_FULL,
			CRYPT_NEWKEYSET) ;//创建以UserName为名的密钥容器
	}

    HCRYPTHASH hHash ;
	HCRYPTKEY hKey ;
    bResult = CryptCreateHash(hCryptoProv, CALG_MD5, 0, 0, &hHash) ;

	// 散列口令
	CryptHashData(hHash, (unsigned char *)m_strPassword.GetBuffer(), m_strPassword.GetLength(), 0);

	// 从散列表中派生密钥
    CryptDeriveKey(hCryptoProv, CALG_RC4, hHash, 0, &hKey);
	//CryptDeriveKey(hCryptoProv, ENCRYPT_ALGORITHM, hHash, 0, &hKey);

	// 删除散列表
	CryptDestroyHash(hHash);
	// 使用密钥

	CString ff = m_strDirectory + _T("\\*.mp3");
	CFileFind cf;
	BOOL bWorking = cf.FindFile(ff);
	while(bWorking) {
	    bWorking = cf.FindNextFile();
		
		m_strProgress = cf.GetFileName() + _T(" 解密处理中......");
		UpdateData(FALSE);

		DecryptFile(hKey, cf.GetFilePath());
	}

	m_strProgress = _T("解密处理结束。");
	UpdateData(FALSE);

    // 销毁密钥
	CryptDestroyKey(hKey) ;
    
	CryptReleaseContext( hCryptoProv, 0 ) ;
}
