#include "Config.h"
#include "SceneMgr.h"
#include "DxLib.h"

//更新
void updateConfig() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //ESCキーが押されていたら
        changeScene(sceneSelect);//シーンをセレクトに変更
    }
}

//描画
void drawConfig() {
    DrawString(0, 0, "設定画面です。", GetColor(255, 255, 255));
    DrawString(0, 20, "ESCキーを押すとセレクト画面へ。", GetColor(255, 255, 255));
}