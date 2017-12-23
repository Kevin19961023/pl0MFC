
// MFCApplication1Dlg.h : 头文件
//

#pragma once
#include <string>
#include "pl0.h"
using namespace std;


// CMFCCompilerDlg 对话框
class CMFCCompilerDlg : public CDialogEx
{
	//自定义
public:
	CPl0 m_compiler;
	void decodePl0(string res);


// 构造
public:
	CMFCCompilerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

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
public:
	afx_msg void OnBnClickedButtonTran();
	CString m_srcCode;
};
