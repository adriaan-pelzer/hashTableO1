#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTableO1.h"

hashTable_t *hashTable_create ( size_t size ) {
    hashTable_t *rc = NULL, *_rc = NULL;

    if ( size < 1 ) {
        goto over;
    }

    if ( ( _rc = malloc ( sizeof ( hashTable_t ) ) ) == NULL ) {
        goto over;
    }

    if ( ( _rc->entries = malloc ( sizeof ( hashTable_entry_t * ) * size ) ) == NULL ) {
        goto over;
    }

    memset ( _rc->entries, 0, sizeof ( hashTable_entry_t * ) * size );

    _rc->size = size;

    rc = _rc;
over:
    if ( rc != _rc && _rc != NULL ) {
        hashTable_free ( _rc );
    }
    return rc;
};

unsigned long hash ( const char *str ) {
    unsigned char *s = ( unsigned char *) str;
    unsigned long rc = 5381;
    unsigned int c;

    while ( ( c = *s++ ) )
        rc = ( ( rc << 5 ) + rc ) + c;

    return rc;
};

void hashTable_entry_free ( hashTable_entry_t *entry ) {
    if ( entry ) {
        if ( entry->key )
            free ( entry->key );
        free ( entry );
    }
};

hashTable_entry_t *hashTable_entry_create ( const char *key, void *value ) {
    hashTable_entry_t *rc = NULL, *_rc = NULL;

    if ( ( _rc = calloc ( 1, sizeof ( hashTable_entry_t ) ) ) == NULL )
        goto  over;

    if ( ( _rc->key = strndup ( key, strlen ( key ) ) ) == NULL )
        goto over;

    _rc->value = value;
    rc = _rc;
over:
    if ( rc != _rc && _rc != NULL )
        hashTable_entry_free ( _rc );
    return rc;
};

hashTable_rc_t hashTable_add_entry ( hashTable_t *hashTable, const char *key, void *value ) {
    unsigned long hashedKey = 0;
    hashTable_entry_t *next = NULL, *last = NULL;

    if ( key == NULL )
        return HT_XINPUT;

    hashedKey = hash ( key ) % hashTable->size;
    next = hashTable->entries[hashedKey];

    if ( next == NULL ) {
        hashTable->entries[hashedKey] = hashTable_entry_create ( key, value );
        return HT_SUCCESS;
    }

    while ( next != NULL && strncmp ( key, next->key, strlen ( key ) ) ) {
        last = next;
        next = next->next;
    }

    if ( next == NULL ) {
        if ( last == NULL )
            return HT_FAILURE;
        last->next = hashTable_entry_create ( key, value );
        return HT_SUCCESS;
    }

    if ( !strncmp ( key, next->key, strlen ( key ) ) )
        return HT_EXISTS;

    return HT_FAILURE;
};

hashTable_entry_t *hashTable_find_entry ( hashTable_t *hashTable, const char *key ) {
    hashTable_entry_t *entry = NULL;
    unsigned long hashedKey = 0;

    hashedKey = hash ( key ) % hashTable->size;
    entry = hashTable->entries[hashedKey];

    while ( entry != NULL && strncmp ( key, entry->key, strlen ( key ) ) )
        entry = entry->next;

    return entry;
};

void *hashTable_find_entry_value ( hashTable_t *hashTable, const char *key ) {
    hashTable_entry_t *entry = hashTable_find_entry ( hashTable, key );
    return entry ? entry->value : NULL;
};

void *hashTable_update_entry ( hashTable_t *hashTable, const char *key, void *value ) {
    void *rc = NULL;
    hashTable_entry_t *entry = NULL;
    unsigned long hashedKey = 0;

    hashedKey = hash ( key ) % hashTable->size;
    entry = hashTable->entries[hashedKey];

    while ( entry != NULL && strncmp ( key, entry->key, strlen ( key ) ) )
        entry = entry->next;

    if ( !entry )
        return rc;

    rc = entry->value;
    entry->value = value;

    return rc;
};

hashTable_rc_t hashTable_remove_entry ( hashTable_t *hashTable, const char *key ) {
    hashTable_entry_t *entry = NULL, *last = NULL;
    unsigned long hashedKey = 0;

    hashedKey = hash ( key ) % hashTable->size;
    entry = hashTable->entries[hashedKey];

    while ( entry != NULL && strncmp ( key, entry->key, strlen ( key ) ) ) {
        last = entry;
        entry = entry->next;
    }

    if ( !entry )
        return HT_NOTEXISTS;

    if ( last )
        last->next = entry->next;
    else
        hashTable->entries[hashedKey] = entry->next;

    hashTable_entry_free ( entry );

    return HT_SUCCESS;
};

void hashTable_free ( hashTable_t *hashTable ) {
    if ( hashTable ) {
        if ( hashTable->entries ) {
            free ( hashTable->entries );
        }
        free ( hashTable );
    }

    return;
};
