#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Structure that will hold info of books*/
struct book
{
	char *title;
	char *author;
	char *publisher;
	int *year;
	char *isbn;
};

int read_lines(char*** dest, int* num_lines)
{
  char buffer[2048];
  char** temp_arr;
  char* temp_line;
  
  *num_lines = 0;
  
  /* Try opening the file */
  FILE* fp = fopen("input.txt", "r");
  if (fp == NULL)
  {
    printf("Error opening file for reading. Exiting.\n");
    return 0;
  }
 
  /* Read the file line by line into the buffer */
  while (fgets(buffer, sizeof(buffer), fp) != NULL)
  {
    int line_length = strlen(buffer);
    char* s;

    /* Check if it's a blank line and skip if it is */
    if (strcmp(buffer, "\r\n") == 0 || strcmp(buffer, "\n") == 0 || strcmp(buffer, "\n\r") == 0)
      continue;
    
    (*num_lines)++;
    
    /* Allocate or reallocate enough space for a pointer to each line we've come across thus far. */
    if (*dest == NULL)
    {
      temp_arr = malloc(*num_lines * sizeof(char*));
    }
    else
    {
      temp_arr = realloc(*dest, *num_lines * sizeof(char*));
    }
    
    if (!temp_arr)
    {
      printf("Error (re)allocating memory. Exiting.\n");
      return 0;
    }
    *dest = temp_arr;
    
    /* Allocate enough space for the current line. */
    temp_line = malloc(line_length * sizeof(char));
    if (!temp_line)
    {
      printf("Error allocating space for the line. Skipping the line.\n");
      (*num_lines)--;
      continue;
    }
    (*dest)[*num_lines-1] = temp_line;
    
    /* Copy the current line into the array */
    strcpy((*dest)[*num_lines-1], buffer);
    
    /* Get rid of that pesky newline for both Mac and Windows */
    s = (*dest)[*num_lines-1];
    if (s[line_length - 2] == '\r' || s[line_length - 2] == '\n')
      s[line_length - 2] = '\0';
    else if (s[line_length - 1] == '\n')
      s[line_length - 1] = '\0';
  }
  
  fclose(fp);
  
  return 1;
}

/* Free up lines of space  */
void free_lines(char*** a, int num, struct book books[])
{
	int i;
	for (i = 0; i < num; i++)
	{
		free((*a)[i]);
	}
  
	free(*a);
	for(i=0; i<= num;i++)
	{
		free(books[i].title);
		free(books[i].author);
		free(books[i].publisher);
		free(books[i].year);
		free(books[i].isbn);
	}
	
}

/* Categorize each value into its corresponding array after being seperated by tab character */
void categorize(char ***final, struct book books[], int num_lines)
{
	char *buffer;
	int i = 0;
	for(i; i < num_lines; i++)
	{
		int s;
		char *line = (*final)[i];
		
		buffer = strtok(line, "\t");
		s = strlen(buffer) + 1;
		books[i].title  = malloc(s*sizeof(char));
		strcpy(books[i].title, buffer);
		
		buffer = strtok(NULL, "\t");
		s = strlen(buffer) + 1;
		books[i].author = malloc(s*sizeof(char));
		strcpy(books[i].author, buffer);
		
		buffer = strtok(NULL, "\t");
		s = strlen(buffer) + 1;
		books[i].publisher = malloc(s*sizeof(char));
		strcpy(books[i].publisher, buffer);
		
		buffer = strtok(NULL, "\t");
		s = atoi(buffer);
		books[i].year = malloc(sizeof(int));
		*books[i].year = s;
		
		buffer = strtok(NULL, "\t");
		s = strlen(buffer) + 1;
		books[i].isbn = malloc(s*sizeof(char));
		strcpy(books[i].isbn, buffer);
		
	}
}

/* Ask the user for year to search for and returns that integer value */
int ask_user(int user_year)
{
	printf("Enter a year to search for: ");
	scanf("%d", &user_year);
	printf("\n");
	return user_year;
}

/* Gets the year entered by the user and returns all information of each book up to date user entered */
void return_user_result(struct book books[], int size, int choice)
{
	int i =0;
	int counter = 0;
	for(i; i < size+1; i++)
	{
		if(*books[i].year == choice)
		{
			printf("%s\nAuthor: %s\nPublisher: %s\nYear: %d\nISBN: %s\n", books[i].title, books[i].author, books[i].publisher, *books[i].year, books[i].isbn);
			printf("\n");
			counter++;
		}
	}
	if(counter == 0)
		{
			printf("No books published in that year found.");
		}
}

/* Add book from user input */
void add_book(struct book books[], int size)
{
	char buffer[30];
	int s;
	
	printf("Enter title of book to add: ");
	fgets(buffer, 30, stdin);
	s = strlen(buffer);
	buffer[s-1] = '\0';
	books[size].title =malloc(s * sizeof(char));
	strcpy(books[size].title, buffer);
	
	printf("Enter author: ");
	fgets(buffer, 30, stdin);
	s = strlen(buffer);
	buffer[s-1] = '\0';
	books[size].author = malloc(sizeof(char));
	strcpy(books[size].author, buffer);
	
	printf("Enter publisher: ");
	fgets(buffer, 30, stdin);
	s = strlen(buffer);
	buffer[s-1] = '\0';
	books[size].publisher = malloc(s*sizeof(char));
	strcpy(books[size].publisher, buffer);
	
	printf("Enter year: ");
	scanf("%d", &s);
	books[size].year = malloc(sizeof(int));
	*books[size].year = s;
	getchar();
	
	
	printf("Enter isbn: ");
	fgets(buffer, 30, stdin);
	s = strlen(buffer);
	buffer[s-1] = '\0';
	books[size].isbn = malloc(sizeof(char));
	strcpy(books[size].isbn, buffer);
	
	printf("\n");
}

int main(void)
{
	int i = 0;
	int user_year;
	int num_lines = 0;
	char **final = NULL;
	
	read_lines(&final, &num_lines);
	struct book books[num_lines+1];
	categorize(&final, books, num_lines);
	add_book(books, num_lines);
	user_year = ask_user(user_year);
	return_user_result(books, num_lines, user_year);
	free_lines(&final, num_lines, books);
	
	return 0;
}	