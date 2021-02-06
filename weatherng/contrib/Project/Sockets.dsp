# Microsoft Developer Studio Project File - Name="Sockets" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Sockets - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sockets.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sockets.mak" CFG="Sockets - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sockets - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Sockets - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sockets - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\lib\R"
# PROP Intermediate_Dir "R"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "..\..\openssl\inc32" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_OPENSSL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "Sockets - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\lib\D"
# PROP Intermediate_Dir "D"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "..\..\openssl\inc32" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "HAVE_OPENSSL" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "Sockets - Win32 Release"
# Name "Sockets - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Base64.cpp
# End Source File
# Begin Source File

SOURCE=..\CircularBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\CTcpSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\HttpDebugSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\HttpGetSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\HttpPostSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\HttpPutSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\HttpsGetSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\HTTPSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\HttpsSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\NullCrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\Parse.cpp
# End Source File
# Begin Source File

SOURCE=..\PoolSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\Socket.cpp
# End Source File
# Begin Source File

SOURCE=..\socket_include.cpp
# End Source File
# Begin Source File

SOURCE=..\SocketHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\SocketThread.cpp
# End Source File
# Begin Source File

SOURCE=..\SSLSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\StdoutLog.cpp
# End Source File
# Begin Source File

SOURCE=..\TcpSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\Thread.cpp
# End Source File
# Begin Source File

SOURCE=..\UdpSocket.cpp
# End Source File
# Begin Source File

SOURCE=..\Utility.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Base64.h
# End Source File
# Begin Source File

SOURCE=..\CircularBuffer.h
# End Source File
# Begin Source File

SOURCE=..\CTcpSocket.h
# End Source File
# Begin Source File

SOURCE=..\HttpDebugSocket.h
# End Source File
# Begin Source File

SOURCE=..\HttpGetSocket.h
# End Source File
# Begin Source File

SOURCE=..\HttpPostSocket.h
# End Source File
# Begin Source File

SOURCE=..\HttpPutSocket.h
# End Source File
# Begin Source File

SOURCE=..\HttpsGetSocket.h
# End Source File
# Begin Source File

SOURCE=..\HTTPSocket.h
# End Source File
# Begin Source File

SOURCE=..\HttpsSocket.h
# End Source File
# Begin Source File

SOURCE=..\ICrypt.h
# End Source File
# Begin Source File

SOURCE=..\NullCrypt.h
# End Source File
# Begin Source File

SOURCE=..\Parse.h
# End Source File
# Begin Source File

SOURCE=..\PoolSocket.h
# End Source File
# Begin Source File

SOURCE=..\Socket.h
# End Source File
# Begin Source File

SOURCE=..\socket_include.h
# End Source File
# Begin Source File

SOURCE=..\SocketHandler.h
# End Source File
# Begin Source File

SOURCE=..\SocketThread.h
# End Source File
# Begin Source File

SOURCE=..\SSLSocket.h
# End Source File
# Begin Source File

SOURCE=..\StdLog.h
# End Source File
# Begin Source File

SOURCE=..\StdoutLog.h
# End Source File
# Begin Source File

SOURCE=..\TcpSocket.h
# End Source File
# Begin Source File

SOURCE=..\Thread.h
# End Source File
# Begin Source File

SOURCE=..\UdpSocket.h
# End Source File
# Begin Source File

SOURCE=..\Utility.h
# End Source File
# End Group
# End Target
# End Project
