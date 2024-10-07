#ifndef KOLLEKTIONS_DYNAMIC_LIST_H
#define KOLLEKTIONS_DYNAMIC_LIST_H

#include <stddef.h>

#include "../lists/AnyList.h"
#include "../fixed_list/fixed_list.h"
#include "../kallok/kallok.h"
#include "../attrib.h"

typedef struct {
    FixedList fixed;

INTERNAL

#if A_CFG_DYN_LIST_OVERALLOC
    size_t cap;
#endif

    Ally ally;
} DynamicList;

// THE RETURNED LIST DEPENDS ON THE GIVEN PTR!!!
MutAnyList DynamicList_asAny(DynamicList *list);

/**
 * @param list Self
 * @param stride The size of each element
 * @param cap The initial maximum amount of elements
 * @param data The array of data. needs to be at least @arg cap * @arg stride
 * @return 0 if ok
 */
int DynamicList_init(DynamicList *list, size_t stride, Ally ally, size_t initCap);

/**
 * @param list Self
 */
void DynamicList_clear(DynamicList *list);

/**
 * @param list Self
 * @param data The pointer to the element
 * @return 0 if ok
 */
int DynamicList_add(DynamicList *list, const void *data);

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_addp(DynamicList *list);

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_setp(DynamicList *list, size_t pos);

/**
 * @param list Self
 * @param data The pointer to the element
 * @return 0 if ok
 */
int DynamicList_set(DynamicList *list, size_t pos, const void *data);

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_addAllp(DynamicList *list, size_t len);

/**
 * @param list Self
 * @param data The array of elements
 * @return 0 if ok
 */
int DynamicList_addAll(DynamicList *list, const void *data, size_t len, size_t stride);

/**
 * @param list Self
 * @param pos At which index to remove.
 */
void DynamicList_removeAt(DynamicList *list, size_t pos);

/**
 * @param list Self
 * @param first The start index where to remove
 * @param last The end index where to remove
 */
void DynamicList_removeRange(DynamicList *list, size_t first, size_t last);

/**
 * @param list Self
 * @param list At what position to insert the element
 * @param data The element to add
 * @return 0 if ok
 */
int DynamicList_insertAllAt(DynamicList *list, size_t index,
                            const void *data, size_t len, size_t stride);

/**
 * @param list Self
 * @param list At what position to insert the element
 * @param data The element to add
 * @return 0 if ok
 */
static inline int DynamicList_insertAt(DynamicList *list, size_t index,
                                       const void *data) {
    return DynamicList_insertAllAt(list, index, data, 1, list->fixed.stride);
}

/**
 * Reserves space for at least x additional elements on top of the length of the list
 * @param list Self
 * @param additional The amount of additional elements to reserve on top of the length of the list
 * @return 0 if ok
 */
#if !A_CFG_DYN_LIST_OVERALLOC 
static 
#endif
int DynamicList_reserve(DynamicList *list, size_t additional)
#if A_CFG_DYN_LIST_OVERALLOC 
; 
#else
{  return 0; }
#endif

/**
 * Reserves space for exactly x additional elements on top of the length of the list
 * @param list Self
 * @param additional The amount of additional elements to reserve on top of the length of the list
 * @return 0 if ok
 */
#if !A_CFG_DYN_LIST_OVERALLOC 
static 
#endif
int DynamicList_reserveExact(DynamicList *list, size_t additional)
#if A_CFG_DYN_LIST_OVERALLOC 
; 
#else
{  return 0; }
#endif

/**
 * Shrinks the list allocation to be as big as the list.
 * @param list Self
 * @return 0 if ok
 */
#if !A_CFG_DYN_LIST_OVERALLOC 
static 
#endif
int DynamicList_shrink(DynamicList *list)
#if A_CFG_DYN_LIST_OVERALLOC 
; 
#else
{  return 0; }
#endif

extern AnyListImpl DynamicList_anyListImpl;
extern MutAnyListImpl DynamicList_mutAnyListImpl;

#endif //KOLLEKTIONS_DYNAMIC_LIST_H
