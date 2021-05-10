//preproccessor directives
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_DEPTH 12

//Type definitions

//huffman tree node
typedef struct tree
{
	char character;
	int frequency;
	struct tree* left;
	struct tree* right;
}tree;

//queue
typedef struct queue
{
	struct tree* node;
	struct queue* next;
}queue;

//priority queue
typedef struct PriorityQueue
{
	struct queue* Queue;
	int size;
}PriorityQueue;

//Huffman codes list node
typedef struct HuffmanCodes
{
	char character;
	int* code;

}HuffmanCodes;

//Huffman codes linked list
typedef struct HuffmanCodesList
{
	struct HuffmanCodes* data;
	struct HuffmanCodesList* next;
}HuffmanCodesList;

//Function prototypes

//priority queue functions
PriorityQueue* InitiateQueue();
PriorityQueue* enqueue(PriorityQueue* head, tree* data);
PriorityQueue* dequeue(PriorityQueue* head);
tree* peek(PriorityQueue* head);
PriorityQueue* TerminateQueue(PriorityQueue* head);

//huffman tree functions
tree* BuildHuffmanTree(PriorityQueue* Frequencies); //takse frequencies priority queue and creates huffman tree
void TerminateTree(tree* root);

//Huffman codes linked list functions
HuffmanCodesList* InitaiteList();
HuffmanCodes* InitiateNode();
HuffmanCodes* TerminateNode(HuffmanCodes* head);
HuffmanCodesList* add(HuffmanCodesList* head, HuffmanCodes* data);
HuffmanCodes* searchwithCharacter(HuffmanCodesList* head, char character);
HuffmanCodes* searchwithcode(HuffmanCodesList* head, int* code);
HuffmanCodesList* Terminatelist(HuffmanCodesList* head);

//Huffman encoding functions
PriorityQueue* readfile(PriorityQueue* head, FILE *ptr); //opens .txt file and stores the characters frequencies in a priority queue
HuffmanCodesList* savecodes(tree* haffmantree, HuffmanCodesList** list); //takes huffman tree and stores the huffman codes in a linked list
int getcodes(tree* root, int arr[MAX_DEPTH], int top, HuffmanCodesList** head); //wrapper function for savecodes()
int printcodes(HuffmanCodesList* codes); //prints the huffman codes on the screen
int HuffmanCodesToFile(FILE* write, HuffmanCodesList* codes, int bits); //saves the huffman codes in a .cod file
int* PrinfHuffmanString(FILE* read, HuffmanCodesList* codes,int* NumberOfBits); //prints huffman string on the screen and returns the huffman string
int HuffmanStringToFile(int* HuffmanString, FILE* write, int NumberOfBits); //saves huffman string in a .com file


//huffman decoding functions
HuffmanCodesList* FileToHuffmanCodes(FILE* read, HuffmanCodesList** codes,int* bits , int* overflow); //reads .cod file and returns huffman codes
int* FileToHuffmanString(FILE* read, int overflow, int NumberOfBits); //reads .com file and returns huffman string
int DecodeHuffmanString(FILE* write, int* HuffmanString, HuffmanCodesList* codes, int overflow, int NumberOfBits); //decodes the huffman string using the huffman codes and restores the original .txt file

//utility functions
int power(int x, int y);
int BinaryToDecimal(int binary[8]);
int DecimalToBinary(int Decimal, int binary[8]);

int main()
{
	int selection;
	char path_read[100], path_write_codes[100], path_write_Huffman_String[100], path_write_text[100],stopper;
	int* HuffmanString;
	int length;
	int overflow = 0;
	int NumberOfBits = 0;
	FILE* read = NULL;
	FILE* write_codes = NULL;
	FILE* write_Huffman_String = NULL;
	FILE* write_txt = NULL;
	PriorityQueue* Nodes = InitiateQueue();
	tree* HuffmanTree = NULL;
	HuffmanCodesList* codes = InitaiteList();
	while (1)
	{
		printf("\nplease select an operation :-\n1) compress file.\n2) decompress file\n3) exit\nenter selection >> ");
		scanf("%d", &selection);
		if (selection == 1)
		{
			stopper = getchar();
			printf("\nenter the relative .txt file path >> ");
			gets_s(path_read, 100);
			strcpy(path_write_codes, path_read);
			strcpy(path_write_Huffman_String, path_read);
			length = strlen(path_read);
			for (int i = 0; i < length; i++)
				if (path_read[i] == '.')
				{
					path_write_codes[i + 1] = 'c';
					path_write_codes[i + 2] = 'o';
					path_write_codes[i + 3] = 'd';
					path_write_Huffman_String[i + 1] = 'c';
					path_write_Huffman_String[i + 2] = 'o';
					path_write_Huffman_String[i + 3] = 'm';
					break;
				}
			read = fopen(path_read, "r");
			write_codes = fopen(path_write_codes, "w");
			write_Huffman_String = fopen(path_write_Huffman_String, "w");
			Nodes = readfile(Nodes, read);
			HuffmanTree = BuildHuffmanTree(Nodes);
			codes = savecodes(HuffmanTree, &codes);
			TerminateTree(HuffmanTree);
			printcodes(codes);
			HuffmanString = PrinfHuffmanString(read, codes, &NumberOfBits);
			HuffmanCodesToFile(write_codes, codes, NumberOfBits);
			HuffmanStringToFile(HuffmanString, write_Huffman_String, NumberOfBits);
			free(HuffmanString);
			fclose(read);
			fclose(write_codes);
			fclose(write_Huffman_String);
			printf("\nCompression sucessfull !!!\n .com file name : %s\n.cod file name : %s",path_write_Huffman_String,path_write_codes);
		}
		else if (selection == 2)
		{
			stopper = getchar();
			printf("\nenter the relative path if the .cod file that contains the huffman codes >> ");
			gets_s(path_read, 100);
			read = fopen(path_read, "r");
			length = strlen(path_read);
			strcpy(path_write_text, path_read);
			for (int i = 0; i < length; i++)
				if (path_read[i] == '.')
				{
					path_write_text[i + 1] = 't';
					path_write_text[i + 2] = 'x';
					path_write_text[i + 3] = 't';
					break;
				}
			codes = FileToHuffmanCodes(read, &codes,&NumberOfBits,&overflow);
			fclose(read);
			printcodes(codes);
			printf("\nenter the relative path if the .com file that contains the path to the huffman string >> ");
			gets_s(path_read, 100);
			read = fopen(path_read, "r");
			HuffmanString = FileToHuffmanString(read, overflow, NumberOfBits);
			fclose(read);
			write_txt = fopen(path_write_text, "w");
			DecodeHuffmanString(write_txt, HuffmanString, codes, overflow, NumberOfBits);
			free(HuffmanString);
			codes = Terminatelist(codes);
			fclose(write_txt);
			printf("\nDecompression sucessfull !!!\n .txt file name : %s", path_write_text);
		}
		else if (selection == 3)
			return 0;
		else
		{
			printf("\nPlease enter a valid input");
			do
			{
				stopper = getchar();
			} while (stopper);
		}

	}
	return 0;
}


PriorityQueue* InitiateQueue()
{
	PriorityQueue* Nodes = (PriorityQueue*)malloc(sizeof(PriorityQueue));
	Nodes->Queue = NULL;
	Nodes->size = 0;
	return Nodes;
}




PriorityQueue* enqueue(PriorityQueue* head, tree* data)
{
	queue* itr = head->Queue;
	queue* node = (queue*)malloc(sizeof(queue));
	node->node = data;
	node->next = NULL;
	if (itr == NULL)
	{
		head->size = 1;
		head->Queue = node;
		return head;
	}

	if(itr->next == NULL)
		if (data->frequency < itr->node->frequency)
		{
			node->next = itr;
			head->Queue = node;
			head->size = 2;
			return head;
		}
		else
		{
			itr->next = node;
			head->size = 2;
			return head;
		}

	while (itr->next->next != NULL)
		if (itr->node->frequency > data->frequency)
		{
			node->next = itr->next;
			itr->next = node;
			head->size++;
			return head;
		}
		else
			itr = itr->next;
	node->next = NULL;
	itr->next->next = node;
	head->size++;
	return head;
}

PriorityQueue* dequeue(PriorityQueue* head)
{
	if (head->Queue == NULL)
	{
		head->size = 0;
		return NULL;
	}
	head->Queue = head->Queue->next;
	head->size--;
	return head;
}

tree* peek(PriorityQueue* head)
{
	return head->Queue->node;
}

PriorityQueue* TerminateQueue(PriorityQueue* head)
{
	if (head->Queue == NULL)
	{
		head->size = 0;
		return head;
	}
		
	queue* tmp = head->Queue;
	head->Queue = head->Queue->next;
	head->size--;
	free(tmp);
	return TerminateQueue(head);
}


HuffmanCodesList* InitaiteList()
{
	HuffmanCodesList* codes = (HuffmanCodesList*)malloc(sizeof(HuffmanCodesList));
	codes->data = NULL;
	codes->next = NULL;
	return codes;
}

HuffmanCodes* InitiateNode()
{
	HuffmanCodes* node = (HuffmanCodes*)malloc(sizeof(HuffmanCodes));
	node->code = (int*)malloc(sizeof(int)* MAX_DEPTH);
	return node;
}

HuffmanCodes* TerminateNode(HuffmanCodes* head)
{
	free(head->code);
	free(head);
	return NULL;
}

HuffmanCodesList* add(HuffmanCodesList* head, HuffmanCodes* data)
{

	if (head->data == NULL)
	{
		head->data = data;
		return head;
	}
	HuffmanCodesList* tmp = (HuffmanCodesList*)malloc(sizeof(HuffmanCodesList));
	tmp->data = data;
	tmp->next = NULL;
	HuffmanCodesList* itr = head;
	while (itr->next != NULL)
		itr = itr->next;
	itr->next = tmp;
	return head;
}

HuffmanCodes* searchwithCharacter(HuffmanCodesList* head,char character)
{
	HuffmanCodesList* itr = head;
	HuffmanCodes* out = InitiateNode();
	while (itr != NULL)
		if (itr->data->character == character)
		{
			out->character = character;
			for (int i = 0; i < MAX_DEPTH; i++)
				out->code[i] = itr->data->code[i];
			return out;
		}
		else
			itr = itr->next;
	return NULL;
}

HuffmanCodes* searchwithcode(HuffmanCodesList* head, int* code)
{
	HuffmanCodesList* itr = head;
	int flag = 1;
	HuffmanCodes* out = InitiateNode();
	while (itr != NULL)
	{
		for (int i = 0; i < MAX_DEPTH; i++)
			if (itr->data->code[i] != code[i])
				flag = 0;
		if (flag == 1)
		{
			out->character = itr->data->character;
			for (int i = 0; i < MAX_DEPTH; i++)
					out->code[i] = itr->data->code[i];
			return out;
		}
		else
			itr = itr->next;
		flag = 1;
	}

	return NULL;
}

HuffmanCodesList* Terminatelist(HuffmanCodesList* head)
{
	if (head == NULL)
		return head;
	HuffmanCodesList* tmp = head;
	head = head->next;
	tmp->data = TerminateNode(tmp->data);
	return Terminatelist(head);
}




PriorityQueue* readfile(PriorityQueue* head, FILE* ptr){
	int ASCII[255];
	tree* input;
	for (int i = 0; i < 255; i++)
		ASCII[i] = 0;
	for (char character = fgetc(ptr); character != EOF; character = fgetc(ptr))
		ASCII[character]++;
	for (int i = 0; i < 255; i++)
		if (ASCII[i] > 0)
		{
			input = (tree*)malloc(sizeof(tree));
			input->character = (char)i;
			input->frequency = ASCII[i];
			input->left = NULL;
			input->right = NULL;
			head = enqueue(head, input);
		}
			
	return head;
}


tree* BuildHuffmanTree(PriorityQueue* Frequencies)
{
	tree* result = NULL;
	//extract the last remaining huffman tree from the priority queue when all the subtrees have been attached together
	if (Frequencies->size == 1)
	{
		result = peek(Frequencies);
		Frequencies->Queue->node = NULL;
		free(Frequencies);
		return result;
	}
	result = (tree*)malloc(sizeof(tree));
	//dequeue the 2 subtrees with lowest frequencies
	tree* node1 = peek(Frequencies);
	dequeue(Frequencies);
	tree* node2 = peek(Frequencies);
	dequeue(Frequencies);
	//add them together to a parent node
	result->left = node1;
	result->right = node2;
	result->frequency = node1->frequency + node2->frequency; //parent frequency = child 1 + child 2
	result->character = NULL;
	//enqueue the parent node and reccur
	enqueue(Frequencies, result);
	return BuildHuffmanTree(Frequencies);
}

void TerminateTree(tree* root)
{
	if (root == NULL)
		return;
	TerminateTree(root->left);
	TerminateTree(root->right);
	free(root);
	root = NULL;
}

HuffmanCodesList* savecodes(tree *haffmantree, HuffmanCodesList** list)
{	
	int arr[MAX_DEPTH], top = 0;
	for (int i = 0; i < MAX_DEPTH; i++)
		arr[i] = 2;
	getcodes(haffmantree, arr, top, list);
	return *list;
}

int getcodes(tree* root,int arr[MAX_DEPTH], int top,HuffmanCodesList** head)
{
	if (root->left != NULL) {
		// Assign 0 to left edge and recur
		arr[top] = 0;
		getcodes(root->left, arr,top + 1, head);
	}

	
	if (root->right != NULL) {
		// Assign 1 to right edge and recur
		arr[top] = 1;
		getcodes(root->right, arr,top + 1, head);
	}
	// Add character and huffman code to the huffman codes list
	if (root->right == NULL && root->left == NULL)
	{
		HuffmanCodes* node = InitiateNode();
		node->character = root->character;
		for (int i = 0; i < MAX_DEPTH; i++)
			node->code[i] = arr[i];
		*head = add(*head, node);
	}

}

int printcodes(HuffmanCodesList* codes)
{
	printf("\nHuffman codes:-");
	for (HuffmanCodesList* itr = codes; itr != NULL; itr = itr->next)
	{
		if(itr->data->character != '\n')
			printf("\ncharacter : %c | code : ", itr->data->character);
		else
			printf("\ncharacter : \\n | code : ");
		for (int i = 0; i < MAX_DEPTH; i++)
			if(itr->data->code[i] == 0 || itr->data->code[i] == 1)
				printf("%d", itr->data->code[i]);
	}
	return 1;
}


int HuffmanStringToFile(int* HuffmanString, FILE* write, int NumberOfBits)
{
	//write the huffman string to file 8 bits at a time
	printf("\n");
	int Buffer[8] = {2,2,2,2,2,2,2,2}; //buffer to store the 8 bits to be written
	// calculate buffer over flow
	int overflow = 8 - NumberOfBits % 8;  
	unsigned char character;
	int start = 0;
	int counter = 0;
	//write binary 0 to the file to adjust the number of bits to be divisable by 8
	if (overflow != 8)
	{
		for (int i = 0; i < overflow; i++)
			Buffer[i] = 0;
		for (start = overflow; start < 8; start++)
			Buffer[start] = HuffmanString[counter++];
		character = (unsigned char)BinaryToDecimal(Buffer);
		fprintf(write, "%c", character);
	}
	start = 0;
	//write the huffman string to the file 8 bits per loop
	for (int i = counter; i <= NumberOfBits + overflow; i++)
	{
		Buffer[start++] = HuffmanString[i];
		if (start % 8 == 0)
		{
			start = 0;
			character = (unsigned char)BinaryToDecimal(Buffer);
			fprintf(write, "%c", character);
		}
	}
	return 1;
}

int* PrinfHuffmanString(FILE* read, HuffmanCodesList* codes, int* NumberOfBits)
{
	printf("\n");
	int* buffer = NULL;
	int bufferCounter = 0;
	rewind(read);
	HuffmanCodes* tmp = NULL;
	//printf huffman string on the screen and return it as an array of integers
	for (char character = fgetc(read); !feof(read); character = fgetc(read))
	{
		tmp = searchwithCharacter(codes, character);
		for (int i = 0; tmp->code[i] == 1 || tmp->code[i] == 0; i++)
		{
			buffer = (int*)realloc(buffer, sizeof(int) * (bufferCounter + 1));
			buffer[bufferCounter++] = tmp->code[i];
			printf("%d", tmp->code[i]);
			*NumberOfBits = *NumberOfBits + 1;
		}
	}
	printf("\nNumber of bits : %d\n", *NumberOfBits);
	return buffer;
}

int HuffmanCodesToFile( FILE* write, HuffmanCodesList* codes,int bits)
{
	fprintf(write,"%d\n%d\n",bits,(bits%8 != 0)? 8 - bits%8 : 0);
	for (HuffmanCodesList* itr = codes; itr != NULL; itr = itr->next)
	{
		fprintf(write, "%c", itr->data->character);
		for (int i = 0; i < MAX_DEPTH; i++)
			if (itr->data->code[i] == 1)
				fprintf(write, "1");
			else if (itr->data->code[i] == 0)
				fprintf(write, "0");
		fprintf(write, "\n");
	}
	return 1;
}

HuffmanCodesList* FileToHuffmanCodes(FILE* read, HuffmanCodesList** codes, int* bits, int* overflow)
{
	rewind(read);
	int counter = 0;
	HuffmanCodes* tmp = (HuffmanCodes*)malloc(sizeof(HuffmanCodes));
	tmp->code = (int*)malloc(sizeof(int) * MAX_DEPTH);
	fscanf(read, "%d\n", bits);
	fscanf(read, "%d\n", overflow);
	for (char character = fgetc(read); character != EOF; character = fgetc(read))
	{
		if (counter == 0)
		{
			if(character != '~')
				tmp->character = character;
			else
				tmp->character = '\n';
		}		
		if (counter > 0)
			tmp->code[counter - 1] = character - 48;
		if (character == '\n' && counter != 0)
		{
			for (int i = counter - 1; i < MAX_DEPTH; i++)
				tmp->code[i] = 2;
			counter = 0;
			*codes = add(*codes, tmp);
			tmp = InitiateNode();
			continue;
		}
		counter++;
	}
	return *codes;
}

int power(int x, int y)
{
	if (y == 0)
		return 1;
	return x * power(x, y - 1);
}

int BinaryToDecimal(int binary[8])
{
	int x;
	int weigth = 7;
	int decimal = 0;
	for (int i = 0; i < 8; i++)
	{
		x = binary[i];
		if(binary[i] == 1)
			decimal += power(2, weigth);
		weigth--;
	}
	return decimal;
}

int DecimalToBinary(int Decimal,int Binary[8])
{
	for(int  i = 7; i >= 0;i--)
	{
		Binary[i] = Decimal % 2;
		Decimal = Decimal / 2;
	}

	return 1;
}

int* FileToHuffmanString(FILE* read, int overflow, int NumberOfBits)
{
	rewind(read);
	int* Buffer = (int*)malloc(sizeof(int)* 8);
	int counter = 0;
	int flag = 0;
	unsigned char character;
	int tmp[8];
	for (fscanf(read,"%c",&character); flag <= (NumberOfBits + overflow / 8); fscanf(read, "%c", &character))
	{
		
		if(counter != 0)
			Buffer = (int*)realloc(Buffer, sizeof(int) * (counter + 1) * 8);
		DecimalToBinary((int)character,tmp);
		for (int i = 0; i < 8; i++)
			Buffer[counter * 8 + i] = tmp[i];
		counter++;
		flag++;
	}
	printf("\nHaffman String : ");
	for (int i = 0; i <= NumberOfBits; i++)
		if (Buffer[i] == 0 || Buffer[i] == 1)
			printf("%d", Buffer[i]);
	return Buffer;
}


int DecodeHuffmanString(FILE* write, int* HuffmanString,HuffmanCodesList* codes, int overflow, int NumberOfBits)
{
	printf("\nText after decoding :\n");
	printf("---------------------\n");
	int buffer[MAX_DEPTH];
	for (int i = 0; i < MAX_DEPTH; i++)
		buffer[i] = 2;
	int counter = 0;
	HuffmanCodes* tmp; 
	for (int i = overflow; i <= NumberOfBits + overflow; i++)
	{
		buffer[counter++] = HuffmanString[i];
		tmp = searchwithcode(codes,buffer);
		if (tmp != NULL)
		{
			printf("%c", tmp->character);
			fprintf(write, "%c", tmp->character);
			for (int i = 0; i < MAX_DEPTH; i++)
				buffer[i] = 2;
			counter = 0;
		}
	}
	printf("\n");
	return 1;
}