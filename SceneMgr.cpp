#include "DxLib.h"

#include "Title.h"
#include "Select.h"
#include "Game.h"
#include "Menu.h"
#include "Config.h"

#include "SceneMgr.h"

static sceneName scene = sceneTitle;    //シーン管理変数

//更新
void updateScene() {
    switch (scene) {       //シーンによって処理を分岐
    case sceneTitle:    //現在の画面がメニューなら
        updateTitle();   //メニュー画面の更新処理をする
        break;//以下略
    case sceneSelect:
        updateSelect();
        break;
    case sceneGame:
        updateGame();
        break;
    case sceneMenu:
        updateMenu();
        break;
    case sceneConfig:
        updateConfig();
        break;
    }
}

//描画
void drawScene() {
    switch (scene) {      //シーンによって処理を分岐
    case sceneTitle:   //現在の画面がメニュー画面なら
        drawTitle();    //メニュー画面の描画処理をする
        break;//以下略
    case sceneSelect:
        drawSelect();
        break;
    case sceneGame:
        drawGame();
        break;
    case sceneMenu:
        drawMenu();
        break;
    case sceneConfig:
        drawConfig();
        break;
    }
}

// 引数 nextScene にシーンを変更する
void changeScene(sceneName nextScene) {
    scene = nextScene;
}