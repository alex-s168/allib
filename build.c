#ifndef CC
# define CC        "clang"
#endif
#ifndef CC_ARGS
# define CC_ARGS   "-O3"
#endif
#ifndef CXX_ARGS
# define CXX_ARGS  "-O3"
#endif

#include "build_c/build.h"

/* ========================================================================= */

enum CompileResult target_clean() {
    rmdir("build/");
    return CR_OK;
}


/* ========================================================================= */

struct CompileData target_germanstr_files[] = {
    DIR("build/"),

    DIR("build/germanstr/"),
    SP(CT_C, "germanstr/germanstr.c"),
};

enum CompileResult target_germanstr() {
    ONLY_IF({
        NOT_FILE("build/germanstr.a");
        CHANGED("germanstr/");
    });

    START;
        DO(compile(LI(target_germanstr_files)));
        DO(linkTask(LI(target_germanstr_files), "build/germanstr.a"));
    END;
}

/* ========================================================================= */

struct CompileData target_kash_files[] = {
    DIR("build/"),

    DIR("build/kash/"),
    SP(CT_C, "kash/fnv1a.c"),
    SP(CT_C, "kash/pearson.c"),
    SP(CT_C, "kash/sbox.c"),

    CCARG("-Wno-shift-count-overflow"),
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

enum CompileResult target_opstr() {
    static struct CompileData files[] = {
        DIR("build/"),
        DIR("build/opstr/"),
#ifdef __AVX512BW__
        SP(CT_C, "opstr/opstr_avx512.c"),
#else
        SP(CT_C, "opstr/opstr_novec.c"),
#endif 
    };
    
    START;
    DO(compile(LI(files)));
    DO(linkTask(LI(files), "build/opstr.a"));

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
    SP(CT_C, "memlib.c"),
};

enum CompileResult target_kallok() {
    ONLY_IF({
        NOT_FILE("build/kallok.a");
        CHANGED("kallok/");
        CHANGED("memlib.c");
        CHANGED("memlib.h");
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
    SP(CT_C, "dynamic_list/anylist_impl.c"),

    DIR("build/fixed_list/"),
    SP(CT_C, "fixed_list/get_and_set.c"),
    SP(CT_C, "fixed_list/indexOf.c"),
    SP(CT_C, "fixed_list/anylist_impl.c"),

    DIR("build/lists/"),
    SP(CT_C, "lists/copy.c"),

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

enum CompileResult target_kollektions() {
    ONLY_IF({
        NOT_FILE("build/kollektions.a");
        CHANGED("linked_list/");
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
    static struct CompileData data[] = {
        DIR("build/"),
        DIR("build/tests/"),

        DEP("build/kollektions.a"),
        DEP("build/kallok.a"),
        DEP("build/kash.a"),
        DEP("build/opstr.a"),
        DEP("build/germanstr.a"),
        LDARG("-lm"),
    };

    return test_dir("tests", LI(data));
}

/* ========================================================================= */

struct Target targets[] = {
	{ .name = "kallok.a",       .run = target_kallok },
	{ .name = "kollektions.a",  .run = target_kollektions },
    { .name = "kash.a",         .run = target_kash },
    { .name = "opstr.a",        .run = target_opstr },
    { .name = "germanstr.a",    .run = target_germanstr },
    { .name = "tests",          .run = target_tests },
    { .name = "clean",          .run = target_clean },
};

#define TARGETS_LEN (sizeof(targets) / sizeof(targets[0]))

int main(int argc, char **argv) {
    return build_main(argc, argv, targets, TARGETS_LEN);
}
