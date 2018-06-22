#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINE 180


struct vehicle{
	int lotNum;
	char model[30];
	char color[15];
	char plateNum[10];
	char ownerName[30];
	char dateImpounded[15];

};

FILE* open(char param);
void mainMenu(void);
void list(void);
void add(void);
void search(void);
void modify(void); //unimplemented
void delete(void; //unimplemented
void clearscr(void);
void getVal(char dest[]);
void backToMenu(void);
char* strupr(char* s);
int getMaxLot();

int main(void) {
	mainMenu();
	return EXIT_SUCCESS;
}

void mainMenu(void){
	int option;
	char lineOpt[MAXLINE];
	int retVal;
	clearscr();
	printf("\t\t\t\t=====Vehicle Inventory System=====\n\n\n");
	printf("Choose from the following options\n");

	printf("1. Search Record\n");
	printf("2. Add Record/s\n");
	printf("3. List All Records\n");
	printf("Select the number of option(e.g 1 for Search): ");
	fgets(lineOpt, sizeof(lineOpt), stdin);
	do{
		retVal = sscanf(lineOpt, "%d", &option);
		if(retVal !=1 || option > 4){
			printf("Please enter a valid option: ");
			fgets(lineOpt, sizeof(lineOpt), stdin);

		}
		fflush(stdin);
	}while(retVal!=1);
	switch(option){
	case 1:
		clearscr();
		search();
		break;
	case 2:
		clearscr();
		add();
		break;
	case 3:
		clearscr();
		list();
		break;
	default:
		printf("Error");
	}

}

FILE *open(char param){
	if (param == 'a')
		return fopen("vehicleDB.dat","a");
	else
		return fopen("vehicleDB.dat","r");
}

void list(void){
	int maxLot = getMaxLot();
	struct vehicle vehicleList[maxLot];
	FILE* db = open('r');
	if(db == NULL)
		printf("vehicleDB.dat is not found.");
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

	printf("\t\t\t\t\t\t=====ALL CARS ON IMPOUND=====\n\n");
	printf("Lot Number\tCar Model\t\tColor\t\tPlate Number\t\tOwner\t\t\tDate Impounded\n");
	for(int i = 0; i < maxLot; i++){
		printf("%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
				vehicleList[i].lotNum,
				vehicleList[i].model,
				vehicleList[i].color,
				vehicleList[i].plateNum,
				vehicleList[i].ownerName,
				vehicleList[i].dateImpounded);
	}
	getchar();
	backToMenu();

}

void add(void){

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

	if(db == NULL)
		fprintf(stderr,"File not found");
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
	backToMenu();

}

void search(){
	char criteria[2];
	char lineSrch[MAXLINE];
	int cri = 0;
	int maxLot = getMaxLot();

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

	FILE* db = open('r');
	if (db == NULL)
		fprintf(stderr,"vehicleDB.dat is not found.");
	else{
		clearscr();

		int numSrch = 0;
		char currLine[MAXLINE],line[MAXLINE];
		char *buff[maxLot];
		while(fgets(line,MAXLINE,db)!=0){
			int currCri = 0;
			char* ptr;
			strcpy(currLine,line);
			buff[numSrch] = (char*)malloc(MAXLINE);

			ptr = strtok(line,",");
			if(strcmp(strdup(ptr),strupr(lineSrch)) == 0){
				sprintf(buff[numSrch], "%s", currLine);
				numSrch++;
			}
			while(currCri < cri){
				ptr = strtok(NULL,",");
				if(strcmp(strdup(ptr),strupr(lineSrch)) == 0){
					sprintf(buff[numSrch], "%s", currLine);
					numSrch++;
					break;
				}
				currCri++;
			}
		}
		if(numSrch == 0)
			printf("No records has been found\n");
		else{
			struct vehicle vehicleSrch[numSrch];
			printf("Lot Number\tCar Model\t\tColor\t\tPlate Number\t\tOwner\t\t\tDate Impounded\n");
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

				printf("%d\t\t%s\t\t%s\t\t%s\t\t%s\t\t%s\n",
						vehicleSrch[i].lotNum,
						vehicleSrch[i].model,
						vehicleSrch[i].color,
						vehicleSrch[i].plateNum,
						vehicleSrch[i].ownerName,
						vehicleSrch[i].dateImpounded);

			}
			printf("\n\nSearch returned %d results", numSrch);
		}
	}
	fclose(db);
	getchar();
	backToMenu();

}

int getMaxLot(void){
	FILE* db = open('r');
	int max = 0;
	if(db == NULL)
		fprintf(stderr,"vehicleDB.dat is not found.");
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

void getVal(char dest[]){
	char *ptr;
	fgets(dest,30,stdin);
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
void backToMenu(void){

	char choice;
	while(1){
		clearscr();
		printf("Would you like to go back to the main menu(Y/N): ");
		scanf("%c",&choice);
		fflush(stdin);
		if(toupper(choice) == 'Y' || toupper(choice) == 'N')
			break;

	}
	if(toupper(choice) == 'Y')
		mainMenu();

}
