#pragma once

#include "DxLib.h"
#include "Game.h"
#include <string>
#include <vector>

#define dire_DOWN  0
#define dire_LEFT  1
#define dire_UP    2
#define dire_RIGHT 3

using namespace std;

class Object {
	public:
		string name = "";
		int posX=0;
		int posY=0;
		int dire = dire_DOWN;
		vector<vector<int>> handle = { {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0},
									   {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0},
									   {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0},
									   {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0} };
		bool through = true;
		string kinds = "";

		Object();
		Object(int _posX, int _posY);
		void drawObject(double magRate, int fieldStartX, int fieldStartY);
		void moveObject();
		void movePlayer();
		void changePos(int dire);
		void changeDire(int _dire);
};

class Air : public Object {
	public:
		Air(int _posX, int _posY);
};

class BrokenFlag : public Object {
	public:
		BrokenFlag(int _posX, int _posY);
};

class Coin : public Object {
	public:
		Coin(int _posX, int _posY);
};

class Door : public Object {
	public:
		Door(int _posX, int _posY);
};

class Empty : public Object {
	public:
		Empty(int _posX, int _posY);
};

class Fill : public Object {
	public:
		Fill(int _posX, int _posY);
};

class Flag : public Object {
	public:
		Flag(int _posX, int _posY);
};

class Gear : public Object {
	public:
		Gear(int _posX, int _posY);
};

class Hole : public Object {
	public:
		Hole(int _posX, int _posY);
};

class Player : public Object {
	public:
		Player(int _posX, int _posY);
};

class Rock : public Object {
	public:
		Rock(int _posX, int _posY);
};

class Tree : public Object {
	public:
		Tree(int _posX, int _posY);
};

class Wall : public Object {
	public:
		Wall(int _posX, int _posY);
};

class kana_A : public Object {
	public:
		kana_A(int _posX, int _posY);
};
class kana_E : public Object {
	public:
		kana_E(int _posX, int _posY);
};
class kana_O : public Object {
	public:
		kana_O(int _posX, int _posY);
};
class kana_KI : public Object {
	public:
		kana_KI(int _posX, int _posY);
};
class kana_KU : public Object {
	public:
		kana_KU(int _posX, int _posY);
};
class kana_KE : public Object {
	public:
		kana_KE(int _posX, int _posY);
};
class kana_KO : public Object {
	public:
		kana_KO(int _posX, int _posY);
};
class kana_SU : public Object {
	public:
		kana_SU(int _posX, int _posY);
};
class kana_TO : public Object {
	public:
		kana_TO(int _posX, int _posY);
};
class kana_NA : public Object {
	public:
		kana_NA(int _posX, int _posY);
};
class kana_MA : public Object {
	public:
		kana_MA(int _posX, int _posY);
};
class kana_RU : public Object {
	public:
		kana_RU(int _posX, int _posY);
};
class kana_WA : public Object {
	public:
		kana_WA(int _posX, int _posY);
};