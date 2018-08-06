#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

typedef enum hashTable_rc {
    HT_FAILURE = -1,
    HT_SUCCESS = 0,
    HT_EXISTS = 1,
    HT_NOTEXISTS = 2,
    HT_XINPUT = 3
} hashTable_rc_t;

typedef struct hashTable_entry {
    char *key;
    void *value;
    struct hashTable_entry *next;
} hashTable_entry_t;

typedef struct hashTable {
    size_t size;
    struct hashTable_entry **entries;
} hashTable_t;

hashTable_t *hashTable_create ( size_t size );
enum hashTable_rc hashTable_add_entry ( struct hashTable *hashTable, const char *key, void *value );
void *hashTable_find_entry_value ( struct hashTable *hashTable, const char *key );
void *hashTable_update_entry ( hashTable_t *hashTable, const char *key, void *value );
enum hashTable_rc hashTable_remove_entry ( struct hashTable *hashTable, const char *key );
void hashTable_free ( struct hashTable *hashTable );

#endif
