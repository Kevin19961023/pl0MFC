
// MFCApplication1Dlg.h : ͷ�ļ�
//

#pragma once
#include <string>
#include "pl0.h"
using namespace std;


// CMFCCompilerDlg �Ի���
class CMFCCompilerDlg : public CDialogEx
{
	//�Զ���
public:
	CPl0 m_compiler;
	void decodePl0(string res);


// ����
public:
	CMFCCompilerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };

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
public:
	afx_msg void OnBnClickedButtonTran();
	CString m_srcCode;
};
