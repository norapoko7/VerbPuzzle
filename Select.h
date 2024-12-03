#pragma once
#include <string>
#include <vector>

using namespace std;

struct Data {
    string stageName; //ステージ名
    string stageFile; //stageファイルのパス
    int mapHeight; //ステージ高さ(マス)
    int mapWidth; //ステージ幅(マス)
    string infKana; //初期文字
    int numKana; //初期文字数数
    bool clear;
    bool getCoin;
    int stageLevel;
};

int calcStageIndex(vector<int> _cursorPos);
int calcStageIndex(int UP, int LR);
int calcTotalCoin();
void changeCursorPos();
void drawHelp_select();
void drawStageDetail();
void drawStageList();
void startStage();
vector<Data> loadStageList(string csvFilePath);
vector<vector<string>> loadVerbList(string csvFilePath);

//更新
void updateSelect();

//描画
void drawSelect();