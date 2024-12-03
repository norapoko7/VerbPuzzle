#include "DxLib.h"
#include "SceneMgr.h"
#include <math.h>

static int mStartTime;      //����J�n����
static int mCount;          //�J�E���^
int gameCount=0;
static float mFps;          //fps
static const int N = 40;	//���ς����T���v����
int FPS = 30;	//�ݒ肵��FPS

extern bool isPrintDebug;

bool Update() {
	if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
		mStartTime = GetNowCount();
	}
	if (mCount == N) { //30�t���[���ڂȂ畽�ς��v�Z����
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
	int tookTime = GetNowCount() - mStartTime;	//������������
	int waitTime = mCount * 1000 / FPS - tookTime;	//�҂ׂ�����
	if (waitTime > 0) {
		Sleep(waitTime);	//�ҋ@
	}
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	SetWindowIconID(333);
	ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�
	SetMainWindowText("Verb Puzzle");
	SetGraphMode(1280, 720, 32);
	while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {//��ʍX�V & ���b�Z�[�W���� & ��ʏ���

		updateScene();  //�X�V
		drawScene();    //�`��

		Update();	//�X�V
		if(isPrintDebug)Draw();		//�f�o�b�O�`��
		Wait();

	}

	DxLib_End(); // DX���C�u�����I������
	return 0;
}