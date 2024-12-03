#pragma once
#include "Object.h"
#include "Select.h"
#include <vector>
#include <string>

const int windowSizeX = 1280;
const int windowSizeY = 720;
//マス目大きさ
const int squareSize=32;
//マップmaxマス目
const int squareHeight = 18;
const int squareWidth = 32;

//描画用定数
const int margin = 20;
//field座標
const int fieldWidth = 1240;
const int fieldHeight = 576;
//kanaList
const int kanaListWidth = 1240;
const int kanaListHeight = 64;
//verbList
const int verbListWidth = 196;
const int verbListHeight = 576;

const int fieldCenterX = 640;
const int fieldCenterY = 392;

//offset
const int fieldXOffset = -108;
const int verbXOffset = -196;

//verbMenu幅
const int menuWidth = 212;

using namespace std;

void backSelectScene();
int calcObjectListIndex(int posX, int posY);
int calcTargetIndex(int _posX, int _posY, int _dire);
vector<int> calcTargetPos(int _posX, int _posY, int _dire);
void createKanaList(vector<string> infKana);
string createRandomMsg();
vector<vector<string>> createStageObject(vector<vector<string>> _stageMap);
void createUsableVerbList();
void doUndo();
void drawClearScene();
void drawHighlightKana(vector<string> _verb);
void drawKanaList();
void drawHelp();
void drawUsableVerbList();
void drawUseVerb(string _verb);
int getUsableVerbListIndex(string key);
int getVerbListIndex(string key);
bool getVerbListValue(string key, string kind);
bool isExistInt(vector<int> array, int target);
bool isExistString(vector<vector<string>> array, string target);
bool isExistString(vector<string> array, string target);
bool isExistVerb(vector<string> _verb);
void openVerbMenu();
void printVector(vector<vector<string>> array);
vector<string> reshapeStringToVector(string _str);
string reshapeVectorToString(vector<string> _str);
void retryStage();
void returnSelect();
void setStageData(Data _stageData);
void showDebug(int x, int y);
vector<string> split(const string& str, char delimiter);
vector<string> splitHiragana(const string& verb);
void updateUndoBuff();
void useLastVerb();
void writeStageList(vector<vector<string>> stageList);
void writeVerbList(vector<vector<string>> verbList);
vector<vector<string>> loadObjectList();
vector<vector<string>> loadStageMap(Data _stageData);

//オブジェクト接触処理
void touchCoin();
void touchFlag();
void touchKana();

void doClearProcess();
void doGetCoin(int _posX, int _posY);
void doGetKana(int _posX, int _posY);

//動詞使用処理
void chooseVerb(string verb);
void useVerb(vector<string> _verb);

void coinGet(vector<string> _verb);
void doorKick(vector<string> _verb);
void doorOpen(vector<string> _verb);
void flagFix(vector<string> _verb);
void flagFold(vector<string> _verb);
void getKana(vector<string> _verb);
void objectErase(vector<string> _verb);
void playerSeeThrough(vector<string> _verb);
void playerSit(vector<string> _verb);
void rockCrack(vector<string> _verb);
void rockJump(vector<string> _verb);
void rockPush(vector<string> _verb);
void rockPushFill(vector<string> _verb);
void sortUsableVerbList();
void treeCut(vector<string> _verb);
void treeFold(vector<string> _verb);
void gearRotate(vector<string> _verb);
void walkPlayer(vector<string> _verb);

//更新
void updateGame();

//描画
void drawGame();


struct kanaUnit {
    string kana = {};
    int numTimes = 0 ;
    int handle = 0;
};