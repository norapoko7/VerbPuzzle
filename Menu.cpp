#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"

//更新
void updateMenu() {
    if (CheckHitKey(KEY_INPUT_2) != 0) { //Enterキーが押されていたら
        changeScene(sceneSelect);               //シーンをセレクトに変更
    }
    else if (CheckHitKey(KEY_INPUT_3) != 0) { //Escキーが押されていたら
        changeScene(sceneGame);               //シーンをゲームに変更
    }
}

//描画
void drawMenu() {
    DrawString(0, 0, "ゲーム画面です。", GetColor(255, 255, 255));
    DrawString(0, 20, "2キーを押すとセレクト画面へ。", GetColor(255, 255, 255));
    DrawString(0, 40, "3キーを押すとゲーム画面へ。", GetColor(255, 255, 255));
}