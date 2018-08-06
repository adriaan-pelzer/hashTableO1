#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <hashTableO1.h>

#define BUF_SIZE 1024

int main ( int argc, char **argv ) {
    int rc = EXIT_FAILURE;
    char *filename = NULL;
    FILE *infile = NULL;
    char buffer[BUF_SIZE];
    char *word;
    uint16_t chunkStart = 0;
    uint16_t readBytes = 0;
    hashTable_t *hashTable = hashTable_create ( 65535 );
    hashTable_rc_t hashTable_rc = HT_FAILURE;

    if ( argc < 2 ) {
        fprintf ( stderr, "Usage: %s file\n", argv[0] );
        goto over;
    }

    filename = argv[1];

    if ( ( infile = fopen ( filename, "r" ) ) == NULL ) {
        fprintf ( stderr, "Cannot open file %s for reading: %s", filename, strerror ( errno ) );
        goto over;
    }

    while ( ( readBytes = fread ( (void *)((uint64_t) &buffer + chunkStart), 1, BUF_SIZE - chunkStart, infile ) ) > 0 ) {
        uint16_t charIndex = chunkStart, i, wordStart = 0;

        while ( charIndex < chunkStart + readBytes ) {
            if ( buffer[charIndex] == '\n' ) {
                buffer[charIndex] = '\0';
                word = buffer + wordStart;
                if ( ( hashTable_rc = hashTable_add_entry ( hashTable, word, (void *) word ) ) != HT_SUCCESS )
                    fprintf ( stderr, "Cannot add entry %s to hashTable: (%d)\n", word, hashTable_rc );
                else
                    printf ( "%s added\n", word );
                wordStart = charIndex + 1;
            }

            charIndex++;
        }

        if ( readBytes == BUF_SIZE - chunkStart ) {
            for ( i = wordStart; i < BUF_SIZE; i++ )
                buffer[i - wordStart] = buffer[i];

            chunkStart = i - wordStart;
        } else {
            break;
        }
    }

    rewind ( infile );
    chunkStart = 0;
    
    while ( ( readBytes = fread ( (void *)((uint64_t) &buffer + chunkStart), 1, BUF_SIZE - chunkStart, infile ) ) > 0 ) {
        uint16_t charIndex = chunkStart, i, wordStart = 0;

        while ( charIndex < chunkStart + readBytes ) {
            if ( buffer[charIndex] == '\n' ) {
                char *value = NULL;
                buffer[charIndex] = '\0';
                word = buffer + wordStart;
                if ( ( value = (char *) hashTable_find_entry_value ( hashTable, word ) ) == NULL )
                    fprintf ( stderr, "Cannot find entry %s in hashTable\n", word );
                else 
                    printf ( "found entry %s: %s\n", word, value );
                wordStart = charIndex + 1;
            }

            charIndex++;
        }

        if ( readBytes == BUF_SIZE - chunkStart ) {
            for ( i = wordStart; i < BUF_SIZE; i++ )
                buffer[i - wordStart] = buffer[i];

            chunkStart = i - wordStart;
        } else {
            break;
        }
    }

    rewind ( infile );
    chunkStart = 0;
    
    while ( ( readBytes = fread ( (void *)((uint64_t) &buffer + chunkStart), 1, BUF_SIZE - chunkStart, infile ) ) > 0 ) {
        uint16_t charIndex = chunkStart, i, wordStart = 0;

        while ( charIndex < chunkStart + readBytes ) {
            if ( buffer[charIndex] == '\n' ) {
                buffer[charIndex] = '\0';
                word = buffer + wordStart;
                if ( ( hashTable_rc = hashTable_remove_entry ( hashTable, word ) ) != HT_SUCCESS )
                    fprintf ( stderr, "Cannot remove entry %s from hashTable: (%d)\n", word, hashTable_rc );
                else 
                    printf ( "%s removed\n", word );
                wordStart = charIndex + 1;
            }

            charIndex++;
        }

        if ( readBytes == BUF_SIZE - chunkStart ) {
            for ( i = wordStart; i < BUF_SIZE; i++ )
                buffer[i - wordStart] = buffer[i];

            chunkStart = i - wordStart;
        } else {
            break;
        }
    }

    rc = EXIT_SUCCESS;
over:
    if ( hashTable )
        hashTable_free ( hashTable );

    if ( infile )
        fclose ( infile );

    return rc;
}
