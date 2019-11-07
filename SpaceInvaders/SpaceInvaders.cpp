// SpaceInvaders.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <vector>
#include <thread>
#include <cstdlib>
#include <iterator>
#include <fstream>
#include <string>
#include <stdio.h>
#include <sstream>
#include <algorithm>

using namespace std;
struct Record {
	string Name = "";
	int points = 0;
};
struct Obj {
	int x;
	int y;
	bool isAlive;
	char spr;
};
int lives;
wchar_t* screen;
int score;
HANDLE console;
bool reset = false;
vector < Record > scoresVector;
vector < Obj > ::iterator enemyIterator;
vector < Obj > ::iterator enemyAttackIterator;
int nSpeedCount;
int enemyMovementVariable;
int enemyCount;
bool fire, isPlaying;
int enemySpeed;
int fx = NULL;
int fy = NULL;
int howHard = NULL;
int x = 10;
int y = 20;
//Enemy* enemies = NULL;
vector < Obj > enems;
vector < Obj > shoots;

void input() {
	if (_kbhit()) {
		switch (_getch()) {
		case 'a':
			//if (x > 1)
			x--;
			break;
		case 'd':
			//if (x <= width - 3)
			x++;
			break;
		case 'w':
			fire = true;
			fy = NULL;
			fx = NULL;
			//y--;
			break;
		case 's':
			//fire = true;
			//y++;
			break;
		default:
			break;
		}
	}
}
void initShot() {
	for (int i = 1; i <= howHard; i++) {
		Obj* temp = new Obj;
		temp -> isAlive = false;
		temp -> x = (i % 10) * 2 + 1;
		temp -> y = (i / 11) * 2;
		temp -> spr = 'o';
		shoots.push_back(*temp);
	}
}
void initEnemy() {
	enemySpeed = 25;
	enemyCount = 30;
	reset = false;
	for (int i = 0; i < 30; i++) {
		Obj* temp = new Obj;
		temp -> isAlive = true;
		temp -> x = (i % 10) * 2 + 1;
		temp -> y = (i / 10) * 2 + 2;
		temp -> spr = 'V';
		enems.push_back(*temp);
	}
}

void logic() {
	//PLAYER MOVEMENT
	input();
	//PLAYER SHOOTING LOGIC
	if (fire && fx == NULL) {
		fx = x;
		fy = y;
	}
	fy--;
	if (fy <= 0) {
		fire = false;
		fx = NULL;
		fy = NULL;
	}
	//CHECK HIT
	for (enemyIterator = enems.begin(); enemyIterator != enems.end(); enemyIterator++) {
		//Just to check if enemy y==player y
		if (enemyIterator -> y >= y && enemyIterator -> isAlive) isPlaying = false;
		if ((enemyIterator -> y == fy && enemyIterator -> x == fx) && enemyIterator -> isAlive) {
			enemyIterator -> isAlive = false;
			fire = false;
			fx = NULL;
			fy = NULL;
			enemyCount--;
			score += 3;
			Beep(523, 100);
		}
	}
	if (enemyCount == 0) reset = true;
	if (reset) {
		enems.clear();
		initEnemy();
	}
	//GAME BOUNDARY FOR PLAYER
	if (x <= 1) x = 1;
	else if (x >= 25) x = 25;
	//ENEMY MOVEMENT
	if (nSpeedCount % enemySpeed == 0) {
		for (enemyIterator = enems.begin(); enemyIterator != enems.end(); enemyIterator++) {
			if (enemyIterator -> x <= 1 && enemyIterator -> isAlive) {
				enemyMovementVariable = 1;
				if (enemySpeed > 5) enemySpeed -= 1;
				vector < Obj > ::iterator e;
				for (e = enems.begin(); e != enems.end(); e++)
					e -> y++;
			}
			else if (enemyIterator -> x >= 25 && enemyIterator -> isAlive) {
				enemyMovementVariable = -1;
				if (enemySpeed > 5) enemySpeed -= 1;
				vector < Obj > ::iterator e;
				for (e = enems.begin(); e != enems.end(); e++)
					e -> y++;
			}

		}
		for (enemyIterator = enems.begin(); enemyIterator != enems.end(); enemyIterator++) {
			enemyIterator -> x += enemyMovementVariable;
		}
	}

	//ENEMY SHOOT LOGIC
	for (enemyAttackIterator = shoots.begin(); enemyAttackIterator != shoots.end(); enemyAttackIterator++) {
		if (nSpeedCount % 200 == 0) {
			if (!enemyAttackIterator -> isAlive) {
				int temp = rand() % 29 + 0;
				Obj tempEnemy = enems[temp];
				while (tempEnemy.isAlive != true) {
					temp = rand() % 29 + 0;
					tempEnemy = enems[temp];
				}
				enemyAttackIterator -> isAlive = true;
				enemyAttackIterator -> x = tempEnemy.x;
				enemyAttackIterator -> y = tempEnemy.y;
			}
		}
		if (enemyAttackIterator -> x == x && enemyAttackIterator -> y == y) {
			lives--;
			//Beep(392, 100);
		}
		if (enemyAttackIterator -> y >= y) {
			enemyAttackIterator -> isAlive = false;
			enemyAttackIterator -> x = NULL;
			enemyAttackIterator -> y = NULL;
		}
		if (enemyAttackIterator -> isAlive) enemyAttackIterator -> y++;
	}
	//CHECK AMOUNT OF LIVES
	if (lives <= 0) isPlaying = false;
	//
	//INCREASE SPEED COUNT
	nSpeedCount++;
	//RESET SPEED COUNT
	if (nSpeedCount >= 1000) nSpeedCount = 0;
}

void draw() {
	for (int i = 0; i < 80 * 30; i++) screen[i] = L' ';
	screen[80 * y + x] = L'A';
	//DRAW BOUNDARY
	for (int i = 0; i < 80 * 30; i++) {
		if (i < 46) screen[i] = L'#';
		if (i % 80 == 0 && i < 80 * (y + 2)) screen[i] = L'|';
		if (i % 80 == 0 && i < 80 * (y + 2)) screen[i + 26] = L'|';
		if (i % 80 == 0 && i < 80 * (y + 2)) screen[i + 46] = L'|';
		if (i % 80 < 46) screen[(y + 1) * 80 + (i % 80)] = L'#';
	}

	//DRAW ENEMIES

	for (enemyIterator = enems.begin(); enemyIterator != enems.end(); enemyIterator++) {
		if (enemyIterator -> isAlive) screen[80 * enemyIterator -> y + enemyIterator -> x] = enemyIterator -> spr;
	}
	//DRAW BULLETS
	for (enemyIterator = shoots.begin(); enemyIterator != shoots.end(); enemyIterator++) {
		if (enemyIterator -> isAlive) screen[80 * enemyIterator -> y + enemyIterator -> x] = enemyIterator -> spr;
	}
	if (fire) screen[80 * fy + fx] = L'.';

	//DRAW SCORE
	swprintf_s(&screen[80 * 3 + 32], 8 + to_string(score).length(), L"SCORE: %d", score);
	//setlocale(LC_ALL, "pl_PL.utf8");
	//for (int i = 0; i < scoresVector.size() / 3; i++) {
	//	string tempText = (scoresVector.at(i) + " " + scoresVector.at(i + 1) + " " + scoresVector.at(i + 2));
	//	swprintf_s(&screen[2 * 80 + 22 + 6], 16, L"SCORE: %s", scoresVector.at(6));
	//	swprintf_s(&screen[80 * (4 + i) + 32], (scoresVector.at(i).length() + scoresVector.at(i + 1).length() + scoresVector.at(i + 2).length()), L"%s",tempText);
	//}

	//DRAW HEART
	for (int i = 0; i < lives; i++)
		swprintf_s(&screen[80 * 4 + 32 + i], 2, L"\u2665");
}

void music() {
	/*	Beep(598.5, 500);//F
	  Beep(780.0, 300);//a
	  Beep(623.3, 500);//C
	  Beep(780.0, 400);//a
	  Beep(598.5, 300);//F
	  Beep(487.3, 300);//d
	  Beep(487.3, 300);//d
	  Beep(487.3, 300);//d
	  */

	Beep(330, 100);
	Sleep(100);
	Beep(330, 100);
	Sleep(300);
	Beep(330, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(100);
	Beep(330, 100);
	Sleep(300);
	Beep(392, 100);
	Sleep(700);
	Beep(196, 100);
	Sleep(700);
	Beep(262, 300);
	Sleep(300);
	Beep(196, 300);
	Sleep(300);
	Beep(164, 300);
	Sleep(300);
	Beep(220, 300);
	Sleep(100);
	Beep(246, 100);
	Sleep(300);
	Beep(233, 200);
	Beep(220, 100);
	Sleep(300);
	Beep(196, 100);
	Sleep(150);
	Beep(330, 100);
	Sleep(150);
	Beep(392, 100);
	Sleep(150);
	Beep(440, 100);
	Sleep(300);
	Beep(349, 100);
	Sleep(100);
	Beep(392, 100);
	Sleep(300);
	Beep(330, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(100);
	Beep(294, 100);
	Sleep(100);
	Beep(247, 100);
	Sleep(500);
	Beep(262, 300);
	Sleep(300);
	Beep(196, 300);
	Sleep(300);
	Beep(164, 300);
	Sleep(300);
	Beep(220, 300);
	Sleep(100);
	Beep(246, 100);
	Sleep(300);
	Beep(233, 200);
	Beep(220, 100);
	Sleep(300);
	Beep(196, 100);
	Sleep(150);
	Beep(330, 100);
	Sleep(150);
	Beep(392, 100);
	Sleep(150);
	Beep(440, 100);
	Sleep(300);
	Beep(349, 100);
	Sleep(100);
	Beep(392, 100);
	Sleep(300);
	Beep(330, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(100);
	Beep(294, 100);
	Sleep(100);
	Beep(247, 100);
	Sleep(900);
	Beep(392, 100);
	Sleep(100);
	Beep(370, 100);
	Sleep(100);
	Beep(349, 100);
	Sleep(100);
	Beep(311, 100);
	Sleep(300);
	Beep(330, 100);
	Sleep(300);
	Beep(207, 100);
	Sleep(100);
	Beep(220, 100);
	Sleep(100);
	Beep(262, 100);
	Sleep(300);
	Beep(220, 100);
	Sleep(100);
	Beep(262, 100);
	Sleep(100);
	Beep(294, 100);
	Sleep(500);
	Beep(392, 100);
	Sleep(100);
	Beep(370, 100);
	Sleep(100);
	Beep(349, 100);
	Sleep(100);
	Beep(311, 100);
	Sleep(300);
	Beep(330, 100);
	Sleep(300);
	Beep(523, 100);
	Sleep(300);
	Beep(523, 100);
	Sleep(100);
	Beep(523, 100);
	Sleep(1100);
	Beep(392, 100);
	Sleep(100);
	Beep(370, 100);
	Sleep(100);
	Beep(349, 100);
	Sleep(100);
	Beep(311, 100);
	Sleep(300);
	Beep(330, 100);
	Sleep(300);
	Beep(207, 100);
	Sleep(100);
	Beep(220, 100);
	Sleep(100);
	Beep(262, 100);
	Sleep(300);
	Beep(220, 100);
	Sleep(100);
	Beep(262, 100);
	Sleep(100);
	Beep(294, 100);
	Sleep(500);
	Beep(311, 300);
	Sleep(300);
	Beep(296, 300);
	Sleep(300);
	Beep(262, 300);
	Sleep(1300);
	Beep(262, 100);
	Sleep(100);
	Beep(262, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(100);
	Beep(294, 100);
	Sleep(300);
	Beep(330, 200);
	Sleep(50);
	Beep(262, 200);
	Sleep(50);
	Beep(220, 200);
	Sleep(50);
	Beep(196, 100);
	Sleep(700);
	Beep(262, 100);
	Sleep(100);
	Beep(262, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(100);
	Beep(294, 100);
	Sleep(100);
	Beep(330, 100);
	Sleep(700);
	Beep(440, 100);
	Sleep(300);
	Beep(392, 100);
	Sleep(500);
	Beep(262, 100);
	Sleep(100);
	Beep(262, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(300);
	Beep(262, 100);
	Sleep(100);
	Beep(294, 100);
	Sleep(300);
	Beep(330, 200);
	Sleep(50);
	Beep(262, 200);
	Sleep(50);
	Beep(220, 200);
	Sleep(50);
	Beep(196, 100);
	Sleep(700);
}
void readScores() {
	string line;
	ifstream file("Scores.txt");
	if (file.is_open()) {
		while (getline(file, line)) {
			istringstream strema(line);
			vector < string > tempLine;
			string tempPart;
			while (getline(strema, tempPart, ',')) {
				//cout << tempPart<<endl;
				tempLine.push_back(tempPart);
			}
			Record tempRecord;
			tempRecord.Name = tempLine.at(1);
			tempRecord.points = stoi(tempLine.at(2));
			//cout << tempRecord.Name<<" "<<tempRecord.points;
			scoresVector.push_back(tempRecord);
		}

		file.close();
	}
	//cout << scoresVector.size();
}
void saveScores() {
	ofstream scores("Scores.txt");
	for (int i = 0; i < scoresVector.size(); i++) {
		//cout << i+1 << ',' << scoresVector.at(i).Name << ',' << scoresVector.at(i).points << "\n";
		scores << i + 1 << ',' << scoresVector.at(i).Name << ',' << scoresVector.at(i).points << "\n";
	}
	scores.close();
}
void addNew() {
	cout << "JAK SIE ZWIERZ?\n";
	string tempName;
	cin >> tempName;
	Record playerRecord;
	playerRecord.Name = tempName;
	playerRecord.points = score;
	bool done = false;
	if (scoresVector.begin() -> points < playerRecord.points) {
		scoresVector.insert(scoresVector.begin(), playerRecord);
		done = true;
	}
	if (!done) {
		auto position = find_if(scoresVector.begin(), scoresVector.end(), [playerRecord](auto record) {
			return record.points < playerRecord.points;
			});
		scoresVector.insert(position, playerRecord);
		done = true;
	}
	if (!done) {
		scoresVector.push_back(playerRecord);
	}
}

void init() {
	score = 0;
	lives = 3;
	readScores();
	while (howHard == NULL) {
		cout << "How Hard? \nEasy -> 2 shoots \nMedium -> 4 shoots \nHard -> 8 shoots\n";
		char temp;
		cin >> temp;
		switch (temp) {
		case 'e':
			howHard = 2;
			break;
		case 'm':
			howHard = 4;
			break;
		case 'h':
			howHard = 8;
			break;
		default:
			temp = NULL;
			break;
		}
	}
	initShot();
	initEnemy();
	fire = false;
	isPlaying = true;
	enemyMovementVariable = 1;
	nSpeedCount = 0;
	screen = new wchar_t[80 * 30];
	console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
}

int main() {
	//thread th1(music, 2);
	//music();
	//th1.join();
	init();
	DWORD bytesWritten = 0;
	while (isPlaying) {
		Sleep(50);
		logic();
		draw();
		WriteConsoleOutputCharacter(console, screen, 80 * 30, {0,0}, &bytesWritten);
	}
	CloseHandle(console);
	//cout << score;
	addNew();
	saveScores();

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file