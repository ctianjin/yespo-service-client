cryptavDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "cryptav.h"
#include "cryptavDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define CRYPT_LENGTH 1024 * 1000

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CcryptavDlg �Ի���




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


// CcryptavDlg ��Ϣ�������

BOOL CcryptavDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_strPassword = _T("shitfromMOUTH");
	UpdateData(FALSE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CcryptavDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
    // �ֶμ���
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
    // �ֶν���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
    WCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
    CString str;

    ZeroMemory(szPath, sizeof(szPath));   

    BROWSEINFO bi;   
    bi.hwndOwner = m_hWnd;   
    bi.pidlRoot = NULL;   
    bi.pszDisplayName = (LPWSTR)szPath;   
    bi.lpszTitle = _T("��ѡ������ļ����ڵ�Ŀ¼��");   
    bi.ulFlags = 0;   
    bi.lpfn = NULL;   
    bi.lParam = 0;   
    bi.iImage = 0;   
    //����ѡ��Ŀ¼�Ի���
    LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

    if(lp && SHGetPathFromIDList(lp, (LPWSTR)szPath))   
    {
		m_strDirectory = szPath;
		UpdateData(FALSE);
    }
    else   
        AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��")); 
}

void CcryptavDlg::OnBnClickedButtoncr()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData() ;

	if(m_strDirectory.IsEmpty()){
        AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��")); 
		return;
	}

	if(m_strPassword.IsEmpty()){
        AfxMessageBox(_T("��Ч�����룬����������")); 
		return;
	}


    BOOL bResult ;

    HCRYPTPROV hCryptoProv ;
	TCHAR UserName[] = TEXT("sych888") ;

	if( !CryptAcquireContext( // �����ڣ�
		&hCryptoProv,         //���ص�CSP���
		UserName,             //����������,ΪNULLʱ��ʾʹ��Ĭ����Կ������
		                      // Ĭ����Կ������Ϊ�û���¼��
		//NULL,
		NULL,                 //NULLʱʹ��Ĭ��CSP����΢��RSA Base Provider��
		PROV_RSA_FULL,        //CSP����
		0) )                   //Flag values
	{
		CryptAcquireContext(
			&hCryptoProv,
			UserName, 
			//NULL,
			NULL,
			PROV_RSA_FULL,
			CRYPT_NEWKEYSET) ;//������UserNameΪ������Կ����
	}

	//������Կ�����ɹ������õ�CSP���
    HCRYPTHASH hHash ;
	//DWORD dwDataLen ;
	HCRYPTKEY hKey ;
	// Get hash object!
    bResult = CryptCreateHash(hCryptoProv, CALG_MD5, 0, 0, &hHash) ;
    //bResult = CryptCreateHash(hCryptoProv, CALG_SHA1, 0, 0, &hHash) ;

	// ɢ�п���
	CryptHashData(hHash, (unsigned char *)m_strPassword.GetBuffer(), m_strPassword.GetLength(), 0);

	// ��ɢ�б���������Կ
    CryptDeriveKey(hCryptoProv, CALG_RC4, hHash, 0, &hKey);
	//CryptDeriveKey(hCryptoProv, ENCRYPT_ALGORITHM, hHash, 0, &hKey);

	// ɾ��ɢ�б�
	CryptDestroyHash(hHash);

	CString ff = m_strDirectory + _T("\\*.mp3");
	CFileFind cf;
	BOOL bWorking = cf.FindFile(ff);
	while(bWorking) {
	    bWorking = cf.FindNextFile();
		
		m_strProgress = cf.GetFileName() + _T(" ���ܴ�����......");
		UpdateData(FALSE);
		CryptFile(hKey, cf.GetFilePath());
	}

	m_strProgress = _T("���ܴ��������");
	UpdateData(FALSE);
    // ������Կ
	CryptDestroyKey(hKey) ;

	CryptReleaseContext( hCryptoProv, 0 ) ;

}


void CcryptavDlg::OnBnClickedButtondecr()
{
	UpdateData() ;

	if(m_strDirectory.IsEmpty()){
        AfxMessageBox(_T("��Ч��Ŀ¼��������ѡ��")); 
		return;
	}

	if(m_strPassword.IsEmpty()){
        AfxMessageBox(_T("��Ч�����룬����������")); 
		return;
	}


	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BOOL bResult;

	HCRYPTPROV hCryptoProv;
	TCHAR UserName[] = TEXT("sych888") ;
	if( !CryptAcquireContext( // �����ڣ�
		&hCryptoProv,         //���ص�CSP���
		UserName,             //����������,ΪNULLʱ��ʾʹ��Ĭ����Կ������
		                      // Ĭ����Կ������Ϊ�û���¼��
		//NULL,
		NULL,                 //NULLʱʹ��Ĭ��CSP����΢��RSA Base Provider��
		PROV_RSA_FULL,        //CSP����
		0) )                   //Flag values
	{
		CryptAcquireContext(
			&hCryptoProv,
			UserName, 
			//NULL,
			NULL,
			PROV_RSA_FULL,
			CRYPT_NEWKEYSET) ;//������UserNameΪ������Կ����
	}

    HCRYPTHASH hHash ;
	HCRYPTKEY hKey ;
    bResult = CryptCreateHash(hCryptoProv, CALG_MD5, 0, 0, &hHash) ;

	// ɢ�п���
	CryptHashData(hHash, (unsigned char *)m_strPassword.GetBuffer(), m_strPassword.GetLength(), 0);

	// ��ɢ�б���������Կ
    CryptDeriveKey(hCryptoProv, CALG_RC4, hHash, 0, &hKey);
	//CryptDeriveKey(hCryptoProv, ENCRYPT_ALGORITHM, hHash, 0, &hKey);

	// ɾ��ɢ�б�
	CryptDestroyHash(hHash);
	// ʹ����Կ

	CString ff = m_strDirectory + _T("\\*.mp3");
	CFileFind cf;
	BOOL bWorking = cf.FindFile(ff);
	while(bWorking) {
	    bWorking = cf.FindNextFile();
		
		m_strProgress = cf.GetFileName() + _T(" ���ܴ�����......");
		UpdateData(FALSE);

		DecryptFile(hKey, cf.GetFilePath());
	}

	m_strProgress = _T("���ܴ��������");
	UpdateData(FALSE);

    // ������Կ
	CryptDestroyKey(hKey) ;
    
	CryptReleaseContext( hCryptoProv, 0 ) ;
}
