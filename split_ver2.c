#include "split_ver2.h"
#include <stdio.h>
#include <stdlib.h>

/* the split funtion will return N x M array of chars
 * containing the number of strings produced by splitting
 * a huge string by a delimiter. However, nothing prevents
 * the user to access the element outside of the memory,
 * so a precautions or a private char** may be used as
 * a safe mode
 * */

#ifdef _DEBUG
#   define DBG_PRINT(TYPE,...) printf(TYPE, __VA_ARGS__)
#else
#   define DBG_PRINT
#endif

/***************************************************************/
//adts

typedef struct _m
{
    void* (*allocate)(size_t);
    void  (*afree)(void*);
} Memory;

typedef struct s
{
    char* m_data;
    struct s* next;
} cnode;

typedef unsigned int list_size;

/***************************************************************/

/***************************************************************/
//definitions:

char** split_ver2(const char* str, char delimiter);
/*this function will retrun to pRet a new string, and will return
 * the current pointer that it sliced to:
 * ex: "abc:def" with delim : will be
 * pRet == abc ; and will return :
 * [hint] consider a hybrid of strchr and strcpy
*/
static const char* strcopy_to_delim(char** pRet, const char* src, char d);
static void push(const char* str);
static char* pop(void);
static int aux_strlen(const char*s);
static char** node_to_str(void);
static  char* slice_str(const char*s, int ,int);
static void* debug_malloc(size_t size);
static void  debug_free(void *pData);
/**********************************************************************/

//PRIVATE:

static cnode* s_head=NULL;
static list_size s_listSize = 0;

static char** s_strings = {0};
static int s_stringsSize = 0;

//debug only trak the malloc and frees using the debug malloc/free functions
#ifdef _DEBUG
static int s_malloc_calls = 0;
static int s_free_calls = 0;
#endif

#ifndef _DEBUG
static Memory s_memory =
{
    .allocate = malloc,
    .afree = free
};
#else
static Memory s_memory =
{
    .allocate = debug_malloc,
    .afree = debug_free
};
#endif

//instances pool




/****************************************************************/
static void debug_free(void* pData)
{
    s_free_calls++;
    free(pData);
}

static void* debug_malloc(size_t size)
{
    s_malloc_calls++;
    return malloc(size);
}

/*WARNING: [malloc used!]
 * a typical array slice you won`t have in C
*/
static char* slice_str(const char*s, int from,int to)
{
    if ( from >= to )
        return 0;
    int size = to - from+1;
    char* str = (char*) s_memory.allocate(size * sizeof(char));
    char* ret = str;
    char* r = &s[from];
    while ( from < to )
    {
        *ret++ = *r++;
        from++;
    }
    return str;
}

/*note: consider this to be a strcpy and strchr all in one
 * copy to delimiter and return the current offset */
static const char* strcopy_to_delim(char** pRet, const char* s, char d)
{
    int i = 0;
    const char* start = s;
    while ( *start != '\0' )
    {
        if ( *start == d )
            break;
        i++; start++;
    }

    char* newstr = (char*) s_memory.allocate(sizeof(char)*i);
    char* ret = newstr;

    int j=0;
    for(; j < i; j++)
        ret[j] = s[j];

    (*pRet) = newstr;
    return start; //should be the first delimiter match
}


char** split_ver2(const char* str, char delimiter)
{
    //1. trim to both ends
    char* begin = str;
    int from, to;
    from = to = 0;
    from = 0;
    to = aux_strlen(str);
    char* end = &str[aux_strlen(str)-1];

    for (; *begin == delimiter; begin++)
        from++;

    for (; *end == delimiter; end-- )
        to--;

    //2. add the slices to a node
    char* tmp_str = slice_str(str, from, to);
    //3. don`t forget to delete tmp_str after the split
    char* it = tmp_str;

    char* dest = 0;
    char* curr = 0 ;

    while ( *it != '\0' )
    {
        curr =  (char*)strcopy_to_delim(&dest, it, delimiter);
        //push a text node
        push(dest);
        while ( *curr == delimiter ) curr++;
        curr--;
        it = curr;
        it++;
    }
    s_stringsSize = s_listSize;
    s_memory.afree(tmp_str);
    return node_to_str(); //retyrn array of char ptrs
}

static int aux_strlen(const char*s)
{
    int i =0;
    while ( *s != '\0')
    {
        i++; s++;
    }
    return i;
}


static void push(const char* str)
{
    if ( 1 )
    {
        cnode* n = (cnode*)s_memory.allocate(sizeof(char));
        if ( !n )
            return;
        s_listSize += 1;
        n->m_data = str;
        n->next = s_head;
        s_head = n;
    }
}


static char* aux_strdup(const char* s)
{
    int i = 0 ;
    i = aux_strlen(s);
    char* r = (char*) s_memory.allocate(sizeof(char)* i+1);
    if ( !r )
        return 0;
    char* ret = r;
    while ( *s != '\0')
    {
        *ret++ = *s++;
    }
    *ret++ = 0;
    return r;
}

static char* pop(void)
{
    cnode* del = s_head;
    char* ret = aux_strdup(s_head->m_data);

    s_head =  s_head->next;
    s_memory.afree(del);

    return ret;
}


/* empty the list and return a 2d array of char pointers */
static char** node_to_str(void)
{
    list_size i=0;
    char** r = s_memory.allocate(s_listSize * sizeof(char*));
    while ( i < s_listSize )
    {
        r[i] = pop();
        i++;
    }
    return r;
}

//can be split if only one delimiter is matched
int aux_canBeSplit(const char* s, char d)
{
    const char* it = s;
    while ( *it != '\0')
    {
        if ( *it == d )
            return 1;
        it++;
    }
    return 0;
}


// these functions will be present to the end user
/************************* RELEASE ***************************/

//1. get the num of splited elements
const unsigned int
getSize(void)
{
    return s_stringsSize;
}

//2. get the element At from the list, 0 if out of bounds
//no asserts
char*
getAt(int i)
{
    if ( i < 0 || i >= s_stringsSize)
        return 0;
    else
        return s_strings[i];
}

//3. split a huge string to substrings by a given delimiter
unsigned int
split(const char *s, char d)
{
    if ( !s )
        return 0;
    if ( !aux_canBeSplit(s, d))
        return 0; //can`t split the string

    s_strings = split_ver2(s, d);
    return s_stringsSize;
}

/************************* RELEASE ***************************/


