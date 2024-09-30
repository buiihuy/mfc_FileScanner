
// FileScannerDlg.h : header file
//

#pragma once
#include <vector>

#define WM_COMPLETE (WM_USER + 1)

// CFileScannerDlg dialog
class CFileScannerDlg : public CDialogEx
{
// Construction
public:
	CFileScannerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILESCANNER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


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
	CString m_strQuery;
	afx_msg void OnEnChangeSearch();
	afx_msg void OnEnChangeBrowse();
	CString m_strSearch;
	CListBox m_ctrlListBox;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	static UINT FileScannerThreadProc(LPVOID Param);
	void BrowseFile(const CString& Path);
	int CheckHexCode(const CString& fileName);
	std::vector<CString> list;
	afx_msg LRESULT OnComplete(WPARAM wParam, LPARAM lParam);
	void LockControls();
	void UnlockControls();  
};
