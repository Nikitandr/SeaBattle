#include <locale.h>
#include <conio.h>
#include <stdlib.h>
#include <iostream>
#include <ctime>

using namespace std;

const int line = 13, column = 53; // параметры карты

void Instructions(char map[line][column]);
void Draw_Ships(char map[line][column]);
void Draw_Shots(char map[line][column]);
int Player_Move(int&, int&);
int Computer_Move(int& computer_horizontal_coordinate, int& computer_vertical_coordinate);
int Victory();

enum directions { horizontal, vertical }; // направления: горизонтальное, вертикальное

class Players
{
public:
	int hits[10][10]; // массив "выстрелов" объекта
	int ships[10][10]; // массив кораблей объекта
};

class Player_Human : public Players
{
public:
	void Start();
	void Ships_Placement(int deck);
	int Checking_Place(int prow_y, int prow_x, directions direction, int deck);
	void Map_Update(int horizontal, int vertical);
};

class Player_Computer : public Players
{
public:
	void Start();
	void Ships_Placement(int deck);
	int Checking_Place(int prow_y, int prow_x, directions direction, int deck);
	void Map_Update(int horizontal, int vertical);
};

Player_Human human;
Player_Computer computer;

char map[line][column] =
{
"####################################################",
"#  |0|1|2|3|4|5|6|7|8|9|  #   |0|1|2|3|4|5|6|7|8|9|#",
"# 0| | | | | | | | | | |  #  0| | | | | | | | | | |#",
"# 1| | | | | | | | | | |  #  1| | | | | | | | | | |#",
"# 2| | | | | | | | | | |  #  2| | | | | | | | | | |#",
"# 3| | | | | | | | | | |  #  3| | | | | | | | | | |#",
"# 4| | | | | | | | | | |  #  4| | | | | | | | | | |#",
"# 5| | | | | | | | | | |  #  5| | | | | | | | | | |#",
"# 6| | | | | | | | | | |  #  6| | | | | | | | | | |#",
"# 7| | | | | | | | | | |  #  7| | | | | | | | | | |#",
"# 8| | | | | | | | | | |  #  8| | | | | | | | | | |#",
"# 9| | | | | | | | | | |  #  9| | | | | | | | | | |#",
"####################################################"
};

int main()
{
	setlocale(LC_ALL, "rus");

	Instructions(map);
	human.Start();
	computer.Start();

	while (Victory())
	{
		int human_horizontal_coordinate, human_vertical_coordinate;
		int computer_horizontal_coordinate, computer_vertical_coordinate;
		int user_input = 0;

		system("cls");
		cout << ":::ИГРА НАЧАЛАСЬ:::\n" << endl;
		Draw_Ships(map);
		Draw_Shots(map);
		while (Player_Move(human_horizontal_coordinate, human_vertical_coordinate))
		{
			cout << "Введите значение из диапазона от 0 до 9!" << endl;
		}
		Computer_Move(computer_horizontal_coordinate, computer_vertical_coordinate);

		human.Map_Update(human_horizontal_coordinate, human_vertical_coordinate);
		computer.Map_Update(computer_horizontal_coordinate, computer_vertical_coordinate);
	}
	system("pause");
}

// инструкция
void Instructions(char map[line][column])
{
	cout << "\tДобро пожаловать в игру Морской бой!" << endl;
	cout << "Используй цифры 0 - 9 для управления игрой и слушай инструкции." << endl;
	system("pause");
	system("cls");
	for (int i = 0; i < line; i++)
	{
		for (int j = 0; j < column; j++)
		{
			cout << map[i][j];
		}
		cout << endl;
	}
}

// расстановка меток кораблей на карте
void Draw_Ships(char map[line][column])
{

	for (int i = 2; i < 12; i++)
	{
		for (int j = 4; j < 23; j++)
		{
			if (j % 2 == 0) // отсекает половину ячеек (те, в которых на карте проставлены "|")
			{
				if (human.ships[i - 2][(j - 4) / 2] == 2)
				{
					map[i][j] = 'К';
				}
			}
		}
	}
}

// расстановка меток выстрелов на картах, отрисовка карты
void Draw_Shots(char map[line][column])
{
	for (int i = 2; i <= 11; i++)
	{
		//карта игрока
		for (int j = 4; j <= 22; j++)
		{
			if (j % 2 == 0) // отсекает половину ячеек (те, в которых на карте проставлены "|")
			{
				if (computer.hits[i - 2][(j - 4) / 2] == 1 && human.ships[i - 2][(j - 4) / 2] == 3)
				{
					map[i][j] = 'x';
				}
				else if (computer.hits[i - 2][(j - 4) / 2] == 1 && human.ships[i - 2][(j - 4) / 2] != 3)
				{
					map[i][j] = 'o';
				}
			}
		}

		//карта компьютера
		for (int j = 31; j <= 49; j++)
		{
			if (j % 2) // отсекает половину ячеек (те, в которых на карте проставлены "|")
			{
				if (human.hits[i - 2][(j - 31) / 2] == 1 && computer.ships[i - 2][(j - 31) / 2] == 3)
				{
					map[i][j] = 'x';
				}
				else if (human.hits[i - 2][(j - 31) / 2] == 1 && computer.ships[i - 2][(j - 31) / 2] != 3)
				{
					map[i][j] = 'o';
				}
			}
		}
	}

	for (int i = 0; i < line; i++) // отрисовка карты
	{
		for (int j = 0; j < column; j++)
		{
			cout << map[i][j];
		}
		cout << endl;
	}
}

// получение координат выстрела игрока
int Player_Move(int& human_horizontal_coordinate, int& human_vertical_coordinate)
{
	cout << "\nВведите номер строки для выстрела: ";
	cin >> human_horizontal_coordinate; // ввод "горизонтальной" координаты

	if ((human_horizontal_coordinate <= 0) && (human_horizontal_coordinate >= 9))
	{
		return 1;
	}

	cout << "\nВведите номер столбца для выстрела: ";
	cin >> human_vertical_coordinate; // ввод "вертикальной" координаты

	if ((human_vertical_coordinate <= 0) && (human_vertical_coordinate >= 9))
	{
		return 1;
	}

	return 0;
}

// получение координат выстрела компьютера
int Computer_Move(int& computer_horizontal_coordinate, int& computer_vertical_coordinate)
{
	while (1)
	{
		srand(time(0));
		computer_horizontal_coordinate = rand() % 10;
		computer_vertical_coordinate = rand() % 10;
		if (computer.hits[computer_vertical_coordinate][computer_horizontal_coordinate] != 1)
		{
			break;
		}
	}
	return 0;
}

// проверка на победу
int Victory()
{
	int flag_for_human = 0;
	int flag_for_computer = 0;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (human.ships[i][j] == 2)
			{
				flag_for_human = 1; // у пользователя ещё остались неповреждённые корабли
			}
			if (computer.ships[i][j] == 2)
			{
				flag_for_computer = 1; // у компьютера ещё остались неповреждённые корабли
			}
		}
	}
	if (flag_for_human == 0)
	{
		system("cls");
		cout << "\nПОБЕДИЛ ИГРОК!" << endl;
		return 0;
	}
	else if (flag_for_computer == 0)
	{
		system("cls");
		cout << "\nПОБЕДИЛ КОМПЬЮТЕР!" << endl;
		return 0;
	}
	return 1;
}

// заполнение массивов hits и ships, установка кораблей игрока
void Player_Human::Start()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			ships[i][j] = 1;
			hits[i][j] = 0;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (i < 1)
		{
			Ships_Placement(4);
		}
		else if (i < 3)
		{
			Ships_Placement(3);
		}
		else if (i < 6)
		{
			Ships_Placement(2);
		}
		else
		{
			Ships_Placement(1);
		}
	}
}

// заполнение массивов hits и ships, установка кораблей компьютера
void Player_Computer::Start()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			ships[i][j] = 1;
			hits[i][j] = 0;
		}
	}
	for (int i = 0; i < 10; i++)
	{
		if (i < 1)
		{
			Ships_Placement(4);
		}
		else if (i < 3)
		{
			Ships_Placement(3);
		}
		else if (i < 6)
		{
			Ships_Placement(2);
		}
		else
		{
			Ships_Placement(1);
		}
	}
}

// установка кораблей игрока
void Player_Human::Ships_Placement(int deck)
{
	directions direction;
	string napravlenie;
	int prow_x, prow_y;

	cout << "\n:::ПРИСТУПАЕМ К ЗАПОЛНЕНИЮ КАРТЫ ИГРОКА:::\n";

	if (deck == 4)
	{
		cout << "\n\tЧЕТЫРЁХПАЛУБНИК\n\n";
	}
	else if (deck == 3)
	{
		cout << "\n\tТРЁХПАЛУБНИК\n\n";
	}
	else if (deck == 2)
	{
		cout << "\n\tДВУХПАЛУБНИК\n\n";
	}
	else if (deck == 1)
	{
		cout << "\n\tОДНОПАЛУБНИК\n\n";
	}

	cout << "Введите направление расположения корабля ('g' - горизонтальное или 'v' - вертикальное): ";
	napravlenie = _getch();

	if (napravlenie == "g")
	{
		direction = horizontal;
		while (1)
		{
			cout << "\nВведите строку - начало корабля: ";
			cin >> prow_y;
			cout << "Введите столбец - начало корабля: ";
			cin >> prow_x;

			if (Checking_Place(prow_y, prow_x, direction, deck))
			{
				for (int i = 0; i < deck; i++)
				{
					ships[prow_y][prow_x + i] = 2;
				}
				break;
			}
		}
	}
	else if (napravlenie == "v")
	{
		direction = vertical;
		while (1)
		{
			cout << "\nВведите строку - начало корабля: ";
			cin >> prow_y;
			cout << "Введите столбец - начало корабля: ";
			cin >> prow_x;

			if (Checking_Place(prow_y, prow_x, direction, deck))
			{
				for (int i = 0; i < deck; i++)
				{
					ships[prow_y + i][prow_x] = 2;
				}
				break;
			}
		}
	}
	system("cls");
	Draw_Ships(map);
	Draw_Shots(map);
}

// установка кораблей компьютера
void Player_Computer::Ships_Placement(int deck)
{
	directions direction;
	int prow_x, prow_y;

	while (1)
	{
		srand(time(0));
		direction = directions(rand() % 2);
		prow_y = rand() % 10;
		prow_x = rand() % 10;

		if (direction == horizontal)
		{
			if (Checking_Place(prow_y, prow_x, direction, deck))
			{
				for (int i = 0; i < deck; i++)
				{
					ships[prow_y][prow_x + i] = 2;
				}
				break;
			}
		}
		else if (direction == vertical)
		{
			if (Checking_Place(prow_y, prow_x, direction, deck))
			{
				for (int i = 0; i < deck; i++)
				{
					ships[prow_y + i][prow_x] = 2;
				}
				break;
			}
			
		}
	}
}

// проверка размещения корабля игрока
int Player_Human::Checking_Place(int prow_y, int prow_x, directions direction, int deck)
{
	if (direction == horizontal)
	{
		// проверка на то, что корабль не выходит за игровое поле игрока
		if ((prow_x >= 0) && (prow_x + deck - 1 <= 9) && (prow_y >= 0) && (prow_y <= 9))
		{
			for (int i = (prow_y - 1); i <= (prow_y + 1); i++)
			{
				for (int j = (prow_x - 1); j <= (prow_x + deck); j++)
				{
					// проверка, что проверяемая точка надходится в границах массива
					if ((j >= 0) && (j <= 9) && (i >= 0) && (i <= 9))
					{
						if (ships[i][j] == 2)
						{
							cout << "Здесь невозможно разместить корабль!" << endl;
							return 0;
						}
					}
				}
			}
		}
		else
		{
			cout << "Здесь невозможно разместить корабль!" << endl;
			return 0;
		}
	}
	else if (direction == vertical)
	{
		// проверка на то, что корабль не выходит за игровое поле игрока
		if ((prow_x >= 0) && (prow_x <= 9) && (prow_y >= 0) && (prow_y + deck - 1 <= 9))
		{
			for (int i = (prow_x - 1); i <= (prow_x + 1); i++)
			{
				for (int j = (prow_y - 1); j <= (prow_y + deck); j++)
				{
					if ((i >= 0) && (i <= 9) && (j >= 0) && (j <= 9))
					{
						if (ships[j][i] == 2)
						{
							cout << "Здесь невозможно разместить корабль!" << endl;
							return 0;
						}
					}
				}
			}
		}
		else
		{
			cout << "Здесь невозможно разместить корабль!" << endl;
			return 0;
		}
	}
	return 1;
}

// проверка размещения корабля компьютера
int Player_Computer::Checking_Place(int prow_y, int prow_x, directions direction, int deck)
{
	if (direction == horizontal)
	{
		// проверка на то, что начало корабля не выходит за игровое поле
		if ((prow_x >= 0) && (prow_x + deck - 1 <= 9) && (prow_y >= 0) && (prow_y <= 9))
		{
			for (int i = (prow_y - 1); i <= (prow_y + 1); i++)
			{
				for (int j = (prow_x - 1); j <= (prow_x + deck); j++)
				{
					if ((j >= 0) && (j <= 9) && (i >= 0) && (i <= 9))
					{
						if (ships[i][j] == 2)
						{
							return 0;
						}
					}
				}
			}
		}
		else
		{
			return 0;
		}
	}
	else if(direction == vertical)
	{
		// проверка на то, что начало корабля не выходит за игровое поле
		if ((prow_x >= 0) && (prow_x <= 9) && (prow_y >= 0) && (prow_y + deck - 1 <= 9))
		{
			for (int i = (prow_x - 1); i <= (prow_x + 1); i++)
			{
				for (int j = (prow_y - 1); j <= (prow_y + deck); j++)
				{
					if ((i >= 0) && (i <= 9) && (j >= 0) && (j <= 9))
					{
						if (ships[j][i] == 2)
						{
							return 0;
						}
					}
				}
			}
		}
		else
		{
			return 0;
		}
	}
	return 1;
}

// изменения после хода игрока
void Player_Human::Map_Update(int horizontal, int vertical)
{
	hits[horizontal][vertical] = 1;
	
	if (computer.ships[horizontal][vertical] == 2)
	{
		computer.ships[horizontal][vertical] = 3;
	}
}

// ход компьютера, изменения после хода
void Player_Computer::Map_Update(int horizontal, int vertical)
{
	hits[vertical][horizontal] = 1;

	if (human.ships[vertical][horizontal] == 2)
	{
		human.ships[vertical][horizontal] = 3;
	}
}
