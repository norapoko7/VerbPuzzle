#pragma once

typedef enum {
    sceneTitle,    //タイトル画面
    sceneSelect,   //ステージセレクト画面
    sceneGame,     //ステージ画面
    sceneMenu,     //メニュー画面
    sceneConfig,     //設定画面
} sceneName;

//更新
void updateScene();

//描画
void drawScene();

// 引数 nextScene にシーンを変更する
void changeScene(sceneName nextScene);