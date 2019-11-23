// VC6022Dlg.h : header file
//

#if !defined(AFX_VC6022DLG_H__6442A3F3_0930_4F16_9A35_13DB2BB4E28B__INCLUDED_)
#define AFX_VC6022DLG_H__6442A3F3_0930_4F16_9A35_13DB2BB4E28B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVC6022Dlg dialog

class CVC6022Dlg : public CDialog
{
// Construction
public:
	CVC6022Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVC6022Dlg)
	enum { IDD = IDD_VC6022_DIALOG };
	CStatic	m_stcDraw;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVC6022Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	short m_nDevIndex;//device index
	int m_nTimeDIV;//timediv
	short m_nType;//type to choose device��Hantek6022BE  is 0��Hantek6022BL is 1
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVC6022Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnRead();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VC6022DLG_H__6442A3F3_0930_4F16_9A35_13DB2BB4E28B__INCLUDED_)
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                