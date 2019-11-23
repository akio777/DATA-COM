VERSION 5.00
Begin VB.Form MainForm 
   Caption         =   "Form1"
   ClientHeight    =   8115
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   13155
   LinkTopic       =   "Form1"
   ScaleHeight     =   8115
   ScaleWidth      =   13155
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton ReadData 
      Caption         =   "Read Data"
      Height          =   615
      Left            =   4920
      TabIndex        =   1
      Top             =   7440
      Width           =   2415
   End
   Begin VB.PictureBox DisplayPicture 
      BackColor       =   &H80000008&
      Height          =   7095
      Left            =   120
      ScaleHeight     =   7035
      ScaleWidth      =   12675
      TabIndex        =   0
      Top             =   120
      Width           =   12735
   End
End
Attribute VB_Name = "MainForm"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub ReadData_Click()
   Dim nReadLen As Long
   Dim nStep As Long
   Dim nTop As Long
   Dim nBottom As Long
   Dim nLeft As Long
   Dim nRight As Long
   Dim nFpt As Double
   Dim nRe As Integer
   Dim i As Integer
    nReadLen = 10240 '10K
    nStep = MainForm.DisplayPicture.ScaleHeight / 300
    nTop = 20 'MainForm.DisplayPicture.Top
    nLeft = 0 'MainForm.DisplayPicture.Left
    nBottom = nTop + nStep
    nRight = MainForm.DisplayPicture.ScaleWidth
    nFpt = 0.1
    nRe = dsoReadHardData_LA(m_nDevIndex, m_nData1(0), m_nData2(0), nReadLen, m_nTimeDIV)
    If nRe > 0 Then
        MainForm.DisplayPicture.Cls
        'Draw CH0-CH7
         For i = 0 To 7
             nTop = nBottom
             nBottom = nTop + nStep
             nRe = HTDrawLASquareSignal_LA_6022VB(MainForm.DisplayPicture.hDC, nLeft, nTop, nRight, nBottom, 0, 255, 0, m_nData1(0), 0, nReadLen, nFpt, i)
        Next
        'Draw CH8-CH15
        For i = 0 To 7
             nTop = nBottom
             nBottom = nTop + nStep
            nRe = HTDrawLASquareSignal_LA_6022VB(MainForm.DisplayPicture.hDC, nLeft, nTop, nRight, nBottom, 0, 255, 0, m_nData2(0), 0, nReadLen, nFpt, i)
       Next
    End If
End Sub
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              