#include "impl_utils.h"

#include <string.h>

/**
 * @param list Self
 * @param data The pointer to the element
 * @return 0 if ok
 */
int DynamicList_add(DynamicList *list, const void *data) 
{
    void* dest = DynamicList_addp(list);
	
	if(!dest)
	{
		return 1;
	}
	
    memcpy(dest, data, list->fixed.stride);
	
    return 0;
}

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_addp(DynamicList *list)
{
    if (DynamicList_reserve(list, 1))
        return 0;

    void *dest = FixedList_get(list->fixed, list->fixed.len);

    list->fixed.len ++;

    return dest;
}

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_setp(DynamicList *list, size_t pos)
{
	if(pos > list->fixed.len)
	{
		if(DynamicList_reserve(list, pos-list->fixed.len+1))
		{
			return 0;
		}
		list->fixed.len = pos+1;
	}
	
    void *dest = FixedList_get(list->fixed, pos);

    return dest;
}

/**
 * @param list Self
 * @param data The pointer to the element
 * @return 0 if ok
 */
int DynamicList_set(DynamicList *list, size_t pos, const void *data) {
    void *dest = DynamicList_setp(list, pos);
    if (dest == NULL) {
        return 1;
    }

    memcpy(dest, data, list->fixed.stride);

    return 0;
}

/**
 * @param list Self
 * @return non 0 if ok
 */
void* DynamicList_addAllp(DynamicList *list, size_t len) {
    if (DynamicList_reserve(list, len))
        return NULL;

    void *dest = FixedList_get(list->fixed, list->fixed.len);

    list->fixed.len += len;

    return dest;
}

/**
 * @param list Self
 * @param data The array of elements
 * @return 0 if ok
 */
int DynamicList_addAll(DynamicList *list, const void *data, size_t len, size_t stride) {
    void *dest = DynamicList_addAllp(list, len);
    if (dest == NULL) {
        return 1;
    }

    strided_memcpy(dest, list->fixed.stride,
                   data, stride,
                   len, list->fixed.elSize);

    return 0;
}
