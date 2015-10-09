#include<iostream>
#include<string>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<random>
#include<thread>
#include<sstream>
using namespace std;
string ChessBoard[20][20];
HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
enum State { UP, DOWN, LEFT, RIGHT} ;

struct Point 
{
	Point(int xx,int yy):x(xx),y(yy){}
	Point() = default;
	int x;
	int y;
};
Point food;
int point = 0;
int SPEED = 500;
int level = 5;
bool hasfood = false;
int direction = 0;
bool canchange = true;
bool operator==(const Point& lhs, const Point& rhs)
{
	if ((lhs.x == rhs.x) && (lhs.y == rhs.y))
	{
		return true;
	}
	return false;
}
struct SnakeNode
{
	SnakeNode(string s = "●", SnakeNode* n = nullptr, SnakeNode* p = nullptr) :ico(s), next(n), previous(p) {}
	~SnakeNode() = default;
	string ico;
	SnakeNode* next;
	SnakeNode* previous;
	Point position;
	Point getNextPosition()
	{
		return this->previous->position;
	}
};
struct Snake
{
	Snake(int dump)
	{
		tail = new SnakeNode;
		Hand = new SnakeNode("★");
		cleanNode = new SnakeNode("  ");
		SnakeNode* node_2 = new SnakeNode;
		SnakeNode* node_3 = new SnakeNode;
		tail->previous = node_2;
		node_2->next = tail;
		node_2->previous = node_3;
		node_3->next = node_2;
		node_3->previous = Hand;
		Hand->next = node_3;
		state = RIGHT;
		tail->next = cleanNode;
		cleanNode->previous = tail;
	}
	void destructSnake()
	{
		SnakeNode* tmpNode = Hand;
		while (tmpNode != nullptr)
		{
			SnakeNode* deleteNode=tmpNode;
			tmpNode = tmpNode->next;
			delete deleteNode;
			
		}
	}
	Snake() = default;
	SnakeNode* cleanNode;
	State state;
	SnakeNode* Hand;
	SnakeNode* tail;
};
void init();
void print();
void up();
void down();
void left();
void right();
void addSnake();
void run();
void setPosition(int x, int y, string str);
void makefood();
bool isinbody(Point &p);
void eatfood();
void catchKey();
void ctrl();
void dead();
void do_run();
void setLevelAndSpeed();


bool isdead = false;
string IntToString(int number);
Snake snk;
void print()
{
	for (int i = 0;i<20;i++)
	{
		for (int j = 0;j<20;j++)
		{
			cout << ChessBoard[i][j];
		}
		cout << endl;
	}
}
void init()
{
	point = 0;
	SPEED = 500;
	level = 5;
	hasfood = false;
	canchange = true;
	isdead = false;
	snk = Snake::Snake(1);
	direction = 0;
	for (int i = 0;i<20;i++)
	{
		for (int j = 0;j<20;j++)
		{
			if (i == 0 && j == 0)
			{
				ChessBoard[i][j] = "┏";
				continue;
			}
			else if (i == 0 && j == 19)
			{
				ChessBoard[i][j] = "┓";
			}
			else if (i == 19 && j == 0)
			{
				ChessBoard[i][j] = "┗";
			}
			else if (i == 19 && j == 19)
			{
				ChessBoard[i][j] = "┛";
			}
			else if (i == 0 || i == 19)
			{
				ChessBoard[i][j] = "─";
			}
			else if (j == 0 || j == 19)
			{
				ChessBoard[i][j] = "│";
			}
			else
			{
				ChessBoard[i][j] = "  ";
			}
		}
	}
	
	addSnake();
	
}
void up()
{
	SnakeNode* tmpNode = snk.cleanNode;
	while (tmpNode != nullptr)
	{
		setPosition(tmpNode->position.y * 2, tmpNode->position.x, tmpNode->ico);
		if (tmpNode == snk.Hand)
		{
			tmpNode->position.x--;
		}
		else
		{
			tmpNode->position = tmpNode->getNextPosition();
		}
		tmpNode = tmpNode->previous;
	}
	eatfood();
}
void down()
{
	SnakeNode* tmpNode = snk.cleanNode;
	while (tmpNode != nullptr)
	{
		setPosition(tmpNode->position.y * 2, tmpNode->position.x, tmpNode->ico);
		if (tmpNode == snk.Hand)
		{
			tmpNode->position.x++;
		}
		else
		{
			tmpNode->position = tmpNode->getNextPosition();
		}
		tmpNode = tmpNode->previous;
	}
	eatfood();
}
void setState()
{
	while (!isdead)
	{
		if (!_kbhit())
		{
			continue;
		}
		if (_getch() == 224)
			direction = _getch();
	}
}
void left()
{
	SnakeNode* tmpNode = snk.cleanNode;
	while (tmpNode != nullptr)
	{
		setPosition(tmpNode->position.y * 2, tmpNode->position.x, tmpNode->ico);
		if (tmpNode == snk.Hand)
		{
			tmpNode->position.y--;
		}
		else
		{
			tmpNode->position = tmpNode->getNextPosition();
		}
		tmpNode = tmpNode->previous;
	}
	eatfood();
}
void right()
{
	SnakeNode* tmpNode = snk.cleanNode;
	while (tmpNode != nullptr)
	{
		setPosition(tmpNode->position.y * 2, tmpNode->position.x, tmpNode->ico);
		if (tmpNode == snk.Hand)
		{
			tmpNode->position.y++;
		}
		else
		{
			tmpNode->position = tmpNode->getNextPosition();
		}
		tmpNode = tmpNode->previous;
	}
	eatfood();
}
void addSnake()
{
	SnakeNode *tmpNode = snk.tail;
	for (int i = 4;tmpNode != nullptr;i++)
	{
		tmpNode->position.x = 7;
		tmpNode->position.y = i;
		tmpNode = tmpNode->previous;
	}
}
void setPosition(int x, int y, string str)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(handle, c);
	cout << str;
}
void makefood()
{
	default_random_engine e(time(NULL));
	uniform_int_distribution<int> dis;
	int food_p_x = dis(e) % 20;
	int food_p_y = dis(e) % 20;
	while (food_p_x == 0 || food_p_y == 0 || food_p_x == 19 || food_p_y == 19 || isinbody(Point(food_p_x, food_p_y)))
	{
		food_p_x = dis(e) % 20;
		food_p_y = dis(e) % 20;
	}
	food = Point(food_p_x, food_p_y);
}
bool isinbody(Point &p)
{
	SnakeNode* tmpNode = snk.cleanNode;
	while (tmpNode != nullptr)
	{
		if (tmpNode->position == p)
			return true;
		tmpNode = tmpNode->previous;
	}
	return false;
}
void eatfood()
{
	if (food == snk.Hand->position)
	{
		SnakeNode* NewNode = new SnakeNode;
		NewNode->previous = snk.tail;
		NewNode->next = snk.cleanNode;
		snk.tail->next = NewNode;
		snk.cleanNode->previous = NewNode;
		snk.tail = NewNode;
		NewNode->position = snk.cleanNode->position;
		point += 100;
		hasfood = false;
	}
	return;
}
void catchKey()
{
	while (!isdead)
	{
		switch (direction)
		{
		case 72: {
			if (snk.state != DOWN&&snk.state != UP&&canchange)
			{
				canchange = false;
				snk.state = UP;
			}

		}break;
		case 77: {
			if (snk.state != LEFT&&snk.state != RIGHT&&canchange)
			{
				canchange = false;
				snk.state = RIGHT;
			}
		};break;
		case 80: {
			if (snk.state != UP&&snk.state != DOWN&&canchange)
			{
				canchange = false;
				snk.state = DOWN;
			}
		};break;
		case 75: {
			if (snk.state != RIGHT&&snk.state != LEFT&&canchange)
			{
				canchange = false;
				snk.state = LEFT;
			}
		}break;
		default:
			break;
		}
	}
}

void dead()
{
	SnakeNode* tmpNode = snk.tail;
	while (tmpNode != snk.Hand)
	{
		if (snk.Hand->position == tmpNode->position)
		{
			isdead = true;
			return;
		}
		tmpNode = tmpNode->previous;
	}
	if (snk.Hand->position.x == 0 || snk.Hand->position.x == 19)
	{
		isdead = true;
		return;
	}
	if (snk.Hand->position.y == 0 || snk.Hand->position.y == 19)
	{
		isdead = true;
		return;
	}
	isdead = false;
}

void do_run()
{
	switch (snk.state)
	{
	case State::UP:up();break;
	case State::DOWN:down();break;
	case State::RIGHT:right();break;
	case State::LEFT:left();break;
	default:
		break;
	}
}

void setLevelAndSpeed()
{
	switch (point)
	{
	case 700: {
		SPEED =400;
		level=4;
	}break;
	case 1400: {
		SPEED = 300;
		level=3;
	}break;
	case 2100: {
		SPEED = 200;
		level = 2;
	}break;
	case 2800: {
		SPEED = 100;
		level=1;
	}break;
	case 3500: {
		SPEED -= 50;
		level=0;
	}break;
	default:
		break;
	}
}


string IntToString(int number)
{
	stringstream ssr;
	ssr << number;
	return ssr.str();
}

void run()
{
	while (!isdead)
	{
		Sleep(SPEED);
		if (!hasfood)
		{
			makefood();
			setPosition(food.y * 2, food.x, "◆");
			setLevelAndSpeed();
			setPosition(0, 21, "你当前的得分是:" + IntToString(point));
			setPosition(0, 22, "你当前的级别是:LEVEL " + IntToString(level));
			hasfood = true;
		}
		do_run();
		canchange = true;
		dead();
	}
}
int main()
{
	while (true)
	{
		init();
		print();
		thread th1(run);
		thread th2(catchKey);
		thread th3(setState);
		th1.join();
		th2.join();
		th3.join();
		setPosition(0, 23, "你已经死亡,你最终的得分是:" + IntToString(point));
		cout << endl;
		cout << "输入y择重新再来，其他输入结束游戏。" << endl;
		char c;
		c = _getch();
		if (c == 'y')
		{
			system("cls");
			snk.destructSnake();
			continue;
		}
		else
		{
			exit(0);
		}
	}
	return 0;
}


