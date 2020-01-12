//for some reason the original speller file from cs50 failed to compile with make, probably because I have accidently, somehow, deleted one of the
//lines. So, instead of having to redownload the file from CS50, I have decided to just create a smaller version of my own, also using my
//custom-made dictionary. The code works with all characters but the hash function only hashes a - d for a table of size 4 so I can test my code easier.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>

#ifndef LENGTH
#define LENGTH 45
#endif

#define N 4

unsigned long hash(const char *word);

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}node;

node *table[N];


int main (void)
{
    //open file
    FILE *fptr = fopen("wordforhashtest", "r");
    if (fptr == NULL)
    {
        perror("Error ");
        return -1;
    }

    //get number of words from number of lines + temp declaration
    char *wordtmp = malloc(LENGTH+1);
    int wordsamount = 0;
    while (fscanf(fptr, "%s", wordtmp) != EOF)
    {
        wordsamount++;
    }
    rewind(fptr);
    printf("Number of words = %d\n", wordsamount);

    //free tmp variable
    free(wordtmp);

    //load words onto memory
    node *words[wordsamount];
    char wordnum[wordsamount][LENGTH+1];
    for (int i = 0; fscanf(fptr, "%s", wordnum[i]) != EOF; i++)
    {
        words[i] = malloc(sizeof(node));
        words[i]->next = NULL;

        //copy word into word node
        strcpy(words[i]->word, wordnum[i]);
    }
    fclose(fptr);

    for (int i = 0; i < N; i++)
    {
        table[i] = malloc(sizeof(node));
        table[i]->next = NULL;
    }

    //hash and then load words into nodes
    for (int i = 0; i < wordsamount; i++)
    {

        //hash word
        int hashindex = hash(words[i]->word);
        if(table[hashindex]->next == NULL)
        {
            //table points to first node
            table[hashindex]->next = words[i];
            //printf("\n%s\n", table[hashindex]->next->word);
        }
        else
        {
            //put words at the beginning
            words[i]->next = table[hashindex]->next;
            //printf("\n%s\n", words[i]->next->word);
            table[hashindex]->next = words[i];
            //printf("\n%s\n", table[hashindex]->next->word);

        }
    }

//-----------------------------------------------------------------------------------------------------
    //for text file, above for dictionary
    FILE * fptrtext = fopen("textforhashtest", "r");

    //gets number of lines
    char *tmptext = malloc(sizeof(node));
    if (tmptext == NULL)
    {
        perror("Error: ");
        return -1;
    }

    //get amount of words
    int textwordamount = 0;
    for (; fscanf(fptrtext, "%s", tmptext)!=EOF; textwordamount++);
    free(tmptext);

    //reset pointer
    rewind(fptrtext);

    //put words onto memory and declare NULL pointer
    char textarr[textwordamount][LENGTH+1];
    node *textwords[textwordamount];
    for (int i = 0; fscanf(fptr, "%s", textarr[i])!=EOF; i++)
    {
        textwords[i] = malloc(LENGTH+1);
        textwords[i]->next = NULL;
        strcpy(textwords[i]->word, textarr[i]);
    }

    //misspelled words linked list
    node *misspelled = malloc(sizeof(node));
    if (misspelled == NULL)
    {
        perror("Error ");
        return -1;
    }

    //correctly spelled words linked list
    node *correctspelled = malloc(sizeof(node));
    if (correctspelled == NULL)
    {
        perror("Error ");
        return -1;
    }

    correctspelled->next = NULL;
    misspelled->next = NULL;

    for (int i = 0; i<textwordamount; i++)
    {
        bool stat = false;
        //hash words
        int hashindex = hash(textwords[i]->word);

        //if match found immediately
        if (strcasecmp (table[hashindex]->next->word, textwords[i]->word) == 0)
        {
            //put into correct list
            if (correctspelled->next == NULL)
            {
                //put first in list
                correctspelled->next = textwords[i];
            }
            else
            {
                //move others and put first in list
                textwords[i]->next = correctspelled->next;
                correctspelled->next = textwords[i];
            }
        }

        //else if not found immediately
        else
        {
            //declare a cursor
            node *tmp = table[hashindex]->next->next;

            //if NULL
            if (tmp==NULL)
            {
                //no match
                if (misspelled->next == NULL)
                {
                    misspelled->next = textwords[i];
                }
                else
                {
                    textwords[i]->next = misspelled->next;
                    misspelled->next = textwords[i];
                }

            }
            else
            {
                //loop until word is found or NULL is found
                while(tmp != NULL && stat == false)
                {
                    //if match found
                    if (strcasecmp(tmp->word, textwords[i]->word) == 0)
                    {
                        //put into correct list
                        if (correctspelled->next == NULL)
                        {
                            //put first in list
                            correctspelled->next = textwords[i];
                            stat = true;
                        }
                        else
                        {
                            //move others and put first in list
                            textwords[i]->next = correctspelled->next;
                            correctspelled->next = textwords[i];
                            stat = true;
                        }
                    }

                    //if at the end
                    else if (tmp == NULL || tmp->next == NULL)
                    {
                        //no match
                        if (misspelled->next == NULL)
                        {
                            misspelled->next = textwords[i];
                            stat = true;
                        }
                        else
                        {
                            textwords[i]->next = misspelled->next;
                            misspelled->next = textwords[i];
                            stat = true;
                        }
                    }
                     tmp = tmp->next;

                }

            }

            free(tmp);
        }

    }


//-----------------------------------------------------------------------------------------------------------------------------------
    for (int i = 0; i < N; i++)
    {
        printf("In dictionary table no.%d, we have ", i);
        while (true)
        {
            //clear and print
            if (table[i]->next != NULL)
            {
                node*tmp = table[i]->next;
                printf("%s ", tmp->word);
                free(table[i]);
                table[i] = tmp;
            }

            //clear only, cus otherwise prints (null)
            else
            {
                node*tmp = table[i]->next; //set tmp to null
                free(table[i]);
                free(tmp);
                break;
            }

        }
        printf("\n");
    }

    printf("\nCorrectly spelled words: ");
    while (correctspelled != NULL)
    {
        node *tmp = correctspelled->next;
        if (tmp == NULL)
        {
            free(tmp);
            break;
        }
        else
        {
            printf("%s ", tmp->word);
            free(correctspelled);
            correctspelled = tmp;
        }

    }
    printf("\n\n Incorrectly spelled words: ");

    while (misspelled != NULL)
    {
        node *tmp = misspelled->next;
        if (tmp == NULL)
        {
            free(tmp);
            break;
        }
        else
        {
            printf("%s ", tmp->word);
            free(misspelled);
            misspelled = tmp;
        }

    }
    printf("\n");
return 0;
}



// Hashes word to a number 
unsigned long hash(const char *word)
{
    int hash = 0;
    if (word[0] == 'A')
        hash = 0;

    else if (word[0] == 'B')
        hash = 1;

    else if (word[0] == 'C')
        hash = 2;

    else if (word[0] == 'D')
        hash = 3;

    return hash;

}
