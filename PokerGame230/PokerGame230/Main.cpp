#include <iostream>
#include <string>
#include <locale>
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
enum suit {Clubs, Diamonds, Hearts, Spades};

//Structs
struct card
{
	int		theNumber;
	suit	theSuit;
	bool	wasReplaced;
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
void	DeleteNode(node*&);
dll*	NewDLL();
void	DeleteDLL(dll*&);
void	AddNodeFirst(dll*, node*);
void	AddNodeLast(dll*, node*);
node*	GetNode(dll*, int);
void	RemoveNode(dll*, node*);
void	CopyNode(dll*, node*);
void	SwapNode(node*, node*);
void	TransferNode(dll*, dll*, node*);
void	SortDLL(dll*);
void	CreateDeck(dll*&);
void	ShuffleDeck(dll*, dll*);
void	PrintDeck(dll*);
void	PrintHand(dll*);
bool	DrawCard(dll*, dll*);
void	DrawNewHand(dll*, dll*, dll*);
void	DiscardCard(dll*, dll*, int);
void	ReplaceCards(char[]);
int		FindCard(dll*, int, int);
void	VictoryCheck(dll*);
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
bool	gamePlaying;
bool	stillPlaying;
bool	badInput;
bool	discarding;
bool	deckPrint;
bool	debugSwap;

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |
		_CRTDBG_LEAK_CHECK_DF);

	// Seed Random
	srand((unsigned)time(NULL));

	// Game Loop
	do
	{
		// Main Menu
		do
		{
			cout << "Welcome to Video Poker!\nEnter \"1\" to start the game\nEnter \"0\" to quit" << endl;

			cin >> inputI;
			inputI = CheckInputI(inputI);

			if (inputI == 1)
			{
				gamePlaying = true;
				stillPlaying = true;
				badInput = false;
			}
			else if (inputI == 0)
			{
				gamePlaying = false;
				stillPlaying = false;
				badInput = false;
			}
			else
			{
				cout << "Input error! Please try again.\n" << endl;
				badInput = true;
			}
		} while (badInput);

		while (gamePlaying)
		{
			// Initiallize Variables
			deck = NewDLL();
			discard = NewDLL();
			hand = NewDLL();
			CreateDeck(discard);
			ShuffleDeck(deck, discard);

			money = 10;
			replaced = 0;
			discarding = false;

			do
			{
				if (money > 0)
				{
					do
					{
						cout << "You currently have $" << money << "\nEnter \"1\" to pay $1 and draw a new hand\nEnter \"0\" to quit" << endl;

						cin >> inputI;
						inputI = CheckInputI(inputI);

						if (inputI == 1)
						{
							stillPlaying = true;
							gamePlaying = true;
							badInput = false;
						}
						else if (inputI == 0)
						{
							stillPlaying = false;
							gamePlaying = false;
							badInput = false;
						}
						else
						{
							cout << "Input error! Please try again.\n" << endl;
							badInput = true;
						}
					} while (badInput);
				}
				else
				{
					do
					{
						cout << "You currently have $" << money << "\nYou do not have enough to buy into another hand\nEnter\"1\" to play again\nEnter \"0\" to quit" << endl;

						cin >> inputI;
						inputI = CheckInputI(inputI);

						if (inputI == 0)
						{
							stillPlaying = false;
							gamePlaying = false;
							badInput = false;
						}
						else if (inputI == 1)
						{
							stillPlaying = false;
							gamePlaying = true;
							badInput = false;
						}
						else
						{
							cout << "Input error! Please try again.\n" << endl;
							badInput = true;
						}
					} while (badInput);
				}

				if (stillPlaying)
				{
					money -= 1;
					DrawNewHand(deck, hand, discard);
					SortDLL(hand);

					do
					{
						discarding = false;
						cout << "Your hand:" << endl;
						PrintHand(hand);
						cout << endl;
						cout << "Choose which cards you want to keep by entering a list of their letters\nAll of the letters must be capitalized and there shouldn't be any spaces\nExample: BCE\nEnter \"0\" if you don't want to keep any cards\nDEBUG: Enter \"deck\" to list all of the cards available in the deck\nDEBUG: Enter \"swap\" to swap specific cards in your hand with others in the deck" << endl;
						
						discarding = true;
						debugSwap = false;
						deckPrint = false;
						cin >> inputS;
						badInput = CheckInputS(inputS);

						if (debugSwap)
						{
							int pos = 0;
							int suit = 0;
							int value = 0;
							bool tempBadInput = false;

							do
							{
								cout << "Which card are you swapping?\nEnter a capital letter A-E" << endl;

								cin >> inputS;
								if (CheckInputS(inputS))
								{
									pos = int(inputS[0]) - 'A';
									tempBadInput = false;
								}
								else
								{
									cout << "Input error! Please try again.\n" << endl;
									tempBadInput = true;
								}

							} while (tempBadInput);

							do
							{
								cout << "What suit do you want?\nEnter a number 1-4\nClubs = 1      Diamonds = 2\nHearts = 3     Spades = 4" << endl;

								cin >> inputI;
								inputI = CheckInputI(inputI);

								if (inputI >= 1 && inputI <= 4)
								{
									suit = inputI;
									tempBadInput = false;
								}
								else
								{
									cout << "Input error! Please try again.\n" << endl;
									tempBadInput = true;
								}
							} while (tempBadInput);

							do
							{
								cout << "What value do you want?\nEnter a number 1-13\nAce = 1       Jack = 11\nQueen = 12    King = 13" << endl;

								cin >> inputI;
								inputI = CheckInputI(inputI);

								if (inputI >= 1 && inputI <= 13)
								{
									value = inputI;
									tempBadInput = false;
								}
								else
								{
									cout << "Input error! Please try again.\n" << endl;
									tempBadInput = true;
								}
							} while (tempBadInput);

							int find = FindCard(hand, suit, value);
							if (find != -1)
							{	cout << "Sorry, that card is already in your hand.\n" << endl;	}
							else
							{
								find = FindCard(deck, suit, value);
								if (find != -1)
								{
									SwapNode(GetNode(hand, pos), GetNode(deck, find));
									SortDLL(hand);
								}
								else
								{
									find = FindCard(discard, suit, value);
									if (find != -1)
									{
										SwapNode(GetNode(hand, pos), GetNode(discard, find));
										SortDLL(hand);
									}
									else
									{	cout << "Huh? We couldn't find your card?\n" << endl;	}
								}

							}
						}
						else if(badInput && !deckPrint)
						{	cout << "Input error! Please try again.\n" << endl;	}
					} while (badInput);

					SortDLL(hand);
					cout << "You kept " << replaced << " cards. Your new hand: " << endl;
					PrintHand(hand);

					VictoryCheck(hand);
					system("pause");

					for (int i = 0; i < 5; i += 1)
					{	DiscardCard(hand, discard, 0);	}

					cout << endl;
				}
			} while (stillPlaying);

			DeleteDLL(deck);
			DeleteDLL(discard);
			DeleteDLL(hand);
		}
	} while (gameRunning);

	_CrtDumpMemoryLeaks();
	return 0;
}

card NewCard(int aNumber, suit aSuit)
{
	card aCard;
	aCard.theNumber = aNumber;
	aCard.theSuit = aSuit;
	aCard.wasReplaced = false;
	
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
void DeleteNode(node* &aNode)
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
void DeleteDLL(dll* &aDLL)
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
	item2->value = tempCard;
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
void CreateDeck(dll* &aDeck)
{
	for (int i = 0; i < 4; i += 1)
	{
		for (int j = 1; j < 14; j += 1)
		{
			switch (i)
			{
			case 0:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Clubs)));
				break;
			case 1:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Diamonds)));
				break;
			case 2:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Hearts)));
				break;
			case 3:
				AddNodeFirst(aDeck, NewNode(NewCard(j, Spades)));
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
void PrintHand(dll* aHand)
{
	dll* sortedDeck = NewDLL();
	node* currentNode = aHand->head;

	while (currentNode != NULL)
	{
		CopyNode(sortedDeck, currentNode);
		currentNode = currentNode->next;
	}
	SortDLL(sortedDeck);

	currentNode = sortedDeck->head;

	for (int i = 0; i < 5; i += 1)
	{
		switch (i)
		{
		case 0:
			cout << "A: ";
			break;
		case 1:
			cout << "B: ";
			break;
		case 2:
			cout << "C: ";
			break;
		case 3:
			cout << "D: ";
			break;
		case 4:
			cout << "E: ";
			break;
		}

		cout << PrintCard(currentNode->value);
		if (currentNode->value.wasReplaced)
		{	currentNode->value.wasReplaced = false;	}
		else if (discarding)
		{	cout << " (Kept)";	}
		cout << endl;
		currentNode = currentNode->next;
	}
	if (currentNode != NULL)
	{
		cout << "Error Here" << endl;

		while (currentNode != NULL)
		{
			cout << PrintCard(currentNode->value) << endl;
		}
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
			GetNode(hand, 0)->value.wasReplaced = true;
			break;
		case 'B':
			SwapNode(deck->head, GetNode(hand, 1));
			DiscardCard(deck, discard, 0);
			GetNode(hand, 1)->value.wasReplaced = true;
			break;
		case 'C':
			SwapNode(deck->head, GetNode(hand, 2));
			DiscardCard(deck, discard, 0);
			GetNode(hand, 2)->value.wasReplaced = true;
			break;
		case 'D':
			SwapNode(deck->head, GetNode(hand, 3));
			DiscardCard(deck, discard, 0);
			GetNode(hand, 3)->value.wasReplaced = true;
			break;
		case 'E':
			SwapNode(deck->head, GetNode(hand, 4));
			DiscardCard(deck, discard, 0);
			GetNode(hand, 4)->value.wasReplaced = true;
			break;
		case '0':
			return;
			break;
		}
	}
}
int FindCard(dll* aDeck, int aSuit, int aValue)
{
	node* currentNode = aDeck->head;
	int count = 0;

	while (currentNode != NULL)
	{
		if (currentNode->value.theSuit == static_cast<suit>(aSuit-1) && currentNode->value.theNumber == aValue)
		{	return count;	}
		else
		{
			currentNode = currentNode->next;
			count += 1;
		}
	}
	return -1;
}
void VictoryCheck(dll* aHand)
{
	dll* tempHand = NewDLL();
	node* currentNode;
	suit aSuit;
	int aNumber;
	int count;
	bool done = false;

	// 0 = 1 Pair
	// 1 = 2 Pair
	// 2 = 3 of a Kind
	// 3 = Straight
	// 4 = Flush
	// 5 = Full House
	// 6 = 4 of a Kind
	// 7 = Straight Flush
	// 8 = Royal Flush
	bool victories[9] = {false,false,false,false,false,false,false,false,false};

	// Checking for matching numbers
	while (aHand->length > 0 && !done)
	{
		count = 0;
		currentNode = aHand->head;
		aNumber = currentNode->value.theNumber;

		while (currentNode != NULL)
		{
			if (currentNode->value.theNumber == aNumber)
			{
				TransferNode(aHand, tempHand, currentNode);
				currentNode = aHand->head;
				count += 1;
			}
			else
			{	currentNode = currentNode->next;	}
		}

		if (count == 2)
		{
			if (victories[0])
			{
				victories[1] = true;
				done = true;
			}
			else if (victories[2])
			{
				victories[5] = true;
				done = true;
			}
			else
			{	victories[0] = true;	}
		}
		else if (count == 3)
		{
			if (victories[0])
			{
				victories[5] = true;
				done = true;
			}
			else
			{	victories[2] = true;	}
		}
		else if (count == 4)
		{
			victories[6] = true;
			done = true;
		}
	}
	
	if(victories[0])
	{	done = true;	}

	// Resetting hand
	while (tempHand->length > 0)
	{	TransferNode(tempHand, aHand, tempHand->head);	}

	if (!done)
	{
		// Checking for straight and/or flush
		int lowest = 15;
		int nextLowest = 14;
		int highest = 0;
		currentNode = aHand->head;
		aSuit = currentNode->value.theSuit;
		count = 0;

		for(int i = 0; i < 5; i += 1)
		{
			if (currentNode->value.theNumber < lowest)
			{
				nextLowest = lowest;
				lowest = currentNode->value.theNumber;
			}
			else if (currentNode->value.theNumber < nextLowest)
			{	nextLowest = currentNode->value.theNumber;	}

			if (currentNode->value.theNumber > highest)
			{	highest = currentNode->value.theNumber;	}

			if (currentNode->value.theSuit == aSuit)
			{	count += 1;	}

			currentNode = currentNode->next;
		}

		if (highest - lowest == 4)
		{	victories[3] = true;	}
		else if (lowest == 1 && nextLowest == 10 && highest == 13)
		{	victories[3] = true;	}
		if (count == 5)
		{	victories[4] = true;	}
		if (victories[3] && victories[4])
		{
			victories[7] = true;
			if (lowest == 1 && nextLowest == 10 && highest == 13)
			{	victories[8] = true;	}
		}
	}

	// Declaring victory
	if (victories[8])
	{
		cout << "Congradulations! You got a Royal Flush and earned $800!!!" << endl;
		money += 800;
	}
	else if (victories[7])
	{
		cout << "Congradulations! You got a Straight Flush and earned $50!!" << endl;
		money += 50;
	}
	else if (victories[6])
	{
		cout << "Congradulations! You got Four of a Kind and earned $25!!" << endl;
		money += 25;
	}
	else if (victories[5])
	{
		cout << "Congradulations! You got a Full House and earned $9!!" << endl;
		money += 9;
	}
	else if (victories[4])
	{
		cout << "Congradulations! You got a Flush and earned $6!" << endl;
		money += 6;
	}
	else if (victories[3])
	{
		cout << "Congradulations! You got a Straight and earned $4!" << endl;
		money += 4;
	}
	else if (victories[2])
	{
		cout << "Congradulations! You got Three of a Kind and earned $3!" << endl;
		money += 3;
	}
	else if (victories[1])
	{
		cout << "Congradulations! You got Two Pairs and earned $2!" << endl;
		money += 2;
	}
	else if (victories[0])
	{
		for (int i = 0; i < 4; i += 1)
		{
			currentNode = aHand->head;
			count = 0;

			while (currentNode != NULL)
			{
				if (i == 0)
				{
					if (currentNode->value.theNumber == 1)
					{	count += 1;	}
				}
				else
				{
					if (currentNode->value.theNumber == i + 10)
					{	count += 1;	}
				}

				currentNode = currentNode->next;
			}

			if (count == 2)
			{
				cout << "Congradulations! You got One Pair (Jacks or Higher) and earned $1!" << endl;
				money += 1;
				break;
			}
		}

		if (count != 2)
		{	cout << "Sorry, your Pair wasn't of Jacks or higher. Better luck next time." << endl;	}
	}
	else
	{	cout << "Sorry, you didn't get anything. Better luck next time." << endl;	}

	DeleteDLL(tempHand);
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
	else if (debugSwap)
	{
		if (input.length() == 1)
		{
			if (input[0] >= 'A' && input[0] <= 'E')
			{	return true;	}
		}

		return false;
	}
	else
	{
		if (input == "deck")
		{
			PrintDeck(deck);
			deckPrint = true;
			cout << endl;
			return discarding;
		}
		if (input == "swap")
		{
			debugSwap = true;
			return discarding;
		}
		if (discarding)
		{
			replaced = 0;

			if (input.length() <= 5 && input.length() > 0)
			{
				if (input == "0")
				{
					char all[5] = { 'A','B','C','D','E' };
					ReplaceCards(all);
					return false;
				}

				char codes[5] = { '0','0','0','0','0' };

				for (int i = 0; i < input.length(); i += 1)
				{
					if (input[i] >= 'A' && input[i] <= 'E')
					{
						for (int j = 0; j < i; j += 1)
						{
							if (input[i] == codes[j])
							{	return true;	}
						}
						
						codes[(input[i] - 'A')] = input[i];
						replaced += 1;
					}
					else
					{	return true;	}
				}

				for (int i = 0; i < 5; i += 1)
				{
					if (codes[i] == '0')
					{	codes[i] = 'A' + i;	}
					else
					{	codes[i] = '0';	}
				}

				for (int i = 0; i < 5; i += 1)
				{
					if (codes[i] == '0')
					{
						for (int j = i; j < 5; j += 1)
						{
							if (codes[j] != '0')
							{
								codes[i] = codes[j];
								codes[j] = '0';
								break;
							}
						}
					}
				}

				ReplaceCards(codes);
				return false;
			}
			else
			{	return true;	}
		}
		else
		{	return true;	}
	}
}