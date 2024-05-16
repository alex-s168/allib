// mutex and rwlock for linux and windows

#ifndef KLIBS_MUTEX_H
#define KLIBS_MUTEX_H

#ifdef _WIN32
#include <windows.h>

typedef HANDLE  mutex_t;

static void mutex_create(mutex_t *mutex) {
    *mutex = CreateMutexA(NULL, false, NULL);
}

static void mutex_lock(mutex_t *mutex)  {
    WaitForSingleObject(*mutex, 0);
}

static void mutex_unlock(mutex_t *mutex) {
    ReleaseMutex(*mutex);
}

static void mutex_destroy(mutex_t *mutex) {
    CloseHandle(*mutex);
}

// TODO: do better

typedef mutex_t rwlock_t;

static void rwlock_create(rwlock_t *rwlock) {
    mutex_create(rwlock);
}

static void rwlock_r_lock(rwlock_t *rwlock) {
    mutex_lock(rwlock);
}

static void rwlock_w_lock(rwlock_t *rwlock) {
    mutex_lock(rwlock);
}

static void rwlock_unlock(rwlock_t *rwlock) {
    mutex_unlock(rwlock);
}

static void rwlock_destroy(rwlock_t *rwlock) {
    mutex_destroy(rwlock);
}

#else
#include <pthread.h>
typedef pthread_mutex_t mutex_t;

static void mutex_create(mutex_t *mutex) {
    pthread_mutex_init(mutex, NULL);
}

static void mutex_lock(mutex_t *mutex)  {
    pthread_mutex_lock(mutex);
}

static void mutex_unlock(mutex_t *mutex) {
    pthread_mutex_unlock(mutex);
}

static void mutex_destroy(mutex_t *mutex) {
    pthread_mutex_destroy(mutex);
}

typedef pthread_rwlock_t rwlock_t;

static void rwlock_create(rwlock_t *rwlock) {
    pthread_rwlock_init(rwlock, NULL);
}

static void rwlock_r_lock(rwlock_t *rwlock) {
    pthread_rwlock_rdlock(rwlock);
}

static void rwlock_w_lock(rwlock_t *rwlock) {
    pthread_rwlock_wrlock(rwlock);
}

static void rwlock_unlock(rwlock_t *rwlock) {
    pthread_rwlock_unlock(rwlock);
}

static void rwlock_destroy(rwlock_t *rwlock) {
    pthread_rwlock_destroy(rwlock);
}

#endif

#endif //KLIBS_MUTEX_H
