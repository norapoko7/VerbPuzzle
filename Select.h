#pragma once
#include <string>
#include <vector>

using namespace std;

struct Data {
    string stageName; //�X�e�[�W��
    string stageFile; //stage�t�@�C���̃p�X
    int mapHeight; //�X�e�[�W����(�}�X)
    int mapWidth; //�X�e�[�W��(�}�X)
    string infKana; //��������
    int numKana; //������������
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

//�X�V
void updateSelect();

//�`��
void drawSelect();