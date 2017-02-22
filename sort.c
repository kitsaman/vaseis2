#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
	int maxRecords = BLOCK_SIZE/sizeof(Record);
	void* block;
	Sorted_Info info; 
	if(BF_ReadBlock(fileDesc,0,&block)<0) {
 	 	BF_PrintError("Could not read block\n");
  		return -1;
  	}
  	memcpy(&info,block,sizeof(Sorted_Info));
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
	}
	return 0;
}

void Sorted_SortFile(char *fileName,int fieldNo) {
	int fileDesc, blocks, recordsinLastBlock, perasmata = 1, i;
	char finalfile[strlen(fileName)+8],intString[2];
	void *block;
	Sorted_Info finfo;
	strcpy(finalfile,fileName);
	strcat(finalfile,"Sorted");
	sprintf(intString,"%d",fieldNo);
	strcat(finalfile,intString);
	fileDesc = BF_OpenFile(fileName);
	if(fileDesc < 0) {
		BF_PrintError("Could not open file\n");
  		return;
	}
	if(BF_ReadBlock(fileDesc,0,&block) < 0) {
 	 	BF_PrintError("Could not read block\n");
  		return;
  	}
  	memcpy(&finfo,block,sizeof(Sorted_Info));
  	memcpy(&blocks,block + 2 * sizeof(int),sizeof(int));
  	blocks--;
  	memcpy(&recordsinLastBlock,block + 3 * sizeof(int),sizeof(int));
  	Bubble_Sort(fileDesc,fileName,fieldNo,blocks,recordsinLastBlock);
  	if(BF_CloseFile(fileDesc) < 0) {
 	 	BF_PrintError("Could not close file\n");
  		return;
  	}
  	while(i < blocks) {
  		perasmata++;
  		i = pow(2,perasmata);
  	}
  	printf("XREIAZETAI %d perasmata\n", perasmata);
  	i = 1;
  	while(i < perasmata) {
  		//kane merge sort
  	}


  	//Edw na ginei to teleutaio merge sort sto teliko arxeio apo ta arxeia "filename(--i)0" kai "filename(--i)1"
  	if(BF_CreateFile(finalfile) < 0) {
 	 	BF_PrintError("Could not create file\n");
  		return;
  	}
  	fileDesc = BF_OpenFile(finalfile);
  	if(fileDesc < 0) {
 	 	BF_PrintError("Could not open file\n");
  		return;
  	}
  	if(BF_AllocateBlock(fileDesc) < 0) {
  		BF_PrintError("Could not allocate block\n");
  		return;
  	}
  	if(BF_ReadBlock(fileDesc,0,&block) < 0) {
 	 	BF_PrintError("Could not read block\n");
  		return;
  	}
  	finfo.isSorted = 1;
  	finfo.fieldNo = fieldNo;
  	memcpy(block,&finfo,sizeof(Sorted_Info));
  	if(BF_WriteBlock(fileDesc,0)) {
  		BF_PrintError("Could not write block\n");
  		return;
  	}
}

void Bubble_Sort(int fileDesc, char *fileName, int fieldNo, int initialBlocks, int recordsinLastBlock) {
	int i, j, k, newFileDesc;
	int maxRecords = BLOCK_SIZE / sizeof(Record);
	void *oldblock, *newblock;
	char intString[2];
	char tempfile[strlen(fileName)+1];
	char newfile[strlen(tempfile)+1];
	Record r1, r2 ,tempRecord;
	strcpy(tempfile,fileName);
	strcat(tempfile,"0");
	for(i = 0 ; i <= initialBlocks ; i++) {
		if(i == initialBlocks -1)
			maxRecords = recordsinLastBlock;
		if(BF_ReadBlock(fileDesc,i+1,&oldblock) < 0) {
  			BF_PrintError("Could not read block\n");
  			return;
  		}
		strcpy(newfile,tempfile);
		sprintf(intString,"%d",i);
		strcat(newfile,intString);
		if(BF_CreateFile(newfile) < 0) {
 	 		BF_PrintError("Could not create file\n");
  			return;
  		}
  		newFileDesc = BF_OpenFile(newfile);
  		if(newFileDesc < 0) {
  			BF_PrintError("Could not open file\n");
  			return;
  		}
  		if(BF_AllocateBlock(newFileDesc) < 0) {
  			BF_PrintError("Could not allocate block\n");
  			return;
  		}
  		if(BF_ReadBlock(newFileDesc,0,&newblock) < 0) {
  			BF_PrintError("Could not read block\n");
  			return;
  		}
  		memcpy(newblock,oldblock,BLOCK_SIZE);
  		if(fieldNo == 0) {
  			for(j = 0 ; j < maxRecords-1 ; j++) {
  				int swapped = 0;
  				for(k = 0 ; k < maxRecords-1-j; k++) {
  					memcpy(&r1,newblock + k * sizeof(Record),sizeof(Record));
  					memcpy(&r2,newblock + (k+1) * sizeof(Record),sizeof(Record));
  					if(r1.id > r2.id) {
  						memcpy(&tempRecord,&r1,sizeof(Record));
  						memcpy(newblock + k * sizeof(Record),&r2,sizeof(Record));
  						memcpy(newblock + (k+1) * sizeof(Record),&tempRecord,sizeof(Record));
  						swapped = 1;
  					}
  				}

  				if(swapped == 0)
  					break;
  			}
  		}
  		if(fieldNo == 1) {
  			for(j = 0 ; j < maxRecords-1 ; j++) {
  				int swapped = 0;
  				for(k = 0 ; k < maxRecords-1-j; k++) {
  					memcpy(&r1,newblock + k * sizeof(Record),sizeof(Record));
  					memcpy(&r2,newblock + (k+1) * sizeof(Record),sizeof(Record));
  					if(strcmp(r1.name,r2.name) > 0) {
  						memcpy(&tempRecord,&r1,sizeof(Record));
  						memcpy(newblock + k * sizeof(Record),&r2,sizeof(Record));
  						memcpy(newblock + (k+1) * sizeof(Record),&tempRecord,sizeof(Record));
  						swapped = 1;
  					}
  				}

  				if(swapped == 0)
  					break;
  			}
  		}
  		if(fieldNo == 2) {
  			for(j = 0 ; j < maxRecords-1 ; j++) {
  				int swapped = 0;
  				for(k = 0 ; k < maxRecords-1-j; k++) {
  					memcpy(&r1,newblock + k * sizeof(Record),sizeof(Record));
  					memcpy(&r2,newblock + (k+1) * sizeof(Record),sizeof(Record));
  					if(strcmp(r1.surname,r2.surname) > 0) {
  						memcpy(&tempRecord,&r1,sizeof(Record));
  						memcpy(newblock + k * sizeof(Record),&r2,sizeof(Record));
  						memcpy(newblock + (k+1) * sizeof(Record),&tempRecord,sizeof(Record));
  						swapped = 1;
  					}
  				}

  				if(swapped == 0)
  					break;

  			}
  		}
  		if(fieldNo == 3) {
  			for(j = 0 ; j < maxRecords-1 ; j++) {
  				int swapped = 0;
  				for(k = 0 ; k < maxRecords-1-j; k++) {
  					memcpy(&r1,newblock + k * sizeof(Record),sizeof(Record));
  					memcpy(&r2,newblock + (k+1) * sizeof(Record),sizeof(Record));
  					if(strcmp(r1.city,r2.city) > 0) {
  						memcpy(&tempRecord,&r1,sizeof(Record));
  						memcpy(newblock + k * sizeof(Record),&r2,sizeof(Record));
  						memcpy(newblock + (k+1) * sizeof(Record),&tempRecord,sizeof(Record));
  						swapped = 1;
  					}
  				}

  				if(swapped == 0)
  					break;

  			}
  		}
  		printf("Gia to arxeio me onoma %s exoume :\n", newfile);
  		for(j=0;j<maxRecords;j++)
  		{
  			memcpy(&r1,newblock + j*sizeof(Record),sizeof(Record));
  			print_record(r1);
  		}
  		if(BF_WriteBlock(newFileDesc,0)) {
  			BF_PrintError("Could not write block\n");
  			return;
  		}
  		if(BF_CloseFile(newFileDesc) < 0) {
 	 		BF_PrintError("Could not close file\n");
  			return;
  		}
	}
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
 	 	printf("File with name %s is NOT Sorted\n", fileName);
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
  		int leftShift = 0, rightShift = 0;
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
  					if(rightShift == 1 && searchBlock + k - 1 == lastBlock && tempRecordInBlock == recordsinLastBlock)
  						break;
  					memcpy(&tempRecord,block + (tempRecordInBlock * sizeof(Record)),sizeof(Record));
  					if(tempRecord.id == idValue) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock + 1) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						rightShift = 1;
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
  		int leftShift = 0, rightShift = 0;
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
  					if(rightShift == 1 && searchBlock + k - 1 == lastBlock && tempRecordInBlock == recordsinLastBlock)
  						break;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.name,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock + 1) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						rightShift = 1;
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
  		int leftShift = 0, rightShift = 0;
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
  					if(rightShift == 1 && searchBlock + k - 1 == lastBlock && tempRecordInBlock == recordsinLastBlock)
  						break;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.surname,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock + 1) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						rightShift = 1;
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
  		int leftShift = 0, rightShift = 0;
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
  					if(rightShift == 1 && searchBlock + k - 1 == lastBlock && tempRecordInBlock == recordsinLastBlock)
  						break;
  					memcpy(&tempRecord,block + (recordInBlock * sizeof(Record)),sizeof(Record));
  					if(strcmp(tempRecord.city,strValue) == 0) {
  						print_record(tempRecord);
  						(*foundRecords)++;
  					}
  					else
  						rightEnd = 1;
  				}
  				else {
  					if((searchBlock + k) != lastBlock + 1) {
  						add_to_array(blocksArray,searchBlock + k);
  						tempRecordInBlock = 0;
  						if(BF_ReadBlock(fileDesc,searchBlock + k,&block) < 0) {
 	 						BF_PrintError("Could not read block\n");
  							return;
  						}
  						printf("Checking additional right block %d\n", searchBlock + k);
  						rightShift = 1;
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