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

void htable_free(htable h) {
    free(h->keys);
    free(h->frequencies);
    free(h);
}
