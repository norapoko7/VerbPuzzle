#include "Select.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Game.h"
#include <algorithm>


string stageListPath = "./data/stage/stages.csv";
string verbListPath = "./data/verbs.csv";

vector<Data> stageList = {};
vector<vector<string>> verbList = {};
vector<int> stageIconHandle = {};
int coinHandle = 0;

int cursorStageList = 0;
int keyLockCount_select = 15;
vector<int> selectCursorPos = { 0,0 };
int totalCoin = 0;

int fontHandleNormal_select = 0;
int fontHandleHelp_select = 0;
int fontHandleDetail_stageNum = 0;
int fontHandleDetail_title = 0;
int fontHandleDetail_level = 0;
int fontHandleCoin = 0;
int fontHandleCoin2 = 0;

int stageIconSize = 170;
int stageIconPadding = 22;
int iconRow = 3; //3*3

int smoothMax = 170;
int smooth = smoothMax;

extern int gameCount;
extern int startTime;

extern int c_white;
extern int c_black;
extern int c_red;
extern int c_green;
extern int c_blue;
extern int c_lightBlue;
int c_clear = GetColor(0, 200, 255);
int c_lightGray = GetColor(230, 230, 230);

extern int SEHandle_intoStage;
extern int SEHandle_exitStage;
extern int SEHandle_moveCursor;
extern int SEHandle_cantUse;

//更新
void updateSelect() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) && keyLockCount_select == 0) { //ESCキーが押されていたら
        keyLockCount_select = 15;
        PlaySoundMem(SEHandle_exitStage, DX_PLAYTYPE_BACK, TRUE);
        changeScene(sceneTitle);               //シーンをタイトルに変更
        smooth = smoothMax;
    }
}

//描画
void drawSelect() {
    //最初だけロード
    if (stageList.empty()) {
        //printfDx("データ読み込み\n");//デバッグ用
        stageList = loadStageList(stageListPath);
        verbList = loadVerbList(verbListPath);

        for (int i = 0; i < stageList.size(); i++) {
            string iconPath = "./data/image/stageIcon" + to_string(i+1) + ".png";
            //printfDx("%s\n", iconPath.c_str());
            stageIconHandle.push_back(LoadGraph(iconPath.c_str()));
        }
        coinHandle = LoadGraph("./data/image/selectCoin.png");
    }

    //操作受付
    changeCursorPos();
    startStage();

    //描画　更新
    drawStageList();
    drawHelp_select();
    drawStageDetail();

    //マイフレーム更新
    if (keyLockCount_select > 0)keyLockCount_select--;
    //printfDx("%d\n", keyLockCount_select);
    

    //DrawString(0, 0, "ステージセレクト画面です。", c_white);


    
}

int calcStageIndex(vector<int> _cursorPos) {
    int posUP = _cursorPos[0] % iconRow;
    int posLR = _cursorPos[1] % iconRow;

    //00 0
    //01 1
    //02 2
    //10 7
    //11 8
    //12 3
    //20 6
    //21 5
    //22 4
    if (posUP == 0) {
        if (posLR == 0) { return 0; }
        else if (posLR == 1) { return 1; }
        else if (posLR == 2) { return 2; }
    }
    else if (posUP == 1) {
        if (posLR == 0) { return 7; }
        else if (posLR == 1) { return 8; }
        else if (posLR == 2) { return 3; }
    }
    else if (posUP == 2) {
        if (posLR == 0) { return 6; }
        else if (posLR == 1) { return 5; }
        else if (posLR == 2) { return 4; }
    }
    
    return -1;
}

int calcStageIndex(int LR, int UP) {
    int posUP = UP % iconRow;
    int posLR = LR % iconRow;

    //00 0
    //01 1
    //02 2
    //10 7
    //11 8
    //12 3
    //20 6
    //21 5
    //22 4
    if (posUP == 0) {
        if (posLR == 0) { return 0; }
        else if (posLR == 1) { return 1; }
        else if (posLR == 2) { return 2; }
    }
    else if (posUP == 1) {
        if (posLR == 0) { return 7; }
        else if (posLR == 1) { return 8; }
        else if (posLR == 2) { return 3; }
    }
    else if (posUP == 2) {
        if (posLR == 0) { return 6; }
        else if (posLR == 1) { return 5; }
        else if (posLR == 2) { return 4; }
    }

    return -1;
}

int calcTotalCoin() {
    int coins = 0;

    for (int i = 0; i < stageList.size(); i++) {
        if (stageList[i].getCoin) {
            coins++;
        }
    }
    return coins;
}

void changeCursorPos() {
    if (CheckHitKey(KEY_INPUT_UP) && keyLockCount_select == 0) {
        PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
        keyLockCount_select = 10;
        selectCursorPos[0] += iconRow - 1;
    }
    else if (CheckHitKey(KEY_INPUT_DOWN) && keyLockCount_select == 0) {
        PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
        keyLockCount_select = 10;
        selectCursorPos[0] += 1;
    }
    else if (CheckHitKey(KEY_INPUT_RIGHT) && keyLockCount_select == 0) {
        PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
        keyLockCount_select = 10;
        selectCursorPos[1] += 1;
    }
    else if (CheckHitKey(KEY_INPUT_LEFT) && keyLockCount_select == 0) {
        PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
        keyLockCount_select = 10;
        selectCursorPos[1] += iconRow - 1;
    }

    cursorStageList = calcStageIndex(selectCursorPos);
}

void drawHelp_select() {
    if (fontHandleNormal_select == 0) {
        fontHandleNormal_select = CreateFontToHandle(NULL, -1, -1);
        fontHandleHelp_select = CreateFontToHandle(NULL, 20, 4);
    }

    vector<string> helpMsg = { "方向キー:移動",
                                "Z:決定",
                                "ESC:タイトル画面へ" };

    DrawFormatStringToHandle(20, 685, c_white, fontHandleHelp_select, "%s", helpMsg[0].c_str());
    DrawFormatStringToHandle(170, 685, c_white, fontHandleHelp_select, "%s", helpMsg[1].c_str());
    DrawFormatStringToHandle(250, 685, c_white, fontHandleHelp_select, "%s", helpMsg[2].c_str());
}

void drawStageDetail() {
    int SX = 22;
    int SY = 122;
    int EX = 330;
    int EY = SY+smoothMax;

    int frame = 5;

    if (fontHandleDetail_stageNum == 0) {
        fontHandleDetail_stageNum = CreateFontToHandle(NULL, 30, 10);
        fontHandleDetail_title = CreateFontToHandle(NULL, 40, 20);
        fontHandleDetail_level = CreateFontToHandle(NULL, 20, 10);
    }

    if (smooth > 0)smooth -= 20;
    if (0 >= smooth)smooth = 0;

    //枠
    DrawBox(SX, SY, EX, EY-smooth, c_white, TRUE);
    DrawBox(SX+frame, SY+frame, EX-frame, EY-smooth-frame, c_black, FALSE);
    DrawBox(SX+frame+1, SY+frame+1, EX-frame-1, EY-smooth-frame-1, c_lightGray, TRUE);

    //文字
    DrawFormatStringToHandle(55, 150, c_black, fontHandleDetail_stageNum, "stage %d", cursorStageList+1);
    DrawFormatStringToHandle(100, 195, c_black, fontHandleDetail_title, "%s", stageList[cursorStageList].stageName.c_str());

    vector<string> levelStar = {};
    for (int i = 0; i < stageList[cursorStageList].stageLevel; i++) {
        levelStar.push_back("★");
    }
    for (int i = 0; i < 4-stageList[cursorStageList].stageLevel; i++) {
        levelStar.push_back("☆");
    }
    DrawFormatStringToHandle(130, 250, c_black, fontHandleDetail_level, "level %s", reshapeVectorToString(levelStar).c_str());
}

void drawStageList() {
    int between = 40;
    int cursorOffset = -20;

    int stageIconDelta = stageIconSize + stageIconPadding;
    int SX = 0;
    int SY = 0;
    int EX = 0;
    int EY = 0;
    int c_stageIcon = c_white;

    int dia = 20;

    if (fontHandleCoin == 0) {
        fontHandleCoin = CreateFontToHandle(NULL, 30, 20);
        fontHandleCoin2 = CreateFontToHandle(NULL, 60, 40);
    }

    //ステージ間線描画
    DrawLine(533, 207, 555, 207, c_white, 3);//1-2
    DrawLine(725, 207, 747, 207, c_white, 3);//2-3
    DrawLine(832, 292, 832, 314, c_white, 3);//3-4
    DrawLine(832, 484, 832, 506, c_white, 3);//4-5
    DrawLine(747, 591, 725, 591, c_white, 3);//5-6
    DrawLine(555, 591, 533, 591, c_white, 3);//6-7
    DrawLine(448, 506, 448, 484, c_white, 3);//7-8
    DrawLine(533, 399, 555, 399, c_white, 3);//8-9
    //ステージアイコン描画
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            SX = 363 + stageIconDelta * i;
            SY = 122 + stageIconDelta * j;
            EX = 533 + stageIconDelta * i;
            EY = 292 + stageIconDelta * j;

            //クリアしていたら水色
            if (stageList[calcStageIndex(i, j)].clear) {
                c_stageIcon = c_clear;
            }
            else {
                c_stageIcon = c_black;
            }

            //アイコン枠
            if (selectCursorPos[0]%iconRow == j && selectCursorPos[1]%iconRow == i) {
                //選択中
                for (int k = 0; k < 5; k++) {
                    DrawBox(SX+k, SY+k, EX-k, EY-k, c_stageIcon, FALSE);
                    if (gameCount % 15 <= 10) {
                        DrawBox(SX-k, SY-k, EX+k, EY+k, c_white, FALSE);
                    }
                }
            }
            else {
                //未選択
                for (int k = 0; k < 5; k++) {
                    DrawBox(SX+k, SY+k, EX-k, EY-k, c_stageIcon, FALSE);
                }
            }
            
            //ステージアイコン描画
            DrawExtendGraph(SX+5, SY+5, EX-5, EY-5, stageIconHandle[calcStageIndex(i,j)], TRUE);

            //Coin
            if (stageList[calcStageIndex(i,j)].getCoin) {
                DrawExtendGraph(EX-48, EY-48, EX, EY, coinHandle, TRUE);
            }

            //coin8無ければ網掛け
            if (i == 1 && j == 1 && calcTotalCoin() < 8) {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
                DrawBox(SX + 5, SY + 5, EX - 5, EY - 5, c_black, TRUE);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
                DrawExtendGraph((SX+EX)/2-70, (SY+EY)/2-30, (SX+EX)/2-6, (SY+EY)/2+34, coinHandle, TRUE);
                DrawFormatStringToHandle((SX+EX)/2-10, (SY+EY)/2, c_white, fontHandleCoin, "×");
                DrawFormatStringToHandle((SX+EX)/2+25, (SY+EY)/2-25, c_white, fontHandleCoin2, "8");
            }

            //ステージ番号
            DrawCircleAA((float)(SX + dia), (float)(SY + dia), (float)dia, 16, c_red, TRUE);
            DrawCircleAA((float)(SX + dia), (float)(SY + dia), (float)(dia-2), 16, c_white, TRUE);
            DrawFormatStringToHandle(SX+12, SY+6, c_black, fontHandleCoin, "%d", calcStageIndex(i,j)+1);
        }
        
    }
    
    //所持コイン
    DrawExtendGraph(1000, 600, 1064, 664, coinHandle, TRUE);
    DrawFormatStringToHandle(1065, 640, c_white, fontHandleCoin, "×");
    DrawFormatStringToHandle(1100, 610, c_white, fontHandleCoin2, "%d",calcTotalCoin());
}

void startStage() {
    //Zキーでステージに入る
    if ((CheckHitKey(KEY_INPUT_Z) || CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_RETURN)) && keyLockCount_select == 0) {
        keyLockCount_select = 15;
        if (cursorStageList == 8 && calcTotalCoin() < 8) {
            PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
        }
        else {
            PlaySoundMem(SEHandle_intoStage, DX_PLAYTYPE_BACK, TRUE);
            changeScene(sceneGame);
            smooth = smoothMax;
            setStageData(stageList[cursorStageList]);

            //クリアまでの時間計測開始
            startTime = gameCount;
        }
    }
}

vector<Data> loadStageList(string csvFilePath) {
    //全ステージ詳細情報を読み込み

    vector<Data> allData = {};//全てのstageData

    string line;//ファイル1行分読み込みの置き場
    ifstream ifs(csvFilePath);//読み込むファイル

    getline(ifs, line);//ヘッダーを飛ばす

    //1行読み込み、その1行をカンマで区切ってtempDataに格納
    for (int i = 0; getline(ifs, line); i++) {
        stringstream ss(line);
        Data tempData = {};//1行分のstageData

        getline(ss, tempData.stageName, ',');
        getline(ss, tempData.stageFile, ',');
        string mapHeight_str="";//intに直接入れられないのでいったん文字列で
        getline(ss, mapHeight_str, ',');
        //printfDx("%d\n", stoi(mapHeight_str.c_str()));
        string mapWidth_str;
        getline(ss, mapWidth_str, ',');
        getline(ss, tempData.infKana, ',');
        string numKana_str;
        getline(ss, numKana_str, ',');
        string clear_str;
        getline(ss, clear_str, ',');
        if (clear_str == "T") {
            tempData.clear = TRUE;
        }
        else {
            tempData.clear = FALSE;
        }
        string getCoin_str;
        getline(ss, getCoin_str, ',');
        if (getCoin_str == "T") {
            tempData.getCoin = TRUE;
        }
        else {
            tempData.getCoin = FALSE;
        }
        string stageLevel_str;
        getline(ss, stageLevel_str, ',');
        
        tempData.mapHeight = stoi(mapHeight_str.c_str());//文字列->intに格納
        tempData.mapWidth = stoi(mapWidth_str.c_str());
        tempData.numKana = stoi(numKana_str.c_str());
        tempData.stageLevel = stoi(stageLevel_str.c_str());

        allData.push_back(tempData);//1行分のstageDataを全てのstageDataに足す
    }
    return allData;

}

vector<vector<string>> loadVerbList(string csvFilePath) {
    vector<vector<string>> verbs = {};//全てのstageData

    string line;//ファイル1行分読み込みの置き場
    ifstream ifs(csvFilePath);//読み込むファイル

    getline(ifs, line);//ヘッダーを飛ばす

    //1行読み込み、その1行をカンマで区切ってtempDataに格納
    for (int i = 0; getline(ifs, line); i++) {
        vector<string> verb = {};
        stringstream ss(line);
        string buff;
        for (int j = 0; j < 3; j++) {
            getline(ss, buff, ',');
            verb.push_back(buff);
        }
        verbs.push_back(verb);//1行分のstageDataを全てのstageDataに足す
    }
    sort(verbList.begin(), verbList.end());
    return verbs;
}

