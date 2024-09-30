
// FileScannerDlg.cpp : implementation file

#include "pch.h"
#include "framework.h"
#include "FileScanner.h"
#include "FileScannerDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CFileScannerDlg dialog



CFileScannerDlg::CFileScannerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILESCANNER_DIALOG, pParent)
	, m_strQuery(_T(""))
	, m_strSearch(_T("1111"))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileScannerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BROWSE, m_strQuery);
	DDX_Text(pDX, IDC_SEARCH, m_strSearch);
	DDX_Control(pDX, IDC_LIST, m_ctrlListBox);
}

BEGIN_MESSAGE_MAP(CFileScannerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_EN_CHANGE(IDC_SEARCH, &CFileScannerDlg::OnEnChangeSearch)
	ON_EN_CHANGE(IDC_BROWSE, &CFileScannerDlg::OnEnChangeBrowse)
	ON_BN_CLICKED(IDOK, &CFileScannerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFileScannerDlg::OnBnClickedCancel)
	ON_MESSAGE(WM_COMPLETE, &CFileScannerDlg::OnComplete)
END_MESSAGE_MAP()


// CFileScannerDlg message handlers

BOOL CFileScannerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileScannerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
// d0n'7 l3@rn t0 h4ck, h4ck t0 l3@rn

void CFileScannerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFileScannerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileScannerDlg::LockControls() {
	auto pBrowseBox = GetDlgItem(IDC_BROWSE);
	auto pSearchBox = GetDlgItem(IDC_SEARCH);
	auto pButtonOk = GetDlgItem(IDOK);
	auto pButtonCancel = GetDlgItem(IDCANCEL);
	pBrowseBox->EnableWindow(FALSE);
	pSearchBox->EnableWindow(FALSE);
	pButtonOk->EnableWindow(FALSE);
}

void CFileScannerDlg::UnlockControls() {
	auto pBrowseBox = GetDlgItem(IDC_BROWSE);
	auto pSearchBox = GetDlgItem(IDC_SEARCH);
	auto pButtonOk = GetDlgItem(IDOK);
	auto pButtonCancel = GetDlgItem(IDCANCEL);
	pBrowseBox->EnableWindow();
	pSearchBox->EnableWindow();
	pButtonOk->EnableWindow();
}


ULONGLONG GetFileSize(const CString& filePath) {
	CFileFind finder;
	if (finder.FindFile(filePath)) {
		finder.FindNextFile();
		ULONGLONG fileSize = finder.GetLength();
		finder.Close();
		return fileSize;
	}
	return 0;
}

std::vector<int> prefix_function(std::string s) {
	int n = (int)s.length();
	std::vector<int> pi(n);
	for (int i = 1; i < n; i++) {
		int j = pi[i - 1];
		while (j > 0 && s[i] != s[j])
			j = pi[j - 1];
		if (s[i] == s[j])
			j++;
		pi[i] = j;
	}
	return pi;
}


int CFileScannerDlg::CheckHexCode(const CString& filePath) {
	CFile file;
	if (file.Open(filePath, CFile::modeRead | CFile::shareDenyNone)) {
		BYTE buffer[16];
		UINT bytesRead;
		std::ostringstream hexStream;

		while ((bytesRead = file.Read(buffer, sizeof(buffer))) > 0) {
			for (UINT i = 0; i < bytesRead; ++i) {
				hexStream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(buffer[i]);
			}
		}
		file.Close();
		std::string strHex = hexStream.str();
		std::string search(static_cast<const char*> (CStringA(m_strSearch)));

		std::vector<int> pi = prefix_function(search);
		int j = 0; // Ham tien to o vi tri dang xet cua xau strHex
		int n = search.length();
		int m = strHex.length();

		for (int i = 0; i < m; i++) {
			while (j > 0 && strHex[i] != search[j])
				j = pi[j - 1];
			if (strHex[i] == search[j])
				j++;
			if (j == n)
				return 1;
		}
		return 0;
	}
	else {
		std::cerr << "Khong the mo tep tin." << std::endl;
		return -1;
	}
}


void CFileScannerDlg::BrowseFile(const CString& Path) {
	CFileFind finder;
	CString searchPath = Path + _T("\\*");

	BOOL bWorking = finder.FindFile(searchPath);

	while (bWorking) {
		bWorking = finder.FindNextFile();
		if (finder.IsDots()) continue;

		if (finder.IsDirectory()) {
			// Handle folder
			CString str = finder.GetFilePath();
			BrowseFile(str);
		}
		else {
			// Handle file
			CString filePath = finder.GetFilePath();
			if (GetFileSize(filePath) > 52428800) {
				m_ctrlListBox.AddString(_T("[!] File ") + filePath + _T(" có kích thước quá lớn; bỏ qua"));
				continue;
			}
			if (CheckHexCode(filePath) != 0) {
				m_ctrlListBox.AddString(_T("[+] Đã quét xong file ") + filePath + _T(", tìm thấy signature trong file!"));
				list.push_back(filePath);
			}
			else m_ctrlListBox.AddString(_T("[-] Đã quét xong file ") + filePath);
		}
	}
	finder.Close();
}


UINT CFileScannerDlg::FileScannerThreadProc(LPVOID Param) {
	
	CFileScannerDlg* myWnd = reinterpret_cast<CFileScannerDlg*>(Param);
	CFileFind finder;
	myWnd->m_strQuery += "*";

	BOOL bWorking = finder.FindFile(myWnd->m_strQuery);
	
	if (!bWorking) {
		// something's wrong, print the current error message
		TCHAR buf[256] = { 0 };
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			buf, sizeof(buf), NULL);
		AfxMessageBox(buf);
	}
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		myWnd->BrowseFile(finder.GetFilePath());
	}
	finder.Close();
	if (myWnd->list.size() != 0) {
		myWnd->m_ctrlListBox.AddString(_T("Danh sách các file tìm được:"));
		for (int i = 0; i < myWnd->list.size(); i++) {
			myWnd->m_ctrlListBox.AddString(myWnd->list[i]);
		}
	}
	myWnd->PostMessageW(WM_COMPLETE, myWnd->list.size());
	myWnd->UnlockControls();
	return 0;
}


void CFileScannerDlg::OnBnClickedOk()
{
	m_ctrlListBox.ResetContent();
	list.clear();
	LockControls();
	UpdateData(TRUE);
	if ((m_strSearch == "") || (m_strSearch.GetLength() % 2 != 0)) {
		MessageBox(_T("Signature sai định dạng"));
		return;
	}
	AfxBeginThread(FileScannerThreadProc, this);
}


void CFileScannerDlg::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}


afx_msg LRESULT CFileScannerDlg::OnComplete(WPARAM wParam, LPARAM lParam) {
	CString res;
	res.Format(TEXT("Tìm thấy %zu file chứa signature"), wParam);
	MessageBox(res);
	return 0;
}


void CFileScannerDlg::OnEnChangeSearch()
{

}


void CFileScannerDlg::OnEnChangeBrowse()
{
	
}