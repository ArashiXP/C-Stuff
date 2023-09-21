/*
Michael Nguyen
11/12/21
Program 5: WordSort
*/

/*
Issues:
    -> I couldn't get the array full of pointers to the tree
    -> Without the array I could not sort the output from descending order 
    -> Copying and pasting on eustis came out with a strange output, but when using an in.txt file there
       were zero issues.
*/

/*
    Other than that, the program runs as intended
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Structs

/*
This stores the word, its frequency, and the depth of the word in the tree
*/
typedef struct words
{
    char storedWord[21];
    int freq;
    int depth;
} words;

/*
This store the nodes that store left and right, as well as a pointer to the word struct
*/
typedef struct treeNode
{
    words *word;            // data
    struct treeNode *left;  // left
    struct treeNode *right; // right
} treeNode;

// prototypes
treeNode *init(char *word);
treeNode *insert(treeNode *root, char *word, int depth);
int search(treeNode *root, char *word, int depth);
void freeStuff(treeNode *root);
void inOrder(treeNode *root);
void preOrder(treeNode *root, long long cases);
void copyToArray(treeNode *root, words *array);
void mergeSort(long long array[], long long startIndex, long long endIndex);
void merge(long long array[], long long start, long long mid, long long end);

// Global Variables
int arrIdx = 0;

/*
Makes one node
*/
treeNode *init(char *word)
{
    treeNode *temp = malloc(sizeof(treeNode)); // Makes the inital node struct
    temp->word = malloc(sizeof(words));        // Allocate memory for the word struct in the node
    strcpy(temp->word->storedWord, word);      // copy the word into the struct
    temp->word->freq = 1;                      // set the frequency of the word to 1, since 0 would not make sense
    temp->word->depth = 0;                     // tree starts at 0, and goes up from there, and if it didn't exist it would be made
    // Set left and right to NULL
    temp->left = NULL;
    temp->right = NULL;

    return temp;
} // end of init

/*
This function inserts a node to the tree, and if it is empty then we will fill from the start
This also increases the frequency of the word if it matches
Also calculates the depth for each word
*/
treeNode *insert(treeNode *root, char *word, int depth)
{
    // Base case, we got to a leaf node, so we make a node by calling init
    if (root == NULL)
    {
        treeNode *temp = init(word);
        return temp;
    }

    // If the word is alphabetically less than the current stored word then we go left
    if (strcmp(word, root->word->storedWord) < 0)
    {
        root->left = insert(root->left, word, depth + 1);
        root->left->word->depth = depth + 1; // Sets the depth
    }
    // Else we go right
    else if (strcmp(word, root->word->storedWord) > 0)
    {
        root->right = insert(root->right, word, depth + 1);
        root->right->word->depth = depth + 1; // sets the depth
    }

    // If the string we are on in the BST is equal to the word we are inserting then
    // we increase the frequency by one
    if (strcmp(word, root->word->storedWord) == 0)
    {
        root->word->freq++;
    }

    return root;
} // end of insert

/*
This function is for the query option(2) If searches through the tree and returns the frequency and the depth
seperated by a space
*/
int search(treeNode *root, char *word, int depth)
{
    // Base case, if we go through the tree and don't find the word, then we return -1
    if (root == NULL)
    {
        return -1;
    }

    // If we find a match then we print out the frequency and depth
    if (strcmp(word, root->word->storedWord) == 0)
    {
        printf("%d %d\n", root->word->freq, depth);
        return 1;
    }
    // Left if alphabetically less than the node
    else if (strcmp(word, root->word->storedWord) < 0)
    {
        return search(root->left, word, depth + 1);
    }
    // Right if alphabetically greater than the node
    else if (strcmp(word, root->word->storedWord) > 0)
    {
        return search(root->right, word, depth + 1);
    }
} // end of search

/*
This just frees the tree recursively
*/
void freeStuff(treeNode *root)
{
    if (root != NULL)
    {
        freeStuff(root->right);
        freeStuff(root->left);
        free(root);
    }
} // End of freeStuff

void preOrder(treeNode *root, long long cases)
{
    if (root == NULL)
    {
        return;
    }

    words *array = calloc(cases, sizeof(words));
    copyToArray(root, array);
    printf("%s %d\n", root->word->storedWord, root->word->freq);
    preOrder(root->left, cases);
    preOrder(root->right, cases);
    free(array);
}


void copyToArray(treeNode *root, words *array)
{
    if (root == NULL)
    {
        return;
    }

    //This bit of code kept on giving errors so I commented it out
    //array[arrIdx++] = root->word;
    copyToArray(root->left, array);
    copyToArray(root->right, array);
}

/*
This function is part 1 of our merge sort, this splits the array into 2 recursively
then calls merge to combine the 2 sorted arrays into 1
*/
void mergeSort(long long array[], long long startIndex, long long endIndex)
{
    // middle of the array, it is a long long because the numbers of array could get big
    long long mid;

    // Make sure we have more than one element for in the sort range
    if (startIndex < endIndex)
    {
        // We find the middle of the array for the merged sort
        mid = (startIndex + endIndex) / 2;

        // Sort the left side
        mergeSort(array, startIndex, mid);

        // Sort the right side
        mergeSort(array, mid + 1, endIndex);

        // Combine the 2 for one array
        merge(array, startIndex, mid + 1, endIndex);
    }
} // end of mergeSort

/*
This function is part 2 of our merged sort, this is where we combine the 2 sorted arrays
*/
void merge(long long array[], long long start, long long mid, long long end)
{
    // Variables
    long long *temp, leftCount, rightCount, length;
    // this is the index for the temp array
    int tempCount = 0;

    // Find the length of the full array
    length = end - start + 1;

    // Dynamically allocate memory for the temp array for the sort
    temp = calloc(length, sizeof(long long));

    // indexes for the 2 arrays
    leftCount = start; // this is the left side, which would start at the begining
    rightCount = mid;  // this is the right side, which would start at the middle

    // This copies the numbers from the main array to our temp one, while numbers exist in both arrays
    while ((leftCount < mid) || (rightCount <= end))
    {
        // We begin bt copying array from the left array, and we check if we don't touch the right side's index
        if (rightCount > end || (leftCount < mid && array[leftCount] < array[rightCount]))
        {
            // The copying into the temp array
            temp[tempCount] = array[leftCount];
            // increasing the index of both the temp array and the left array, since we are in a while loop and not a for
            tempCount++;
            leftCount++;
        }

        // Now we copy the right array into the temp with where we left off
        else
        {
            // copying into the temp array
            temp[tempCount] = array[rightCount];
            // increasing the index of both the temp array and the right array
            tempCount++;
            rightCount++;
        }
    }

    // Copy the temp array into the main array
    for (int i = start; i <= end; i++)
        array[i] = temp[i - start];

    // free the temp array
    free(temp);

} // end of merge

int main()
{
    // Variables
    long long cases, i;
    char *tempWord; // I store the inputted word into this temp
    // numcases
    scanf("%lld", &cases);
    // Make the inital tree and point it to NULL
    treeNode *tree = NULL;

    // Main case loop
    for (i = 1; i <= cases; i++)
    {

        // Variables
        int action, value;                    // value is what is returned by the search/query function
        tempWord = malloc(21 * sizeof(char)); // Allocate memory for the tempWord
        scanf("%d %s", &action, tempWord);
        // 1 = insert
        // 2 = query

        // I used a switch function for my actions
        // I could have used a if else, but this was much more clean
        switch (action)
        {
        case 1:                               // Option 1, you insert the word into the tree
            tree = insert(tree, tempWord, 0); // the 0 is the starting depth
            break;
        case 2:                                // Option 2, you search for the word and return the frequency and depth
            value = search(tree, tempWord, 0); // 0 = starting depth
            // Now if we return -1 from the search function then we will print out -1 -1, since if it didn't
            // exist then it would not have a depth or frequency
            if (value == -1)
            {
                printf("-1 -1\n");
            }
            break;
        default:
            break;
        }
        free(tempWord); // free the tempWord we used
    }
    preOrder(tree, cases);

    // free everything else
    freeStuff(tree);

    return 0; // end credits
} // End of main