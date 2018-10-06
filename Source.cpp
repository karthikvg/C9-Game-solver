#include<iostream>
#include<deque>
#include<string>
#include<algorithm>
#include<fstream>
#define endl '\n'
using namespace std;

struct TrieNode
{
	TrieNode* a[10];
	int flag;
};

struct Board
{
	string s[3];
	int x, y;
	string parent;
	int level;
};

string getCurrentPattern(Board& board)
{
	string temp;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			temp += board.s[i][j];
		}
	}
	return temp;
}

void displayBoard(Board& board)
{
	cout << "---------------------------------" << endl;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << board.s[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
	cout << board.x << " " << board.y << endl;
	cout << board.parent << endl;
	cout << board.level << endl;
	return;
}

TrieNode* createTrieNode()
{
	TrieNode* temp = new TrieNode;
	for (int i = 0; i < 10; i++)
	{
		temp->a[i] = NULL;
	}
	temp->flag = false;
	return temp;
}

TrieNode* inserting(TrieNode* root, Board& board)
{
	string s = getCurrentPattern(board);
	TrieNode* temp = root;
	for (int i = 0; i < s.size(); i++)
	{
		if (temp->a[s[i] - '0'] == NULL)
		{
			temp->a[s[i] - '0'] = createTrieNode();
		}
		temp = temp->a[s[i] - '0'];
	}
	temp->flag = true;
	return root;
}

bool finding(TrieNode* root, Board& board)
{
	string s = getCurrentPattern(board);
	TrieNode* temp = root;
	for (int i = 0; i < s.size(); i++)
	{
		if (temp->a[s[i] - '0'] == NULL)
			return false;
		temp = temp->a[s[i] - '0'];
	}
	return temp&&temp->flag;
}

bool isLeftMovePossible(Board& board)
{
	if (board.y == 0)
		return false;
	return true;
}

bool isRightMovePossible(Board& board)
{
	if (board.y == 2)
		return false;
	return true;
}

bool isDownMovePossible(Board& board)
{
	if (board.x == 2)
		return false;
	return true;
}

bool isUpMovePossible(Board& board)
{
	if (board.x == 0)
		return false;
	return true;
}

Board moveLeft(Board& board)
{
	Board temp = board;
	swap(temp.s[temp.x][temp.y - 1], temp.s[temp.x][temp.y]);
	temp.parent = getCurrentPattern(board);
	temp.level = board.level + 1;
	temp.y = board.y - 1;
	return temp;
}

Board moveRight(Board& board)
{
	Board temp = board;
	swap(temp.s[temp.x][temp.y + 1], temp.s[temp.x][temp.y]);
	temp.parent = getCurrentPattern(board);
	temp.level = board.level + 1;
	temp.y = board.y + 1;
	return temp;
}

Board moveUp(Board& board)
{
	Board temp = board;
	swap(temp.s[temp.x - 1][temp.y], temp.s[temp.x][temp.y]);
	temp.parent = getCurrentPattern(board);
	temp.level = board.level + 1;
	temp.x = board.x - 1;
	return temp;
}

Board moveDown(Board& board)
{
	Board temp = board;
	swap(temp.s[temp.x + 1][temp.y], temp.s[temp.x][temp.y]);
	temp.parent = getCurrentPattern(board);
	temp.level = board.level + 1;
	temp.x = board.x + 1;
	return temp;
}

void writeBoardToFile(Board& board)
{
	ofstream fout;
	fout.open(getCurrentPattern(board) + ".txt");
	fout << board.s[0] << endl << board.s[1] << endl << board.s[2] << endl;
	fout << board.x << " " << board.y << endl;
	fout << board.parent << endl;
	fout << board.level << endl;
	fout.close();
}

Board readBoardFromFile(string s)
{
	ifstream fin;
	fin.open(s + ".txt");
	if (fin.is_open()){
		Board temp;
		fin >> temp.s[0] >> temp.s[1] >> temp.s[2];
		fin >> temp.x >> temp.y;
		fin >> temp.parent;
		fin >> temp.level;
		fin.close();
		return temp;
	}
	else
		throw "exception had occured no file found";

}

void preprocess()
{
	Board temp = { { "123", "456", "780" }, 2, 2, "", 0 };
	displayBoard(temp);


	deque<Board> d;
	d.push_back(temp);

	TrieNode* root = createTrieNode();
	inserting(root, temp);

	int flag = 0;
	while (!d.empty())
	{
		temp = d.front(); d.pop_front();
		if (isLeftMovePossible(temp))
		{
			Board left = moveLeft(temp);
			if (finding(root, left) == false)
			{
				inserting(root, left);
				d.push_back(left);
			}
		}

		if (isRightMovePossible(temp))
		{
			Board right = moveRight(temp);
			if (finding(root, right) == false)
			{
				inserting(root, right);
				d.push_back(right);
			}
		}

		if (isUpMovePossible(temp))
		{
			Board up = moveUp(temp);
			if (finding(root, up) == false)
			{
				inserting(root, up);
				d.push_back(up);
			}
		}

		if (isDownMovePossible(temp))
		{
			Board down = moveDown(temp);
			if (finding(root, down) == false)
			{
				inserting(root, down);
				d.push_back(down);
			}
		}
		flag++;
		if (flag % 200 == 0)
			cout << flag << endl;
		writeBoardToFile(temp);
	}
}
void query()
{
	cout << "enter the initial pattern" << endl;
	string s;
	cin >> s;
	cout << endl;
	int moves = 1;
	try{
		while (true)
		{
			Board board = readBoardFromFile(s);
			cout << "move count:" << moves++ << endl;
			displayBoard(board);
			s = board.parent;
			if (board.level == 1)
			{
				break;
			}
		}
	}
	catch (...)
	{
		cout << "invalid board configuration" << endl;
	}
	system("pause");
}
int main()
{
	//preprocess();
	query();
	return 0;
}
