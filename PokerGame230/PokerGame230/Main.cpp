#include <iostream>
#include <string>
#include <locale>>
#include <cstdlib>
#include <ctime>
#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC_NEW
#ifdef _DEBUG
   #ifndef DBG_NEW
      #define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
      #define new DBG_NEW
   #endif
#endif

using namespace std;

//Enums
enum suit {Spades, Hearts, Clubs, Diamonds};

//Structs
struct card
{
	int		theNumber;
	suit	theSuit;
};
struct node
{
	card	value;
	node*	next;
	node*	last;
};
struct dll
{
	node*	head;
	node*	tail;
	int		length;
};

//Functions
card	NewCard(int, suit);
string	PrintCard(card);
node*	NewNode(card);
void	DeleteNode(node*);
dll*	NewDLL();
void	DeleteDLL(dll*);
void	AddNodeFirst(dll*, node*);
void	AddNodeLast(dll*, node*);
node*	GetNode(dll*, int);
void	RemoveNode(dll*, node*);
void	CopyNode(dll*, node*);
void	SwapNode(node*, node*);
void	TransferNode(dll*, dll*, node*);
void	SortDLL(dll*);
void	CreateDeck(dll*);
void	ShuffleDeck(dll*, dll*);
void	PrintDeck(dll*);
bool	DrawCard(dll*, dll*);
void	DrawNewHand(dll*, dll*);
void	ReplaceCards(char[]);
int		CheckInputI(int);
bool	CheckInputS(string);

//Variables
dll*	deck;
dll*	discard;
dll*	hand;

string	inputS;
int		money;
int		replaced;
int		inputI;
bool	gameRunning;
bool	replacingCards;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);

	//Seed Random
	srand((unsigned)time(NULL));

	//Initiallize Variables
	discard = NewDLL();
	CreateDeck(discard);
	replaced = 0;
	replacingCards = false;


	cout << PrintCard(GetNode(discard, 38)->value) << endl;
	system("pause");

	DeleteDLL(discard);

	_CrtDumpMemoryLeaks();
	return 0;
}

card NewCard(int aNumber, suit aSuit)
{
	card aCard;
	aCard.theNumber = aNumber;
	aCard.theSuit = aSuit;
	
	return aCard;
}
string PrintCard(card aCard)
{
	string cardName;

	if (aCard.theNumber > 1 && aCard.theNumber < 11)
	{	cardName = to_string(aCard.theNumber) + " of ";	}
	if (aCard.theNumber == 1)
	{	cardName = "Ace of ";	}
	else if (aCard.theNumber == 11)
	{	cardName = "Jack of ";	}
	else if(aCard.theNumber == 12)
	{	cardName = "Queen of ";	}
	else if (aCard.theNumber == 13)
	{	cardName = "King of ";	}

	switch (aCard.theSuit)
	{
	case Spades:
		cardName += "Spades";
		break;
	case Hearts:
		cardName += "Hearts";
		break;
	case Clubs:
		cardName += "Clubs";
		break;
	case Diamonds:
		cardName += "Diamonds";
		break;
	}

	return cardName;
}
node* NewNode(card aCard)
{
	node* aNode = new node;
	aNode->value = aCard;
	aNode->next = NULL;
	aNode->last = NULL;

	return aNode;
}
void DeleteNode(node* aNode)
{
	delete aNode;
	aNode = NULL;
}
dll* NewDLL()
{
	dll* aDLL = new dll;
	aDLL->head = NULL;
	aDLL->tail = NULL;
	aDLL->length = 0;

	return aDLL;
}
void DeleteDLL(dll* aDLL)
{
	while (aDLL->length > 0)
	{	RemoveNode(aDLL, aDLL->head);	}

	delete aDLL;
	aDLL = NULL;
}
void AddNodeFirst(dll* aDLL, node* newItem)
{
	newItem->next = aDLL->head;
	if (aDLL->head != NULL)
	{	aDLL->head->last = newItem;	}
	aDLL->head = newItem;
	if (aDLL->tail == NULL)
	{	aDLL->tail = newItem;	}
	aDLL->length += 1;
}
void AddNodeLast(dll* aDLL, node* newItem)
{
	newItem->last = aDLL->tail;
	if (aDLL->tail != NULL)
	{	aDLL->tail->next = newItem;	}
	aDLL->tail = newItem;
	if (aDLL->head == NULL)
	{	aDLL->head = newItem;	}
	aDLL->length += 1;
}
node* GetNode(dll* aDLL, int pos)
{
	if (pos < aDLL->length / 2)
	{
		node* aNode = aDLL->head;
		for (int i = 0; i < pos; i += 1)
		{	aNode = aNode->next;	}
		return aNode;
	}
	else if (pos < aDLL->length)
	{
		node* aNode = aDLL->tail;
		for (int i = aDLL->length - 1; i > pos; i -= 1)
		{	aNode = aNode->last;	}
		return aNode;
	}
}
void RemoveNode(dll* aDLL, node* oldItem)
{
	if (oldItem->last != NULL)
	{	oldItem->last->next = oldItem->next;	}
	else
	{	aDLL->head = oldItem->next;	}
	if (oldItem->next != NULL)
	{	oldItem->next->last = oldItem->last;	}
	else
	{	aDLL->tail = oldItem->last;	}
	
	DeleteNode(oldItem);
	aDLL->length -= 1;
}
void CopyNode(dll* newDLL, node* aNode)
{	AddNodeFirst(newDLL, NewNode(aNode->value));	}
void SwapNode(node* item1, node* item2)
{
	card tempCard = item1->value;

	item1->value = item2->value;
	item2->value = item1->value;
}
void TransferNode(dll* oldDLL, dll* newDLL, node* aNode)
{
	AddNodeFirst(newDLL, NewNode(aNode->value));
	RemoveNode(oldDLL, aNode);
}
void SortDLL(dll* aDLL)
{
	node* currentNode = aDLL->head;
	node* checkNode;
	card currentCard;
	card checkCard;

	for (int i = 0; i < aDLL->length - 1; i += 1)
	{
		checkNode = currentNode;

		for (int j = i; j < aDLL->length - 1; j += 1)
		{
			checkNode = checkNode->next;
			currentCard = currentNode->value;
			checkCard = checkNode->value;

			if (currentCard.theSuit > checkCard.theSuit)
			{	SwapNode(currentNode, checkNode);	}
			else if(currentCard.theSuit == checkCard.theSuit)
			{
				if (currentCard.theNumber > checkCard.theNumber)
				{	SwapNode(currentNode, checkNode);	}
			}
		}

		currentNode = currentNode->next;
	}
}
void CreateDeck(dll* aDeck)
{
	for (int i = 0; i < 4; i += 1)
	{
		for (int j = 1; j < 14; j += 1)
		{
			switch (i)
			{
			case 0:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Spades)));
				break;
			case 1:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Hearts)));
				break;
			case 2:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Clubs)));
				break;
			case 3:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Diamonds)));
				break;
			}
		}
	}
}
void ShuffleDeck(dll* aDeck, dll* aDiscard)
{
	while (aDiscard->length > 0)
	{	TransferNode(aDiscard, aDeck, GetNode(aDiscard, rand() % aDiscard->length));	}
}
void PrintDeck(dll* aDeck)
{
	dll* sortedDeck = NewDLL();
	node* currentNode = aDeck->head;

	while (currentNode != NULL)
	{
		CopyNode(sortedDeck, currentNode);
		currentNode = currentNode->next;
	}
	SortDLL(sortedDeck);
	
	currentNode = sortedDeck->head;
	
	while (currentNode != NULL)
	{
		cout << PrintCard(currentNode->value) << endl;
		currentNode = currentNode->next;
	}

	DeleteDLL(sortedDeck);
}
bool DrawCard(dll* aDeck, dll* aHand)
{
	if (aDeck->length > 0)
	{
		TransferNode(aDeck, aHand, aDeck->head);
		return true;
	}
	else
	{	return false;	}
}
void DrawNewHand(dll* aDeck, dll* aHand, dll* aDiscard)
{
	for (int i = 0; i < 5; i += 1)
	{
		if (!DrawCard(aDeck, aHand))
		{
			ShuffleDeck(aDeck, aDiscard);
			DrawCard(aDeck, aHand);
		}
	}
}
void DiscardCard(dll* aDeck, dll* aDiscard, int pos)
{	TransferNode(aDeck, aDiscard, GetNode(aDeck, pos));	}
void ReplaceCards(char theCodes[])
{
	for (int i = 0; i < 5; i += 1)
	{
		switch (theCodes[i])
		{
		case 'A':
			SwapNode(deck->head, GetNode(hand, 0));
			DiscardCard(deck, discard, 0);
			replaced += 1;
			break;
		case 'B':
			SwapNode(deck->head, GetNode(hand, 1));
			DiscardCard(deck, discard, 0);
			replaced += 1;
			break;
		case 'C':
			SwapNode(deck->head, GetNode(hand, 2));
			DiscardCard(deck, discard, 0);
			replaced += 1;
			break;
		case 'D':
			SwapNode(deck->head, GetNode(hand, 3));
			DiscardCard(deck, discard, 0);
			replaced += 1;
			break;
		case 'E':
			SwapNode(deck->head, GetNode(hand, 4));
			DiscardCard(deck, discard, 0);
			replaced += 1;
			break;
		case '0':
			return;
			break;
		}
	}
}
int CheckInputI(int input)
{
	if (!cin)
	{
		cin.clear();
		cin.ignore();
		return -1;
	}
	else
	{	return input;	}
}
bool CheckInputS(string input)
{
	if (!cin)
	{
		cin.clear();
		cin.ignore();
		return false;
	}
	else
	{
		if (!replacingCards)
		{
			if (input == "deck")
			{
				PrintDeck(deck);
				return true;
			}
			if (input == "swap")
			{
				replacingCards = true;
				return true;
			}

			return false;
		}
		else
		{
			if (input.length() <= 5 && input.length() > 0)
			{
				char codes[5] = { '0','0','0','0','0' };

				for (int i = 0; i < input.length(); i += 1)
				{
					if (input[i] >= 'A' && input[i] <= 'E')
					{
						for (int j = 0; j < i; j += 1)
						{
							if (input[i] == codes[j])
							{	return false;	}
						}

						codes[i] = input[i];
					}
					else
					{	return false;	}
				}

				ReplaceCards(codes);
				replacingCards = false;
				return true;
			}
			else
			{	return false;	}
		}
	}
}