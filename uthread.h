// define _UTHREAD_H_IMPL in a C file and include this file 

#ifndef _UTHREAD_H
#define _UTHREAD_H

#ifdef _WIN32 
# ifdef _UTHREAD_H_IMPL
#  include <windows.h>
# endif 
#else 
# include <pthread.h>
# ifdef _UTHREAD_H_IMPL
#  include <signal.h>
# endif
#endif

#if defined(_WIN32) && defined(_UTHREAD_H_IMPL)
struct __uthread_th_dispatch_s {
    void *(*start_routine)(void *);
    void * arg;
    void * ret;
};
#endif 

typedef struct uthread_t {
#ifdef _WIN32 
    int impl_code;
    void * impl; // HANDLE*
    struct __uthread_th_dispatch_s* impl_disp;
#else 
    pthread_t impl;
#endif 
} uthread_t;

int uthread_create(
        uthread_t * th,
        void *(*start_routine)(void *),
        void * arg);

int uthread_join(
        uthread_t * th,
        void ** ret_out);

void uthread_kill(
        uthread_t * th);

void uthread_join_all(
        size_t count,
        uthread_t * threads,
        int   * optExitCodeOuts,
        void ** optOutOuts);

#ifdef _UTHREAD_H_IMPL

#ifdef _WIN32 

unsigned long __uthread_th_dispatch(void * arg)
{
    struct __uthread_th_dispatch_s disp = * (struct __uthread_th_dispatch_s*) arg;
    disp.ret = disp.start_routine(disp.arg);
    return 0;
}

int uthread_create(
        uthread_t * th,
        void *(*start_routine)(void *),
        void * arg)
{
    th->impl_code = 0;

    struct __uthread_th_dispatch_s * disp =
        HeapAlloc(
                GetProcessHeap(),
                HEAP_ZERO_MEMORY,
                sizeof(struct __uthread_th_dispatch_s));
    th->impl_disp = disp;
    disp->arg = arg;
    disp->start_routine = start_routine;

    th->impl = CreateThread(NULL, 0, __uthread_th_dispatch, disp, 0, NULL);
    if (th->impl == NULL) return 1;
    return 0;
}

int uthread_join(
        uthread_t * th,
        void ** ret_out)
{
    if (th->impl == NULL) {
        if (ret_out) *ret_out = NULL;
        return th->impl_code;
    }
    WaitForSingleObject((HANDLE) th->impl, INFINITE);
    if (ret_out)
        *ret_out = th->impl_disp->ret;
    DWORD dw;
    GetExitCodeThread((HANDLE) th->impl, &dw);
    th->impl_code = (int) dw;
    CloseHandle((HANDLE) th->impl);
    th->impl = NULL;
    HeapFree(GetProcessHeap(), 0, th->impl_disp);
    return (int) dw;
}

void uthread_join_all(
        size_t count,
        uthread_t * threads,
        int   * optExitCodeOuts,
        void ** optOutOuts)
{
    HANDLE handles[count];

    size_t actual = 0;
    for (size_t i = 0; i < count; i ++) {
        uthread_t* th = &threads[i];
        if (th->impl == NULL) continue;
        handles[actual ++] = (HANDLE) th->impl;
    }

    WaitForMultipleObjects(actual, (HANDLE) handles, TRUE, INFINITE);
    for (size_t i = 0; i < count; i ++) {
        uthread_t* th = &threads[i];
        if (th->impl == NULL) {
            if (optExitCodeOuts)
                optExitCodeOuts[i] = th->impl_code;
            if (optOutOuts)
                optOutOuts[i] = NULL;
            continue;
        }
        DWORD ec;
        GetExitCodeThread((HANDLE) th->impl, &ec);
        th->impl_code = (int) ec;
        if (optExitCodeOuts)
            optExitCodeOuts[i] = (int) ec;
        if (optOutOuts)
            optOutOuts[i] = th->impl_disp->ret;
        CloseHandle((HANDLE) th->impl);
        th->impl = NULL;
        HeapFree(GetProcessHeap(), 0, th->impl_disp);
    }
}

void uthread_kill(
        uthread_t * th)
{
    if (th->impl == NULL) return;
    TerminateThread((HANDLE*) th->impl, 1);
    CloseHandle((HANDLE) th->impl);
    th->impl = NULL;
    th->impl_code = 1;
    HeapFree(GetProcessHeap(), 0, th->impl_disp);
}

#else 

int uthread_create(
        uthread_t * th,
        void *(*start_routine)(void *),
        void * arg)
{
    return pthread_create(&th->impl, NULL, start_routine, arg);
}

int uthread_join(
        uthread_t * th,
        void ** ret_out)
{
    return pthread_exit(th->impl, ret_out);
}

void uthread_kill(
        uthread_t * th)
{
    (void) pthread_kill(th->impl, SIGABRT);
}

void uthread_join_all(
        size_t count,
        uthread_t * threads,
        int   * optExitCodeOuts,
        void ** optOutOuts)
{
    for (size_t i = 0; i < count; i ++)
    {
        int code = uthread_join(&threads[i], &optOutOuts[i]);
        if (optExitCodeOuts)
            optExitCodeOuts[i] = code;
    }
}

#endif

#endif 

#endif
