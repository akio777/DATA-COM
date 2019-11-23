// VC6022Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "VC6022.h"
#include "VC6022Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVC6022Dlg dialog

CVC6022Dlg::CVC6022Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVC6022Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVC6022Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVC6022Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVC6022Dlg)
	DDX_Control(pDX, IDC_STC_DRAW, m_stcDraw);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CVC6022Dlg, CDialog)
	//{{AFX_MSG_MAP(CVC6022Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_READ, OnBtnRead)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVC6022Dlg message handlers

BOOL CVC6022Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_nDevIndex = 0;
	m_nTimeDIV = 1;//
	m_nType = 0;//Hantek6022BL
	if(dsoOpenDevice(m_nDevIndex))
	{
		dsoChooseDevice(m_nDevIndex,m_nType);
		dsoSetTimeDIV(m_nDevIndex,m_nTimeDIV);
	}
	else
	{
		AfxMessageBox("No device was found!");
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CVC6022Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CVC6022Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CVC6022Dlg::OnBtnRead() 
{
	// TODO: Add your control notification handler code here
	short* pData1;  //CH0-CH7
	short* pData2;  //CH8-CH15
	ULONG nReadLen = 10240;//10k
	short m_nStartData = 0;//start to draw
	double m_dbDrawPTSpace = 0.1;
	CRect rc;
	m_stcDraw.GetClientRect(&rc);
	CRect rect;
	rect = rc;
	double nStep = rc.Height()/17;
	rect.bottom = (long)(rect.top + nStep);
	pData1 = new short[nReadLen];
	pData2 = new short[nReadLen];	
	short nRe = dsoReadHardData_LA(m_nDevIndex,pData1,pData2,nReadLen,m_nTimeDIV);
	if(nRe > 0)
	{
		m_stcDraw.GetDC()->FillSolidRect(&rc,RGB(0,0,0));
	//Draw CH0-CH7
		for(int i=0;i<8;i++)
		{
			rect.top = rect.bottom;
			rect.bottom =(long)( rect.top + nStep);
			HTDrawLASquareSignal_LA_6022(m_stcDraw.GetDC()->GetSafeHdc(),rect,RGB(0,255,0),pData1,m_nStartData,nReadLen-m_nStartData,m_dbDrawPTSpace,i);
		}

	//Draw CH8-CH15
		for(i=0;i<8;i++)
		{
			rect.top = rect.bottom;
			rect.bottom =(long)( rect.top + nStep);
			HTDrawLASquareSignal_LA_6022(m_stcDraw.GetDC()->GetSafeHdc(),rect,RGB(0,255,0),pData2,m_nStartData,nReadLen-m_nStartData,m_dbDrawPTSpace,i);
		}
	}

	delete pData1;
	delete pData2;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 