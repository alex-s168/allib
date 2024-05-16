#ifndef CC
# define CC        "tcc"
#endif
#ifndef CC_ARGS
# define CC_ARGS   "-O2"
#endif
#ifndef CXX_ARGS
# define CXX_ARGS  "-O2"
#endif

#include "build_c/build.h"

#define MINIRENT_IMPLEMENTATION
#include "minirent.h"

/* ========================================================================= */

enum CompileResult target_clean() {
    rmdir("build/");
    return CR_OK;
}

/* ========================================================================= */

struct CompileData target_kash_files[] = {
    DIR("build/"),

    DIR("build/kash/"),
    SP(CT_C, "kash/fnv1a.c"),
};

enum CompileResult target_kash() {
    ONLY_IF({
        NOT_FILE("build/kash.a");
        CHANGED("kash/");
    });

    START;
        DO(compile(LI(target_kash_files)));
        DO(linkTask(LI(target_kash_files), "build/kash.a"));
    END;
}

/* ========================================================================= */

enum CompileResult target_smallstr() {
#ifdef __AVX512BW__
    static struct CompileData files[] = {
        DIR("build/"),
        DIR("build/smallstr/"),
        SP(CT_C, "smallstr/smallstr_avx512.c"),
    };
#else
    static struct CompileData files[] = {
        DIR("build/"),
        DIR("build/smallstr/"),
        SP(CT_C, "smallstr/smallstr_novec.c"),
    };
#endif 
    
    START;
    DO(compile(LI(files)));
    DO(linkTask(LI(files), "build/smallstr.a"));

    END;
}
/* ========================================================================= */

struct CompileData target_kallok_files[] = {
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
    SP(CT_C, "kallok/multi.c"),
};

enum CompileResult target_kallok() {
    ONLY_IF({
        NOT_FILE("build/kallok.a");
        CHANGED("kallok/");
    });

    START;
        DO(compile(LI(target_kallok_files)));
        DO(linkTask(LI(target_kallok_files), "build/kallok.a"));
    END;
}

/* ========================================================================= */

struct CompileData target_kollektions_files[] = {
    DIR("build/"),

    DIR("build/dynamic_list/"),
    SP(CT_C, "dynamic_list/add_and_addAll.c"),
    SP(CT_C, "dynamic_list/init_and_clear.c"),
    SP(CT_C, "dynamic_list/insertAt_and_insertAllAt.c"),
    SP(CT_C, "dynamic_list/removeAt_and_removeRange.c"),
    SP(CT_C, "dynamic_list/reserve_and_shrink.c"),

    DIR("build/fixed_list/"),
    SP(CT_C, "fixed_list/get_and_set.c"),
    SP(CT_C, "fixed_list/indexOf.c"),

    DIR("build/lists/"),
    SP(CT_C, "lists/copy.c"),

    DIR("build/blocking_list/"),
    SP(CT_C, "blocking_list/access.c"),
    SP(CT_C, "blocking_list/init_and_destroy.c"),

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
};

enum CompileResult target_kollektions() {
    ONLY_IF({
        NOT_FILE("build/kollektions.a");
        CHANGED("linked_list/");
        CHANGED("blocking_list/");
        CHANGED("lists/");
        CHANGED("dynamic_list/");
        CHANGED("fixed_list/");

        CHANGED("attrib.h");
        CHANGED("mutex.h");
    });

    START;
        DO(compile(LI(target_kollektions_files)));
        DO(linkTask(LI(target_kollektions_files), "build/kollektions.a"));
    END;
}

/* ========================================================================= */

enum CompileResult target_tests() {
    START_TESTING;

    // TODO: add this to build.c

    DIR *dir = opendir("tests/");

    size_t id = 0;
    struct dirent *dp = NULL;
    while ((dp = readdir(dir))) {
        if (id < 2) { // . and ..
            id ++;
            continue;
        }

        static char infile[256];
        sprintf(infile, "tests/%s", dp->d_name);

        static char outfile[256];
        sprintf(outfile, "build/tests/%s.exe", dp->d_name);

        static char objfile[256];
        sprintf(objfile, "build/tests/%s.o", dp->d_name);

        static struct CompileData data[] = {
            DIR("build/"),
            DIR("build/tests/"),
            { .type = CT_C, .srcFile = infile, .outFile = objfile },
            RUN(outfile),
            DEP("build/kallok.a"),
            DEP("build/kollektions.a"),
            DEP("build/smallstr.a"),
        };

        resTemp = test_impl(outfile, id - 1, LI(data));
        if (resTemp != CR_OK) res = resTemp;
    
        id ++;
    }

    (void) closedir(dir);

    END_TESTING;
}

/* ========================================================================= */

struct Target targets[] = {
	{ .name = "kallok.a",       .run = target_kallok },
	{ .name = "kollektions.a",  .run = target_kollektions },
    { .name = "kash.a",         .run = target_kash },
    { .name = "smallstr.a",     .run = target_smallstr },
	{ .name = "tests",          .run = target_tests },
    { .name = "clean",          .run = target_clean },
};

#define TARGETS_LEN (sizeof(targets) / sizeof(targets[0]))

int main(int argc, char **argv) {
    return build_main(argc, argv, targets, TARGETS_LEN);
}
