
// DES CryptoanalysisDlg.h : header file
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS

// CDESCryptoanalysisDlg dialog
class CDESCryptoanalysisDlg : public CDialogEx
{
// Construction
public:
	CDESCryptoanalysisDlg(CWnd* pParent = nullptr);	// standard constructor
	CFont font;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCRYPTOANALYSIS_DIALOG };
#endif

	protected:

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedRadio2();
};
