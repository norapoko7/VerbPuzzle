#include "DxLib.h"
#include <string>
#include "Object.h"
#include "Game.h"

extern int gameCount;
extern Data stageData;
extern vector<vector<string>> stageMap;
extern vector<Object*> objectList;
extern Object* player;

extern bool isMovePlayer;
extern bool isOpenVerbMenu;
extern int keyLockCount;
extern int smoothMove;
extern int maxSmoothMove; //小さいほど早く開く
extern int startTime;
extern bool isSitPlayer;
extern bool isSeeThroughPlayer;

extern int speechBubble;
extern string speechVerb;

int sitPlayerHandle = 0;

extern int SEHandle_walkPlayer;


Object::Object() {
	handle[dire_DOWN][0] = LoadGraph("./data/image/none.png");
	kinds = "object";
}

Object::Object(int _posX, int _posY) {
	name = "";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/none.png");
	through = true;
	kinds = "object";
}

Air::Air(int _posX, int _posY) {
	name = "A";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/air.png");
	through = true;
	kinds = "object";
}

BrokenFlag::BrokenFlag(int _posX, int _posY) {
	name = "B";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/broken_flag.png");
	through = true;
	kinds = "object";
}

Coin::Coin(int _posX, int _posY) {
	name = "C";
	posX = _posX;
	posY = _posY;
	LoadDivGraph("./data/image/coin.png", 8, 4, 2, 32, 32, handle[dire_DOWN].data());
	//handle[dire_DOWN][0] = LoadGraph("./data/image/coin.png");
	through = true;
	kinds = "object";
}

Door::Door(int _posX, int _posY) {
	name = "D";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/door.png");
	through = false;
	kinds = "object";
}

Empty::Empty(int _posX, int _posY) {
	name = "E";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/empty.png");
	through = true;
	kinds = "object";
}

Fill::Fill(int _posX, int _posY) {
	name = "Fi";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/fill.png");
	through = true;
	kinds = "object";
}

Flag::Flag(int _posX, int _posY) {
	name = "F";
	posX = _posX;
	posY = _posY;
	LoadDivGraph("./data/image/flag.png", 8, 4, 2, 32, 32, handle[dire_DOWN].data());
	through = true;
	kinds = "object";
}

Gear::Gear(int _posX, int _posY) {
	name = "G";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/gear.png");
	through = false;
	kinds = "object";
}

Hole::Hole(int _posX, int _posY) {
	name = "H";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/hole.png");
	through = false;
	kinds = "object";
}

Player::Player(int _posX, int _posY) {
	name = "P";
	posX = _posX;
	posY = _posY;
	dire = dire_DOWN;
	LoadDivGraph("./data/image/player_down.png", 19, 4, 5, 32, 32, handle[dire_DOWN].data());
	LoadDivGraph("./data/image/player_left.png", 19, 4, 5, 32, 32, handle[dire_LEFT].data());
	LoadDivGraph("./data/image/player_up.png", 19, 4, 5, 32, 32, handle[dire_UP].data());
	LoadDivGraph("./data/image/player_right.png", 19, 4, 5, 32, 32, handle[dire_RIGHT].data());
	through = false;
	kinds = "player";
}



Rock::Rock(int _posX, int _posY) {
	name = "R";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/rock.png");
	through = false;
	kinds = "object";
}

Tree::Tree(int _posX, int _posY) {
	name = "T";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/tree.png");
	through = false;
	kinds = "object";
}

Wall::Wall(int _posX, int _posY) {
	name = "W";
	posX = _posX;
	posY = _posY;
	handle[dire_DOWN][0] = LoadGraph("./data/image/wall.png");
	through = false;
	kinds = "object";
}

kana_A::kana_A(int _posX, int _posY) {
	name = "あ";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_E::kana_E(int _posX, int _posY) {
	name = "え";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_O::kana_O(int _posX, int _posY) {
	name = "お";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_KI::kana_KI(int _posX, int _posY) {
	name = "き";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_KU::kana_KU(int _posX, int _posY) {
	name = "く";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_KE::kana_KE(int _posX, int _posY) {
	name = "け";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_KO::kana_KO(int _posX, int _posY) {
	name = "こ";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_SU::kana_SU(int _posX, int _posY) {
	name = "す";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_TO::kana_TO(int _posX, int _posY) {
	name = "と";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_NA::kana_NA(int _posX, int _posY) {
	name = "な";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_MA::kana_MA(int _posX, int _posY) {
	name = "ま";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_RU::kana_RU(int _posX, int _posY) {
	name = "る";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}
kana_WA::kana_WA(int _posX, int _posY) {
	name = "わ";
	posX = _posX;
	posY = _posY;
	string handlePath = "./data/image/kana_" + name + ".png";
	handle[dire_DOWN][0] = LoadGraph(handlePath.c_str());
	through = true;
	kinds = "kana";
}

void Object::changeDire(int _dire) {
	updateUndoBuff();
	dire = _dire;
}

void Object::changePos(int dire) {
	switch (dire) {
		case dire_DOWN:
			if (objectList[stageData.mapWidth * (posX + 1) + posY]->through == true || isSeeThroughPlayer) {
				PlaySoundMem(SEHandle_walkPlayer, DX_PLAYTYPE_BACK, TRUE);
				posX += 1;//下
				useVerb(reshapeStringToVector("あるく"));
			}
			return;
		case dire_LEFT:
			if (objectList[stageData.mapWidth * posX + posY - 1]->through == true || isSeeThroughPlayer) {
				PlaySoundMem(SEHandle_walkPlayer, DX_PLAYTYPE_BACK, TRUE);
				posY -= 1;//左
				useVerb(reshapeStringToVector("あるく"));
			}
			return;
		case dire_UP:
			if (objectList[stageData.mapWidth * (posX - 1) + posY]->through == true || isSeeThroughPlayer) {
				PlaySoundMem(SEHandle_walkPlayer, DX_PLAYTYPE_BACK, TRUE);
				posX -= 1;//上
				useVerb(reshapeStringToVector("あるく"));
			}
			return;
		case dire_RIGHT:
			if (objectList[stageData.mapWidth * posX + posY + 1]->through == true || isSeeThroughPlayer) {
				PlaySoundMem(SEHandle_walkPlayer, DX_PLAYTYPE_BACK, TRUE);
				posY += 1;//右
				useVerb(reshapeStringToVector("あるく"));
			}
			return;
		default:
			return;
	}
}

void Object::drawObject(double magRate, int fieldStartX, int fieldStartY) {
	//memo 描画XYとオブジェクトijの方向の違いからXYがpos_@が逆転する
	int fieldOffset = (int)smoothMove * fieldXOffset / maxSmoothMove;

	//start マス目左上座標X,Y
	int squareSX = (int)(fieldStartX + fieldOffset + posY * squareSize * magRate);
	int squareSY = (int)(fieldStartY + posX * squareSize * magRate);
	//end マス目右下座標X,Y
	int squareEX = (int)(fieldStartX + fieldOffset + (posY + 1) * squareSize * magRate - 1);
	int squareEY = (int)(fieldStartY + (posX + 1) * squareSize * magRate - 1);
	if (name == "C") {
		int index = 0;
		double N = 0.3;//回転スピード
		int noMovetime = 90;
		int movetime = -2 * log(8.0 / 7.5 - 1) / N;//2 * (atanh(7.8/4.0 - 1) / N);
		int time = (gameCount - startTime) % (noMovetime + movetime);
		if (time <= noMovetime)index = 0;
		else index = 8 / (1 + exp(-0.3 * (time - noMovetime - movetime / 2)));//(tanh( (time - noMovetime - movetime*(5/8.0))*N) + 1 ) * 4;

		//printfDx("%d -- %f\n", index, 8 / (1 + exp(-0.3 * (time - noMovetime - movetime / 2))));

		DrawExtendGraph(squareSX, squareSY, squareEX, squareEY, handle[dire_DOWN][index], TRUE);
	}
	else if (name == "F") {
		DrawExtendGraph(squareSX, squareSY, squareEX, squareEY, handle[dire_DOWN][gameCount % 24 / 3], TRUE);
	}
	else if (name == "P") {
		if (isSeeThroughPlayer) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
		}

		if (isSitPlayer) {
			if (sitPlayerHandle == 0) {
				sitPlayerHandle = LoadGraph("./data/image/sit.png");
			}
			DrawExtendGraph(squareSX, squareSY, squareEX, squareEY, sitPlayerHandle, TRUE);
		}
		else {
			DrawExtendGraph(squareSX, squareSY, squareEX, squareEY, handle[dire][gameCount % 38 / 2], TRUE);
		}

		if (isSeeThroughPlayer) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		}
	}
	else {
		DrawExtendGraph(squareSX, squareSY, squareEX, squareEY, handle[this->dire][0], TRUE);
	}
}

void Object::moveObject() {
	posX += 1;
}

void Object::movePlayer() {
	
	//"とまるがあったら動けない"
	if (isExistVerb(reshapeStringToVector("とまる"))) {
		speechBubble = 5;
		speechVerb = "とまる";
		return;
	}

	if (isMovePlayer && keyLockCount == 0 && (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT) || CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT))) {
		keyLockCount = 4;

		//同時押しは移動しない
		if ((CheckHitKey(KEY_INPUT_RIGHT)&& CheckHitKey(KEY_INPUT_LEFT)) || (CheckHitKey(KEY_INPUT_UP)&& CheckHitKey(KEY_INPUT_DOWN))) {
			return;
		}

		isSitPlayer = false;

		//左右優先
		bool isPushLShift = CheckHitKey(KEY_INPUT_LSHIFT);
		bool isCanWalk = isExistVerb(reshapeStringToVector("あるく"));
		bool isFieldEdge = false;
		//printfDx("H%d/W%d\n", stageData.mapHeight, stageData.mapWidth);
		//printfDx("%d-%d\n", player->posX, player->posY);
		if (CheckHitKey(KEY_INPUT_RIGHT)) {
			changeDire(dire_RIGHT);
			if (!isPushLShift && isCanWalk && player->posY != stageData.mapWidth - 1) {//シフト押していない & あるくがある & フィールド端でない　なら移動する
				changePos(dire_RIGHT);//右
			}
		}
		else if (CheckHitKey(KEY_INPUT_LEFT)) {
			changeDire(dire_LEFT);
			if (!isPushLShift && isCanWalk && player->posY != 0) {
				changePos(dire_LEFT);//左
			}
		}
		else if (CheckHitKey(KEY_INPUT_UP)) {
			changeDire(dire_UP);
			if (!isPushLShift && isCanWalk && player->posX != 0) {
				changePos(dire_UP);//上
			}
		}
		else if (CheckHitKey(KEY_INPUT_DOWN)) {
			changeDire(dire_DOWN);
			if (!isPushLShift && isCanWalk && player->posX != stageData.mapHeight - 1) {
				changePos(dire_DOWN);//下
			}
		}

	}
}


