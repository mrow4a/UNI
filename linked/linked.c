#include <math.h>
#include <stdio.h>  
#include <stdlib.h>   
#include <stdio_ext.h> 

/*
#define FINISH 2

void *my_malloc(size_t size)
{
	static int finish = 0;
	if (finish++ == FINISH)
		return NULL;
	return malloc(size);
}
#define malloc my_malloc


void *my_calloc(size_t count, size_t size)
{
	static int finish = 0;
	if (finish++ == FINISH)
		return NULL;
	return calloc(count, size);
}
#define calloc my_calloc

*/

struct my_struct
{
	char *data;
	struct my_struct *next;  
};

typedef struct my_struct Node;

Node * init(Node *head, Node *first, Node *second)
{
	head=malloc(sizeof(Node));
	first=malloc(sizeof(Node));
	second=malloc(sizeof(Node));

	head->data="ALA";
	head->next=first;
	first->data="ELA";
	first->next=second;
	second->data="OLA";
	second->next=NULL;
	return head;
}

int main(int argc, char* argv[])
{
	Node *init();
	Node *push();
	char* pop();
	Node *poplist();
	Node *sort();
	Node *destroy();
	Node *append();
	Node *copy();
	Node *reverse();
	void display();
	int count();
	Node *head=NULL;
	Node *first=NULL;
	Node *second=NULL;
	Node *newhead=NULL;
	Node *new=NULL;
	int counter;
	char *element;
	



	printf("====LINKED LIST====\n\n");

	printf("=INIT AND COUNT=\n");
	head=init(head,first,second);
	counter=count(head);
	printf("%d",counter);

	printf("\n\n=DISPLAY=\n");
	display(head);

	printf("\n\n=PUSH NEW ELEMENT AND DISPLAY=\n");
	element="WIOLA";
	head=push(head,element);
	display(head);

	printf("\n\n=POP ELEMENT AND LIST=\n");
	element=pop(head);
	printf("%s\n",element);
	head=poplist(head);
	display(head);

	printf("\n\n=DESTROY LIST=\n");
	head=destroy(head);
	display(head);

	printf("\n\n=INITIALIZES OLD LIST AND DISPLAY=\n");
	head=init(head,first,second);
	display(head);

	printf("\n\n=APPEND NEW ELEMENT AND DISPLAY=\n");
	element="WIOLA";
	head=append(head,element);
	display(head);

	printf("\n\n=COPY LIST AND DISPLAY=\n");
	newhead=copy(head, newhead, new);

	printf("\n\n=REVERSE COPIED LIST AND DISPLAY BOTH=\n");
	newhead=reverse(newhead);
	display(newhead);

	printf("\n\n=SORTED COPIED LIST AND DISPLAY BOTH=\n");
	newhead=sort(newhead);
	display(newhead);

	printf("\n\n=ENDING, ALL MEMORY DELETED=\n");
	newhead=destroy(newhead);
	head=destroy(head);
	display(newhead);
	printf("\n");
	display(head);
	
	printf("\n");
	return 0;
}



void display(Node *head)
{
	Node *current=head;
	if(head==NULL) { printf("List is empty"); }

	while(current!=NULL)
	{
		printf("%s\t",current->data);
		current=current->next;
	}
	
}

int count(Node *head)
{
	int counter=0;
	Node *current=head;

	while(current!=NULL)
	{
		counter++;
		current=current->next;
	}
	return counter;
}

Node * push(Node *head,char *element)
{
	Node *newnode=NULL;
	
	newnode=malloc(sizeof(Node));
	
	newnode->data=element;
	newnode->next=head;
	
	return newnode;
}

char* pop(Node *head)
{
	Node *current=head;
	int counter;
	char *element;
	
	counter=count(head);
	if(counter==0)	{ return NULL; }
	if(counter==1) 	{ element=head->data; return element; } 
	while(current->next!=0)
	{
		current=current->next;
	}
	element=current->data;
	
	return element;
}

Node * poplist(Node *head)
{
	Node *current=head;
	int counter,i;
	
	counter=count(head);
	if(counter==1) {  free(head); return head; }
	if(counter==2) {  current=head->next; head->next=NULL; free(current); return head; }
	current=head->next;
	while(current->next!=NULL)
	{
		current=current->next;
	}
	free(current);
	current=head->next;
	for(i=3;i<=counter;i++)
	{
		if(i==counter) { current->next=NULL; }
		current=current->next;
	}
	
	return head;
}

Node * destroy(Node *head)
{
	Node *current=head;
	
	while(head!=NULL)
	{
		current=head->next;
		free(head);
		head=current;
	}
	
	return head;
}

Node * append(Node *head, char *element)
{
	Node *newnode=NULL;
	Node *current=head;
	newnode=malloc(sizeof(Node));
	
	while(current->next!=NULL)
	{
		current=current->next;
	}
	current->next=newnode;
	newnode->data=element;
	newnode->next=NULL;
	
	return head;
}



Node * reverse(Node *head)
{
	Node * temp;
    	Node * prev = NULL;
    	while(head!= NULL) 
	{
		temp = head->next;
		head->next = prev;
		prev = head;
		head = temp;
    	}
	return prev;
}

Node *copy(Node *head, Node *newhead, Node *new)
{
	Node *current=head;
	char *element;
	int i,counter;

	counter=count(head);
	newhead=malloc(sizeof(Node));
	while(current->next!=0)
	{
		current=current->next;
	}
	element=current->data;
	newhead->data=element;
	newhead->next=new;
	while(counter>=1)
	{
		current=head;
		for(i=2;i<counter;i++)
		{
			current=current->next;
		}
		element=current->data;
		if(counter>1) { newhead=push(newhead,element); }
		counter--;
	}
	display(newhead);
	return newhead;
}

Node *sort(Node *head)
{
	Node *current=head;
	Node *check=head;
	char *tmp;
	int convert();
	int t=0;
	
	check=head->next;
	
	do
	{
		if( convert(current->data)>convert(check->data) ) 
		{ 
			tmp=current->data; 
			current->data=check->data; 
			check->data=tmp; 
			if(check->next==NULL) { current=current->next; check=current->next; }
			else { check=check->next; }
		}
		
		if(current->next==NULL) { break; }	
	}while(t);
	
	return head;
}

int convert(char *element)
{
	char l;
	
	l=*element;
	return l;
}






