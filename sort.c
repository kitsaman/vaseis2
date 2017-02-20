#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BF.h"
#include "sort.h"


int Sorted_CreateFile(char *fileName) {
	int fileDesc;
	void *block;
	Sorted_Info* info = malloc(sizeof(Sorted_Info)); 
	if(BF_CreateFile(fileName) < 0) {
  		BF_PrintError("Could not create file\n");
  		return -1;
	}
	fileDesc = BF_OpenFile(fileName);
	if(fileDesc < 0) {
		BF_PrintError("Could not open file\n");
  		return -1;
	}
	if(BF_AllocateBlock(fileDesc) < 0) {
  		BF_PrintError("Could not allocate block\n");
  		return -1;
  	}
  	info->isSorted = 1;
  	info->fieldNo = -1;
  	info->lastBlock = 1;
  	info->recordsinLastBlock = 0;
  	if(BF_ReadBlock(fileDesc,0,&block) < 0) {
 	 	BF_PrintError("Could not read block\n");
  		return -1;
  	}
	memcpy(block,info,sizeof(Sorted_Info));
	if(BF_WriteBlock(fileDesc,0)) {
		BF_PrintError("Could not write to block\n");
  		return -1;
	}
	if(BF_AllocateBlock(fileDesc) < 0) {
  		BF_PrintError("Could not allocate block\n");
  		return -1;
  	}
	free(info);
	printf("Created Sorted File with filedesc %d\n",fileDesc);
	return 0;
}

int Sorted_OpenFile(char *fileName) {
	int fileDesc;
	void* block;
	Sorted_Info info; 
	fileDesc = BF_OpenFile(fileName);
	if(fileDesc < 0) {
		BF_PrintError("Could not open file\n");
  		return -1;
	}
	if(BF_ReadBlock(fileDesc,0,&block) < 0) {
 	 	BF_PrintError("Could not read block\n");
  		return -1;
  	}
  	memcpy(&info,block,sizeof(Sorted_Info));
  	if(info.isSorted != 1)
  		return -1;
	return fileDesc;
}

int Sorted_CloseFile(int fileDesc) {
	if(BF_CloseFile(fileDesc) < 0) {
  		BF_PrintError("Could not close file\n");
  		return -1;
  	}
  	printf("Closed the Sorted File\n");
	return 0;
}

int insertEntry(int fileDesc,Record Record) {
	int maxRecords = BLOCK_SIZE/sizeof(Record);
	void* block;
	Sorted_Info info; 
	if(BF_ReadBlock(fileDesc,0,&block)<0) {
 	 	BF_PrintError("Could not read block\n");
  		return -1;
  	}
  	memcpy(&info,block,sizeof(Sorted_Info));
  	info.isSorted = 0;
  	info.fieldNo = -1;
	if(info.recordsinLastBlock < maxRecords) {
		int tempBlock = info.lastBlock;
		int tempRecord = info.recordsinLastBlock;
		info.recordsinLastBlock++;
		memcpy(block,&info,sizeof(Sorted_Info));
		if(BF_WriteBlock(fileDesc,0)) {
			BF_PrintError("Could not write to block\n");
  			return -1;
		}
		if(BF_ReadBlock(fileDesc,tempBlock,&block)<0) {
 	 		BF_PrintError("Could not read block\n");
  			return -1;
  		}
  		memcpy(block+tempRecord*sizeof(Record),&Record,sizeof(Record));
  		if(BF_WriteBlock(fileDesc,tempBlock)) {
			BF_PrintError("Could not write to block\n");
  			return -1;
		}
		printf("Inserted record in block %d that now has %d records\n",tempBlock,++tempRecord);
	}
	else {
		info.lastBlock++;
		info.recordsinLastBlock = 1;
		int tempBlock = info.lastBlock;
		memcpy(block,&info,sizeof(Sorted_Info));
		if(BF_WriteBlock(fileDesc,0)) {
			BF_PrintError("Could not write to block\n");
  			return -1;
		}
		if(BF_AllocateBlock(fileDesc) < 0) {
  			BF_PrintError("Could not allocate block\n");
  			return -1;
  		}
  		if(BF_ReadBlock(fileDesc,tempBlock,&block)<0) {
 	 		BF_PrintError("Could not read block\n");
  			return -1;
  		}
  		memcpy(block,&Record,sizeof(Record));
  		if(BF_WriteBlock(fileDesc,tempBlock)) {
			BF_PrintError("Could not write to block\n");
  			return -1;
		}
		printf("Inserted record in new block %d\n",tempBlock);
	}
	return 0;
}

int Sorted_InsertEntry(int fileDesc,Record Record) {

	return 0;
}

void Sorted_SortFile(char *fileName,int fieldNo) {

}

int Sorted_checkSortedFile(char *fileName,	int fieldNo) {
	int fileDesc;
	void *block;
	Sorted_Info info; 
	fileDesc = BF_OpenFile(fileName);
	if(fileDesc < 0) {
		BF_PrintError("Could not open file\n");
  		return -1;
	}
	if(BF_ReadBlock(fileDesc,0,&block) < 0) {
 	 	BF_PrintError("Could not read block\n");
  		return -1;
  	}
  	memcpy(&info,block,sizeof(Sorted_Info));
  	if(info.isSorted == 1 && info.fieldNo == fieldNo) {
  		printf("File with name %s is Sorted\n", fileName);
		return 0;
	}
	else {
		printf("File with name %s is NOT Sorted\n", fileName);
		return -1;
	}
}

void Sorted_GetAllEntries(int fileDesc,int* fieldNo,void *value) {
	int i;
	int current_block;
	int lastBlock;
	int recordsinLastBlock;
	int foundRecords = 0;
	int maxRecords = BLOCK_SIZE/sizeof(Record);
	Record tempRecord;
	void* block;
	if(BF_ReadBlock(fileDesc,0,&block) < 0) {
 	 	BF_PrintError("Could not read block\n");
  		return;
  	}
  	memcpy(&lastBlock,block+2*sizeof(int),sizeof(int));
  	memcpy(&recordsinLastBlock,block+3*sizeof(int),sizeof(int));
  	// Print all records
	if(value == NULL) {
		current_block = 1;
		while(current_block <= lastBlock) {
			if(BF_ReadBlock(fileDesc,current_block,&block) < 0) {
 	 			BF_PrintError("Could not read block\n");
  				return;
  			}
  			if(current_block != lastBlock) {
  				for(i=0 ; i < maxRecords ; i++) {
  					memcpy(&tempRecord,block+i*sizeof(Record),sizeof(Record));
  					print_record(tempRecord);
  					foundRecords++;
  				}
  			}
  			else {
  				for(i=0 ; i < recordsinLastBlock ; i++) {
  					memcpy(&tempRecord,block+i*sizeof(Record),sizeof(Record));
  					print_record(tempRecord);
  					foundRecords++;
  				}
  			}
  			current_block++;
  		}
  		printf("\nFound %d records in all %d blocks\n\n",foundRecords,lastBlock - 1);
	}
	else {
		int totalRecords, l;
		int blocksArray[(lastBlock/2)+1];
		for(l = 0; l <= (lastBlock/2)+1; l++)
			blocksArray[l] = 0;
		totalRecords = ((lastBlock - 1) * maxRecords) + recordsinLastBlock;
		binary_search(0,totalRecords,&foundRecords,fileDesc,*fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
		for(l=0;l<=(lastBlock/2)+1;l++)
			printf("Element in array position %d : %d\n",l,blocksArray[l]);
		printf("\nChecked %d blocks and found %d records\n\n",array_size(blocksArray),foundRecords);
	}
}

void binary_search(int firstRecord, int lastRecord, int* foundRecords, int fileDesc, int fieldNo, void* value, int lastBlock, int recordsinLastBlock, int maxRecords, int blocksArray[]) {
	if(firstRecord > lastRecord)
		return;
	int midRecord;
	void* block;
	int searchBlock = 0;
	int recordCounter = 0;
	int recordInBlock;
	Record tempRecord;
	// Find block that has the midRecord and its place in the block
	midRecord = (lastRecord + firstRecord) / 2;
	while(recordCounter < midRecord) {
		recordCounter = recordCounter + maxRecords;
		searchBlock++;
	}
	recordInBlock = midRecord - ((searchBlock - 1) * maxRecords ) - 1;
	if(BF_ReadBlock(fileDesc,searchBlock,&block) < 0) {
 	 	BF_PrintError("Could not read block\n");
  		return;
  	}
  	add_to_array(blocksArray,searchBlock);
  	printf("Checking block %d\n", searchBlock);
  	if(fieldNo == 0) {
  		int idValue = *((int *) value);
  		int leftShift = 0;
  		memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  		if(idValue < tempRecord.id)
  			binary_search(firstRecord,midRecord - 1,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else if (idValue > tempRecord.id)
  			binary_search(midRecord + 1,lastRecord,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else {
  			print_record(tempRecord);
  			(*foundRecords)++;
  			int tempRecordInBlock = recordInBlock;
  			int k = 1;
  			int leftEnd = 0;
  			int rightEnd = 0;
  			while(leftEnd == 0) {
  				if(recordInBlock > 0) {
  					recordInBlock--;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(tempRecord.id == idValue) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						leftEnd = 1;
  				}
  				else {
  					if((searchBlock - k) != 0) {
  						add_to_array(blocksArray,searchBlock - k);
  						recordInBlock = 7;
  						if(BF_ReadBlock(fileDesc,searchBlock - k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional left block %d\n", searchBlock - k);
  						leftShift = 1;
  						k++;
  						memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  						if(tempRecord.id == idValue) {
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							leftEnd = 1;
  					}
  					else
  						leftEnd = 1;
  				}
  			}
  			if(leftShift == 1) {
  				if(BF_ReadBlock(fileDesc,searchBlock,&block) < 0) {
 	 				BF_PrintError("Could not read block\n");
  					return;
  				}
  			}
  			k = 1;
  			while(rightEnd == 0) {
  				if(tempRecordInBlock < 7) {
  					tempRecordInBlock++;
  					memcpy(&tempRecord,block + (tempRecordInBlock * sizeof(Record)),sizeof(Record));
  					if(tempRecord.id == idValue) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						k++;
  						memcpy(&tempRecord,block + (tempRecordInBlock * sizeof(Record)),sizeof(Record));
  						if(tempRecord.id == idValue) {
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							rightEnd = 1;
  					}
  					else
  						rightEnd = 1;
  				}
  			}
  		}
  	}
  	else if(fieldNo == 1) {
  		int leftShift = 0;
  		char strValue[20];
  		strcpy(strValue,value);
  		memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  		if(strcmp(tempRecord.name,strValue) < 0)
  			binary_search(firstRecord,midRecord - 1,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else if(strcmp(tempRecord.name,strValue) > 0)
  			binary_search(midRecord + 1,lastRecord,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else {
  			print_record(tempRecord);
  			(*foundRecords)++;
  			int tempRecordInBlock = recordInBlock;
  			int k = 1;
  			int leftEnd = 0;
  			int rightEnd = 0;
  			while(leftEnd == 0) {
  				if(recordInBlock > 0) {
  					recordInBlock--;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.name,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						leftEnd = 1;
  				}
  				else {
  					if((searchBlock - k) != 0) {
  						add_to_array(blocksArray,searchBlock - k);
  						recordInBlock = 7;
  						if(BF_ReadBlock(fileDesc,searchBlock - k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional left block %d\n", searchBlock - k);
  						leftShift = 1;
  						k++;
  						memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  						if(strcmp(tempRecord.name,strValue) == 0){
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							leftEnd = 1;
  					}
  					else
  						leftEnd = 1;
  				}
  			}
  			if(leftShift == 1) {
  				if(BF_ReadBlock(fileDesc,searchBlock,&block) < 0) {
 	 				BF_PrintError("Could not read block\n");
  					return;
  				}
  			}
  			k = 1;
  			while(rightEnd == 0) {
  				if(tempRecordInBlock < 7) {
  					tempRecordInBlock++;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.name,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						k++;
  						memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  						if(strcmp(tempRecord.name,strValue) == 0){
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							rightEnd = 1;
  					}
  					else
  						rightEnd = 1;
  				}
  			}
  		}
  	}
  	else if(fieldNo == 2) {
  		int leftShift = 0;
  		char strValue[20];
  		strcpy(strValue,value);
  		memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  		if(strcmp(tempRecord.surname,strValue) < 0)
  			binary_search(firstRecord,midRecord - 1,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else if(strcmp(tempRecord.surname,strValue) > 0)
  			binary_search(midRecord + 1,lastRecord,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else {
  			print_record(tempRecord);
  			(*foundRecords)++;
  			int tempRecordInBlock = recordInBlock;
  			int k = 1;
  			int leftEnd = 0;
  			int rightEnd = 0;
  			while(leftEnd == 0) {
  				if(recordInBlock > 0) {
  					recordInBlock--;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.surname,strValue) == 0){
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						leftEnd = 1;
  				}
  				else {
  					if((searchBlock - k) != 0) {
  						add_to_array(blocksArray,searchBlock - k);
  						recordInBlock = 7;
  						if(BF_ReadBlock(fileDesc,searchBlock - k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional left block %d\n", searchBlock - k);
  						leftShift = 1;
  						k++;
  						memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  						if(strcmp(tempRecord.surname,strValue) == 0) {
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							leftEnd = 1;
  					}
  					else
  						leftEnd = 1;
  				}
  			}
  			if(leftShift == 1) {
  				if(BF_ReadBlock(fileDesc,searchBlock,&block) < 0) {
 	 				BF_PrintError("Could not read block\n");
  					return;
  				}
  			}
  			k = 1;
  			while(rightEnd == 0) {
  				if(tempRecordInBlock < 7) {
  					tempRecordInBlock++;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.surname,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						k++;
  						memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  						if(strcmp(tempRecord.surname,strValue) == 0) {
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							rightEnd = 1;
  					}
  					else
  						rightEnd = 1;
  				}
  			}
  		}
  	}
  	else if(fieldNo == 3) {
  		int leftShift = 0;
  		char strValue[20];
  		strcpy(strValue,value);
  		memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  		if(strcmp(tempRecord.city,strValue) < 0)
  			binary_search(firstRecord,midRecord - 1,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else if(strcmp(tempRecord.city,strValue) > 0)
  			binary_search(midRecord + 1,lastRecord,foundRecords,fileDesc,fieldNo,value,lastBlock,recordsinLastBlock,maxRecords,blocksArray);
  		else {
  			print_record(tempRecord);
  			(*foundRecords)++;
  			int tempRecordInBlock = recordInBlock;
  			int k = 1;
  			int leftEnd = 0;
  			int rightEnd = 0;
  			while(leftEnd == 0) {
  				if(recordInBlock > 0) {
  					recordInBlock--;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.city,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						leftEnd = 1;
  				}
  				else {
  					if((searchBlock - k) != 0) {
  						add_to_array(blocksArray,searchBlock - k);
  						recordInBlock = 7;
  						if(BF_ReadBlock(fileDesc,searchBlock - k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional left block %d\n", searchBlock - k);
  						leftShift = 1;
  						k++;
  						memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  						if(strcmp(tempRecord.city,strValue) == 0) {
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							leftEnd = 1;
  					}
  					else
  						leftEnd = 1;
  				}
  			}
  			if(leftShift == 1) {
  				if(BF_ReadBlock(fileDesc,searchBlock,&block) < 0) {
 	 				BF_PrintError("Could not read block\n");
  					return;
  				}
  			}
  			k = 1;
  			while(rightEnd == 0) {
  				if(tempRecordInBlock < 7) {
  					tempRecordInBlock++;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.city,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						k++;
  						memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  						if(strcmp(tempRecord.city,strValue) == 0) {
  							print_record(tempRecord);
  							(*foundRecords)++;
  						}
  						else
  							rightEnd = 1;
  					}
  					else
  						rightEnd = 1;
  				}
  			}
  		}
  	}
}

void add_to_array(int blocksArray[], int element) {
	int i = 0;
	while(blocksArray[i] != 0) {
		if(blocksArray[i] == element)
			break;
		i++;
	}
	blocksArray[i] = element;
}

int array_size(int blocksArray[]) {
	int i = 0;
	while(blocksArray[i] != 0)
		i++;
	return i;
}

void print_record(Record rec) {
	printf("\nID : %d\nName : %s\nSurname : %s\nCity : %s\n\n",rec.id,rec.name,rec.surname,rec.city);
}