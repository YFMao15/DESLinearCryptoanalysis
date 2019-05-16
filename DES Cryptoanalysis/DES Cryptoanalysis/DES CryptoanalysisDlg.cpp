// DES CryptoanalysisDlg.cpp : implementation file
// Every .cpp file needs stdafx, since it is a MFC program

#include "stdafx.h"
#include "cmath"
#include "string.h"
#include "CFolderDlg.h"
#include "Process.h"
#include "Linear Attack.h"
#include "UtilFunction.h"
#include "DES Cryptoanalysis.h"
#include "DES CryptoanalysisDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define _CRT_SECURE_NO_WARNINGS
#endif


////////////////////////////////////////////////////////////////////////////
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

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////////////
// CDESCryptoanalysisDlg dialog

CDESCryptoanalysisDlg::CDESCryptoanalysisDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DESCRYPTOANALYSIS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

BEGIN_MESSAGE_MAP(CDESCryptoanalysisDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDESCryptoanalysisDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDESCryptoanalysisDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CDESCryptoanalysisDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &CDESCryptoanalysisDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDESCryptoanalysisDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CDESCryptoanalysisDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CDESCryptoanalysisDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON7, &CDESCryptoanalysisDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &CDESCryptoanalysisDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON11, &CDESCryptoanalysisDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_RADIO2, &CDESCryptoanalysisDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CDESCryptoanalysisDlg message handlers

BOOL CDESCryptoanalysisDlg::OnInitDialog()
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
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDESCryptoanalysisDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDESCryptoanalysisDlg::OnPaint()
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
HCURSOR CDESCryptoanalysisDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/////////////////////////////////////////////////////////////////////////////////
// Self Define Components
/////////////////////////////////////////////////////////////////////////////////
// DES Implementation Part

// Global Variables
string in_implement_content;
string out_implement_content;
string default_key = "CaiXuKun";
// max_iteration should be smaller than 17
int max_iteration;

// Encrypt button of DES implementation
void CDESCryptoanalysisDlg::OnBnClickedButton1()
{
	CString iteration_time;
	GetDlgItemText(IDC_COMBO7, iteration_time);
	TCHAR* temp1 = iteration_time.GetBuffer();
	int size1 = WideCharToMultiByte(CP_ACP, 0, temp1, -1, NULL, 0, NULL, FALSE);
	char* temp2 = new char[sizeof(char) * size1];
	WideCharToMultiByte(CP_ACP, 0, temp1, -1, temp2, size1, NULL, FALSE);
	string temp3(temp2);
	max_iteration = atoi(temp3.c_str());

	CString key;
	GetDlgItemText(IDC_COMBO6, key);
	TCHAR* temp4 = key.GetBuffer();
	int size2 = WideCharToMultiByte(CP_ACP, 0, temp4, -1, NULL, 0, NULL, FALSE);
	char* temp5 = new char[sizeof(char) * size2];
	WideCharToMultiByte(CP_ACP, 0, temp4, -1, temp5, size2, NULL, FALSE);
	string input_key(temp5);

	if ((size2 != 1) && (size2 != 9))
	{
		const char* message = "Wrong key input. Make sure the key contain 8 digits .";
		int length = sizeof(TCHAR)*(strlen(message) + 1);
		LPTSTR tcBuffer = new TCHAR[length];
		memset(tcBuffer, 0, length);
		MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
		LPCTSTR message_out = (LPCTSTR)tcBuffer;
		MessageBox(message_out);
	}
	else
	{
		if ((max_iteration > 2) && (max_iteration < 17))
		{
			if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == 1)
			{
				string key = default_key;
				out_implement_content = MessageProcess(in_implement_content, key, max_iteration, false, false);
			}
			else
			{
				string key = input_key;
				out_implement_content = MessageProcess(in_implement_content, input_key, max_iteration, false, false);
			}
			const char* message = "Document encrypted.";
			int length = sizeof(TCHAR)*(strlen(message) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			MessageBox(message_out);
		}
		else
		{
			const char* message = "Wrong iteration input";
			int length = sizeof(TCHAR)*(strlen(message) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			MessageBox(message_out);
		}
	}
}

// Decrypt button of DES implementation
void CDESCryptoanalysisDlg::OnBnClickedButton2()
{
	// Read the iteration time, and check whether it is integer or not
	CString iteration_time;
	GetDlgItemText(IDC_COMBO7, iteration_time);
	TCHAR* temp1 = iteration_time.GetBuffer();
	int size1 = WideCharToMultiByte(CP_ACP, 0, temp1, -1, NULL, 0, NULL, FALSE);
	char* temp2 = new char[sizeof(char) * size1];
	WideCharToMultiByte(CP_ACP, 0, temp1, -1, temp2, size1, NULL, FALSE);
	string temp3(temp2);
	max_iteration = atoi(temp3.c_str());

	// Read the key in combobox, and check the number of digits
	CString key;
	GetDlgItemText(IDC_COMBO6, key);
	TCHAR* temp4 = key.GetBuffer();
	int size2 = WideCharToMultiByte(CP_ACP, 0, temp4, -1, NULL, 0, NULL, FALSE);
	char* temp5 = new char[sizeof(char) * size2];
	WideCharToMultiByte(CP_ACP, 0, temp4, -1, temp5, size2, NULL, FALSE);
	string input_key(temp5);
	
	// Because there is '\0' on the last digit of string
	// A 8-digit key will take 9 bytes
	if ((size2 != 1) && (size2 != 9))
	{
		const char* message = "Wrong key input. Make sure the key contain 8 digits .";
		int length = sizeof(TCHAR)*(strlen(message) + 1);
		LPTSTR tcBuffer = new TCHAR[length];
		memset(tcBuffer, 0, length);
		MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
		LPCTSTR message_out = (LPCTSTR)tcBuffer;
		MessageBox(message_out);
	}
	else
	{
		if ((max_iteration > 2) && (max_iteration < 17))
		{
			if (((CButton*)GetDlgItem(IDC_RADIO1))->GetCheck() == 1)
			{
				string key = default_key;
				out_implement_content = MessageProcess(in_implement_content, key, max_iteration, false, true);
			}
			else
			{
				string key = input_key;
				out_implement_content = MessageProcess(in_implement_content, input_key, max_iteration, false, true);
			}
			const char* message = "Document decrypted.";
			int length = sizeof(TCHAR)*(strlen(message) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			MessageBox(message_out);
		}
		else
		{
			const char* message = "Wrong iteration input";
			int length = sizeof(TCHAR)*(strlen(message) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			MessageBox(message_out);
		}
	}
}

// Read button of DES implementation
void CDESCryptoanalysisDlg::OnBnClickedButton4()
{
	CString file_path;
	GetDlgItemText(IDC_COMBO5,file_path);
	TCHAR* temp1 = file_path.GetBuffer();
	int size = WideCharToMultiByte(CP_ACP, 0, temp1, -1, NULL, 0, NULL, FALSE);
	char* temp2 = new char[sizeof(char) * size];
	WideCharToMultiByte(CP_ACP, 0, temp1, -1, temp2, size, NULL, FALSE);
	string input_file_path(temp2);
	in_implement_content = ReadDocument(input_file_path);

	if (in_implement_content.length() != 0)
	{
		const char* message = "File extracted.";
		int length = sizeof(TCHAR)*(strlen(message) + 1);
		LPTSTR tcBuffer = new TCHAR[length];
		memset(tcBuffer, 0, length);
		MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
		LPCTSTR message_out = (LPCTSTR)tcBuffer;
		MessageBox(message_out);
	}
	else
	{
		const char* message = "File path incorrect.";
		int length = sizeof(TCHAR)*(strlen(message) + 1);
		LPTSTR tcBuffer = new TCHAR[length];
		memset(tcBuffer, 0, length);
		MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
		LPCTSTR message_out = (LPCTSTR)tcBuffer;
		MessageBox(message_out);
	}
}

// Save button of DES implementation
void CDESCryptoanalysisDlg::OnBnClickedButton6()
{
	bool output_flag;
	CString file_path1;
	GetDlgItemText(IDC_COMBO5, file_path1);
	TCHAR* temp1 = file_path1.GetBuffer();
	int size1 = WideCharToMultiByte(CP_ACP, 0, temp1, -1, NULL, 0, NULL, FALSE);
	char* temp2 = new char[sizeof(char) * size1];
	WideCharToMultiByte(CP_ACP, 0, temp1, -1, temp2, size1, NULL, FALSE);
	string input_file_path(temp2);
	
	// Extract file name and format from input file
	char* sub_string = NULL;
	char* temp_name = NULL;
	for (sub_string = strtok(const_cast<char*>(input_file_path.c_str()), "\\"); sub_string != NULL; sub_string = strtok(NULL, "\\"))
	{
		if (sub_string == strtok(NULL, "\\") == NULL)
		{
			temp_name = sub_string;
		}
	}
	char* file_name = strtok(temp_name, ".");
	char* file_format = strtok(NULL, ".");
	string input_file_name(file_name);
	
	// Write into new file into output path
	CString file_path2;
	GetDlgItemText(IDC_COMBO3, file_path2);
	TCHAR* temp3 = file_path2.GetBuffer();
	int size2 = WideCharToMultiByte(CP_ACP, 0, temp3, -1, NULL, 0, NULL, FALSE);
	char* temp4 = new char[sizeof(char) * size2];
	WideCharToMultiByte(CP_ACP, 0, temp3, -1, temp4, size2, NULL, FALSE);
	string output_file_path(temp4);
	output_file_path.append("\\");
	output_file_path.append(file_name);
	output_file_path.append("_new");
	output_file_path.append(".");
	output_file_path.append(file_format);
	
	output_flag = SaveDocument(output_file_path, out_implement_content);
	if (output_flag)
	{
		const char* message = "File saved.";
		int length = sizeof(TCHAR)*(strlen(message) + 1);
		LPTSTR tcBuffer = new TCHAR[length];
		memset(tcBuffer, 0, length);
		MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
		LPCTSTR message_out = (LPCTSTR)tcBuffer;
		MessageBox(message_out);
	}
}

// Select buttons of DES Implementation
// Select the path to read the original file
void CDESCryptoanalysisDlg::OnBnClickedButton3()
{
	TCHAR szFilter[] = _T("TXT File(*.txt)|*.txt|ALL(*.*)|*.*||");  
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString string_file_path;

	if (IDOK == fileDlg.DoModal())
	{
		string_file_path = fileDlg.GetPathName();
		SetDlgItemText(IDC_COMBO5, string_file_path);
	}
}

// Select the path to save the processed file
void CDESCryptoanalysisDlg::OnBnClickedButton5()
{
	CString string_file_path;
	// Show dialog of file path selection
	string_file_path = CFolderDlg::Show();
	SetDlgItemText(IDC_COMBO3, string_file_path);
}


//////////////////////////////////////////////////////////////////////////////////
// DES Cryptoanalysis Part

string plaintext;
string ciphertext;
// enc_iteration should be smaller than 17, since a larger number means a longer time in exhaustive search
int enc_iteration;
// head is the pointer received to analyze then key bits
struct Pair* head_p_to_c;
struct Pair* head_c_to_p;

// Select buttons in DES cryptoanalysis
// Select the path to read input plaintext file 
void CDESCryptoanalysisDlg::OnBnClickedButton8()
{
	TCHAR szFilter[] = _T("TXT File(*.txt)|*.txt|ALL(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString string_file_path;

	if (IDOK == fileDlg.DoModal())
	{
		string_file_path = fileDlg.GetPathName();
		SetDlgItemText(IDC_COMBO2, string_file_path);
	}
}

// Select the path to read input ciphertext file 
void CDESCryptoanalysisDlg::OnBnClickedButton7()
{
	TCHAR szFilter[] = _T("TXT File(*.txt)|*.txt|ALL(*.*)|*.*||");
	CFileDialog fileDlg(TRUE, _T("txt"), NULL, 0, szFilter, this);
	CString string_file_path;

	if (IDOK == fileDlg.DoModal())
	{
		string_file_path = fileDlg.GetPathName();
		SetDlgItemText(IDC_COMBO4, string_file_path);
	}
}

// Read button of plaintext file in DES cryptoanalysis
void CDESCryptoanalysisDlg::OnBnClickedButton9()
{
	CString plaintext_path;
	GetDlgItemText(IDC_COMBO2, plaintext_path);
	TCHAR* temp1 = plaintext_path.GetBuffer();
	int size1 = WideCharToMultiByte(CP_ACP, 0, temp1, -1, NULL, 0, NULL, FALSE);
	char* temp2 = new char[sizeof(char) * size1];
	WideCharToMultiByte(CP_ACP, 0, temp1, -1, temp2, size1, NULL, FALSE);
	string input_file_path1(temp2);
	plaintext = ReadDocument(input_file_path1);

	CString ciphertext_path;
	GetDlgItemText(IDC_COMBO4, ciphertext_path);
	TCHAR* temp3 = ciphertext_path.GetBuffer();
	int size2 = WideCharToMultiByte(CP_ACP, 0, temp3, -1, NULL, 0, NULL, FALSE);
	char* temp4 = new char[sizeof(char) * size2];
	WideCharToMultiByte(CP_ACP, 0, temp3, -1, temp4, size2, NULL, FALSE);
	string input_file_path2(temp4);
	ciphertext = ReadDocument(input_file_path2);

	if ((ciphertext.length() != 0) && (plaintext.length() != 0))
	{
		const char* message = "File extracted.";
		int length = sizeof(TCHAR)*(strlen(message) + 1);
		LPTSTR tcBuffer = new TCHAR[length];
		memset(tcBuffer, 0, length);
		MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
		LPCTSTR message_out = (LPCTSTR)tcBuffer;
		MessageBox(message_out);
	}
	else
	{
		const char* message = "File path incorrect.";
		int length = sizeof(TCHAR)*(strlen(message) + 1);
		LPTSTR tcBuffer = new TCHAR[length];
		memset(tcBuffer, 0, length);
		MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
		LPCTSTR message_out = (LPCTSTR)tcBuffer;
		MessageBox(message_out);
	}
}

// Choose the same iteration time as encrypting algorithm
void CDESCryptoanalysisDlg::OnBnClickedRadio2()
{
	CString iteration_time;
	GetDlgItemText(IDC_COMBO7, iteration_time);
	SetDlgItemText(IDC_COMBO8, iteration_time);
}

// Linear analysis of DES
void CDESCryptoanalysisDlg::OnBnClickedButton11()
{
	// Simple S_box analysis
	// Editbox in the dialog should select MultiLine into True
	string box_analysis;
	box_analysis.append("The result of Mitsuru algorithm:\r\n\r\n");
	for (int box_num = 1; box_num <= 8; box_num++)
	{
		string temp_analysis;
		int* box_result;
		box_result = FindSBoxLinearity(box_num - 1);
		temp_analysis.append("BOX No. ");
		temp_analysis.append(to_string(box_num));
		temp_analysis.append("--\r\n");
		if (box_result[2] > 32)
		{
			temp_analysis.append("The maximal T is ");
			temp_analysis.append(to_string(box_result[2]));
			temp_analysis.append(". ");
		}
		else
		{
			temp_analysis.append("The minimal T is ");
			temp_analysis.append(to_string(box_result[2]));
			temp_analysis.append(". ");
		}
		temp_analysis.append("\r\nThe maximum difference with N/2 is NS(");
		temp_analysis.append(to_string(box_result[0]));
		temp_analysis.append(" ,");
		temp_analysis.append(to_string(box_result[1]));
		temp_analysis.append(")= ");
		temp_analysis.append(to_string(box_result[3]));
		temp_analysis.append("\r\n\r\n");
		box_analysis += temp_analysis;
	}
	const char* message = box_analysis.c_str();
	int length = sizeof(TCHAR)*(strlen(message) + 1);
	LPTSTR tcBuffer = new TCHAR[length];
	memset(tcBuffer, 0, length);
	MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
	LPCTSTR message_out = (LPCTSTR)tcBuffer;
	SetDlgItemText(IDC_EDIT2, message_out);

	// Do not forget to pad plaintext before pairing
	if (plaintext.length() % 8 != 0)
	{
		string padding_string;
		int pad_length = (plaintext.length() / 8 + 1) * 8 - plaintext.length();
		char pad_number = char(pad_length + 48);
		for (int count = 0; count < pad_length; count++)
		{
			padding_string += pad_number;
		}
		plaintext.append(padding_string);
	}
	int pair_length = plaintext.length();

	// If the iteration time has the same value as the parameter above
	if (((CButton*)GetDlgItem(IDC_RADIO2))->GetCheck() == 1)
	{
		CString iteration_time;
		GetDlgItemText(IDC_COMBO7, iteration_time);
		TCHAR* temp1 = iteration_time.GetBuffer();
		int size1 = WideCharToMultiByte(CP_ACP, 0, temp1, -1, NULL, 0, NULL, FALSE);
		char* temp2 = new char[sizeof(char) * size1];
		WideCharToMultiByte(CP_ACP, 0, temp1, -1, temp2, size1, NULL, FALSE);
		string temp3(temp2);
		enc_iteration = atoi(temp3.c_str());	

		if ((enc_iteration > 2) && (enc_iteration < 17))
		{
			// Figure out the percentage from the results of designated plaintext-cipher pairs
			float percentage_p_to_c,percentage_c_to_p;
			// From plaintext to ciphertext
			percentage_p_to_c = MaxLinearProbabilityEstimation(plaintext, ciphertext, pair_length, enc_iteration);
			// From ciphertext to plaintext, noted that the inverse operation is acutally the same as the original process
			percentage_c_to_p = MaxLinearProbabilityEstimation(ciphertext, plaintext, pair_length, enc_iteration);
			// Find out the key bits in the right side of the equation in Algorithm 1
			head_p_to_c = KeyBitSelection(enc_iteration);
			// Similarly, apply the algorithm to inverse process, which regards original ciphertext as plaintext input
			// and original plaintext as ciphertext input
			head_c_to_p = InvKeyBitSelection(head_p_to_c);
			struct Pair* temp_p_to_c = head_p_to_c;
			struct Pair* temp_c_to_p = head_c_to_p;
			// original_bit is the integer set collecting the number of bit in the 64-bit key
			int original_bit[100] = {0};
			int keybit_count = 0;
			for (temp_p_to_c; temp_p_to_c != NULL; temp_p_to_c = temp_p_to_c->next)
			{
				int iteration = temp_p_to_c->iteration;
				int keybit = temp_p_to_c->keybit;
				for (int bit = 0; bit < 64; bit++)
				{
					bitset<64> bit_input = pow(2, bit);
					bitset<56> bit_detect;
					bit_detect = PC1_BitPermutation(bit_input);
					for (int temp_iter = 0; temp_iter < iteration; temp_iter++)
					{
						bit_detect=KeyTransformation(bit_detect, temp_iter, false);
					}
					bitset<48> bit_result;
					bit_result = PC2_Transformation(bit_detect);
					if (bit_result[keybit] == 1)
					{
						original_bit[keybit_count] = bit;
						keybit_count++;
						break;
					}
				}
			}
			for (temp_c_to_p; temp_c_to_p != NULL; temp_c_to_p = temp_c_to_p->next)
			{
				int iteration = temp_c_to_p->iteration;
				int keybit = temp_c_to_p->keybit;
				for (int bit = 0; bit < 64; bit++)
				{
					bitset<64> bit_input = pow(2, bit);
					bitset<56> bit_detect;
					bit_detect = PC1_BitPermutation(bit_input);
					bit_detect = InitializeKey(bit_detect, enc_iteration);
					for (int temp_iter = iteration - 1; temp_iter >= 0; temp_iter--)
					{
						bit_detect = KeyTransformation(bit_detect, temp_iter, true);
					}
					bitset<48> bit_result;
					bit_result = PC2_Transformation(bit_detect);
					if (bit_result[keybit] == 1)
					{
						original_bit[keybit_count] = bit;
						keybit_count++;
						break;
					}
				}
			}
			for (int a = 1; a < keybit_count; a++)
			{
				for (int b = 0; b < keybit_count - a; b++)
				{
					if (original_bit[b] > original_bit[b + 1])
					{
						int temp = original_bit[b];
						original_bit[b] = original_bit[b + 1];
						original_bit[b + 1] = temp;
					}
				}
			}

			string message = "The percentage of zeros in the results of Mitsuru algorithm 1 is: \r\n";
			message.append("From plaintext to ciphertext: ");
			message.append(to_string(percentage_p_to_c));
			message.append("\r\n");
			message.append("From ciphertext to plaintext: ");
			message.append(to_string(percentage_c_to_p));
			if (percentage_p_to_c > 0.5)
			{
				if ((enc_iteration == 3) || (enc_iteration == 5) || (enc_iteration == 7) || (enc_iteration == 11) || (enc_iteration == 13) || (enc_iteration == 15))
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 1");
				}
				else
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 0");
				}
			}
			else
			{
				if ((enc_iteration == 3) || (enc_iteration == 5) || (enc_iteration == 7) || (enc_iteration == 11) || (enc_iteration == 13) || (enc_iteration == 15))
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 0");
				}
				else
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 1");
				}
			}
			message.append("\r\n\r\nThe following key bits in the original key are revealed:\r\n");
			for (int turn = 0; turn < keybit_count; turn++)
			{
				if (original_bit[turn] != original_bit[turn + 1])
				{
					message.append("K[");
					message.append(to_string(original_bit[turn]));
					message.append("] ");
					if (turn % 4 == 3)
					{
						message.append("\r\n");
					}
				}
			}

			int length = sizeof(TCHAR)*(strlen(message.c_str()) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message.c_str(), strlen(message.c_str()), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			SetDlgItemText(IDC_EDIT1, message_out);
		}
		else
		{
			const char* message = "Wrong iteration input.";
			int length = sizeof(TCHAR)*(strlen(message) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			MessageBox(message_out);
		}
	}

	// Input self-defined iteration time into combox
	// Not using the iteration time from the DES implementation part
	else
	{
		CString iteration_time;
		GetDlgItemText(IDC_COMBO8, iteration_time);
		TCHAR* temp1 = iteration_time.GetBuffer();
		int size1 = WideCharToMultiByte(CP_ACP, 0, temp1, -1, NULL, 0, NULL, FALSE);
		char* temp2 = new char[sizeof(char) * size1];
		WideCharToMultiByte(CP_ACP, 0, temp1, -1, temp2, size1, NULL, FALSE);
		string temp3(temp2);
		enc_iteration = atoi(temp3.c_str());
		if ((enc_iteration > 2) && (enc_iteration < 17))
		{
			// Figure out the percentage from the results of designated plaintext-cipher pairs
			float percentage_p_to_c, percentage_c_to_p;
			// From plaintext to ciphertext
			percentage_p_to_c = MaxLinearProbabilityEstimation(plaintext, ciphertext, pair_length, enc_iteration);
			// From ciphertext to plaintext, noted that the inverse operation is acutally the same as the original process
			percentage_c_to_p = MaxLinearProbabilityEstimation(ciphertext, plaintext, pair_length, enc_iteration);
			// Find out the key bits in the right side of the equation in Algorithm 1
			head_p_to_c = KeyBitSelection(enc_iteration);
			// Similarly, apply the algorithm to inverse process, which regards original ciphertext as plaintext input
			// and original plaintext as ciphertext input
			head_c_to_p = InvKeyBitSelection(head_p_to_c);
			struct Pair* temp_p_to_c = head_p_to_c;
			struct Pair* temp_c_to_p = head_c_to_p;
			// original_bit applies the number of bit in the 56-bit key after PC1_Transformation
			int original_bit[100] = { 0 };
			int keybit_count = 0;
			for (temp_p_to_c; temp_p_to_c != NULL; temp_p_to_c = temp_p_to_c->next)
			{
				int iteration = temp_p_to_c->iteration;
				int keybit = temp_p_to_c->keybit;
				for (int bit = 0; bit < 64; bit++)
				{
					bitset<64> bit_input = pow(2, bit);
					bitset<56> bit_detect;
					bit_detect = PC1_BitPermutation(bit_input);
					for (int temp_iter = 0; temp_iter < iteration; temp_iter++)
					{
						bit_detect = KeyTransformation(bit_detect, temp_iter, false);
					}
					bitset<48> bit_result;
					bit_result = PC2_Transformation(bit_detect);
					if (bit_result[keybit] == 1)
					{
						original_bit[keybit_count] = bit;
						keybit_count++;
						break;
					}
				}
			}
			for (temp_c_to_p; temp_c_to_p != NULL; temp_c_to_p = temp_c_to_p->next)
			{
				int iteration = temp_c_to_p->iteration;
				int keybit = temp_c_to_p->keybit;
				for (int bit = 0; bit < 64; bit++)
				{
					bitset<64> bit_input = pow(2, bit);
					bitset<56> bit_detect;
					bit_detect = PC1_BitPermutation(bit_input);
					bit_detect = InitializeKey(bit_detect, enc_iteration);
					for (int temp_iter = iteration-1; temp_iter >= 0; temp_iter--)
					{
						bit_detect = KeyTransformation(bit_detect, temp_iter, true);
					}
					bitset<48> bit_result;
					bit_result = PC2_Transformation(bit_detect);
					if (bit_result[keybit] == 1)
					{
						original_bit[keybit_count] = bit;
						keybit_count++;
						break;
					}
				}
			}
			for (int a = 1; a < keybit_count; a++)
			{
				for (int b = 0; b < keybit_count - a; b++)
				{
					if (original_bit[b] > original_bit[b + 1])
					{
						int temp = original_bit[b];
						original_bit[b] = original_bit[b + 1];
						original_bit[b + 1] = temp;
					}
				}
			}

			string message = "The percentage of zeros in the results of Mitsuru algorithm 1 is: \r\n";
			message.append("From plaintext to ciphertext: ");
			message.append(to_string(percentage_p_to_c));
			message.append("\r\n");
			message.append("From ciphertext to plaintext: ");
			message.append(to_string(percentage_c_to_p));
			if (percentage_p_to_c > 0.5)
			{
				if ((enc_iteration == 3) || (enc_iteration == 5) || (enc_iteration == 7) || (enc_iteration == 11) || (enc_iteration == 13) || (enc_iteration == 15))
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 1");
				}
				else
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 0");
				}
			}
			else
			{
				if ((enc_iteration == 3) || (enc_iteration == 5) || (enc_iteration == 7) || (enc_iteration == 11) || (enc_iteration == 13) || (enc_iteration == 15))
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 0");
				}
				else
				{
					message.append("\r\n\r\n");
					message.append("Therefore, we guess the XOR result of relevant keys with 1");
				}
			}
			message.append("\r\n\r\nThe following key bits in the original key are revealed:\r\n");
			for (int turn = 0; turn < keybit_count; turn++)
			{
				if (original_bit[turn] != original_bit[turn + 1])
				{
					message.append("K[");
					message.append(to_string(original_bit[turn]));
					message.append("] ");
					if (turn % 4 == 3)
					{
						message.append("\r\n");
					}
				}
			}

			int length = sizeof(TCHAR)*(strlen(message.c_str()) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message.c_str(), strlen(message.c_str()), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			SetDlgItemText(IDC_EDIT1, message_out);
		}
		else
		{
			const char* message = "Wrong iteration input.";
			int length = sizeof(TCHAR)*(strlen(message) + 1);
			LPTSTR tcBuffer = new TCHAR[length];
			memset(tcBuffer, 0, length);
			MultiByteToWideChar(CP_ACP, 0, message, strlen(message), tcBuffer, length);
			LPCTSTR message_out = (LPCTSTR)tcBuffer;
			MessageBox(message_out);
		}
		free(head_c_to_p);
		free(head_p_to_c);
	}
}