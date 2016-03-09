#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int menu();
void returnToMenu();
int initFile();
void createAccount();
void printAllAccounts();
void findAccount();
void makePayment();
void makeTransfer();
void makeWithdrawal();
void handleChoice(int choice);

typedef struct {
	char name[21];
	char surname[21];
	char address[31];
	int id;
	int accno;
	float balance;
} account;

int menu () {
	char *end;
	char buffer[10];
	int choice;
	do {
		system("clear");
		printf("=== BANKING SYSTEM MENU ===\n(1) Create new account\n(2) Display info about all accounts\n(3) Find an account\n(4) Make a transfer\n(5) Make a payment\n(6) Make a withdrawal\n(0) Quit\n\nYour choice: ");
		scanf("%10s", buffer);
		choice=strtol(buffer,&end,10);
    } while (*end != '\0' || (choice<0 || choice>6));
    return choice;
}

void returnToMenu() {
	printf("\nPress enter to return to menu");
	getchar();getchar();
	handleChoice(menu());
}

int initFile() {
	FILE *fp;
	printf("Attempting to load file \"accounts.dat\"...\n");
	fp=fopen("accounts.dat","rb");
	if (fp==NULL) {
		printf("File does not exist, creating new blank one... ");
		fp=fopen("accounts.dat","wb");
		if (fp==NULL) {
			printf("failed for unknown reasons.\n");
			return 0;
		} else {
			printf("done.\n");
			fclose(fp);
			return 1;
		}
	} else {
		printf("File exists and will be used.\n");
		fclose(fp);
		return 1;
	}
}

void printAllAccounts() {
	int count=0;
	FILE *fp;
	account read;
	
	system("clear");	
	fp=fopen("accounts.dat","rb");
	if (!fp) {
		perror("Failed to read the file");
		return;
	}
	printf("=== LISTING ALL ACCOUNTS: ===\n\n");
	
	while(!feof(fp)) {
		
		if (fread(&read, sizeof(account),1,fp)==1) {
			count++;
			printf("ACCOUNT #%d\nName:\t\t%s\nSurname:\t%s\nAddress:\t%s\nID:\t\t%d\nAccount no.:\t%d\nBalance:\t%.2f\n\n",count,read.name,read.surname,read.address,read.id,read.accno,read.balance); 
		} else {
			if (count==0)
				printf("No accounts present in database.\n");
			break;
		}
	}
	fclose(fp);
	returnToMenu();
}

void createAccount() {
	char newname[21]="";
	char newsurname[21]="";
	char newaddress[31]="";
	int newid=0;
	int newaccno=0;
	float newbalance=0.0;
	FILE *fp;
	account new;
	
	char buffer[10];
	/*char *end;*/
	
	system("clear");
	fflush(stdin);
	printf("=== CREATING NEW ACCOUNT ===\n\n");
	
	/* READ INPUT */
	do {
		printf("Input name for a new user: ");
		scanf("%20s",newname);
	} while (newname[0]=='\0');
	do {
		printf("Input surname for a new user: ");
		scanf("%20s",newsurname);
	} while (newsurname[0]=='\0');
	do {
		printf("Input address for a new user: ");
		scanf("%30s",newaddress);
	} while (newaddress[0]=='\0');
	do {
		printf("Input ID for a new user: ");
		scanf("%8d",&newid);
	} while (newid<0);
	do {
		printf("Input account number for a new user: ");
		scanf("%d",&newaccno);
	} while (newaccno<0);
	do {
		printf("Input balance for a new user: ");
		scanf("%f",&newbalance);
	} while (0);
	/* END READ INPUT */
	
	fflush(stdin);
	printf("\n=== YOU ENTERED: ===\nName:\t\t%s\nSurname:\t%s\nAddress:\t%s\nID:\t\t%d\nAccount No.:\t%d\nBalance:\t%.2f\n\n",newname,newsurname,newaddress,newid,newaccno,newbalance);
	do {
		printf("Are you sure you want to create account with the above data? (y/n): ");
		scanf("%50s",buffer);
	} while (buffer[0]!='y' && buffer[0]!='n');
	if (buffer[0]=='y') {
		printf("Saving new user... ");
		/* FILE UPDATE */
		
		strcpy(new.name, newname);
		strcpy(new.surname, newsurname);
		strcpy(new.address, newaddress);
		new.id=newid;
		new.accno=newaccno;
		new.balance=newbalance;
		fp=fopen("accounts.dat","ab+");
		fwrite (&new,sizeof(account),1,fp);
		fclose(fp);
		printf("done.");
				
		returnToMenu();
	} else if (buffer[0]=='n') {
		returnToMenu();
	}
}

void findAccount() {
	char *end;
	char buffer[10];
	int choice;
	
	do {
		system("clear");
		fflush(stdin);
		printf("=== SEARCHING FOR AN ACCOUNT ===\n");
		printf("Choose field which will be checked:\n(1) Name\n(2) Surname\n(3) Address\n(4) ID\n(5) Account number\n(6) Balance\n(0) Return to main menu\n\nYour choice: ");
		scanf("%10s",buffer);
		choice=strtol(buffer,&end,10);
	} while (choice<0 || choice>6 || *end != '\0');
	
	
}

void makePayment() {
	int count=0,user;
	char buffer[50];
	char *end;
	float currentBalance,payment;
	FILE *fp;
	account read;
	
	system("clear");
	
	fp=fopen("accounts.dat","r+b");
	if (!fp) {
		perror("Failed to read the file");
		return;
	}
	
	while (!feof(fp)) {
		if (fread(&read,sizeof(account),1,fp)==1)
		  count++;
	}
	rewind(fp);
		
	
	if (count==0) {
		printf("No accounts available, cannot make any payment\n");
		returnToMenu();
	}
	
	do {
		printf("Which account to use? (%d accounts available): ", count);
		scanf("%50s", buffer);
		user=strtol(buffer,&end,10);
	} while (user<1 || user>count);
	
	printf("Using account %d:\n",user);
		
	fseek(fp,(user-1)*sizeof(account),SEEK_SET);
	fread(&read,sizeof(account),1,fp);
	
	currentBalance=read.balance;
	printf("Current balance for account %d is %.2f\n", user, currentBalance);
	
	do {
		printf("What payment you wish to make? ");
		scanf("%50s",buffer);
		payment=strtod(buffer, NULL);
	} while (payment<0.0);
	
	printf("\n");
	do {
		printf("Are you sure you want to make a payment of %.2f? (y/n): ",payment);
		scanf("%50s",buffer);
	} while (buffer[0]!='y' && buffer[0]!='n');
	
	if (buffer[0]=='y') {
		printf("\nCommiting payment of %.2f to account %d... ",payment,user);
		/* FILE INSERT GOES HERE */
		
		read.balance+=payment;
		fseek(fp,(user-1)*sizeof(account), SEEK_SET);
		fwrite(&read,sizeof(account),1,fp);
		fclose(fp);
		printf("done.\n\n");
		
		returnToMenu();
	} else {
		fclose(fp);
		returnToMenu();
	}
	
}

void makeTransfer() {
	int count=0,userFrom,userTo;
	char buffer[50];
	char *end;
	float currentBalanceFrom,currentBalanceTo,amount;
	FILE *fp;
	account from, to;
	
	system("clear");
	
 	fp=fopen("accounts.dat","r+b");
	if (!fp) {
		perror("Failed to open the file");
		return;
	}
	
	/* see how many accounts there are in file */
	while (!feof(fp)) {
		if (fread(&from,sizeof(account),1,fp)==1) {
			count++;
		} else {
			break;
		}
	}
	rewind(fp);
	
	if (count<2) {
		printf("Not enough accounts available, cannot make any transfer\n");
		returnToMenu();
	}
	
	do {
		printf("Which account to use? (%d accounts available)\n", count);
		printf("Account from: ");
		scanf("%50s", buffer);
		userFrom=strtol(buffer,&end,10);
		printf("Account to: ");
		scanf("%50s", buffer);
		userTo=strtol(buffer,&end,10);
	} while (userTo<1 || userFrom<1 || userTo>count || userFrom> count || userFrom==userTo);
	
	printf("Will transfer from account %d to account %d:\n",userFrom,userTo);
	
	fseek(fp,(userFrom-1)*sizeof(account),SEEK_SET);	
	fread(&from,sizeof(account),1,fp);
	fseek(fp,(userTo-1)*sizeof(account),SEEK_SET);	
	fread(&to,sizeof(account),1,fp);
	
	currentBalanceFrom=from.balance;
	currentBalanceTo=to.balance;
	
	printf("Current balance for account %d is %.2f\n", userFrom, currentBalanceFrom);
	printf("Current balance for account %d is %.2f\n", userTo, currentBalanceTo);
	
	do {
		printf("What amount you wish to transfer? ");
		scanf("%50s",buffer);
		amount=strtod(buffer, NULL);
		if (amount>currentBalanceFrom)
			printf("Too much money requested.\n");
	} while (amount<0 || amount>currentBalanceFrom);
	
	printf("\n");
	do {
		printf("Are you sure you want to transfer %.2f from account %d to account %d? (y/n): ", amount,userFrom,userTo);
		scanf("%50s",buffer);
	} while (buffer[0]!='y' && buffer[0]!='n');
	
	if (buffer[0]=='y') {
		printf("\nCommiting transfer of %.2f from account %d to account %d... ",amount,userFrom,userTo);
		/* FILE CHANGE GOES HERE */
		
		from.balance=currentBalanceFrom-amount;
		to.balance=currentBalanceTo+amount;
		fseek(fp,(userFrom-1)*sizeof(account),SEEK_SET);	
		fwrite(&from,sizeof(account),1,fp);
		fseek(fp,(userTo-1)*sizeof(account),SEEK_SET);	
		fwrite(&to,sizeof(account),1,fp);
		
		fclose(fp);
		printf("done.\n\n");
		
		returnToMenu();
	} else {
		fclose(fp);
		returnToMenu();
	}
}

void makeWithdrawal() {
	int count=0,user;
	char buffer[10];
	char *end;
	float currentBalance,amount;
	FILE *fp;
	account read;
	
	system("clear");
	
	fp=fopen("accounts.dat","r+b");
	if (!fp) {
		perror("Failed to open the file");
		return;
	}
	
	/* see how many accounts there are in file */
	while (!feof(fp)) {
		if (fread(&read,sizeof(account),1,fp)==1) {
			count++;
		} else {
			break;
		}
	}
	rewind(fp);
	
	if (count<1) {
		printf("No accounts available, cannot make any withdrawal\n");
		returnToMenu();
	}
	
	do {
		printf("Which account to use? (%d accounts available): ", count);
		scanf("%10s", buffer);
		user=strtol(buffer,&end,10);
	} while (user<1 || user>count);
	
	printf("Using account %d:\n",user);
	
	fseek(fp,(user-1)*sizeof(account),SEEK_SET);	
	fread(&read,sizeof(account),1,fp);
	currentBalance=read.balance;
	
	printf("Current balance for account %d is %.2f\n", user, currentBalance);
	
	do {
		printf("What withdrawal you wish to make? ");
		scanf("%50s",buffer);
		amount=strtod(buffer, NULL);
		if (amount>currentBalance)
			printf("Too much money requested\n");
	} while (amount<0 || amount>currentBalance);
	
	printf("\n");
	do {
		printf("Are you sure you want to make a withdrawal of %.2f? (y/n): ",amount);
		scanf("%10s",buffer);
	} while (buffer[0]!='y' && buffer[0]!='n');
	
	if (buffer[0]=='y') {
		printf("\nCommiting withdrawal of %.2f from account %d... ",amount,user);
		/* FILE INSERT GOES HERE */
		read.balance=currentBalance-amount;
		fseek(fp,(user-1)*sizeof(account),SEEK_SET);
		fwrite(&read,sizeof(account),1,fp);
		fclose(fp);
		printf("done.\n\n");
		
		returnToMenu();
	} else {
		fclose(fp);
		returnToMenu();
	}
}

void handleChoice(int choice) {
	system("clear");
	switch (choice) {
		case 1:
			createAccount();
		break;
		case 2:
			printAllAccounts();
		break;
		case 3:
			findAccount();
		break;
		case 4:
			makeTransfer();
		break;
		case 5:
			makePayment();
		break;
		case 6:
			makeWithdrawal();
		break;
		case 0:
			printf("Quitting...\n");
			return;
		break;
	}
}

int main(int argc, char* argv[]) {	
	
	initFile();	
	handleChoice(menu());	
	
	return 0;
}


