// cryptavDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "Wincrypt.h"

// CcryptavDlg 对话框
class CcryptavDlg : public CDialog
{
// 构造
public:
	CcryptavDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CRYPTAV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
