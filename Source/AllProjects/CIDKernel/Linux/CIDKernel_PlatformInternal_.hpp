//
// FILE NAME: CIDKernel_PlatformInternal_.Hpp
//
// AUTHOR: Will Mason
//
// CREATED: 10/11/1998
//
// COPYRIGHT: Charmed Quark Systems, Ltd - 2019
//
//  This software is copyrighted by 'Charmed Quark Systems, Ltd' and 
//  the author (Dean Roddey.) It is licensed under the MIT Open Source 
//  license:
//
//  https://opensource.org/licenses/MIT
//
// DESCRIPTION:
//
//  This file is included into the CIDKernel_.Hpp file, so that it is visible
//  to all files within the kernel. So it is used to bring in any platform
//  specific stuff that needs to be visible within the kernel, such as the
//  system headers.
//
// CAVEATS/GOTCHAS:
//
// LOG:
//
//  $_CIDLib_Log_$
//

// Place all platform includes here, so the precompiled headers will be at
// maximum efficiency.
#include <arpa/inet.h>
#include <ctype.h>
#include <curses.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <langinfo.h>
#include <limits.h>

//
//  An error in the headers, this is also included in socket.h below and 
//  causes duplicate definitions. If that is later fixed, we'll have to
//  include this again (or make this conditional somehow.)
//
// #include <linux/uio.h>
//

#include <math.h>
#include <mntent.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <pwd.h>
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/kd.h>
#include <sys/mman.h>
#include <sys/resource.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/vfs.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>
#include <utime.h>
#include <wchar.h>
#include <wctype.h>
#include <term.h>


//
// Forward delcarations
//
namespace TKrnlLinux
{
    class TRecursiveMutex;
    class TTermFifo;
    class TTermValueNode;
}


//
// Type declarations
//
#if defined(_SEM_SEMUN_UNDEFINED)
union semun
{
    int                 val;
    struct semid_ds*    idbuf;
    unsigned short int* array;
    struct seminfo*     infobuf;
};
#endif


//
// Internal structures
//
struct TCommHandleImpl
{
    tCIDLib::TSInt      hComm;
};


struct TConsoleHandleImpl
{
    struct termios              TermInfo;
    TKrnlLinux::TTermFifo*      pfifoThis;
    TKrnlLinux::TTermValueNode* pnodeTree;
    tCIDLib::TBoolean           bSingleCharMode;
    tCIDLib::TBoolean           bValid;
};

struct TDirSearchHandleImpl
{
    DIR*                pDir;
    struct dirent*      pNextDirEntry;
    tCIDLib::TSCh*      pszDirName;
    tCIDLib::TSCh*      pszFileSpec;
};

struct TEventHandleImpl
{
    // Pthread stuff
    pthread_mutex_t     mtxThis;
    pthread_cond_t      condThis;
    tCIDLib::TBoolean   bSet;

    // System V IPC stuff, for named events
    tCIDLib::TSInt      iSysVSemId;
    tCIDLib::TBoolean   bSysVOwner;

    // Standard handle stuff, enables bDuplicate() method
    tCIDLib::TCard4     c4RefCount;
};

struct TFileHandleImpl
{
    tCIDLib::TSInt      iFd;
};

struct TMemoryHandleImpl
{
    // System V IPC stuff
    tCIDLib::TSInt               iShmId;
    tCIDLib::TBoolean            bOwner;

    // To synchronize access to c4RefCount
    TKrnlLinux::TRecursiveMutex* prmtxThis;

    // Standard handle stuff, enables bDuplicate() method
    tCIDLib::TCard4              c4RefCount;
};

struct TModuleHandleImpl
{
    tCIDLib::TVoid*     pHandle;
    tCIDLib::TCh*       pszFullPath;
};

struct TMutexHandleImpl
{
    // Pthread stuff
    TKrnlLinux::TRecursiveMutex* prmtxThis;

    // System V IPC stuff, for named mutexes
    tCIDLib::TSInt               iSysVSemId;
    tCIDLib::TBoolean            bSysVOwner;
    tCIDLib::TCard4              c4LockCount;

    // Standard handle stuff, enables bDuplicate() method
    tCIDLib::TCard4              c4RefCount;
};

struct TProcessHandleImpl
{
    pid_t               pidThis;
    tCIDLib::EExitCodes eExit;
    tCIDLib::TCard4     c4JiffyStartTime;
    struct timeval      UserTime;
    struct timeval      KernelTime;
    struct timeval      EndTime;
    tCIDLib::TBoolean   bAlreadyClean;
};

struct TSemaphoreHandleImpl
{
    // Pthread stuff
    pthread_mutex_t     mtxThis;
    pthread_cond_t      condThis;
    tCIDLib::TCard4     c4CurCount;

    // System V IPC stuff, for named semaphores
    tCIDLib::TSInt      iSysVSemId;
    tCIDLib::TBoolean   bSysVOwner;

    // Standard handle stuff, enables bDuplicate() method
    tCIDLib::TCard4     c4RefCount;
};

struct TSocketHandleImpl
{
    tCIDLib::TSInt      iDescr;
};

struct TThreadHandleImpl
{
    tCIDLib::TThreadId  tidThis;
    tCIDLib::EExitCodes eExit;
    tCIDLib::TBoolean   bJoined;
};
