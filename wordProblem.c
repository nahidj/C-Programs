/*
A program to solve the coding challenge. 
INPUT: 2 words, firstWord and secondWord, of equal length x, and a wordlist containing words of length x.
OUTPUT: A sequence of words from the wordlist, that begins with firstWord and ends with secondWord, 
and such that each word differs from the previous word in exactly 1 character only.
*/  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

/*
Implementation of a FIFO Queue for Breadth First Search.
Elements are enqueued at the tail and dequeued at the head.
*/
 
struct queue{
	struct node* head;
	struct node* tail;
};

struct node{
	int wordId;
	struct node* next;
};

struct queue* createQueue(){
	struct queue* newQueue = malloc(sizeof(struct queue));
	newQueue->head = NULL;
	newQueue->tail = NULL;
	return newQueue;
}

struct queue* enqueue(struct queue* q, int x){
	struct node* newNode = malloc(sizeof(struct node));
	newNode->wordId = x;
	newNode->next = NULL;

	if(q->tail){
		q->tail->next = newNode;}
	else{
		q->head = newNode;}

	q->tail = newNode;

	//printf("\n Head is %d, tail is %d,", q->head->wordId, q->tail->wordId);
	return q;
}

//As our nodes will contain positive integers only, we return -1 if the queue is empty.
int dequeue(struct queue* q){
	if(!q->head){
		return -1;}
	int x = q->head->wordId;
	struct node* temp = q->head;
	if(q->head == q->tail){
		q->tail = NULL;}
	q->head = q->head->next;
	free(temp); 
	return x;
}

void deleteQueue(struct queue* q){
	while(q->head){
		dequeue(q);}
	
	free(q);
}

void printQueue(struct queue* q)
{
	printf("\nPrinting Queue: ");
	struct node* temp = q->head;
	while(temp){
		printf("%d ",temp->wordId);
		temp = temp->next;
	}
}


/*
   Function to check if 2 strings differ in exactly 1 character.
   Inputs: strings x and y
   Output: true if x and y differ in 1 character, false otherwise
*/
bool stringsDifferByOneChar(char* x, char* y){
	int n = strlen(x);
	int count = 0;
	for(int i=0; i<n; i++){
		if(x[i]!=y[i]){
			count++;
			if(count>1){
				return 0;
			}
		}
	}
	if(count==0){return 0;}

	return 1;
}

/* 
   Function that implements classical Breadth First Search.  
   Note: A modified version of the below function that halts once the destination node is found would have also sufficed.
   Inputs: number of vertices in graph, edges in graph, source vertex integer, dest vertex integer
   Output: the predecessor graph
*/
int* BreadthFirstSearch(int numVertices, bool** edges, int source, int dest){

	/*0:unvisited, 1:visited but has unvisited adjacent nodes, 2:visited and no unvisited adjacent nodes*/ 
	int visited[numVertices];

	int* predecessor = malloc(sizeof(int)* numVertices);

	for(int i=0; i<numVertices; i++){
		visited[i] = 0;
		predecessor[i] = -1;
	}
	
	struct queue* q = createQueue();
	
	visited[source] = 1;
	enqueue(q,source);

	while(q->head)
	{
		int x = dequeue(q);
		
		for(int i=0; i<numVertices;i++){
			
			if(edges[x][i]==1 && visited[i]==0){
				visited[i] = 1;
				predecessor[i] = x;
				enqueue(q,i);
			}
			
		}

		visited[x] = 2;
	}
	
	deleteQueue(q);
	return predecessor;
}


int main(){

/*Note: the longest words in all present day English dictionaries have less than 50 characters.*/
char firstWord[50];
char secondWord[50];
int wordListSize;

/*Get the inputs, i.e. 2 words and a wordlist, and validate them.*/
printf("\nEnter first word: ");
scanf("%s", firstWord);

printf("\nEnter second word: ");
scanf("%s", secondWord);

if(strlen(firstWord)!=strlen(secondWord)){
	printf("\nError: The two words must be of equal length.");
	return 0;
}

printf("\nHow many words are there in the wordlist?");
scanf("%d", &wordListSize);

char **wordList;
wordList = malloc(wordListSize * sizeof(char*));

for(int i=0; i<wordListSize; i++){
	wordList[i] = (char*)malloc(sizeof(char)*50);
}

int firstWordPos=-1;
int secondWordPos=-1;

for(int i=0; i<wordListSize; i++){
	printf("\nEnter word no. %d = ", i);
	scanf("%s",wordList[i]);
	//printf("\nWord %d is %s", i, wordList[i]);
	
	if(strlen(wordList[i])!=strlen(firstWord)){
		printf("\nError: All words in the wordlist should be the same length as the 2 words input earlier");
		return 0;	
	}

	if(firstWordPos<0 && !strcmp(firstWord,wordList[i])){
		firstWordPos=i;}
	
	if(secondWordPos<0 && !strcmp(secondWord,wordList[i])){
		secondWordPos=i;}
}

if(firstWordPos<0 || secondWordPos<0){
	printf("\nError: The wordlist should contain the two words input earlier.");
	return 0;
}

/*Special case: firstWord and secondWord are identical*/
if(!strcmp(firstWord,secondWord)){
	printf("\nSolution: %s", firstWord);
	return 1;
}

/*Form an unweighted, undirected graph whose nodes are the words in the wordlist. 
An edge exists between all pairs of words that differ in a single character.*/
bool *edges[wordListSize]; 
for (int i=0; i<wordListSize; i++){
         edges[i] = (bool*)malloc(wordListSize * sizeof(bool));}

for(int i=0; i<wordListSize; i++){

	for(int j=0;j<wordListSize;j++){
		if(stringsDifferByOneChar(wordList[i],wordList[j])){
			edges[i][j]=1;
			edges[j][i]=1;
		}
		else{
			edges[i][j]=0;
			edges[j][i]=0;
		}
	}
}

/*Find a path from secondWord to firstWord. 
This is equivalent to finding a path from firstWord to secondWord and makes printing of the path easier.*/  
int* predecessor = BreadthFirstSearch(wordListSize,edges,secondWordPos,firstWordPos);

/*Print the path from firstWord to secondWord.*/
int y = predecessor[firstWordPos];

if(y<0){
	printf("\nNo solution exists");}

printf("\n Solution: %s, ", firstWord);

while(y>0){
	printf("%s, ", wordList[y]);
	y = predecessor[y];
}

/*Free up dynamically allocated memory*/
for(int i=0; i<wordListSize;i++){
	free(edges[i]);
	free(wordList[i]);
}

free(wordList);
free(predecessor);

return 1;
}


