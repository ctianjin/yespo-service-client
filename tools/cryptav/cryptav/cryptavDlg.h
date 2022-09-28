// cryptavDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "Wincrypt.h"

// CcryptavDlg �Ի���
class CcryptavDlg : public CDialog
{
// ����
public:
	CcryptavDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CRYPTAV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	void CryptFile(HCRYPTKEY hKey, LPCTSTR fn);
	void DecryptFile(HCRYPTKEY hKey, LPCTSTR fn);

public:
	afx_msg void OnBnClickedButtonfd();
	CString m_strDirectory;
	CString m_strProgress;
	CString m_strPassword;
	afx_msg void OnBnClickedButtoncr();
	afx_msg void OnBnClickedButtondecr();
};
