#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Record {
	int id;
	char name[15];
	char surname[20];
	char city[25];
} Record;


typedef struct Sorted_Info {
    int isSorted;		/*για να γνωρίζουμε αν το αρχείο είναι ταξινομημένο  */
	int fieldNo;			/*Ανάλογα με ποιο πεδίο έχει γίνει η ταξινόμηση */
	int lastBlock;			/*Ο αριθμός του τελευταίου block που έχει εισαχθεί record */
	int recordsinLastBlock;
} Sorted_Info;


int Sorted_CreateFile( char *fileName	/* όνομα αρχείου */ );

int Sorted_OpenFile( char *fileName		/* όνομα αρχείου */ );

int Sorted_CloseFile( int fileDesc		/*ανωριστικός αριθμός ανοίγματος αρχείου */ );

int insertEntry(int fileDesc,Record Record);

int Sorted_InsertEntry( 
	int fileDesc,						/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
	Record Record						/* δομή που προσδιορίζει την εγγραφή */ );

void Sorted_SortFile( 
	char *fileName,						/* όνομα αρχείου */
	int fieldNo							/* αύξων αριθμός πεδίου προς σύγκριση */);

int Sorted_checkSortedFile( 
	char *fileName,						/* όνομα αρχείου */
	int fieldNo							/* αύξων αριθμός πεδίου προς σύγκριση */ );

void Sorted_GetAllEntries( 
	int fileDesc,						/* αναγνωριστικός αριθμός ανοίγματος αρχείου */
	int* fieldNo,						/* αύξων αριθμός πεδίου προς σύγκριση */
	void *value							/* τιμή του πεδίου προς σύγκριση */ );

void binary_search(int firstRecord, int lastRecord, int* foundRecords, int fileDesc, int fieldNo, void* value, int lastBlock, int recordsinLastBlock, int maxRecords, int blocksArray[]);
void Bubble_Sort(int fileDesc, char *fileName, int fieldNo, int initialBlocks, int recordsinLastBlock);

void add_to_array(int blocksArray[], int element);
int array_size(int blocksArray[]);
void print_record(Record rec);