#include "WPipe.h"
#include "Source/MainWindow.h"
#include "Source/utils.h"
#include <windows.h> 
#include <stdio.h> 
#include <tchar.h>
#include <strsafe.h>

#define BUFSIZE 512


DWORD dwThreadId;
WPipe::WPipe()
:IPC()
{

}

WPipe::~WPipe()
{
}

void WPipe::create_server()
{
    fConnected = FALSE; 
    dwThreadId = 0; 
    hPipe = INVALID_HANDLE_VALUE, hThread = NULL; 
    DCONSOLE_SERVER = TEXT("\\\\.\\pipe\\dconsole_pipe"); 
}

DWORD WINAPI InstanceThread(LPVOID tif)
{ 
    HANDLE hHeap      = GetProcessHeap();
    Message* pchRequest = (Message*)HeapAlloc(hHeap, 0, sizeof(Message));

    DWORD cbBytesRead = 0, cbReplyBytes = 0, cbWritten = 0; 
    BOOL fSuccess = FALSE;
    HANDLE hPipe  = NULL;

    ThreadInterface* lpTif = (ThreadInterface*)tif;

    if (lpTif->lp == NULL)
    {
        printf( "\nERROR - Pipe Server Failure:\n");
        printf( "   InstanceThread got an unexpected NULL value in lpvParam.\n");
        printf( "   InstanceThread exitting.\n");
        if (pchRequest != NULL) HeapFree(hHeap, 0, pchRequest);
        return (DWORD)-1;
    }

    if (pchRequest == NULL)
    {
        printf( "\nERROR - Pipe Server Failure:\n");
        printf( "   InstanceThread got an unexpected NULL heap allocation.\n");
        printf( "   InstanceThread exitting.\n");
        return (DWORD)-1;
    }

    printf("InstanceThread created, receiving and processing messages.\n");
    
    hPipe = (HANDLE) lpTif->lp; 
    while (1) 
    { 
        fSuccess = ReadFile( 
            hPipe,        // handle to pipe 
            pchRequest,    // buffer to receive data 
            sizeof(Message), // size of buffer 
            &cbBytesRead, // number of bytes read 
            NULL);        // not overlapped I/O 

        if (!fSuccess || cbBytesRead == 0)
        {   
            if (GetLastError() == ERROR_BROKEN_PIPE)
            {
                _tprintf(TEXT("InstanceThread: client disconnected.\n")); 
            }
            else
            {
                _tprintf(TEXT("InstanceThread ReadFile failed, GLE=%d.\n"), GetLastError()); 
            }
            break;
        }

        //fprintf(stderr,"****** %s",pchRequest->data);
        lpTif->windowLoc->UpdateMessage(*pchRequest);

    }

    // Flush the pipe to allow the client to read the pipe's contents 
    // before disconnecting. Then disconnect the pipe, and close the 
    // handle to this pipe instance. 
    
    FlushFileBuffers(hPipe); 
    DisconnectNamedPipe(hPipe); 
    CloseHandle(hPipe); 

    HeapFree(hHeap, 0, pchRequest);

    printf("InstanceThread exiting.\n");
    return 1;
}

void WPipe::recieve_message_loop()
{
    for (;;) 
   { 
      _tprintf( TEXT("\nPipe Server: Main thread awaiting client connection on %s\n"), DCONSOLE_SERVER);
      hPipe = CreateNamedPipe( 
          DCONSOLE_SERVER,             // pipe name 
          PIPE_ACCESS_DUPLEX,       // read/write access 
          PIPE_TYPE_MESSAGE |       // message type pipe 
          PIPE_READMODE_MESSAGE |   // message-read mode 
          PIPE_WAIT,                // blocking mode 
          PIPE_UNLIMITED_INSTANCES, // max. instances  
          BUFSIZE,                  // output buffer size 
          BUFSIZE,                  // input buffer size 
          0,                        // client time-out 
          NULL);                    // default security attribute 

      if (hPipe == INVALID_HANDLE_VALUE) 
      {
          _tprintf(TEXT("CreateNamedPipe failed, GLE=%d.\n"), GetLastError()); 
          exit_helper("NamedPipe creation Error",-1);
      }
 
      // Wait for the client to connect; if it succeeds, 
      // the function returns a nonzero value. If the function
      // returns zero, GetLastError returns ERROR_PIPE_CONNECTED. 
 
      fConnected = ConnectNamedPipe(hPipe, NULL) ? 
         TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 
 
      if (fConnected) 
      { 
         printf("Client connected, creating a processing thread.\n"); 
         ThreadInterface* tif = new ThreadInterface();
         tif->lp = (LPVOID) hPipe;
         tif->windowLoc = parent_window;
         // Create a thread for this client. 
         hThread = CreateThread( 
            NULL,              // no security attribute 
            0,                 // default stack size 
            InstanceThread,    // thread proc
            tif,    // thread parameter 
            0,                 // not suspended 
            &dwThreadId);      // returns thread ID 

         if (hThread == NULL) 
         {
            _tprintf(TEXT("CreateThread failed, GLE=%d.\n"), GetLastError()); 
            exit_helper("NamedPipe creation Error",-1);
         }
         else CloseHandle(hThread); 
       } 
      else 
        // The client could not connect, so close the pipe. 
         CloseHandle(hPipe); 
   } 
}
