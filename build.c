#ifndef CC
# define CC        "clang"
#endif
#ifndef CC_ARGS
# define CC_ARGS   "-O3"
#endif
#ifndef CXX_ARGS
# define CXX_ARGS  "-O3"
#endif

#ifndef HAVE_WINSOCK
# define HAVE_WINSOCK 1 
#endif

#include "build_c/build.h"

/* ========================================================================= */

enum CompileResult target_clean() {
    rmdir("build/");
    return CR_OK;
}

/* ========================================================================= */

struct CompileData germanstr_files[] = {
    DIR("build/"),
    DIR("build/germanstr/"),
    SP(CT_C, "germanstr/germanstr.c"),
};

struct CompileData kash_files[] = {
    DIR("build/"),
    DIR("build/kash/"),
    SP(CT_C, "kash/fnv1a.c"),
    SP(CT_C, "kash/pearson.c"),
    SP(CT_C, "kash/sbox.c"),
    CCARG("-Wno-shift-count-overflow"),
};

struct CompileData kallok_files[] = {
    DIR("build/"),
    DIR("build/kallok/"),
    SP(CT_C, "kallok/libc.c"),
    SP(CT_C, "kallok/statistic.c"),
    SP(CT_C, "kallok/pages.c"),
    SP(CT_C, "kallok/virtual.c"),
    SP(CT_C, "kallok/fixed_basic.c"),
    SP(CT_C, "kallok/paged.c"),
    SP(CT_C, "kallok/basic.c"),
    SP(CT_C, "kallok/single_fixed.c"),
    SP(CT_C, "kallok/fastalloc.c"),
	SP(CT_C, "kallok/multi.c"),
};

struct CompileData dynamic_list_files[] = {
    DIR("build/"),
    DIR("build/dynamic_list/"),
    SP(CT_C, "dynamic_list/add_and_addAll.c"),
    SP(CT_C, "dynamic_list/init_and_clear.c"),
    SP(CT_C, "dynamic_list/insertAt_and_insertAllAt.c"),
    SP(CT_C, "dynamic_list/removeAt_and_removeRange.c"),
    SP(CT_C, "dynamic_list/reserve_and_shrink.c"),
    SP(CT_C, "dynamic_list/anylist_impl.c"),
};

struct CompileData fixed_list_files[] = {
    DIR("build/"),
    DIR("build/fixed_list/"),
    SP(CT_C, "fixed_list/get_and_set.c"),
    SP(CT_C, "fixed_list/indexOf.c"),
    SP(CT_C, "fixed_list/anylist_impl.c"),
};

struct CompileData linked_list_files[] = {
    DIR("build/"),
    DIR("build/linked_list/"),
    SP(CT_C, "linked_list/add.c"),
    SP(CT_C, "linked_list/addAll.c"),
    SP(CT_C, "linked_list/clear.c"),
    SP(CT_C, "linked_list/find.c"),
    SP(CT_C, "linked_list/findLast.c"),
    SP(CT_C, "linked_list/flatten.c"),
    SP(CT_C, "linked_list/fromFixed.c"),
    SP(CT_C, "linked_list/fromLinks.c"),
    SP(CT_C, "linked_list/linkAt.c"),
    SP(CT_C, "linked_list/remove.c"),
    SP(CT_C, "linked_list/removeMultiple.c"),
    SP(CT_C, "linked_list/insert.c"),
    SP(CT_C, "linked_list/anylist_impl.c"),
};

struct CompileData filelib_files[] = {
    DIR("build/"),
    DIR("build/filelib"),
    SP(CT_C, "filelib/copy.c"),
    SP(CT_C, "filelib/readFile.c"),
    SP(CT_C, "filelib/readLine.c"),
    SP(CT_C, "filelib/readSplit.c"),
};

struct CompileData niglob_files[] = {
    DIR("build/"),
    DIR("build/niglob"),
    SP(CT_C, "niglob/niglob.c"),
};

struct CompileData tcp_files[] = {
#if HAVE_WINSOCK
    DIR("build/"),
    DIR("build/tcp"),
    SP(CT_C, "tcp/wintcpclient.c"),
    SP(CT_C, "tcp/posixtcpclient.c"),
#endif
};

struct CompileData always_recomp_files[] = {
    DIR("build/lists/"),
    SP(CT_C, "lists/copy.c"),
    SP(CT_C, "memlib.c"),
    SP(CT_C, "miniconf.c"),
    SP(CT_C, "minifmt.c"),
};

enum CompileResult target_all() {
    START;

    bool notOut = !exists("build/all.a");
    if (notOut)
        puts("recompiling all");

    VaList comp = newVaList();
    comp = vaListConcat(comp, ASVAR(always_recomp_files));

    if (notOut || file_changed("germanstr/"))
        comp = vaListConcat(comp, ASVAR(germanstr_files));

    if (notOut || file_changed("kallok/"))
        comp = vaListConcat(comp, ASVAR(kallok_files));

    if (notOut || file_changed("kash/"))
        comp = vaListConcat(comp, ASVAR(kash_files));

    if (notOut || file_changed("dynamic_list/"))
        comp = vaListConcat(comp, ASVAR(dynamic_list_files));

    if (notOut || file_changed("fixed_list/"))
        comp = vaListConcat(comp, ASVAR(fixed_list_files));

    if (notOut || file_changed("linked_list/"))
        comp = vaListConcat(comp, ASVAR(linked_list_files));

    if (notOut || file_changed("filelib/"))
        comp = vaListConcat(comp, ASVAR(filelib_files));

    if (notOut || file_changed("niglob/"))
        comp = vaListConcat(comp, ASVAR(niglob_files));

    if (notOut || file_changed("tcp/"))
        comp = vaListConcat(comp, ASVAR(tcp_files));

    DO(compile(VLI(comp)));

    VaList all = newVaList();
    all = vaListConcat(all, ASVAR(germanstr_files));
    all = vaListConcat(all, ASVAR(kallok_files));
    all = vaListConcat(all, ASVAR(kash_files));
    all = vaListConcat(all, ASVAR(dynamic_list_files));
    all = vaListConcat(all, ASVAR(fixed_list_files));
    all = vaListConcat(all, ASVAR(linked_list_files));
    all = vaListConcat(all, ASVAR(filelib_files));
    all = vaListConcat(all, ASVAR(niglob_files));
    all = vaListConcat(all, ASVAR(tcp_files));
    all = vaListConcat(all, ASVAR(always_recomp_files));

    DO(linkTask(VLI(all), "build/all.a"));

    END;
}

/* ========================================================================= */

enum CompileResult target_tests() {
    static struct CompileData data[] = {
        DIR("build/"),
        DIR("build/tests/"),

        DEP("build/all.a"),
        LDARG("-lm"),
    };

    return test_dir("tests", LI(data));
}

/* ========================================================================= */

struct Target targets[] = {
	{ .name = "all.a", .run = target_all },
    { .name = "tests", .run = target_tests },
    { .name = "clean", .run = target_clean },
};

automain(targets);
