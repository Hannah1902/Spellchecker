#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "hashtable.h"


/*** Read the word list and create a dictionary ***/
// Input: the name of the dictionary file
// Output: a HashTable structure
HashTable *readWordList(HashTable *dictionary, char *dictFile) {
    
    // Open the word file
    FILE *fp = fopen(dictFile, "r");
    if (fp <= 0) {
        perror("fopen");
        exit(1);
    }
    
    char temp[64];

    // Loop until fgets returns NULL, indicating end of file
    while (fgets(temp, sizeof(temp), fp)) {
        temp[strlen(temp) - 1] = '\0';  // strip terminating newline
        
        // Copy word to a new string before storing in table
        char *newWord = malloc(strlen(temp) + 1);
        strcpy(newWord, temp);
        
        // Insert into the dictionary
        hashtableInsert(dictionary, newWord);
    }
    
    fclose(fp);
    
    return dictionary;
}


/*** Test if a word is in the dictionary ***/
int checkWord(char *word, HashTable *dict) {

    HashNode *node = hashLookup(dict, word, 0);
    return node != NULL;

}

char *insertChar(char *word, const char *c, int position) {
    char *copy = malloc(strlen(word) + 2);
    memset(copy, 0, strlen(word) + 10);
    
    strncpy(copy, word, position);
    int len = strlen(copy);
    
    strcpy(copy + len, c);
    len = strlen(copy);
    
    strcpy(copy + len, word + position);

    return copy;
}

char *removeChar(char *word, int position) {
    char *copy = malloc(strlen(word) + 2);
    memset(copy, 0, strlen(word) + 10);
    
    strncpy(copy, word, position);
    strncpy(copy + position, word + position + 1, strlen(word) - 1 - position);
    
    return copy;
}
 
void suggestions(HashTable *dict, char *word) {
    char *letters = "abcdefghijklmnopqrstuvwxyz";
    
    for (int i = 0; i < strlen(word); i++) {
        char saved = word[i];
        
        //alternate letter
        for (int j = 0; j < strlen(letters); j++) {
            word[i] = letters[j];
            
            if (checkWord(word, dict) != 0) {
                printf("Suggestion for you: %s\n", word);
            }
        }
        word[i] = saved;
        
        //forgotten letter
        for (int k = 0; k < strlen(letters); k++) {
            char *c = malloc(sizeof(char));
            *c = letters[k];
            
            char *copy = malloc(strlen(word) + 2);
            copy = insertChar(word, c, i);
            
            if (checkWord(copy, dict) != 0) {
                printf("Suggestion for you: %s\n", copy);
            }
        }
        
    }
    
    //extra letter
    for (int i = 0; i < strlen(word); i++) {
        char *copy = malloc(strlen(word) + 2);
        copy = removeChar(word, i);
        
        if (checkWord(copy, dict) != 0) {
            printf("Suggestion for you: %s\n", copy);
        }
    }
}

/*** Spellcheck a file ***/
void spellcheck(HashTable *dictionary, char *file) {
    
    // Read word list and create the dictionary
    readWordList(dictionary, "words.txt");

    FILE *fp = fopen(file, "r");
    if (fp <= 0) {
        perror("fopen");
        exit(1);
    }
    
    char line[1000];
    char *token;
    char *delim = " \n";

    // Loop until fgets returns NULL, indicating end of file
    while (fgets(line, sizeof(line), fp) != NULL) {
        token = strtok(line, delim);

        while (token != NULL) {
            for(int i = 0; i < strlen(token); i++) {
                token[i] = tolower(token[i]);
            }
            
            if (checkWord(token, dictionary) == 0) {
                printf("misspelled word: %s\n", token);
                suggestions(dictionary, token);
            }
            token = strtok(NULL, delim);
        }
    }
    
    fclose(fp);
    return;
}


int main(int argc, char *argv[]) {

    HashTable *h = hashtableCreate();
    
    spellcheck(h, argv[1]);
    
    return 0;
}
