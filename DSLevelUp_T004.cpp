#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int total_user=0;

void cls(){
	for(int i=0; i<45; i++) printf("\n");
}

void pause(){
	printf("Press enter to continue..\n");
	getchar();
}

struct trans{
	char name[32];
	char item_name[32];
	char category[32];
	long price;
	long qty;
	struct trans *next;
	struct trans *prev;
}*t_head = NULL, *t_tail = NULL;

void priority_queue_push(char name[], char item_name[], char category[], long price, long qty){
	struct trans *curr, *temp;
	curr = (struct trans*)malloc(sizeof(struct trans));
	
	strcpy(curr->name, name);
	strcpy(curr->item_name, item_name);
	strcpy(curr->category, category);
	curr->price = price;
	curr->qty = qty;
	curr->next = NULL;
	
	if (t_head == NULL)
	{
		t_head = curr;
	}
	else if (strcmp(t_head->name, name) >= 0)
	{
		curr->next = t_head;
		t_head = curr;
	}
	else
	{
		temp = t_head;
		while(temp->next != NULL && strcmp(temp->next->name, name) <= 0)
		{
			temp = temp->next;
		}
		curr->next = temp->next;
		temp->next = curr;
	}
}

unsigned long hash(char *str);

struct user{
	char id[7];
	char username[40];
	char phone_number[14];
	char address[14];
	int balance;
	user *next;
}*head[100];

struct user *createNewNode(char id[], char username[], char phone_number[], char address[], int balance){
	user *newNode = (user*)malloc(sizeof(user));
	
	strcpy(newNode->id, id);
	strcpy(newNode->username, username);
	strcpy(newNode->phone_number, phone_number);
	strcpy(newNode->address, address);
	newNode->balance = balance;
	newNode->next = NULL;
	
	return newNode;
}

int push(char id[], char username[], char phone_number[], char address[], int balance){
	int idx = hash(id);
	struct user *newNode = createNewNode(id, username, phone_number, address, balance);
	if(head[idx] == NULL) head[idx] = newNode;
	else{
		user *temp = head[idx];
		while(temp->next != NULL ) temp = temp->next;
		temp->next = newNode;
	}
}

unsigned long hash(char *str){
	unsigned long hash = 5381;
    int h;
	int i=0;
    while ( h = str[i++] ) hash = ((hash << 5) + hash) + h; 	
    return hash % 100;
}

void admin_page_menu(){
	printf("Welcome, Admin,\n");
	printf("+-------------+\n");
	printf("1. Register a Customer\n");
	printf("2. View Transactions\n");
	printf("3. View Customers\n");
	printf("4. Exit\n");
	printf(">> ");
}

void genereate_id(char id[]){
	strcpy(id, "AAAAA");
	id[4] = id[4] + total_user%26;
}

int validate_uname(char uname[]){
	for(int i=0; i<100; i++){
		if(head[i] != NULL){
			user *temp = head[i];
			while(temp != NULL){
				if(strcmp(temp->username, uname)==0) return 0;
				temp = temp->next;
			}
		}
	}
	return 1;	
}

void register_a_customer(){
	char username[50], phone_number[14], address[83];
	long balance=0;
	do{
		printf("Input UserName(not empty|unique): ");
		scanf("%[^\n]", username); getchar();
		if(validate_uname(username) == 1 && strlen(username) > 0) break;
	}while(1);
	do{
		printf("Input Phone Number(10-12 length): ");
		scanf("%[^\n]", phone_number); getchar();
	}while(strlen(phone_number) < 10 || strlen(phone_number) > 12);
	do{
		printf("Input Address(8-80): ");
		scanf("%[^\n]", address); getchar();
	}while(strlen(address) < 8 || strlen(address) > 80);
	do{
		printf("Input User Balance(1000-800000000) ");
		scanf("%ld", &balance); getchar();
	}while(balance < 1000 || balance > 800000000);
	
	char id[7];
	genereate_id(id);
	
	// insert customer
	push(id, username, phone_number, address, balance);
	
	total_user++;
	printf("Register success\n");
}

void view_transactions(){
	struct trans *curr = t_head;
	int i = 1;
	
	printf("+======================================================================================+\n");
	printf("| %-20s | %-20s | %-15s | %-12s | %-5s |\n", "UserName", "Item Name", "Category", "Price", "Qty");
	printf("+======================================================================================+\n");
	
	while(curr){
		printf("| %-20s | %-20s | %-15s | %-12ld | %-5ld |\n", curr->name, curr->item_name, curr->category, curr->price, curr->qty);
		curr = curr->next;
		i++;
	}
	printf("+======================================================================================+\n");
}

void view_customers(){
	printf("+=============================================================================================================+\n");
	printf("| %-5s | %-25s | %-13s | %-40s | %-12s |\n", "ID", "UserName", "PhoneNumber", "Address", "Balance");
	printf("+=============================================================================================================+\n");
	int flag =0;
	for(int i=0; i<100; i++){
		if(head[i] != NULL){
			user *temp = head[i];
			while(temp != NULL){
				flag=1;
				printf("| %-5s | %-25s | %-13s | %-40s | %-12d |\n", temp->id, temp->username, temp->phone_number, temp->address, temp->balance);
				temp = temp->next;
			}
		}
	}
	if(flag == 0) 
		printf("| %-11s  %-25s  %-13s  %-40s %-12s|\n", "No Data", " ", " ", " ", " ");
	printf("+=============================================================================================================+\n");
	
}

void admin_page(){
	int menu=0;
	do{
		cls();
		admin_page_menu();
		scanf("%d", &menu); getchar();
		if(menu==1) register_a_customer();
		else if(menu==2) view_transactions();
		else if(menu==3) view_customers();
		else if(menu==4) break;
		pause();
	}while(1);
}

void admin_menu(){
	char password[30];
	printf("Input password, admin: ");
	scanf("%[^\n]", password); getchar();
	if(strcmp(password, "admin") == 0) admin_page();
}

void do_transaction(){
	if(total_user <= 0){
		printf("\n\nNo user, register first!\n");
		return;
	}
	char username[50], item_name[70], category[50];
	long qty=0, price=0;
	do{
		printf("Input UserName(not empty|must be registered UsernName): ");
		scanf("%[^\n]", username); getchar();
		// validate uname
		if(validate_uname(username) == 0) break;
	}while(1);
	do{
		printf("Input item Name(5-60 length): ");
		scanf("%[^\n]", item_name); getchar();
	}while(strlen(item_name) < 5 || strlen(item_name ) > 60);
	do{
		printf("Input item Category(Food And Beverages|Electronics|Clothes|Furniture)(Case Sensitive): ");
		scanf("%[^\n]", category); getchar();
		if(strcmp(category, "Food And Beverages") == 0 || strcmp(category, "Electronics") == 0 ||
		strcmp(category, "Clothes") == 0 ||strcmp(category, "Furniture") == 0)
			break;
	}while(1);
	do{
		printf("Input Item Price(1000-100000000): ");
		scanf("%ld", &price); getchar();
	}while(price < 1000 || price > 100000000);
	do{
		printf("Input Item Qty(1-100): ");
		scanf("%ld", &qty); getchar();
	}while(qty < 1 || qty > 100);
	
	long total = (price * qty);
	printf("Total price will be Rp. %ld\n", total);
	
	char confirm[10];
	do{
		printf("Confirm this transaction(y/n)? ");
		scanf("%s", confirm);
		if(strcmp(confirm, "y")==0) break;
		if(strcmp(confirm, "n")==0) return;
	}while(1);
	
	// insert transactions
	priority_queue_push(username, item_name, category, price, qty);
}

void print_menu(){
	printf("Welcome to JP. ID\n");
	printf("+===============+\n");
	printf("1. Admin Menu\n");
	printf("2. Do transaction\n");
	printf("3. Exit\n");
	printf(">> ");
}

void main_logo(){
	printf("     _      ____                ____\n");
	printf("  U |\"| u U|  _\"\\ u     ___    |  _\"\\\n");
	printf(" _ \| |/  \| |_) |/    |_\"_|  /| | | |\n");
	printf("| |_| |_,-.|  __/       | |   U| |_| |\\\n");
	printf(" \\___/-(_/ |_|   _    U/| |\\u  |____/ u\n");
	printf("  _//      ||>>_(\").-,_|___|_,-.|||_\n");
	printf(" (__)     (__)__)\"  \_)-' '-(_/(__)_)\n");
	printf("\n");
}

void program_first_run(){
	main_logo();
	pause();
}

void exit_program(){
	printf("\n\nIN HONESTY AND HARDWORK, WE STRIVE FOR EXCELLENCE\n");
	pause();
}

int main(){
	program_first_run();
	int menu=0;
	do{
		cls();
		print_menu();
		scanf("%d", &menu); getchar();
		if(menu==1) admin_menu();
		else if(menu==2) do_transaction();
		else if(menu==3){
			exit_program();	
			break;
		} 
		pause();
	}while(1);
	return 0;
}
