#include <DxLib.h>
#include <string>
#include "Sound.h"

string path = "./data/sound/";


//システム
int SEHandle_closeMenu = 0;
int SEHandle_exitStage = 0;
int SEHandle_intoStage = 0;
int SEHandle_moveCursor = 0;
int SEHandle_openMenu = 0;
int SEHandle_restartStage = 0;

//ステージ内
int SEHandle_cantUse = 0;
int SEHandle_clearStage = 0;
int SEHandle_clearStageCoin = 0;

int SEHandle_cutTree = 0;
int SEHandle_crackRock = 0;
int SEHandle_deleteKana = 0;
int SEHandle_doUndo = 0;
int SEHandle_eraseObject = 0;
int SEHandle_fixFlag = 0;
int SEHandle_foldFlag = 0;
int SEHandle_getCoin = 0;
int SEHandle_getKana = 0;
int SEHandle_jumpRock = 0;
int SEHandle_kickDoor = 0;
int SEHandle_makeVerb = 0;
int SEHandle_makeVerbAlready = 0;
int SEHandle_openDoor = 0;
int SEHandle_pushRock = 0;
int SEHandle_rotateGear = 0;
int SEHandle_selectKana = 0;
int SEHandle_sitPlayer = 0;
int SEHandle_walkPlayer = 0;


void loadSE() {
	if (SEHandle_openDoor == 0) {
		// MP3ファイルが入っているディレクトリのパス
		SEHandle_closeMenu = LoadSoundMem("./data/sound/closeMenu.mp3");
		SEHandle_exitStage = LoadSoundMem("./data/sound/exitStage.mp3");
		SEHandle_intoStage = LoadSoundMem("./data/sound/intoStage.mp3");
		SEHandle_moveCursor = LoadSoundMem("./data/sound/moveCursor.mp3");
		SEHandle_openMenu = LoadSoundMem("./data/sound/openMenu.mp3");
		SEHandle_restartStage = LoadSoundMem("./data/sound/restartStage.mp3");

		SEHandle_cantUse = LoadSoundMem("./data/sound/cantUse.mp3");
		SEHandle_clearStage = LoadSoundMem("./data/sound/clearStage.mp3");
		SEHandle_clearStageCoin = LoadSoundMem("./data/sound/clearStageCoin.mp3");

		SEHandle_cutTree = LoadSoundMem("./data/sound/cutTree.mp3");
		SEHandle_crackRock = LoadSoundMem("./data/sound/crackRock.mp3");
		SEHandle_deleteKana = LoadSoundMem("./data/sound/deleteKana.mp3");
		SEHandle_doUndo = LoadSoundMem("./data/sound/doUndo.mp3");
		SEHandle_eraseObject = LoadSoundMem("./data/sound/eraseObject.mp3");
		SEHandle_fixFlag = LoadSoundMem("./data/sound/fixFlag.mp3");
		SEHandle_foldFlag = LoadSoundMem("./data/sound/foldFlag.mp3");
		SEHandle_getCoin = LoadSoundMem("./data/sound/getCoin.mp3");
		SEHandle_getKana = LoadSoundMem("./data/sound/getKana.mp3");
		SEHandle_jumpRock = LoadSoundMem("./data/sound/jumpRock.mp3");
		SEHandle_kickDoor = LoadSoundMem("./data/sound/kickDoor.mp3");
		SEHandle_makeVerb = LoadSoundMem("./data/sound/makeVerb.mp3");
		SEHandle_makeVerbAlready = LoadSoundMem("./data/sound/makeVerbAlready.mp3");
		SEHandle_openDoor = LoadSoundMem("./data/sound/openDoor.mp3");
		SEHandle_pushRock = LoadSoundMem("./data/sound/pushRock.mp3");
		SEHandle_rotateGear = LoadSoundMem("./data/sound/rotateGear.mp3");
		SEHandle_selectKana = LoadSoundMem("./data/sound/selectKana.mp3");
		SEHandle_sitPlayer = LoadSoundMem("./data/sound/sitPlayer.mp3");
		SEHandle_walkPlayer = LoadSoundMem("./data/sound/walkPlayer.mp3");
	}
}
