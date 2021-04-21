#include <iostream>
#include <fstream>

using namespace std;

struct Node {
	string data;
	struct Node* c1;
	struct Node* c2;
	struct Node* c3;
};

//allocates new node 
struct Node* newNode(string data) {
	// declare and allocate new node  
	struct Node* Node = new struct Node();

	Node->data = data;    // Assign data to this node

	// Initialize left and right children as NULL 
	Node->c1 = NULL;
	Node->c2 = NULL;
	Node->c3 = NULL;
	return(Node);
}
struct DataTable {
	int nextState;
	bool isValid;
} StateTable[16][14] = {
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1},
	{0,1,0,1,2,1,3,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1},
	{2,1,0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1,2,1},
	{3,1,3,1,3,1,4,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1},
	{3,1,3,1,0,1,4,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1,3,1}, //STATE 5
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1},
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1 },
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1 },
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1 },
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1 },
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,1,0,0,0,0,0,0 }, //State 11
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,12,1,15,1 },
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,14,1,0,0,0,0 },
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,13,1,14,1,0,0 },
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,14,1,0,0,0,0 },
	{0,1,0,1,1,1,9,1,5,1,6,1,7,1,8,1,10,1,0,0,15,1,0,0,15,1 },
};

/*state first from 0 to 15 (diagram state -1) and inputs
1 - space, tab
2 - newline
3 - /
4 - *
5 - (
6 - )
7 - +
8 - -
9 - :
10 - =
11 - some digit, 0 through 9
12- .
13 - letters
14 - error
*/
int jinput_to_input(char input);
struct Node Parser(string name);
struct Node ParserHelper(struct Node CTree);
void CoutTree(struct Node *Root, int TabCounter);
string scanneradjusted(string name);
string Tokens;
string Token = "";
/*
RULES FOR PARSER
<program> → <stmt list> $$
<stmt list> → <stmt> <stmt list> | nothing
<stmt> → id assign <expr> | read id | write <expr>
<expr> → <term> <term tail>
<term tail> → <add op> <term> <term tail> | nothing
<term> → <factor> <fact tail>
<fact tail> → <mult op> <factor> <fact tail> | nothing
<factor> → lparen <expr> rparen | id | number
<add op> → plus | minus
<mult op> → times | div
*/
bool ErrorFlag = 1;
bool ErrorFlag2 = 0;
string delimiter = " ";
size_t pos = 0;
void NextToken();
int main()
{
	struct Node* Test = newNode("program");
	*Test = Parser("test.txt");
	if (ErrorFlag || ErrorFlag2) {
		cout << "Error";
	}
	else
		CoutTree(Test, 0);
}
void CoutTree(struct Node*Root, int TabCounter) 
/*
the if and else if statements get a bit messy, as i didnt fully read the instructions properly for the output. So many of
token types nodes were not given a child token node. While this isnt necessary for the tree, it does complicate the output a bit.
*/
{
	if (Root != NULL)
	{
		if ((Root->data == "program")|| (Root->data == "stmt list")|| (Root->data == "stmt")|| (Root->data == "expr") || (Root->data == "term tail") 
			|| (Root->data == "term") || (Root->data == "fact tail") || (Root->data == "factor") || (Root->data == "add op") || (Root->data == "mult op")|| (Root->data == "id") || (Root->data == "number")) // all the non-terminals
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;
			TabCounter = TabCounter + 1;
			CoutTree(Root->c1, TabCounter);
			CoutTree(Root->c2, TabCounter);
			CoutTree(Root->c3, TabCounter);
			for (int i = TabCounter-1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "read") || (Root->data == "write"))
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;	
			TabCounter = TabCounter + 1;
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout <<Root->data << endl;
			for (int i = TabCounter-1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "lparen"))
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;
			TabCounter = TabCounter + 1;
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "(" << endl;
			for (int i = TabCounter - 1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "rparen"))
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;
			TabCounter = TabCounter + 1;
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << ")" << endl;
			for (int i = TabCounter - 1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "times"))
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;
			TabCounter = TabCounter + 1;
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "*" << endl;
			for (int i = TabCounter - 1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "div"))
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;
			TabCounter = TabCounter + 1;
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "/" << endl;
			for (int i = TabCounter - 1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "assign"))
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;
			TabCounter = TabCounter + 1;
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << ":=" << endl;
			for (int i = TabCounter - 1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "plus"))
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "<" + Root->data + ">" << endl;
			TabCounter = TabCounter + 1;
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << "+" << endl;
			for (int i = TabCounter - 1; i > 0; i--)
				cout << "  ";
			cout << "</" + Root->data + ">" << endl;
		}
		else if ((Root->data == "minus"))
		{
		for (int i = TabCounter; i > 0; i--)
			cout << "  ";
		cout << "<" + Root->data + ">" << endl;
		TabCounter = TabCounter + 1;
		for (int i = TabCounter; i > 0; i--)
			cout << "  ";
		cout << ")" << endl;
		for (int i = TabCounter - 1; i > 0; i--)
			cout << "  ";
		cout << "</" + Root->data + ">" << endl;
		}
		else
		{
			for (int i = TabCounter; i > 0; i--)
				cout << "  ";
			cout << Root->data << endl;
			TabCounter = TabCounter + 1;
			CoutTree(Root->c1, TabCounter);
			CoutTree(Root->c2, TabCounter);
			CoutTree(Root->c3, TabCounter);
		}
	}
}
void NextToken() {
	if ((pos = Tokens.find(delimiter)) != std::string::npos)
	{
		Token = Tokens.substr(0, pos);
		Tokens.erase(0, pos + delimiter.length());
	}
	else
	{
		Token = "";
		ErrorFlag = 0;
	}
}
struct Node Parser(string name) {
	struct Node* Tree = newNode("program");
	Tokens = scanneradjusted(name);
	//need to read token string and put each term into the connected tree. max children is 3. NOT a binary tree. 
	if (Tokens==".error") {
		cout << "Error";
	}
	else
	{//below should get me token by token, used in the parser helper
		Tokens = Tokens + " EOF"; //to help mitigate issues with NextToken function
		if ((pos = Tokens.find(delimiter)) != std::string::npos) 
		{
			Token = Tokens.substr(0, pos);
			Tokens.erase(0, pos + delimiter.length());
		}
		else
		{
			Token = "";
		}
		*Tree = ParserHelper(*Tree);
		return (*Tree);
	}
}
struct Node ParserHelper(struct Node CTree) 
{
	if ((CTree.data == "program")) 
	{
		if (Token != "") 
		{
			CTree.c1 = newNode("stmt list");
			*CTree.c1 = ParserHelper(*CTree.c1);
			return (CTree);
		}
		else
		{
			ErrorFlag = 0;
			return (CTree);
		}
	}
	if ((CTree.data == "stmt list") && (Token != ""))
	{
		CTree.c1 = newNode("stmt");
		*CTree.c1 = ParserHelper(*CTree.c1);
		CTree.c2 = newNode("stmt list");
		*CTree.c2 = ParserHelper(*CTree.c2);
		return (CTree);
	}
	else if (CTree.data == "stmt list")
	{
		return (CTree);
	}
	if (CTree.data == "stmt") 
	{	
		if (Token == "id")
		{

			CTree.c1 = newNode("id");
			*CTree.c1 = ParserHelper(*CTree.c1);
			CTree.c2 = newNode("assign");
			NextToken();
			CTree.c3 = newNode("expr");
			*CTree.c3 = ParserHelper(*CTree.c3);
			return (CTree);
		}
		else if (Token == "read")
		{
			CTree.c1 = newNode("read");
			NextToken();
			CTree.c2 = newNode("id");
			*CTree.c2 = ParserHelper(*CTree.c2);
			return (CTree);
		}
		else if (Token == "write")
		{
			CTree.c1 = newNode("write");
			NextToken();
			CTree.c2 = newNode("expr");
			*CTree.c2 = ParserHelper(*CTree.c2);
			return (CTree);
		}
		else
		{
			ErrorFlag2 = 1;
			return (CTree);
		}
	}
	if ((CTree.data == "id")) //terminal
	{
		NextToken();
		CTree.c1 = newNode(Token);
		NextToken();
		return (CTree);
	}
	if ((CTree.data == "number")) //terminal
	{
		NextToken();
		CTree.c1 = newNode(Token);
		NextToken();
		return (CTree);
	}
	if ((CTree.data == "expr"))
	{
		CTree.c1 = newNode("term");
		*CTree.c1 = ParserHelper(*CTree.c1);
		CTree.c2 = newNode("term tail");
		*CTree.c2 = ParserHelper(*CTree.c2);
		return (CTree);

	}
	if ((CTree.data == "term"))
	{
		CTree.c1 = newNode("factor");
		*CTree.c1 = ParserHelper(*CTree.c1);
		CTree.c2 = newNode("fact tail");
		*CTree.c2 = ParserHelper(*CTree.c2);
		return (CTree);
	}
	if ((CTree.data == "factor"))
	{
		if (Token == "lparen")
		{
			CTree.c1 = newNode("lparen");
			NextToken();
			CTree.c2 = newNode("expr");
			*CTree.c2 = ParserHelper(*CTree.c2);
			CTree.c3 = newNode("rparen");
			NextToken();
			return (CTree);
		}
		else if (Token == "id")
		{
			CTree.c1 = newNode("id");
			*CTree.c1 = ParserHelper(*CTree.c1);
			return (CTree);
		}
		else if (Token == "number")
		{
			CTree.c1 = newNode("number");
			*CTree.c1 = ParserHelper(*CTree.c1);
			return (CTree);
		}
		else
		{
			ErrorFlag2 = 1;
			return (CTree);
		}
	}
	if ((CTree.data == "fact tail"))
	{
		if ((Token == "times")||(Token == "div"))
		{
			CTree.c1 = newNode("mult op");
			*CTree.c1 = ParserHelper(*CTree.c1);
			CTree.c2 = newNode("factor");
			*CTree.c2 = ParserHelper(*CTree.c2);
			CTree.c3 = newNode("fact tail");
			*CTree.c3 = ParserHelper(*CTree.c3);
			return (CTree);
		}
		else
			return(CTree);
	}
	if ((CTree.data == "term tail"))
	{
		if ((Token == ""))
			return (CTree);
		else if ((Token=="plus")||(Token=="minus"))
		{
			CTree.c1 = newNode("add op");
			*CTree.c1 = ParserHelper(*CTree.c1);
			CTree.c2 = newNode("term");
			*CTree.c2 = ParserHelper(*CTree.c2);
			CTree.c3 = newNode("term tail");
			*CTree.c3 = ParserHelper(*CTree.c3);
			return (CTree);
		}
		else
		{
			return (CTree);
		}
	}//<add op> → plus | minus
	//<mult op> → times | div * /
	if ((CTree.data == "add op"))
	{
		if (Token == "plus")
		{
			CTree.c1 = newNode("plus");
			NextToken();
			return (CTree);
		}
		else if (Token == "minus")
		{
			CTree.c1 = newNode("minus");
			NextToken();
			return (CTree);
		}
		else
		{
			ErrorFlag2 = 1;
			return (CTree);
		}
	}
	if ((CTree.data == "mult op"))
	{
		if (Token == "times")
		{
			CTree.c1 = newNode("times");
			NextToken();
			return (CTree);
		}
		if (Token == "minus")
		{
			CTree.c1 = newNode("div");
			NextToken();
			return (CTree);
		}
	}
}
string scanneradjusted(string name) {
	ifstream inputfile;
	inputfile.open(name);
	string readorwrite;
	string IDorNum;
	bool inputcheck = 0;
	bool changedstate = 0;
	bool breakflag = 0;
	int tempinput;
	char input;
	int jinput;
	int iinput = 0;
	bool flag = 1;
	bool first = 1;
	bool longesttoken = 1;
	string idOrNum = "";
	string tokenstring = "";
	while (!(inputfile.eof())) {
		readorwrite = "";
		IDorNum = "";
		if ((longesttoken)) {
			inputfile.get(input);
			if (inputfile.eof())
			{
				break;
			}
		}
		else
			longesttoken = 1;
		jinput = jinput_to_input(input);//made into a function for ease in use of scanner
		if (StateTable[iinput][jinput].isValid)
		{
			if ((jinput == 2))
			{
				if ((StateTable[iinput][jinput].nextState != 2) && (StateTable[iinput][jinput].nextState != 3))
				{
					if ((inputfile.peek() == 42) || (inputfile.peek() == 47));
					else 
					{
						if (first)
						{
							tokenstring = tokenstring + "div";
							first = 0;
						}
						else
						{
							tokenstring = tokenstring + " div";
						}
					}
					
					
				}
			}
			else if ((jinput == 3))
			{
				if (StateTable[iinput][jinput].nextState == 9)
				{
					if (first)
					{
						tokenstring = tokenstring + "times";
						first = 0;
					}
					else
					{
						tokenstring = tokenstring + " times";
					}
				}
				else if (StateTable[iinput][jinput].nextState == 4)
				{
					if (inputfile.peek() == 47)
					{
						inputfile.get(input);
						iinput = StateTable[iinput][jinput].nextState;
						jinput = jinput_to_input(input);
					}
				}
			}
			else if ((jinput == 4)&& ((StateTable[iinput][jinput].nextState !=2)&& (StateTable[iinput][jinput].nextState != 3)&& (StateTable[iinput][jinput].nextState != 4)))
			{
				if (first)
				{
					tokenstring = tokenstring + "lparen";
					first = 0;
				}
				else
				{
					tokenstring = tokenstring + " lparen";
				}
			}
			else if ((jinput == 5) && ((StateTable[iinput][jinput].nextState != 2) && (StateTable[iinput][jinput].nextState != 3) && (StateTable[iinput][jinput].nextState != 4)))
			{
				if (first)
				{
					tokenstring = tokenstring + "rparen";
					first = 0;
				}
				else
				{
					tokenstring = tokenstring + " rparen";
				}
			}
			else if ((jinput == 6)&&((StateTable[iinput][jinput].nextState != 2) && (StateTable[iinput][jinput].nextState != 3) && (StateTable[iinput][jinput].nextState != 4)))
			{
				if (first)
				{
					tokenstring = tokenstring + "plus";
					first = 0;
				}
				else
				{
					tokenstring = tokenstring + " plus";
				}
			}
			else if ((jinput == 7) && ((StateTable[iinput][jinput].nextState != 2) && (StateTable[iinput][jinput].nextState != 3) && (StateTable[iinput][jinput].nextState != 4)))
			{
				if (first)
				{
					tokenstring = tokenstring + "minus";
					first = 0;
				}
				else
				{
					tokenstring = tokenstring + " minus";
				}
			}
			else if (jinput == 8); //incomplete token but valid, so continue
			else if ((jinput == 9) && ((StateTable[iinput][jinput].nextState != 2) && (StateTable[iinput][jinput].nextState != 3) && (StateTable[iinput][jinput].nextState != 4))) //assumed to be valid based on entry if statement, therefor assignment token is complete
			{
				if (first)
				{
					tokenstring = tokenstring + "assign";
					first = 0;
				}
				else
				{
					tokenstring = tokenstring + " assign";
				}
			}
			else if ((jinput == 10)&& ((StateTable[iinput][jinput].nextState != 2)&& (StateTable[iinput][jinput].nextState != 3)))
			{
				inputcheck = 1;
				if (inputfile.eof())
				{
					if (StateTable[iinput][jinput].nextState == 15) // if the next state is 15, a letter was introduced previously for the id
					{
						if (first)
						{
							tokenstring = tokenstring + "id";
							first = 0;
							break;
						}
						else
						{
							tokenstring = tokenstring + " id";
							break;
						}
					}
					else // if state isnt 15 and is valid, then it had to be a number.
					{
						if (first)
						{
							tokenstring = tokenstring + "number";
							first = 0;
							break;
						}
						else
						{
							tokenstring = tokenstring + " number";
							break;
						}
					}
				}
				else
				{
					while (longesttoken)
					{
						IDorNum = IDorNum + input;

						inputfile.get(input);
						if (!(inputfile.eof())) {
							iinput = StateTable[iinput][jinput].nextState;
							jinput = jinput_to_input(input);
						}
						if (StateTable[iinput][jinput].nextState == 15)//checking if 15
						{
							//iinput = 15;	
							if (StateTable[iinput][jinput].isValid)
							{
								if ((StateTable[iinput][jinput].nextState != 15) || (inputfile.eof())) // if its still 15, keep going. Not reached longest token yet.
								{
									longesttoken = 0;
									if (first)
									{
										tokenstring = tokenstring + "id";
										tokenstring = tokenstring + " " + IDorNum;
										IDorNum = "";
										first = 0;
										changedstate = 1;
									}
									else
									{
										tokenstring = tokenstring + " id";
										tokenstring = tokenstring + " " + IDorNum;
										IDorNum = "";
										changedstate = 1;
									}
								}
							}
							else //reached a nonvalid state
							{
								//cout << "error";
								breakflag = 1;
								break;
							}
						}
						else // if its not state 15, then it must be 13/14
						{
							//iinput = StateTable[iinput][tempinput].nextState;
							if (StateTable[iinput][jinput].isValid)
							{
								if ((StateTable[iinput][jinput].nextState < 13) || (inputfile.eof()))
								{
									longesttoken = 0;
									if (first)
									{
										tokenstring = tokenstring + "number";
										tokenstring = tokenstring + " " + IDorNum;
										IDorNum = "";
										first = 0;
										changedstate = 1;
									}
									else
									{
										tokenstring = tokenstring + " number";
										tokenstring = tokenstring + " " + IDorNum;
										IDorNum = "";
										changedstate = 1;
									}
								}
							}
							else
							{
								//cout << "error";
								breakflag = 1;
								break;
							}
						}
					}

				}
			}//input 11 '.' can be skipped as it doesnt make a token.
			else if ((jinput == 12)&&((StateTable[iinput][jinput].nextState != 2) && (StateTable[iinput][jinput].nextState != 3) && (StateTable[iinput][jinput].nextState != 4)))
			{
				inputcheck = 1;
				if (inputfile.eof())
				{
					if (first)
					{
						tokenstring = tokenstring + "id";
						first = 0;
						break;
					}
					else
					{
						tokenstring = tokenstring + " id";
						break;
					}
				}
				else
				{
					readorwrite = readorwrite + input;

					while (longesttoken)//we know next state had to be 15
					{
						IDorNum = IDorNum + input;

						inputfile.get(input);
						if (!(inputfile.eof())) {
							iinput = StateTable[iinput][jinput].nextState;
							jinput = jinput_to_input(input);
						}
						//	iinput = 15;
						if (StateTable[iinput][jinput].isValid)
						{
							if ((StateTable[iinput][jinput].nextState != 15) || (inputfile.eof()))
							{
								longesttoken = 0;
								if (first)
								{
									if (readorwrite == "read")
									{
										tokenstring = tokenstring + "read";
										first = 0;
										changedstate = 1;
									}
									else if (readorwrite == "write")
									{
										tokenstring = tokenstring + "write";
										first = 0;
										changedstate = 1;
									}
									else
									{
										tokenstring = tokenstring + "id";
										first = 0;
										tokenstring = tokenstring + " " + IDorNum;
										IDorNum = "";
										changedstate = 1;
									}
								}
								else
								{
									if (readorwrite == "read")
									{
										tokenstring = tokenstring + " read";
										first = 0;
										changedstate = 1;
									}
									else if (readorwrite == "write")
									{
										tokenstring = tokenstring + " write";
										first = 0;
										changedstate = 1;
									}
									else
									{
										tokenstring = tokenstring + " id";
										tokenstring = tokenstring + " " + IDorNum;
										IDorNum = "";
										first = 0;
										changedstate = 1;
									}
								}
							}
							readorwrite = readorwrite + input;
						}
						else
						{
							//cout << "error";
							breakflag = 1;
							break;
						}
					}
				}
			}
			else if (jinput == 13)
			{
				//cout << "error";
				breakflag = 1;
				break;
			}

			if (!inputcheck)// so theres no issue with id and number checking
				iinput = StateTable[iinput][jinput].nextState;
			else
				inputcheck = 0;
		}
		else
		{
			//cout << "error";
			breakflag = 1;
			break;
		}
		if ((inputfile.eof()))
			break;
	}//end of while loop
	if (!breakflag)
	{
		return tokenstring;
	}
	else
		return ".error";
}

int jinput_to_input(char input)
{
	int jinput;
	if (input == (' ') || input == ('\t'))
		jinput = 0;
	else if (input == ('\n'))
		jinput = 1;
	else if (input == ('/'))
		jinput = 2;
	else if (input == ('*'))
		jinput = 3;
	else if (input == ('('))
		jinput = 4;
	else if (input == (')'))
		jinput = 5;
	else if (input == ('+'))
		jinput = 6;
	else if (input == ('-'))
		jinput = 7;
	else if (input == (':'))
		jinput = 8;
	else if (input == ('='))
		jinput = 9;
	else if ((input >= 48) && (input <= 57))
		jinput = 10;
	else if (input == ('.'))
		jinput = 11;
	else if (((input >= 65) && (input <= 90)) || ((input >= 97) && (input <= 122)))
		jinput = 12;
	else
	{
		jinput = 13;
	}
	return jinput;
}


