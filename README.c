// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

//hash prototype
unsigned long hash(const char *word);

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table - or TABLE_SIZE
const unsigned int N = 1000;

// Hash table
node *table[N];

//Num of words
int NumberofWords = 0;

//order load hash size check unload



// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    //open a dict file
    FILE*fptrr = fopen("write", "r");
    if(fptrr == NULL)
    {
        return false;
    }

    //set all pointers to NULL
    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        table[i]->next = NULL;
    }

    //main declaration
    char *word = malloc(LENGTH + 1);
    bool leave = false;
    node*words[NumberofWords];
    do
    {
        //get num of words
        if (fscanf(fptrr, "%s", word) != EOF)
        {
            NumberofWords++; //need this for memory allocation
        }

        //move words to memory
        else
        {
            //set exit status and reset file pointer
            leave = true;
            fseek(fptrr, 0, SEEK_SET);

            //free heap and declare new array
            free(word);
            char wordarr[NumberofWords][LENGTH + 1];

            //load words onto memory
            for (int i = 0; fscanf(fptrr, "%s", wordarr[i]) != EOF; i++);

            fclose(fptrr);

            //create a new node for every word
            for (int i = 0; i < NumberofWords; i++)
            {
                int hashvalue = hash(wordarr[i]);

                //copy info from wordarr[i] into words[i]->word
                strcpy(words[i]->word, wordarr[i]);

                //table[hashvalue] functions as the head of each linked list
                words[i] -> next = NULL;
                if (table[hashvalue]->next == NULL)
                {
                    table[hashvalue]->next = words[i];
                }

                else //if not equal to NULL
                {
                    words[i]->next = table[hashvalue]->next;
                    table[hashvalue]->next = words[i];
                }
            }
        }

    }while (!leave);


return true;


}

// Hashes word to a number
unsigned long hash(const char *word)
{
    unsigned long hash = 5381;

    for (int i = 0; i < strlen(word); i++)
    {
        hash = (hash*33) + word[i];
    }

    return hash % N;

//modified version of @Daniel Bernstein's djb
}



// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{


    return NumberofWords;
}




// Returns true if word is in dictionary else false
bool check(const char *input)
{
    for (int i = 0; i < N; i++)
    {
        int hashindex = hash(input);
        for (node *temp = table[hashindex]; temp->next != NULL; temp = temp->next)
        {
            if (strcasecmp(temp->word, input) == 0)
            {
                return true;
            }

        }

        return false;
    }
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        while (table[i] != NULL)
        {
            tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }

    return true;
}


