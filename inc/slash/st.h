/* This is a public domain general purpose hash table package written by Peter Moore @ UCB. */
/* @(#) st.h 5.1 89/12/14 */

#ifndef ST_INCLUDED

#define ST_INCLUDED

#include "mem.h"

#if SIZEOF_LONG == SIZEOF_VOIDP
typedef unsigned long st_data_t;
#elif SIZEOF_LONG_LONG == SIZEOF_VOIDP
typedef unsigned LONG_LONG st_data_t;
#else
# error ---->> st.c requires sizeof(void*) == sizeof(long) to be compiled. <<---
-
#endif
#define ST_DATA_T_DEFINED

typedef struct st_table st_table_t;

struct st_hash_type {
    int (*compare)();
    int (*hash)();
};

struct st_table {
    sl_gc_arena_t* arena;
    struct st_hash_type *type;
    int num_bins;
    int num_entries;
    struct st_table_entry **bins;
};

typedef struct st_table_entry st_table_entry;

struct st_table_entry
{
    unsigned int hash;
    st_data_t key;
    st_data_t record;
    st_table_entry *next;
};

#define st_is_member(table,key) st_lookup(table,key,(st_data_t *)0)

enum st_retval {ST_CONTINUE, ST_STOP, ST_DELETE, ST_CHECK};

#ifndef _
# define _(args) args
#endif
#ifndef ANYARGS
# ifdef __cplusplus
#   define ANYARGS ...
# else
#   define ANYARGS
# endif
#endif

st_table_t *st_init_table _((sl_gc_arena_t* arena, struct st_hash_type *));
st_table_t *st_init_table_with_size _((sl_gc_arena_t* arena, struct st_hash_type *, int));
st_table_t *st_init_numtable _((sl_gc_arena_t* arena));
st_table_t *st_init_numtable_with_size _((sl_gc_arena_t* arena, int));
st_table_t *st_init_strtable _((sl_gc_arena_t* arena));
st_table_t *st_init_strtable_with_size _((sl_gc_arena_t* arena, int));
int st_delete _((st_table_t *, st_data_t *, st_data_t *));
int st_delete_safe _((st_table_t *, st_data_t *, st_data_t *, st_data_t));
int st_insert _((st_table_t *, st_data_t, st_data_t));
int st_lookup _((st_table_t *, st_data_t, st_data_t *));
int st_foreach _((st_table_t *, int (*)(ANYARGS), st_data_t));
void st_add_direct _((st_table_t *, st_data_t, st_data_t));
void st_free_table _((st_table_t *));
void st_cleanup_safe _((st_table_t *, st_data_t));
st_table_t *st_copy _((st_table_t *));

#define ST_NUMCMP	((int (*)()) 0)
#define ST_NUMHASH	((int (*)()) -2)

#define st_numcmp	ST_NUMCMP
#define st_numhash	ST_NUMHASH

int st_strhash();

#endif /* ST_INCLUDED */
