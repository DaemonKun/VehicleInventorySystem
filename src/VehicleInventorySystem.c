#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXLINE 180


struct vehicle{
	int lotNum;
	char model[30];
	char color[15];
	char plateNum[10];
	char ownerName[30];
	char dateImpounded[15];

};



void mainMenu(void);
int searchMenu(void);
int listMenu(void);
int addMenu(void);
int modifyMenu(void);
int deleteMenu(void);
FILE* open(char param);
void clearscr(void);
void getVal(char* dest);
int backToMenu(void);
char* strupr(char* s);
int getMaxLot();
void printSrch(char* buff[], int numSrch);

int search(char lineSrch[],char* buff[], int* numSrch, int cri);
int deleteFromFile(int pos, char update, char *updateItem, int cri);

int main(void) {
	mainMenu();
	return EXIT_SUCCESS;
}

void mainMenu(void){
	int retVal;

	do{
		char option;
		clearscr();
		retVal = 1;

		printf("\t\t\t\t=====Vehicle Inventory System=====\n\n\n");
		printf("Choose from the following options\n");

		printf("1. Search Record\n");
		printf("2. Add Record/s\n");
		printf("3. List All Records\n");
		printf("4. Update record\n");
		printf("5. Delete record\n");
		printf("6. Exit Program\n");
		printf("Select the number of option(e.g 1 for Search): ");
		getVal(&option);
		switch(option){
		case '1':
			clearscr();
			retVal = searchMenu();
			break;
		case '2':
			clearscr();
			retVal = addMenu();
			break;
		case '3':
			clearscr();
			retVal = listMenu();
			break;
		case '4':
			clearscr();
			retVal = modifyMenu();
			break;
		case '5':
			clearscr();
			retVal = deleteMenu();
			break;
		case '6':
			retVal = 0;
			break;
		default:
			printf("Not a valid option\n");
			break;
		}
	}while(retVal == 1);
	if(retVal != -1){
		printf("Program is exiting. Thank you for using...\n");
		getchar();
	}


}

FILE *open(char param){
	if (param == 'a')
		return fopen("vehicleDB.dat","a");
	else if(param == 'r')
		return fopen("vehicleDB.dat","r");
	else
		return fopen("temp.dat","wb");
}

int listMenu(void){
	int maxLot = getMaxLot();
	struct vehicle vehicleList[maxLot];
	FILE* db = open('r');
	if(db == NULL){
		fprintf(stderr,"vehicleDB.dat is not found. Program will now exit...\n");
		getchar();
		return -1;
	}
	else{
		char line[MAXLINE];
		char *ptr;
		int i = 0;
		while(fgets(line, sizeof(line), db) != 0){


			ptr = strtok(line,",");
			vehicleList[i].lotNum = atoi(ptr);

			ptr = strtok(NULL,",");
			strcpy(vehicleList[i].model,strdup(ptr));

			ptr = strtok(NULL,",");
			strcpy(vehicleList[i].color,strdup(ptr));

			ptr = strtok(NULL,",");
			strcpy(vehicleList[i].plateNum,strdup(ptr));

			ptr = strtok(NULL,",");
			strcpy(vehicleList[i].ownerName,strdup(ptr));

			ptr = strtok(NULL,"\n");
			strcpy(vehicleList[i].dateImpounded,strdup(ptr));

			i++;

		}

	}

	fclose(db);

	printf("\t\t\t\t\t=====ALL CARS ON IMPOUND=====\n\n");
	printf("%-20s %-30s %-15s %-15s %-30s %-15s\n","Lot Number"
			,"Car Model","Color","Plate Number","Owner Name","Date Impounded");
	for(int i = 0; i < maxLot; i++){
		printf("%-20d %-30s %-15s %-15s %-30s %-15s\n",
				vehicleList[i].lotNum,
				vehicleList[i].model,
				vehicleList[i].color,
				vehicleList[i].plateNum,
				vehicleList[i].ownerName,
				vehicleList[i].dateImpounded);
	}
	getchar();
	return backToMenu();

}

int addMenu(void){

	int currMax = getMaxLot();
	int numRec = 1,retVal;
	char lineRec [MAXLINE];

	printf("Input how many records to be added: ");
	fgets(lineRec, sizeof(lineRec), stdin);
	do{
		retVal = sscanf(lineRec, "%d", &numRec);
		if(retVal !=1){
			printf("Please enter a valid number: ");
			fgets(lineRec, sizeof(lineRec), stdin);
		}
	}while(retVal!=1);


	FILE *db = open('a');
	struct vehicle vehicleAdd[numRec];

	if(db == NULL){
		fprintf(stderr,"vehicleDB.dat is not found. Program will now exit...\n");
		return -1;
	}

	else{
		for(int i = 0; i < numRec; i++){
			char *buffer = (char*)malloc(MAXLINE);
			vehicleAdd[i].lotNum = currMax + i + 1;
			printf("For Lot Number %d\n", vehicleAdd[i].lotNum);

			printf("Please enter the model to be added: ");
			getVal(vehicleAdd[i].model);

			printf("Please enter the color of the car: ");
			getVal(vehicleAdd[i].color);

			printf("Please enter the Plate Number of the car: ");
			getVal(vehicleAdd[i].plateNum);

			printf("Please enter the name of the car owner: ");
			getVal(vehicleAdd[i].ownerName);

			printf("Please enter the impounding date of the car[e.g. Dec 23, 2007]: ");
			getVal(vehicleAdd[i].dateImpounded);

			sprintf(buffer,"%d,%s,%s,%s,%s,%s\n",
					vehicleAdd[i].lotNum,
					vehicleAdd[i].model,
					vehicleAdd[i].color,
					vehicleAdd[i].plateNum,
					vehicleAdd[i].ownerName,
					vehicleAdd[i].dateImpounded);

			fprintf(db, "%s", strupr(buffer));
		}



	}
	fclose(db);
	printf("Records has been successfully added.\n");
	getchar();
	return backToMenu();

}

int searchMenu(){

	int cri = 0,numSrch = 0;;
	int maxLot = getMaxLot();
	char criteria[2];
	char lineSrch[MAXLINE];
	char* buff[maxLot];

	while(1){
		printf("Criteria for search:\n");
		printf("A. Lot Number\n");
		printf("B. Car model\n");
		printf("C. Color\n");
		printf("D. Plate Number\n");
		printf("E. Owner Name\n");
		printf("F. Impounding Date\n");
		printf("Please select a valid criteria (e.g. A for Lot number): ");
		getVal(criteria);

		if(toupper(criteria[0]) == 'A'){
			clearscr();
			printf("Please input the Lot number to be searched: ");
			break;
		}
		else if(toupper(criteria[0]) == 'B'){
			clearscr();
			cri = 1;
			printf("Please input the Car model to be searched: ");
			break;
		}
		else if(toupper(criteria[0]) == 'C'){
			cri = 2;
			clearscr();
			printf("Please input the Color to be searched: ");
			break;
		}
		else if(toupper(criteria[0]) == 'D'){
			cri = 3;
			clearscr();
			printf("Please input the Plate Number to be searched: ");
			break;
		}
		else if(toupper(criteria[0]) == 'E'){
			cri = 4;
			clearscr();
			printf("Please input the Owner Name to be searched: ");
			break;
		}
		else if(toupper(criteria[0]) == 'F'){
			cri = 5;
			clearscr();
			printf("Please input the impounding date to be searched: ");
			break;
		}
		else
			clearscr();
	}


	getVal(lineSrch);

	int retVal = search(lineSrch,buff,&numSrch,cri);

	if(retVal == -1){
		return retVal;
	}

	if(numSrch == 0)
		printf("No records has been found\n");
	else
		printSrch(buff,numSrch);
	getchar();

	retVal = backToMenu();
	return 1;


}

int search(char lineSrch[],char *buff[], int* numSrch, int cri){
	FILE* db = open('r');
	if (db == NULL){
		fprintf(stderr,"vehicleDB.dat is not found. Program will now exit...\n");
		getchar();
		return -1;
	}
	else{
		clearscr();


		char currLine[MAXLINE],line[MAXLINE];

		while(fgets(line,MAXLINE,db)!=0){
			int currCri = 0;
			char* ptr;
			strcpy(currLine,line);
			buff[*numSrch] = (char*)malloc(MAXLINE);

			ptr = strtok(line,",");
			if(strcmp(strdup(ptr),strupr(lineSrch)) == 0){
				sprintf(buff[*numSrch], "%s", currLine);
				*numSrch = *numSrch + 1;
			}
			while(currCri < cri){
				if(currCri == 4)
					ptr = strtok(NULL,"\n");

				else
					ptr = strtok(NULL,",");

				if(strcmp(strdup(ptr),strupr(lineSrch)) == 0){
					sprintf(buff[*numSrch], "%s", currLine);
					*numSrch = *numSrch + 1;
					break;
				}

				currCri++;
			}

		}

	}
	fclose(db);
	return EXIT_SUCCESS;
}

int getMaxLot(void){
	FILE* db = open('r');
	int max = 0;
	if(db == NULL)
		return 0;
	else{
		char line[180];
		char *ptr;
		while(fgets(line, sizeof(line), db) != 0){


			ptr = strtok(line,",");
			max = atoi(ptr);
		}
	}
	fclose(db);
	return max;


}
void clearscr(void)
{
#ifdef _WIN32
	system("cls");
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
	system("clear");
	//add some other OSes here if needed
#else
#error "OS not supported."
	//you can also throw an exception indicating the function can't be used
#endif
}

void getVal(char* dest){
	char *ptr;
	fgets(dest,sizeof(dest),stdin);
	ptr = strchr(dest, '\n');
	if(ptr)
		*ptr = '\0';
	fflush(stdin);
}

char* strupr(char* s)
{
	char* tmp = s;

	for (;*tmp;++tmp) {
		*tmp = toupper((unsigned char) *tmp);
	}

	return s;
}
int backToMenu(void){

	char choice;
	int retVal = 0;
	do{

		clearscr();
		printf("Would you like to go back to the main menu(Y/N): ");
		getVal(&choice);
		if(toupper(choice) == 'Y' || toupper(choice) == 'N')
			retVal = 1;
	}while(retVal  == 0);
	if(toupper(choice) == 'Y')
		return 1;
	else
		return EXIT_SUCCESS;

}
int modifyMenu(void){
	int cri = 0,numSrch = 0;
	int maxLot = getMaxLot();
	char criteria;
	char lineSrch[MAXLINE];
	char *buff[maxLot];
	char *updateItem = (char*)malloc(MAXLINE);
	int modCri;
	struct vehicle vehicleMod;
	while(1){
		char* ptr;
		char choice;

		printf("Criteria to search:\n");
		printf("A. Lot Number\n");
		printf("B. Plate Number\n");
		printf("Please select a valid criteria (e.g. A for Lot number): ");
		getVal(&criteria);

		if(toupper(criteria) == 'A'){
			clearscr();
			printf("Please input the Lot number to be searched: ");
		}
		else if(toupper(criteria) == 'B'){
			cri = 3;
			clearscr();
			printf("Please input the Plate Number to be searched: ");
		}
		else{
			clearscr();
			continue;
		}

		getVal(lineSrch);
		int retVal = search(lineSrch,buff,&numSrch,cri);
		if(retVal == -1){
			return -1;
		}
		if (numSrch == 0){
			printf("Program can't find the record to be updated. Returning to search...");
			getchar();
			clearscr();
			continue;
		}


		ptr = strtok(buff[0],",");
		vehicleMod.lotNum = atoi(ptr);

		ptr = strtok(NULL,",");
		strcpy(vehicleMod.model,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleMod.color,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleMod.plateNum,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleMod.ownerName,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleMod.dateImpounded,strdup(ptr));


		while(1){
			printf("Is Lot number %d with plate number %s to be updated (Y/N): "
					,vehicleMod.lotNum, vehicleMod.plateNum);
			getVal(&choice);

			if(toupper(choice) == 'Y' ||
					toupper(choice) == 'N')
				break;
			else
				printf("Invalid choice \n");

		}
		if(toupper(choice) == 'Y'){
			clearscr();
			break;
		}
		else if(toupper(choice) == 'N'){
			clearscr();
			continue;
		}
	}
	while(1){
		printf("What will you be updating?\n");
		printf("A. Car Model\n");
		printf("B. Color\n");
		printf("C. Plate Number\n");
		printf("D. Owner Name\n");
		printf("E. Impounding Date\n");
		printf("Selection (e.g. A for car model: ");
		getVal(&criteria);

		if(toupper(criteria) == 'A'){
			clearscr();
			modCri = 1;
			printf("Please input the new Car Model(Previous Data:%s): ",vehicleMod.model);
			break;
		}
		else if(toupper(criteria) == 'B'){
			modCri = 2;
			clearscr();
			printf("Please input the new Color(Previous Data:%s): ",vehicleMod.color);
			break;
		}
		else if(toupper(criteria) == 'C'){
			modCri = 3;
			clearscr();
			printf("Please input the new Plate Number(Previous Data:%s): ",vehicleMod.plateNum);
			break;
		}
		else if(toupper(criteria) == 'D'){
			modCri = 4;
			clearscr();
			printf("Please input the new Owner Name(Previous Data:%s): ",vehicleMod.ownerName);
			break;
		}
		else if(toupper(criteria) == 'E'){
			modCri = 5;
			clearscr();
			printf("Please input the new Impounding Date(Previous Data:%s): ",vehicleMod.dateImpounded);
			break;
		}
		else{
			clearscr();
			continue;

		}

	}
	getVal(updateItem);
	strupr(updateItem);
	int retVal = deleteFromFile(vehicleMod.lotNum,'Y',updateItem,modCri);
	if(retVal == -1){
		return -1;
	}
	free(updateItem);
	printf("Record has been updated\n");
	getchar();
	return backToMenu();
}
void printSrch(char* buff[], int numSrch){
	struct vehicle vehicleSrch[numSrch];
	printf("%-20s %-30s %-15s %-15s %-30s %-15s\n","Lot Number"
			,"Car Model","Color","Plate Number","Owner Name","Date Impounded");
	for (int i = 0; i < numSrch; i++){
		char* ptr;
		ptr = strtok(buff[i],",");
		vehicleSrch[i].lotNum = atoi(ptr);

		ptr = strtok(NULL,",");
		strcpy(vehicleSrch[i].model,strdup(ptr));

		ptr = strtok(NULL,",");
		strcpy(vehicleSrch[i].color,strdup(ptr));

		ptr = strtok(NULL,",");
		strcpy(vehicleSrch[i].plateNum,strdup(ptr));

		ptr = strtok(NULL,",");
		strcpy(vehicleSrch[i].ownerName,strdup(ptr));

		ptr = strtok(NULL,"\n");
		strcpy(vehicleSrch[i].dateImpounded,strdup(ptr));

		printf("%-20d %-30s %-15s %-15s %-30s %-15s\n",
				vehicleSrch[i].lotNum,
				vehicleSrch[i].model,
				vehicleSrch[i].color,
				vehicleSrch[i].plateNum,
				vehicleSrch[i].ownerName,
				vehicleSrch[i].dateImpounded);

	}
	printf("\n\nSearch returned %d results", numSrch);
}

int deleteFromFile(int pos, char update, char* updateItem, int cri){
	int currPos = 1;
	FILE* db = open('r');
	FILE* tempDB = open('w');
	char line[MAXLINE],currLine[MAXLINE];

	if (db == NULL || tempDB == NULL){
		fprintf(stderr,"vehicleDB.dat is not found. Program will now exit...\n");
		getchar();
		return -1;
	}

	if(update == 'N'){
		while(fgets(line,MAXLINE,db)!=0){
			if(currPos != pos){
				fputs(line,tempDB);
			}
			currPos++;
		}
	}
	else if(update == 'Y'){
		while(fgets(line,MAXLINE,db)!=0){
			if(currPos != pos){
				fputs(line,tempDB);
			}
			else{
				int i = 1;
				char* ptr;
				char* buff = (char*)malloc(MAXLINE);
				char* end = (char*)malloc(2);
				memset(buff,'\0',MAXLINE);
				memset(end,'\0',2);
				strcpy(end,",");
				strcpy(currLine,line);

				ptr = strtok(line,",");
				if(cri == 0){
					strcat(buff,strdup(updateItem));
					strcat(buff,",");
				}
				else{
					strcat(buff,strdup(ptr));
					strcat(buff,end);
				}

				while(i < 6){
					if (i == 5){
						ptr = strtok(NULL,"\n");
						strcpy(end,"\n");
					}

					else
						ptr = strtok(NULL,",");

					if(i == cri){
						strcat(buff,strdup(updateItem));
						strcat(buff,end);
					}
					else{
						strcat(buff,strdup(ptr));
						strcat(buff,end);
					}
					i++;
				}
				fputs(buff,tempDB);

			}
			currPos++;
		}
	}
	fclose(db);
	fclose(tempDB);
	remove("vehicleDB.dat");
	rename("temp.dat","vehicleDB.dat");
	return EXIT_SUCCESS;

}

int deleteMenu(void){
	int cri = 0,numSrch = 0;
	int maxLot = getMaxLot();
	char criteria;
	char lineSrch[MAXLINE];
	char *buff[maxLot];
	struct vehicle vehicleDelete;
	while(1){
		char* ptr;
		char choice;

		printf("Criteria to search:\n");
		printf("A. Lot Number\n");
		printf("B. Plate Number\n");
		printf("Please select a valid criteria (e.g. A for Lot number): ");
		getVal(&criteria);

		if(toupper(criteria) == 'A'){
			clearscr();
			printf("Please input the Lot number to be searched: ");
		}
		else if(toupper(criteria) == 'B'){
			cri = 3;
			clearscr();
			printf("Please input the Plate Number to be searched: ");
		}
		else{
			clearscr();
			continue;
		}

		getVal(lineSrch);
		int retVal = search(lineSrch,buff,&numSrch,cri);
		if(retVal == -1)
			return -1;

		if (numSrch == 0){
			printf("Program can't find the record to be updated. Returning to search...");
			getchar();
			clearscr();
			continue;
		}


		ptr = strtok(buff[0],",");
		vehicleDelete.lotNum = atoi(ptr);

		ptr = strtok(NULL,",");
		strcpy(vehicleDelete.model,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleDelete.color,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleDelete.plateNum,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleDelete.ownerName,strdup(ptr));
		ptr = strtok(NULL,",");
		strcpy(vehicleDelete.dateImpounded,strdup(ptr));


		while(1){
			printf("Is Lot number %d with plate number %s to be deleted (Y/N): "
					,vehicleDelete.lotNum, vehicleDelete.plateNum);
			getVal(&choice);

			if(toupper(choice) == 'Y' ||
					toupper(choice) == 'N')
				break;
			else
				printf("Invalid choice \n");

		}
		if(toupper(choice) == 'Y'){
			clearscr();
			break;
		}
		else if(toupper(choice) == 'N'){
			clearscr();
			continue;
		}
	}
	int retVal = deleteFromFile(vehicleDelete.lotNum,'N',NULL,0);
	if(retVal == -1){
		return -1;
	}
	printf("Record has been deleted\n");
	getchar();
	return backToMenu();
}
