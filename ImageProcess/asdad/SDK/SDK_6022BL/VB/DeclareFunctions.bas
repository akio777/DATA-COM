Attribute VB_Name = "DeclareFunctions"
'Draw
Public Declare Function HTDrawLASquareSignal_LA_6022VB Lib "HTDisplayDll.dll" (ByVal hDC As Long, ByVal Left As Long, ByVal Top As Long, ByVal Right As Long, ByVal Bottom As Long, ByVal R As Integer, ByVal G As Integer, ByVal B As Integer, ByRef pData As Integer, ByVal nStartData As Long, ByVal nLen As Long, ByVal fpt As Double, ByVal signal As Integer) As Integer

'Hard
Public Declare Function dsoOpenDevice Lib "HTMarch.dll" (ByVal nDeviceIndex As Integer) As Integer
Public Declare Function dsoChooseDevice Lib "HTMarch.dll" (ByVal nDeviceIndex As Integer, ByVal nType As Integer) As Integer
Public Declare Function dsoSetTimeDIV Lib "HTMarch.dll" (ByVal nDeviceIndex As Integer, ByVal nTimeDIV As Long) As Integer
Public Declare Function dsoReadHardData_LA Lib "HTMarch.dll" (ByVal nDeviceIndex As Integer, pData1 As Integer, pData2 As Integer, ByVal nReadLen As Long, ByVal nTimeDIV As Long) As Integer

                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         