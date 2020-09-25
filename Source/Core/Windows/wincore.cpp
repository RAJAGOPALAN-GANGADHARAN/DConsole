/**
 * MIT License
 * 
 * Copyright (c) 2020 RAJAGOPALAN-GANGADHARAN
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#include "wincore.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include <iostream>
#define BUFSIZE 4096

int WinCore::SpawnProcess(int argc, char **argv)
{
    SECURITY_ATTRIBUTES saAttr;

    printf("\n->Start of parent execution.\n");
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&recieverStderr, &recieverStderrWrite, &saAttr, 0))
        ErrorExit(TEXT("Stderr CreatePipe Failed"));

    if (!SetHandleInformation(recieverStderr, HANDLE_FLAG_INHERIT, 0))
        ErrorExit(TEXT("Stderr SetHandleInformation Failed"));

    if (!CreatePipe(&recieverStdout, &recieverStdoutWrite, &saAttr, 0))
        ErrorExit(TEXT("Stdout CreatePipe Failed"));

    if (!SetHandleInformation(recieverStdout, HANDLE_FLAG_INHERIT, 0))
        ErrorExit(TEXT("Stdout SetHandleInformation Failed"));

    // if (argc == 1)
    //     ErrorExit(TEXT("Please specify a child process.\n"));

    CreateChildProcess("child");

    printf("\n->Contents of child process STDOUT:\n\n");
    ReadFromPipe();

    printf("\n->End of parent execution.\n");

    return 0;
}

void WinCore::CreateChildProcess(const char *process)
{
    TCHAR szCmdline[101];
    _tcscpy_s(szCmdline, _T(process));

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFO siStartInfo;
    BOOL bSuccess = FALSE;

    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.hStdError = recieverStderr;
    siStartInfo.hStdOutput = recieverStdout;
    siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

    bSuccess = CreateProcess(NULL,
                             szCmdline,    // command line
                             NULL,         // process security attributes
                             NULL,         // primary thread security attributes
                             TRUE,         // handles are inherited
                             0,            // creation flags
                             NULL,         // use parent's environment
                             NULL,         // use parent's current directory
                             &siStartInfo, // STARTUPINFO pointer
                             &piProcInfo); // receives PROCESS_INFORMATION

    if (!bSuccess)
        ErrorExit(TEXT("CreateProcess"));
    else
    {
        CloseHandle(piProcInfo.hProcess);
        CloseHandle(piProcInfo.hThread);
        CloseHandle(recieverStderrWrite);
        CloseHandle(recieverStdoutWrite);
    }
}

void WinCore::ReadFromPipe()
{
    DWORD dwRead, dwWritten;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;
    HANDLE hParentStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

    for (;;)
    {
        bSuccess = ReadFile(recieverStdout, chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0)
            break;

        bSuccess = WriteFile(hParentStdOut, chBuf,
                             dwRead, &dwWritten, NULL);
        if (!bSuccess)
            break;
    }
}

void WinCore::ErrorExit(PTSTR lpszFunction)
{
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf,
        0, NULL);

    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                      (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf,
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("%s failed with error %d: %s"),
                    lpszFunction, dw, lpMsgBuf);
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
    ExitProcess(1);
}
