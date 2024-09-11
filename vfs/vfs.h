#ifndef VFS_H 
#define VFS_H


#include "../attrib.h"

#include <stdint.h>

// paths are absolute ; dirs and file name delimetered by /

// can be or-ed together 
typedef enum {
    ACCESS_META   =     0b1,
    ACCESS_READ   =    0b10,
    ACCESS_WRITE  =   0b100,
    ACCESS_DELETE =  0b1000,
    ACCESS_CREATE = 0b10000,
} VFSFileAccessFlags;

typedef struct {
    bool     is_dir;
    size_t   bytes;
    uint64_t created_timestamp; // unix time 
    uint64_t modified_timestamp; // unix time 
} VFSFileMeta;

typedef struct VFSImpl VFSImpl;

typedef struct {
    VFSImpl * impl; 
    void    * userdata;
} VFS;

typedef struct {
    VFSImpl * impl;
    void    * userdata;
} VFSFile;

typedef struct {
    // returns malloc()-ed null-terminated string 
    char* (*basename)(void*);

    // returns malloc()-ed null-terminated string 
    char* (*parent_abs_path)(void*);

    // 0 if ok 
    // if ok, changed acces perms to flags 
    int   (*request_access)(void*, VFSFileAccessFlags);

    // 0 if ok ; delete this file or directory ; requires ACCESS_DELETE perms
    int   (*remove)(void*);

    // 0 if ok ; requires ACCESS_META 
    int   (*meta)(void*, VFSFileMeta * dest);

    // requires ACCESS_META
    bool  (*exists)(void*);

    // 0 if ok ; requires ACCESS_READ
    int   (*read)(void*, size_t offset, char * dest, size_t num);

    // 0 if ok ; requires ACCESS_WRITE 
    int   (*write)(void*, size_t offset, const char * src, size_t num);

    // important!!
    void  (*drop)(void*);
} VFSFileImpl;

struct VFSImpl {
    VFSFileImpl file_impl;

    // returns malloc()-ed null-terminated string that can be null 
    char* (*readlink)(void*, const char * file_path);

    // returned file needs to be VFS_f(file, drop) ed after finish using ; file does not need to exist 
    VFSFile (*open)(void*, const char * file_path);

    // iter needs to be NULL at start of iterating 
    // after iterating done, needs to be dropped 
    // not allowed to be dropped during iterating 
    // dir needs to be the same across iterations
    //
    // requires ACCESS_READ of dir 
    // iter gets perms:
    //   if dir has perms ACCESS_WRITE -> ACCESS_DELETE 
    void (*dir_next)(VFSFile dir, VFSFile * iter);

    // 0 if ok ; dir needs to have ACCESS_CREATE
    int (*dir_new_file)(VFSFile dir, const char * file_name);
};

// example usage:  VFS_(fs, exists, "hello.txt")
#define VFS_fs(vfs, command, ...) \
    ((vfs).impl->command((vfs).userdata __VA_OPT__(,) __VA_ARGS__))

#define VFS_f(file, command, ...) \
    VFS_fs(file, file_impl.command __VA_OPT__(,) __VA_ARGS__ )

#include <stdio.h>

// libc file system (using fopen, fclose, ...)
VFS VFS_libc(void);

// takes ownership of the file* and closes it when vfs file dropped
VFSFile VFSFile_from_libc(FILE* libc);

#include "../kallok/kallok.h"

VFS VFS_ramfs_create(Ally allocator);
void VFS_ramfs_destroy(VFS);

#endif
