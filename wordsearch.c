#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
void printPath(int** arr);
char* Upper(char* word);
bool searchArray(char** arr, int** path, char* word);
bool checkAdjacent(char** arr, int** path, char* word, int row, int col, int order);
int bSize;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
	// This function will print out the complete puzzle grid (arr). 
    // It must produce the output in the SAME format as the samples 
    // in the instructions.
    // Your implementation here...
	
	int i, j;
	for (i = 0; i < bSize; i++) {
		for (j = 0; j < bSize - 1; j++) {
			printf("%c ", *(*(arr + i) + j));
		}
		printf("%c\n", *(*(arr + i) + j));
	}
}

// Prints the path matrix
void printPath(int** arr) {
    printf("\nWord Found!\n");
    printf("Printing the search path: \n");

    // Double for loop to print out our search path matrix
    for (int i = 0; i < bSize; i++){
		for (int j = 0; j < bSize; j++){
			
			int value = *(*(arr + i) + j);
			printf("%d", value);

            // Counts digits
            int digits = 0;

            // If more than one digit
            if (value >= 10){
                while (value != 0){
                    value = value / 10;
                    digits++;
                }
				
                // Add padding depending on number of digits
                for (int r = 0; r < (8 - digits); r++){
                    printf(" ");
                }
            } else {
				// pad 7 spaces if one digit
                printf("       ");
            }
		}
		printf("\n");
	}
}

// Upper function capitalizes the word
char* Upper(char* word){
    char *letter;
	
    // The result array with terminating character
    char *result;
    result = (char*) malloc(strlen(word) + 1);

    strcpy(result, word);

    // Check until terminating character
    for (letter = result; *letter != '\0'; letter++){

        // Check if character is undercase letter
        if (*letter >= 'a' && *letter <= 'z'){
            // Convert to uppercase by subtracting the ASCII value offset from A to a which is 32
            *letter -= 32;
        }
    }

    // And return our resulting word
    return result;
}

bool searchArray(char** arr, int** path, char* word){
    bool found = false;
    
    //search arr until we find the first char of word
    for(int i = 0; i < bSize; i++) {
		for(int j = 0; j < bSize; j++) {
            if(*(*(arr + i) + j) == *(word)) {
				// Pass 2 into the order value because we use checkAdjacent to check chars in word starting from the second char
                if (checkAdjacent(arr, path, word + 1, i, j, 2)){
                    if(*(*(path + i) + j) == 0){
						*(*(path + i) + j) = 1;
                    }
                    else {
                        *(*(path + i) + j) = *(*(path + i) + j)*10 + 1;
                    }
                    found = true;
                }
            }
		}
	}
    return found;

}
   
// recursively checks adjacent values of a found letter until the full word is completed
bool checkAdjacent(char** arr, int** path, char* word, int row, int col, int order){
	// We are done if we hit the terminating character
    if ( *(word) == '\0'){
        return true;
    }

	// start at top left corner and move right
	// 1 2 3
	// 4 5 6
	// 7 8 9
    for (int i = row - 1; i <= row + 1; i++){
        for (int j = col - 1; j <= col + 1; j++){
            // continue if we pass the edge of the puzzle or if we meet the previous/center letter (5)
            if (i < 0 || i >= bSize || j < 0 || j >= bSize || (i == row && j == col)){
                continue;
            }

            if (*(*(arr + i) + j) == *(word)) {
                if (checkAdjacent(arr, path, word + 1, i, j, order + 1)) {
					// if visited for the first time, replaces value with our order value
                    if (*(*(path + i) + j) == 0) {
                        *(*(path + i) + j) = order;
                    } else {
						// if visited before, multiply by ten to add space for a new digit, then add by our order value
						// so if this place was visited before at our last letter 3, and we visit again at our second letter it will display the path as 3 to 32
						*(*(path + i) + j) = *(*(path + i) + j)*10 + order;
                    }
                    return true;
                }    
            }
        }
    }
    return false;

}

void searchPuzzle(char** arr, char* word) {
    // This function checks if arr contains the search word. If the 
    // word appears in arr, it will print out a message and the path 
    // as shown in the sample runs. If not found, it will print a 
    // different message as shown in the sample runs.
    // Your implementation here...

	 // Capitalize the word
    word = Upper(word);

    int i, j;
	bool found;
    int **pathMatrix = (int**)malloc(bSize*sizeof(int*));
    
    // Fill new matrix with 0
    for (i = 0; i < bSize; i++){
        *(pathMatrix + i) = (int*)malloc(bSize*sizeof(int));
        for (j = 0; j < bSize; j++){
            *(*(pathMatrix + i) + j) = 0;
        }
    }
	
	// Search for the word and build matrix
	found = searchArray(arr, pathMatrix, word);
	
	// Print if a complete word has been found
	if (found) {
		printPath(pathMatrix);
	} else {
		printf("\nWord not found!\n");
	}
	
	// free memory
	for (i = 0; i < bSize; i++){
		free(*(pathMatrix + i));
	}
	free(pathMatrix);
}
