#ifndef KOLLEKTIONS_ATTRIB_H
#define KOLLEKTIONS_ATTRIB_H


#define INTERNAL /* internal: */
#define UNSAFE   /* unsafe: */

#define EXPERIMENTAL_FN(name) experimental_ ## name

#define PACKED __attribute__ ((packed))
#define NORETURN __attribute__ ((noreturn))

#define TYPES(...) /* */


// setting this saves two pointers and speeds up memory allocation, at the cost at disabling all allocs except libc
#ifndef A_CFG_ALLY_ONLY_LIBC
#define A_CFG_ALLY_ONLY_LIBC 0
#endif 

// setting this to 0 removes the cap field -> saves one size_t per DynamicList
#ifndef A_CFG_DYN_LIST_OVERALLOC
#define A_CFG_DYN_LIST_OVERALLOC 1
#endif 


#endif //KOLLEKTIONS_ATTRIB_H
