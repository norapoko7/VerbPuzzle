#include "Title.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Sound.h"

int lockKeyCount_title = 0;

int titleImage = 0;

int titleFont = 0;
int titleFont2 = 0;

extern int SEHandle_intoStage;
extern int c_black;
extern int c_white;

//更新
void updateTitle() {
    if (CheckHitKeyAll() && lockKeyCount_title == 0) { //Enterキーが押されていたら
        lockKeyCount_title = 15;
        PlaySoundMem(SEHandle_intoStage, DX_PLAYTYPE_BACK, TRUE);
        changeScene(sceneSelect);//シーンをセレクトに変更
    }
}

//描画
void drawTitle() {
    loadSE();

    if (lockKeyCount_title > 0)lockKeyCount_title--;

    if (titleImage == 0) {
        titleImage = LoadGraph("./data/image/title.png");
        titleFont = CreateFontToHandle("MS ゴシック", 80, 50);
        titleFont2 = CreateFontToHandle("MS ゴシック", 76, 50);
    }

    DrawExtendGraph(0, 0, 1280, 720, titleImage, TRUE);
    //DrawGraph(100, 100, titleImage, TRUE);

    //DrawString(0, 0, "タイトル画面です。", GetColor(255, 255, 255));
    //DrawString(0, 20, "キーを押すとセレクト画面へ。", GetColor(255, 255, 255));
    DrawFormatStringToHandle(390, 620, c_white, titleFont, "PRESS ANY KEY");
    DrawFormatStringToHandle(400, 630, c_black, titleFont2, "PRESS ANY KEY");
}