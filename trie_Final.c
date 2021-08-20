#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// * Constants
#define ALPHABET_RANGE 26
#define WORD_SIZE 50
#define DATAFILE "dataset.txt"

// * Parameterised macros.
#define CHAR_TO_INDEX(c)((int)tolower(c) -(int)'a') // Macro to convert character to index.
#define INDEX_TO_CHAR(i)(char)((int)'a' + i)

// * Create a new datatype node.
typedef struct TrieNode {
    // Children is an array of pointers, hence we have O(1) access.
    struct TrieNode* children [ALPHABET_RANGE];
    // Mark the end of a word.
    bool isEnd;
} node;

/************************************************************
 *                      TRIE OPERATIONS                     *
 ************************************************************/

// Returns a new node structure.
node* createNode() {
    node *newNode =(node *)malloc(sizeof(node));
    int i;

    // Mark as not the end of the word.
    newNode->isEnd = false;
    // Set all children to null.
    for(i = 0; i < ALPHABET_RANGE; i++)
        newNode->children [i] = NULL;

    return newNode;
}

// Insert a node into the tree.
void insert(node *root, char *word) {
    // Return for null word.
    if(*word == '\0') return;

    do {
        // Check if alphabet was added as a child. If not, add.
        int i = CHAR_TO_INDEX(*word);
        if(root->children [i] == NULL)
            root->children [i] = createNode();
        // Go down the level of the subtrie.
        root = root->children [i];
    } while(*(++word) != '\0'); // Till the word isn't terminated.
    // Finally mark the last letter as end of word.
    root->isEnd = true;
}

// Returns true if the node is a leaf node.
bool isLeaf(node *t) {
	int i;
    // Check if all the children are null.
    for(i = 0; i < ALPHABET_RANGE; i++) {
        if(t->children [i] != NULL)
            return false;
    }
    return true;
}

bool search(node *root,char *word) {

     if(*word == '\0') return false;
    
    do{
        int i = CHAR_TO_INDEX(*word);
        if (root->children [i] == NULL) {
            return false;
            }
            // Go down the level of the subtrie.
            root = root->children [i];
            }while(*(++word) != '\0');
        
        if (root != NULL && root->isEnd == false) {
            return false;
        }
        return true;
    }

// ! NOTE: I am omitting the search function as it is not required.

/************************************************************
 *                      DEBUG FUNCTIONS                     * 
 ************************************************************/

// Helper function to print the word found
void printWord(char* str, int n)
{
	int i;
    printf("\n");
    for(i = 0; i < n; i++)
    {
        printf("%c ", str [i]);
    }
}
// Print all words in Trie
void printAllWords(node* root, char* wordArray, int pos)
{
	int i;
   if(root == NULL)
      return;
   
   if(root->isEnd)
   {
      printWord(wordArray, pos);
   }
   for(i=0; i<ALPHABET_RANGE; i++)
   {
      if(root->children [i] != NULL)
      {
         wordArray [pos] = i+'a';
         printAllWords(root->children [i], wordArray, pos+1);
      }
   }
}

/************************************************************
 *                   AUTOSUGGEST SYSTEM                     *
 ************************************************************/

// Recursive function to print auto-suggestions for given node.
void suggestionsRec(node *root, char prefix []) {
	int i;
    // Found a string in Trie with the given prefix.
    if(root->isEnd)
        printf("%s\n", prefix);

    // All children struct node pointers are NULL.
    if(isLeaf(root)) return;
 
    for(i = 0; i < ALPHABET_RANGE; i++) {
        if(root->children [i]) {
            // append current character to current prefix string.
            char newPrefix [ALPHABET_RANGE];
            strcpy(newPrefix, prefix);
            strcat(newPrefix,(char [2]){INDEX_TO_CHAR(i), '\0'});
            // recur over the rest.
            suggestionsRec(root->children [i], newPrefix);
        }
    }
}
 
// Print suggestions for given query prefix.
int getSuggestions(node* trie, char query []) {
	
	int i;
 
    // Check if query is present and find the node(of last level) with last character of given query.
    int n = strlen(query);
    for(i = 0; i < n; i++) {
        int index = CHAR_TO_INDEX(query [i]);
        // No string in the Trie has this prefix, return. Else go down a level.
        if(!trie->children [index]) return 0;
        else trie = trie->children [index];
    }

    // If query is present as a word.
    bool isWord = trie->isEnd;
    // If query is last node of tree(has no children).
    bool isLast = isLeaf(trie);
 
    // If the entire query is present but no subtree.
    if(isWord && isLast) {
        printf("%s", query);
        return -1;
    }
 
    // If there are are nodes below last matching character.
    if(!isLast) {
        char prefix [100];
        strcpy(prefix, query); // since pass by reference.
        suggestionsRec(trie, prefix);
        return 1;
    }

    return 0;
}

/************************************************************
 *                        MAIN PROGRAM                      *
 ************************************************************/

// Set up the Trie.
void setup(node **trie, char filename []) {
    // Open the file in read mode and scan individual words.
    FILE *fp = fopen(filename, "r");
    while(!feof(fp)) {
        char word [100];
        fscanf(fp, "%s", word);
        insert(*trie, word);
    }
    fclose(fp);
}

void insertintofile(char *word,char filename[])
{
	FILE *fp = fopen(filename,"a");
	fputs(word, fp);
	fclose(fp);
}



// Execute program.
int main() {
    // Create tree.
    node *trie = createNode();
    // Set it up with dataset.
    setup(&trie, DATAFILE);

    // Input.
    char input[WORD_SIZE] = "";
    char q[WORD_SIZE] = "";
    
    int flag=0;
    
    while(1) 
	{
		int ch;
		
		printf("\n\n**********************\n");
        printf("1.AUTOCOMPLETE\n2.SEARCH\n3.EXIT\n");
        printf("**********************\n\n");
        
        
		printf("\nEnter choice : ");
        scanf("%d", &ch);
        
        switch (ch)
        {
        	case 1:
        		 printf("\nENTER WORD %s", q);
				 scanf("%s", input);
				 strcat(q, input);
				 getSuggestions(trie, q);
				 break;
			
			case 2:
			scanf("%s", input);
			if(search(trie,input))
			{
				printf("\nWORD IS PRESENT\n");
				
			}
			else{
				printf("\nWORD IS NOT PRESENT\n");
				
				printf("\nIF YOU WANT TO ADD THE WORD ENTER 1 ELSE ENTER 0\n");
				scanf("%d", &flag);
				if(flag)
				{
						insertintofile(input,DATAFILE);
						insertintofile("\n",DATAFILE);
						insert(trie,input);
						printf("\nWORD INSERTED SUCESSFULLY!!\n");
				}
				else
				{
					printf("\nWORD NOT INSERTED!!\n");
					
				}
				}
			break;
			
			case 3:
				exit(0);
			
			default:
				printf("Wrong choice, Please enter correct choice  ");
				break;
		}
	} 
 
}
