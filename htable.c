#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mylib.h"
#include "htable.h"

struct htablerec {
    int capacity;
    int num_keys;
    char **keys;
    int *frequencies;
};

htable htable_new(int capacity) {
    int i;
    
    htable result = emalloc(sizeof *result);
    result->capacity = capacity;
    result->num_keys = 0;
    result->frequencies = emalloc(result->capacity * sizeof result->frequencies[0]);
    result->keys = emalloc(result->capacity * sizeof result->keys[0]);
    
    for(i=0; i<capacity; i++){
        result->frequencies[i] = 0;
        result->keys[i] = NULL;
    }
    
    return result;
}

static unsigned int htable_word_to_int(char *word) {
    unsigned int result = 0;
    
    while (*word != '\0') {
        result = (*word++ + 31 * result);
    }
    
    return result;
}

static unsigned int htable_step(htable h, unsigned int i_key) {
    return 1 + (i_key % (h->capacity -1));
}

int htable_search(htable h, char *str) {
    int collision = 0;
    unsigned long int key = htable_word_to_int(str);
    unsigned long int i = htable_step(h, key);
    
    while (h->keys[i] != NULL && strcmp(str, h->keys[i]) != 0 && collision <= h->capacity) {
        i = htable_step(h, i);
        collision++;
    }

    if (h->keys[i] == NULL || collision == h->capacity){
        return 0;
    }else {
        return h->frequencies[i];
    }
}


 /*
   @param h the hashtable to add the string to
   @param str the word to be added to the hash table
   
   val is the key value to be added
   i is the iex of the key
 */
int htable_insert(htable h, char *str) {
    unsigned long int key = htable_word_to_int(str);
    unsigned long int position = htable_step(h, key);
    int c =0;

    while(c <= h->capacity){
        c++;
        
        if(h->keys[position] == NULL){
            
            h->keys[position] = emalloc((strlen(str) + 1) * sizeof str[0]);
            strcpy(h->keys[position], str);
            h->frequencies[position] =1;
            h->num_keys++;
            return 1;
        
        }else if(strcmp(h->keys[position], str) == 0){
            
            h->frequencies[position]++;
            return h->frequencies[position];
        
        }else{
            position = htable_step(h, position);
            
        }
    }
    return 0;
}

void htable_print(htable h, FILE *stream) {
    int i;
    for(i=0; i<h->capacity; i++){
        if(h->keys[i] != NULL){
            fprintf(stream, "%d, %s\n", h->frequencies[i], h->keys[i]);
        }
    }
}

void htable_print_entire_table(htable h, FILE *stream){
    int i;
    for(i=0; i<h->capacity; i++){
        if(h->keys[i] != NULL){
            fprintf(stream, "%5d %5d %5d   %s\n", i, i, h->frequencies[i], h->keys[i]);
        }
    }
}

void htable_free(htable h) {
    free(h->keys);
    free(h->frequencies);
    free(h);
}

/**
 * Prints out a table showing what the following attributes were like
 * at regular intervals (as determined by num_stats) while the
 * hashtable was being built.
 *
 * @li Percent At Home - how many keys were placed without a collision
 * occurring.
 * @li Average Collisions - how many collisions have occurred on
 *  average while placing all of the keys so far.
 * @li Maximum Collisions - the most collisions that have occurred
 * while placing a key.
 *
 * @param h the hashtable to print statistics summary from.
 * @param stream the stream to send output to.
 * @param num_stats the maximum number of statistical snapshots to print.
 *
void htable_print_stats(htable h, FILE *stream, int num_stats) {
    int i;

    fprintf(stream, "\n%s\n\n", h->method == LINEAR_P ? "Linear Probing" : "Double Hashing"); 
    fprintf(stream, "Percent   Current   Percent    Average      Maximum\n");
    fprintf(stream, " Full     Entries   At Home   Collisions   Collisions\n");
    fprintf(stream, "-----------------------------------------------------\n");
    for (i = 1; i <= num_stats; i++) {
        print_stats_line(h, stream, 100 * i / num_stats);
    }
    fprintf(stream, "-----------------------------------------------------\n\n");
}

/**
 * Prints out a line of data from the hash table to reflect the state
 * the table was in when it was a certain percentage full.
 * Note: If the hashtable is less full than percent_full then no data
 * will be printed.
 *
 * @param h - the hash table.
 * @param stream - a stream to print the data to.
 * @param percent_full - the point at which to show the data from.
 *
static void print_stats_line(htable h, FILE *stream, int percent_full) {
    int current_entries = h->capacity * percent_full / 100;
    double average_collisions = 0.0;
    int at_home = 0;
    int max_collisions = 0;
    int i = 0;

    if (current_entries > 0 && current_entries <= h->num_keys) {
        for (i = 0; i < current_entries; i++) {
            if (h->stats[i] == 0) {
                at_home++;
            } 
            if (h->stats[i] > max_collisions) {
                max_collisions = h->stats[i];
            }
            average_collisions += h->stats[i];
        }
    
        fprintf(stream, "%4d %10d %10.1f %10.2f %11d\n", percent_full, 
                current_entries, at_home * 100.0 / current_entries,
                average_collisions / current_entries, max_collisions);
    }
}*/
