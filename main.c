#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "BF.h"
#include "sort.h"


int main(int argc, char **argv) {
    
	if(argc>2) {
		printf("Incorrect arguments\n");
		exit(EXIT_FAILURE);
	}
	int fd;
	FILE *fp;
	char *token;
	char line[256];
	int fieldNo = 0;
	int value = 6;
	//char value[20];
	//strcpy(value, "Keratsini");

	Record record;
	BF_Init();
	Sorted_CreateFile("shitsort");
	fd = Sorted_OpenFile("shitsort");

	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp == NULL) {
			printf("There is no file with the name %s\n",argv[1]);
			return -1;
		}
		else {
			while(fgets(line,256,fp) != NULL) {	//Read every line
		        token = strtok(line,",");
		        record.id = atoi(token);

		        token = strtok(NULL, ",");
		        token++;
		        token[strlen(token) - 1] = 0;
		        strncpy(record.name, token, sizeof(record.name));

		        token = strtok(NULL, ",");
		        token++;
		        token[strlen(token) - 1] = 0;
		        strncpy(record.surname, token, sizeof(record.surname));

		        token = strtok(NULL, "\"");
		        strncpy(record.city, token, sizeof(record.city));

		        insertEntry(fd, record);
		    }
		}
	}

	Sorted_checkSortedFile("shitsort", 1);
	Sorted_GetAllEntries(fd,&fieldNo,&value);
    
    return 1;

	/*
	int fd;
    int fieldNo;
   
    BF_Init();
    
    //create heap file
    if(Sorted_CreateFile(fileName) == -1  )
        printf("Error creating file!\n");
    
    fd = Sorted_OpenFile(fileName);
    if( fd == -1  )
        printf("Error opening file!\n");
    insert_entries(fd);
    
    
    //sort heap file using 2-way merge-sort
    if(Sorted_SortFile(fileName,0) == -1  )
        printf("Error sorting file!\n");
    
    if(Sorted_checkSortedFile("heapFileSorted0",0) == -1  )
        printf("Error sorting file!\n");
    
    //get all entries with value
    //char value[20];
    //strcpy(value, "Keratsini");
    fieldNo = 0;
    int value = 11903588;
    
    fd = Sorted_OpenFile("heapFileSorted0");
    if( fd == -1  )
        printf("Error opening file!\n");
    
    Sorted_GetAllEntries(fd,&fieldNo,&value);

	*/
}
