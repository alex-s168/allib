// Define MINIPROC_IMPL in a seperate C file and include this header

/*
Copyright 2024 Alexander Nutz

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef MINIPROC_H
#define MINIPROC_H

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# ifdef MINIPROC_IMPL
#   include <io.h>
#   include <fcntl.h>
# endif
#else
# include <unistd.h>
# include <sys/wait.h>
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    FILE *to;
    FILE *from;

    char  *impl__lastReadBuf;
#ifdef _WIN32
    HANDLE impl__hRead[2];
    HANDLE impl__hWrite[2];
    HANDLE impl__hProc;
    HANDLE impl__hThread;
#else
    pid_t impl__pid;
#endif
} ChildProc;

#ifdef _WIN32
FILE *openhandle(const HANDLE file, const bool write)
#ifndef MINIPROC_IMPL
;
#else
{
    const int nHandle = _open_osfhandle((intptr_t) file, write ? _O_WRONLY : _O_RDONLY);
    if (nHandle == -1) { 
        CloseHandle(file);   //case 1
        return NULL; 
    }
    FILE* fp = _fdopen(nHandle, write ? "wb" : "rb");
    if (fp == NULL)
        _close(nHandle);
    return fp;
}
#endif
#endif

/** if returnvalue.from == NULL, error is in errno */
ChildProc cpopen(const char *file, const char **args)
#ifndef MINIPROC_IMPL
;
#elif _WIN32
{
    ChildProc proc;
    
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;

    //  this write into impl__hRead[1] and other read from impl__hRead[0]
    //  other write into impl__hWrite[1] and we read from impl__hWrite[0]
    
    CreatePipe(&proc.impl__hRead[0], &proc.impl__hRead[1], &sa, 0);
    CreatePipe(&proc.impl__hWrite[0], &proc.impl__hWrite[1], &sa, 0);
    
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    ZeroMemory(&pi, sizeof(pi));
    si.cb = sizeof(si);
    si.hStdInput = proc.impl__hRead[0];
    si.hStdOutput = proc.impl__hWrite[1];
    si.dwFlags |= STARTF_USESTDHANDLES;

    size_t cmd_len = 0;
    {
        const char **arg = args;
        while (*arg != NULL) {
            cmd_len += strlen(*arg++) + 1;
        }
    }
    const size_t flen = strlen(file);
    char *cmd = malloc(flen + 1 + cmd_len);
    {
        char *np = cmd;
        memcpy(np, file, flen);
        np += flen;
        *np++ = ' ';
        
        const char **arg = args;
        while (*arg != NULL) {
            const size_t len = strlen(*arg);
            memcpy(np, *arg, len);
            np += len;
            *np++ = ' ';
            arg++;
        }
        *--np = '\0';
    }

    const bool ok = CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
    if (ok) {
        proc.from = openhandle(proc.impl__hWrite[0], false);
        proc.to = openhandle(proc.impl__hRead[1], true);
    } else {
        proc.from = NULL;
        proc.to = NULL;
        errno = GetLastError();
    }
    
    proc.impl__hProc = pi.hProcess;
    proc.impl__hThread = pi.hThread;
    proc.impl__lastReadBuf = NULL;

    free(cmd);
    
    return proc;
}
#else
{
    int inpipefd[2];
    int outpipefd[2];

    ChildProc proc;
    
    pipe(inpipefd);
    pipe(outpipefd);
    proc.impl__pid = fork();
    
    if (proc.impl__pid == 0) {
        // Child process
        dup2(outpipefd[0], STDIN_FILENO);
        dup2(inpipefd[1], STDOUT_FILENO);
        dup2(inpipefd[1], STDERR_FILENO);
        // Close unused pipe ends
        close(outpipefd[1]);
        close(inpipefd[0]);
        
        execvp(file, (char *const*) args);
        exit(1);
    }
    
    // Parent process
    close(outpipefd[0]);
    close(inpipefd[1]);

    proc.to = fdopen(outpipefd[1], "w");
    proc.from = fdopen(inpipefd[0], "r");

    setbuf(proc.from, NULL);
    setbuf(proc.to, NULL);

    proc.impl__lastReadBuf = NULL;
    
    return proc;
}
#endif

bool cpalive(ChildProc cp)
#ifndef MINIPROC_IMPL
;
#elif _WIN32
{
    return WaitForSingleObject(cp.impl__hProc, 0) == WAIT_TIMEOUT;
}
#else
{
    int status = 124905; // random num
    waitpid(cp.impl__pid, &status, WNOHANG);
    return status == 124905;
}
#endif

int cpwait(ChildProc cp)
#ifndef MINIPROC_IMPL
;
#elif _WIN32
{
    WaitForSingleObject(cp.impl__hProc, INFINITE);
    DWORD exit;
    GetExitCodeProcess(cp.impl__hProc, &exit);
    return (int) exit;
}
#else
{
    int status;
    waitpid(cp.impl__pid, &status, 0);
    return status;
}
#endif

int cpclose(ChildProc cp)
#ifndef MINIPROC_IMPL
;
#elif _WIN32
{
    const int status = cpwait(cp);
    
    CloseHandle(cp.impl__hRead[0]);
    CloseHandle(cp.impl__hRead[1]);
    CloseHandle(cp.impl__hWrite[0]);
    CloseHandle(cp.impl__hWrite[1]);
    
    CloseHandle(cp.impl__hProc);
    CloseHandle(cp.impl__hThread);

    if (cp.impl__lastReadBuf)
        free(cp.impl__lastReadBuf);
        
    return status;
}
#else
{
    int status;
    kill(cp.impl__pid, SIGKILL);
    waitpid(cp.impl__pid, &status, 0);
    return status;
}
#endif

void cpterm(ChildProc cp)
#ifndef MINIPROC_IMPL
;
#elif _WIN32
{
    CloseHandle(cp.impl__hRead[0]);
    CloseHandle(cp.impl__hRead[1]);
    CloseHandle(cp.impl__hWrite[0]);
    CloseHandle(cp.impl__hWrite[1]);
    
    CloseHandle(cp.impl__hProc);
    CloseHandle(cp.impl__hThread);
}
#else
{
    kill(cp.impl__pid, SIGTERM);
}
#endif

/**
 * Return the amount of chars that can be read at this moment in time.
 *
 * Example:
 * \code 
 * while (cpalive(proc)) {
 *     copyNFile(stdout, proc.from, cpcountnext(proc));
 * }
 * copyNFile(stdout, proc.from, cpcountnext(proc));
 * cpclose(proc);
 * \endcode 
 */
size_t cpcountnext(ChildProc *cp)
#ifndef MINIPROC_IMPL
;
#else
{
    fseek(cp->from, 0, SEEK_END);
    const size_t total = ftell(cp->from);
    fseek(cp->from, 0, SEEK_SET);
    return total;
}
#endif

char *cpreadavail(ChildProc *proc)
#ifndef MINIPROC_IMPL
;
#else
{
    if (proc->impl__lastReadBuf) {
        char *buf = proc->impl__lastReadBuf;
        proc->impl__lastReadBuf = NULL;
        return buf;
    }
    
    const size_t counted = cpcountnext(proc);
    char *buf = malloc(counted + 1);
    const size_t actual = fread(buf, 1, counted, proc->from);

    buf[actual] = '\0';

    return buf;
}
#endif

char *cpreadavailuntil(ChildProc *proc, char until)
#ifndef MINIPROC_IMPL
;
#else
{
    char *buf = cpreadavail(proc);

    char *end = buf;
    while (*end != '\0' && *end != until) {
        end ++;
    }

    if (*end == '\0') {
        return buf; // char $until never occured
    }

    const size_t care = end - buf;
    const size_t buflen = strlen(buf);
    
    char *unread = malloc(buflen - care + 1);
    memcpy(unread, end, buflen - care + 1);
    proc->impl__lastReadBuf = unread;

    *end = '\0';

    return buf;
}
#endif

char *cpreaduntil(ChildProc *proc, const char until)
#ifndef MINIPROC_IMPL
;
#else
{
    char *res = malloc(1);
    *res = '\0';
    size_t resLen = 0;

    while (cpalive(*proc)) {
        char *temp = cpreadavailuntil(proc, until);
        if (*temp == '\0')
            continue;
        const size_t tempLen = strlen(temp);
        res = realloc(res, resLen + tempLen + 1);
        memcpy(res + resLen, temp, tempLen + 1);
        resLen += tempLen;
        free(temp);
        if (res[resLen - 1] == until)
            break;
    }

    return res;
}
#endif

#endif //MINIPROC_H
