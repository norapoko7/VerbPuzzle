#include "DxLib.h"
#include "SceneMgr.h"
#include <math.h>

static int mStartTime;      //測定開始時刻
static int mCount;          //カウンタ
int gameCount=0;
static float mFps;          //fps
static const int N = 40;	//平均を取るサンプル数
int FPS = 30;	//設定したFPS

extern bool isPrintDebug;

bool Update() {
	if (mCount == 0) { //1フレーム目なら時刻を記憶
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //30フレーム目なら平均を計算する
		int t = GetNowCount();
		mFps = 1000.f / ((t - mStartTime) / (float)N);
		mCount = 0;
		mStartTime = t;
	}
	mCount++;
	gameCount++;
	return true;
}

void Draw() {
	DrawFormatString(1195, 0, GetColor(255, 255, 255), "FPS: %.1f", mFps);
	DrawFormatString(1150, 20, GetColor(255, 255, 255), "count: %d", gameCount);
}

void Wait() {
	int tookTime = GetNowCount() - mStartTime;	//かかった時間
	int waitTime = mCount * 1000 / FPS - tookTime;	//待つべき時間
	if (waitTime > 0) {
		Sleep(waitTime);	//待機
	}
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	SetWindowIconID(333);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //ウィンドウモード変更と初期化と裏画面設定
	SetMainWindowText("Verb Puzzle");
	SetGraphMode(1280, 720, 32);
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {//画面更新 & メッセージ処理 & 画面消去

		updateScene();  //更新
		drawScene();    //描画

		Update();	//更新
		if(isPrintDebug)Draw();		//デバッグ描画
		Wait();

	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}