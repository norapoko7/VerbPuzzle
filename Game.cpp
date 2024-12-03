#include "Game.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Select.h"
#include "Object.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include "Sound.h"

//なぜかヘッダ内では動かなかったので仕方なく…ここ以外では使わないので
void createObjectList(vector<Object*> *list);
void createPlayerObject();
Object* generateObject(string _objName, int _posX, int _posY);

using namespace std;

extern int gameCount;
extern vector<vector<string>> verbList;
extern vector<Data> stageList;
extern int cursorStageList;

const string objectFilePath = "./data/objects.csv";
extern string stageListPath;
extern string verbListPath;

extern int FPS;


Data stageData = {};
vector<vector<string>> stageMap = {}; //ステージのマップを格納
vector<vector<string>> stageObject = {}; //ステージに出てくるオブジェクトとその画像パスの配列を格納
vector<Object*> objectList = {};//ステージ内のすべてのオブジェクト
vector<Object*> undoObjectList = {};//1手戻す用
Object* player = {};
int undoPlayerPosX = 0;//1手戻す用
int undoPlayerPosY = 0;
int undoPlayerDire = 0;
vector<kanaUnit> kanaList = {};
vector<kanaUnit> undoKanaList = {};
vector<string> usableVerbList = {};
string lastVerb = {};//あるく以外、最後に使用した動詞　Cキー再使用のため
vector<string> lastUseVerb = {};//最後に使用した動詞
vector<string> showVerbList = {};//動詞メニューの一覧
vector<int> unusable = {};//動詞一覧で？？になるもののインデックス一覧
vector<int> autoVerb = {};


double magRate = 1;//マップ拡大表示の係数
int fieldStartX = 0;//マップ表示左上の座標
int fieldStartY = 0;

int clearTime = 0;
int startTime = 0;
int elapseTime = 0;//経過時間
int startTimeBuff;// restart用
int keyLockCount = 0;
bool isOpenVerbMenu = false;
bool isMovePlayer = true;
bool isControlVerbMenu = true;
int returnCount = 0;
int retryCount = 0;
int cursorUPPos = 0;
int cursorLRPos = 0;
int smoothMove = 0;
const int kanaImageMag = 2;
vector<string> makingVerb = {};
string randMsg = "";
bool isGetCoin = false;
bool undoIsGetCoin = false;
int maxSmoothMove = 60; //小さいほど早く開く
int colorClear = 0;
int speechBubble = 0;
string speechVerb = "";
bool isPrintDebug = false;
int kanaHighLightTime = 0;
bool isUpdateUsableVerbList = true;//trueの時usableVerbListを更新、文字入手でT、更新したらFにする
bool isSitPlayer = false;
bool undoIsSitPlayer = false;
bool isSeeThroughPlayer = false;
bool undoIsSeeThroughPlayer = false;
bool isCanUndo = false;

int fontHandleNormal=0;
int fontHandleHelp = 0;
int fontHandleMsg = 0;
int fontHandleClear = 0;
int fontHandleDetail = 0;
int fontHandleSpeechVerb = 0;
int fontHandleUndo = 0;


//SoundHandle
//システム
extern int SEHandle_closeMenu;
extern int SEHandle_exitStage;
extern int SEHandle_intoStage;
extern int SEHandle_moveCursor;
extern int SEHandle_openMenu;
extern int SEHandle_restartStage;
//ステージ内
extern int SEHandle_cantUse;
extern int SEHandle_clearStage;
extern int SEHandle_clearStageCoin;
//
extern int SEHandle_cutTree;
extern int SEHandle_crackRock;
extern int SEHandle_deleteKana;
extern int SEHandle_doUndo;
extern int SEHandle_eraseObject;
extern int SEHandle_fixFlag;
extern int SEHandle_foldFlag;
extern int SEHandle_getCoin;
extern int SEHandle_getKana;
extern int SEHandle_jumpRock;
extern int SEHandle_kickDoor;
extern int SEHandle_makeVerb;
extern int SEHandle_makeVerbAlready;
extern int SEHandle_openDoor;
extern int SEHandle_pushRock;
extern int SEHandle_rotateGear;
extern int SEHandle_selectKana;
extern int SEHandle_sitPlayer;
extern int SEHandle_walkPlayer;


//colorHandle
int c_white = GetColor(255, 255, 255);
int c_black = GetColor(0, 0, 0);
int c_red = GetColor(255, 0, 0);
int c_green = GetColor(0, 255, 0);
int c_blue = GetColor(0, 0, 255);
int c_yellow = GetColor(255, 255, 0);
int c_pink = GetColor(255, 0, 255);
int c_lightBlue = GetColor(0, 255, 255);

int c_autoVerb = GetColor(0, 100, 200);
int c_shade = GetColor(25, 25, 25);
int c_shade2 = GetColor(50, 50, 50);

//更新
void updateGame() {
    if (CheckHitKey(KEY_INPUT_2) != 0) { //ESCキーが押されていたら
        backSelectScene();
    }
}

//描画
void drawGame() {
    //初回のみ実行
    if (stageMap.empty()) {
        stageMap = loadStageMap(stageData); //mapデータ取得
        //stageObject = createStageObject(stageMap);

        //kanaList作成
        createKanaList(split(stageData.infKana.c_str(), '-'));

        //オブジェクトの作成
        createObjectList(&objectList);
        
        //usableVerbList作成
        createUsableVerbList();
        
        //magRateなど描画に必要な設定 magRate: 拡大率 描画エリア内でできるだけ大きく
        magRate = floor(min(squareHeight/(double)stageData.mapHeight, squareWidth/(double)stageData.mapWidth) * 2)/2;//0.5刻み切り捨て
        fieldStartX = (int)(fieldCenterX - squareSize * stageData.mapWidth / 2.0 * magRate);
        fieldStartY = (int)(fieldCenterY - squareSize * stageData.mapHeight / 2.0 * magRate);

        //undo用初期値
        undoObjectList = objectList;
        undoPlayerPosX = player->posX;
        undoPlayerPosY = player->posY;
        undoPlayerDire = player->dire;
        undoKanaList = kanaList;
        undoIsGetCoin = isGetCoin;
        undoIsSitPlayer = isSitPlayer;
        undoIsSeeThroughPlayer = isSeeThroughPlayer;
    }

    //オブジェクトの描画
    for (int i = 0; i < objectList.size(); i++) {
        objectList[i]->drawObject(magRate, fieldStartX, fieldStartY);
    }
    player->drawObject(magRate, fieldStartX, fieldStartY);

    //undo用の処理
    //updateUndoBuff();
    doUndo();
    //printfDx("%d\n", startTime);
    //printfDx("%d, %d\n\n", undoPlayerPosX, undoPlayerPosY);

    //画面描画
    drawKanaList();
    openVerbMenu();
    
    drawHelp();
    drawUseVerb(speechVerb);

    //操作受付＆オブジェクト更新
    player->movePlayer();
    useLastVerb();

    //マイフレーム更新
    //createUsableVerbLit(); マイフレームは重すぎる
    if (keyLockCount > 0)keyLockCount--;

    //オブジェクト間の処理
    touchCoin();
    touchKana();

    touchFlag();//ゴール処理するので一番後ろに
    returnSelect();
    retryStage();

    //DrawString(0, 0, "Game画面です。", GetColor(255, 255, 255));
    //DrawString(0, 20, "2キーを押すとセレクト画面へ。", GetColor(255, 255, 255));

    //デバッグ、最後に消す
    //showDebug(0,60);
}

void backSelectScene() {
    //メモリ初期化
    //stageData = {};
    stageMap = {};
    stageObject = {};
    objectList = {};
    clearTime = 60;
    startTime = 0;
    elapseTime = 0;
    kanaList = {};
    usableVerbList = {};
    isOpenVerbMenu = false;
    isMovePlayer = true;
    cursorUPPos = 0;
    cursorLRPos = 0;
    smoothMove = 0;
    lastVerb = "";
    isGetCoin = false;
    speechBubble = 0;
    speechVerb = "";
    isUpdateUsableVerbList = true;
    isSitPlayer = false;
    isSeeThroughPlayer = false;

    changeScene(sceneSelect);//シーンをセレクトに変更
}

int calcObjectListIndex(int posX, int posY) {
    return stageData.mapWidth * posX + posY;
}

int calcTargetIndex(int _posX, int _posY, int _dire) {
    if (_dire == dire_DOWN) {
        return stageData.mapWidth * (_posX + 1) + _posY;
    }
    else if (_dire == dire_LEFT) {
        return stageData.mapWidth * _posX + _posY - 1;
    }
    else if (_dire == dire_UP) {
        return stageData.mapWidth * (_posX - 1) + _posY;
    }
    else if (_dire == dire_RIGHT) {
        return stageData.mapWidth * _posX + _posY + 1;
    }
    return 0;
}

vector<int> calcTargetPos(int _posX, int _posY, int _dire) {
    if (_dire == dire_DOWN) {
        return { _posX + 1, _posY };
    }
    else if (_dire == dire_LEFT) {
        return { _posX, _posY - 1 };
    }
    else if (_dire == dire_UP) {
        return { _posX - 1, _posY };
    }
    else if (_dire == dire_RIGHT) {
        return { _posX, _posY + 1 };
    }
    return {0,0};
}

void createKanaList(vector<string> _infKana) {
    for (int i = 0; i < stageData.numKana; i++) {
        kanaUnit buffKana = {};
        buffKana.kana = _infKana[i];
        buffKana.numTimes = -1;
        string handlePath = "./data/image/kana2_" + buffKana.kana + ".png";
        buffKana.handle = LoadGraph(handlePath.c_str());
        kanaList.push_back(buffKana);
    }
}

void createObjectList(vector<Object*> *list) {
    for (int i = 0; i < stageMap.size(); i++) {
        for (int j = 0; j < stageMap[i].size(); j++) {
            if (stageMap[i][j] == "P") {
                list->push_back(generateObject("A", i, j));
                player = generateObject(stageMap[i][j], i, j);
            }
            else {

                list->push_back(generateObject(stageMap[i][j], i, j));
            }
        }
    }
}

void createPlayerObject() {
    for (int i = 0; i < stageMap.size(); i++) {
        for (int j = 0; j < stageMap[i].size(); j++) {
            if (stageMap[i][j] == "P")  player = new Player(i, j);
            else                        player = new Object(i, j);

        }
    }
}

string createRandomMsg() {
    const struct randomMsg{
        string msg;
        int weight;
    };

    vector<randomMsg> msgs = {
        {"TABで動詞作成モード！",100},
        {"青い動詞は自動で使うよ",30},
        {"知らない動詞は??で表示",20},
        {"?の個数は文字数のヒント",20},
        {"シフトで向きだけ変わる",20},
        {"Cで直前の動詞をもう1度",20},
        {"コイン,取れたらうれしい",20},
        {"動詞を使ってどうしよう",5},
    };

    int sumWeight = 0;
    for (int i = 0; i < msgs.size(); i++) {
        sumWeight += msgs[i].weight;
    }

    int randNum = rand() % (sumWeight + 1);
    for (int j = 0; j < msgs.size(); j++) {
        randNum -= msgs[j].weight;
        if (randNum <= 0) {
            return msgs[j].msg;
        }
    }
    return "このメッセージは出ないはず";
}

vector<vector<string>> createStageObject(vector<vector<string>> _stageMap) {
    vector<vector<string>> stageObject = {};
    vector<vector<string>> objectList = loadObjectList();
    for (int i = 0; i < _stageMap.size(); i++) {
        for (int j = 0; j < _stageMap[0].size(); j++) {
            vector<string> object = {};//1行分
            if (!isExistString(stageObject, _stageMap[i][j].c_str()) && _stageMap[i][j]!="") {
                //はじめて出てきたもの&""でないものをリストに追加
                object.push_back(_stageMap[i][j].c_str());

                //その文字のイメージファイルパスを追加
                for (int k = 0; k < objectList.size(); k++) {
                    if (_stageMap[i][j] == objectList[k][0]) {
                        object.push_back(objectList[k][1]);
                    }
                }
                stageObject.push_back(object);
                
            }
        }

    }
    //printVector(stageObject);
        
    return stageObject;
}

void createUsableVerbList() {
    int n = (int)kanaList.size();
    vector<string> makableVerbList = {};
    usableVerbList = {};

    // ビットマスクを使って部分集合を生成
    for (int i = 1; i < (1 << n); ++i) {
        vector<string> subset;

        // iの各ビットに対応するkanaListの要素を部分集合に追加
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.push_back(kanaList[j].kana);
            }
        }

        sort(subset.begin(), subset.end());

        // 部分集合の順列を生成
        do {
            // 順列をprintfDxで表示
            string result;
            for (const auto& kana : subset) {
                result += kana;
            }
            makableVerbList.push_back(result);
            //printfDx("%s\n", result.c_str());
        } while (next_permutation(subset.begin(), subset.end()));
    }

    //makableとverbListの積集合をusableにする
    for (int i = 0; i < makableVerbList.size(); i++) {
        for (int j = 0; j < verbList.size(); j++) {
            if (makableVerbList[i] == verbList[j][0]) {
                usableVerbList.push_back(makableVerbList[i]);
                //printfDx("%s\n", makableVerbList[i].c_str());
            }
        }
    }

    sortUsableVerbList();
}

void doUndo() {
    if (CheckHitKey(KEY_INPUT_B) && keyLockCount == 0 && isMovePlayer) {
        keyLockCount = 10;
        if (isCanUndo) {
            isCanUndo = false;
            PlaySoundMem(SEHandle_doUndo, DX_PLAYTYPE_BACK, TRUE);

            objectList = undoObjectList;
            player->posX = undoPlayerPosX;
            player->posY = undoPlayerPosY;
            player->dire = undoPlayerDire;
            kanaList = undoKanaList;
            isGetCoin = undoIsGetCoin;
            isSitPlayer = undoIsSitPlayer;
            isSeeThroughPlayer = undoIsSeeThroughPlayer;

            createUsableVerbList();
            isUpdateUsableVerbList = true;
            if (isPrintDebug)printfDx("undo\n");
        }
        else {
            PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
        }
    }

}

void drawClearScene() {
    if (fontHandleClear==0) {
        fontHandleClear = CreateFontToHandle(NULL, 255, 20);
        fontHandleDetail = CreateFontToHandle(NULL, 50, 5);
    }

    if (isGetCoin) {
        colorClear = c_yellow;
    }
    else {
        colorClear = c_lightBlue;
    }

    if (elapseTime == 0) {
        elapseTime = gameCount - startTime;
    }

    DrawStringToHandle(windowSizeX / 2 - 325, windowSizeY / 2 -200, "CLEAR", colorClear, fontHandleClear);
    DrawFormatStringToHandle(windowSizeX / 2 - 265, windowSizeY / 2 + 50, colorClear, fontHandleDetail, "Clear time: %.1f 秒", (double)elapseTime/FPS);
    //printfDx("%d\n", gameCount - startTime);
}

void drawKanaList() {
    int color = c_white;
    int color2 = c_black;
    
    for (int i = 0; i < kanaList.size(); i++) {
        int SX = margin + (int)(squareSize * kanaImageMag * 1.5) * i;
        int SY = margin;
        int EX = margin + squareSize * kanaImageMag + (int)(squareSize * kanaImageMag * 1.5) * i;
        int EY = margin + squareSize * kanaImageMag;

        //awFormatString(100 + 30 * i, 50, color, "%s", kanaList[i].kana.c_str());
        //DrawFormatString(100 + 30 * i, 70, color, "%d", kanaList[i].numTimes);

        //kana
        DrawExtendGraph(SX, SY, EX, EY, kanaList[i].handle, TRUE);

        //highlight
        //選択中の動詞で使う文字をハイライト
        if (isOpenVerbMenu && isControlVerbMenu && getVerbListValue(usableVerbList[cursorUPPos % usableVerbList.size()], "make")) {
            drawHighlightKana(reshapeStringToVector(usableVerbList[cursorUPPos % usableVerbList.size()]));
        }
        //使った文字をハイライトする
        if (kanaHighLightTime > 0 && !lastUseVerb.empty()) {
            for (int j = 0; j < lastUseVerb.size(); j++) {
                if (kanaList[i].kana == lastUseVerb[j]) {
                    int SX = margin + (int)(squareSize * kanaImageMag * 1.5) * i;
                    int SY = margin;
                    int EX = margin + squareSize * kanaImageMag + (int)(squareSize * kanaImageMag * 1.5) * i;
                    int EY = margin + squareSize * kanaImageMag;

                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, kanaHighLightTime * 6);
                    for (int k = 0; k < 5; k++) {
                        DrawCircleAA((float)((EX + SX) / 2.0), (float)((EY + SY) / 2.0), (float)(squareSize * kanaImageMag / 2) - k, 16, c_pink, FALSE);
                    }
                    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
                }
            }
        }
        if (kanaHighLightTime > 0)kanaHighLightTime--;
    }
    for (int i = 0; i < kanaList.size(); i++) {
        int SX = margin + (int)(squareSize * kanaImageMag * 1.5) * i;
        int SY = margin;
        int EX = margin + squareSize * kanaImageMag + (int)(squareSize * kanaImageMag * 1.5) * i;
        int EY = margin + squareSize * kanaImageMag;
        //num
        DrawCircleAA((float)EX-2, (float)(EY-2-squareSize* kanaImageMag /4), (float)squareSize* kanaImageMag /4-1, 16, color, TRUE);
        DrawCircleAA((float)EX-2, (float)(EY-2-squareSize* kanaImageMag /4), (float)squareSize* kanaImageMag /4-1, 16, color2, FALSE);
        DrawFormatString(EX-squareSize* kanaImageMag /8, EY-squareSize* kanaImageMag *3/8, color2, "%s", kanaList[i].numTimes>=0 ? to_string(kanaList[i].numTimes).c_str() : "∞");
    }

    
}

void drawHelp() {
    if (fontHandleNormal == 0) {
        fontHandleNormal = CreateFontToHandle(NULL, -1, -1);
        fontHandleHelp = CreateFontToHandle(NULL, 20, 4);
        fontHandleUndo = CreateFontToHandle(NULL, 25, 10);
    }

    vector<string> helpMsg = { "B:1手戻る",
                               "Z:決定",
                               "X:メニュー開閉",
                               "Shift+方向キー:向き変更",
                               "C:最後に使用した動詞\"" + lastVerb +"\"",
                               "Tab:メニュー切り替え",
                               "Enter:動詞作成",
                               "backspace:1文字消す",
                               "R:リスタート",
                               "ESC:ステージ選択へ"};
    
    DrawFormatStringToHandle(20, 680, isCanUndo? c_white :c_shade2 , fontHandleUndo, "%s", helpMsg[0].c_str());
    DrawFormatStringToHandle(170, 685, c_white, fontHandleHelp, "%s", helpMsg[1].c_str());
    DrawFormatStringToHandle(250, 685, c_white, fontHandleHelp, "%s", helpMsg[2].c_str());

    DrawFormatStringToHandle(1010, 690, c_white, fontHandleNormal, "%s", helpMsg[8].c_str());
    DrawFormatStringToHandle(1120, 690, c_white, fontHandleNormal, "%s", helpMsg[9].c_str());

    if (!isOpenVerbMenu) {
        //メニューを開いていない
        DrawFormatStringToHandle(410, 685, c_white, fontHandleHelp, "%s", helpMsg[3].c_str());
        DrawFormatStringToHandle(660, 685, c_white, fontHandleHelp, "%s", helpMsg[4].c_str());
    }
    else if (isControlVerbMenu) {
        //メニュー開いていて　動詞一覧
        DrawFormatStringToHandle(410, 685, c_white, fontHandleHelp, "%s", helpMsg[5].c_str());
    }
    else {
        //メニュー開いていて　文字一覧
        DrawFormatStringToHandle(410, 685, c_white, fontHandleHelp, "%s", helpMsg[5].c_str());
        DrawFormatStringToHandle(630, 685, c_white, fontHandleHelp, "%s", helpMsg[6].c_str());
        DrawFormatStringToHandle(790, 685, c_white, fontHandleHelp, "%s", helpMsg[7].c_str());
    }
}

void drawHighlightKana(vector<string> _verb) {
    for (int i = 0; i < kanaList.size(); i++) {
        int SX = margin + (int)(squareSize * kanaImageMag * 1.5) * i;
        int SY = margin;
        int EX = margin + squareSize * kanaImageMag + (int)(squareSize * kanaImageMag * 1.5) * i;
        int EY = margin + squareSize * kanaImageMag;

        //highlight
        for (int j = 0; j < _verb.size(); j++) {
            if (kanaList[i].kana == _verb[j]) {
                for (int k = 0; k < 5; k++) {
                    DrawCircleAA((float)((EX + SX) / 2.0), (float)((EY + SY) / 2.0), (float)(squareSize * kanaImageMag / 2) - k, 16, c_red, FALSE);
                }
            }
        }
    }
}

void drawUsableVerbList() {
    int verbOffset = (int)smoothMove * verbXOffset / maxSmoothMove;
    vector<string> showUseVerb = {};//使える動詞リスト
    vector<string> showNotUseVerb = {};//使えない動詞リスト　全部？？
    string showVerb = "";

    int paragraphOffset = 0;
    int c_verb = c_black;
    
    if (isUpdateUsableVerbList) {
        showVerbList = {};
        unusable = {};
        autoVerb = {};
        //?か動詞表示するか分ける
        for (int i = 0; i < usableVerbList.size(); i++) {
            showVerb = usableVerbList[i];
            if (getVerbListValue(usableVerbList[i], "make")) {//作ったことある動詞
                if (getVerbListValue(usableVerbList[i], "auto")) {
                    //autoの動詞のインデックス
                    autoVerb.push_back(i);
                }
                showUseVerb.push_back(showVerb);
            }
            else {
                //なかったら文字数そのまま？？？になる
                showVerb = "";
                for (int j = 0; j < splitHiragana(usableVerbList[i]).size(); j++) {
                    showVerb.push_back('?');
                }
                unusable.push_back(i);
                showNotUseVerb.push_back(showVerb);
            }
            showVerbList.push_back(showVerb);

        }
        isUpdateUsableVerbList = false;
    }
    for (int i = 0; i < usableVerbList.size(); i++) {
        if (isExistInt(autoVerb, i)) {
            //オート動詞なら
            c_verb = c_autoVerb;
        }
        else {
            c_verb = c_black;
        }
        if (i >= 12)paragraphOffset = 90;
        DrawFormatString(1280 + verbOffset + 40 + paragraphOffset, 122 + (int)(squareSize * 1.5) * (i % 12), c_verb, "%s", showVerbList[i].c_str());
    }
}

void drawUseVerb(string _verb) {
    int verbOffset = (int)smoothMove * verbXOffset / maxSmoothMove;

    int fieldOffset = (int)smoothMove * fieldXOffset / maxSmoothMove;
    //player 座標
    int pPosSX = (int)(fieldStartX + fieldOffset + player->posY * squareSize * magRate);
    int pPosSY = (int)(fieldStartY + player->posX * squareSize * magRate);
    int pPosEX = (int)(fieldStartX + fieldOffset + (player->posY + 1) * squareSize * magRate - 1);
    int pPosEY = (int)(fieldStartY + (player->posX + 1) * squareSize * magRate - 1);

    int ovalCenterX = 0;
    int ovalCenterY = 0;
    int ovalWidth = 256;
    int ovalHeight = 96;

    int triTopX = 0;
    int triTopY = 0;

    int strPosX = 0;
    int strPosY = 0;

    if (speechBubble > 0) {
        //speechBubble増減処理
        speechBubble--;

        if (!isOpenVerbMenu) {
            //吹き出しの描画
            //マップ上半分？
            if (stageData.mapHeight / 2 >= player->posX) { ovalCenterY = pPosEY + ovalHeight / 2; triTopY = pPosEY; }
            else { ovalCenterY = pPosSY - ovalHeight / 2; triTopY = pPosSY; }
            //マップ左半分？
            if (stageData.mapWidth / 2 >= player->posY) { ovalCenterX = pPosEX + ovalWidth / 2; triTopX = pPosEX; }
            else { ovalCenterX = pPosSX - ovalWidth / 2; triTopX = pPosSX; }
            DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_white, TRUE);
            DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_black, FALSE);
            DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_white, TRUE);
            DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_black, FALSE);

            //文字大きさ設定
            if (fontHandleSpeechVerb == 0) {
                fontHandleSpeechVerb = CreateFontToHandle(NULL, 36, 20);
            }

            //文字位置設定(文字数ごとに)
            int charSize = 2;
            //printfDx("size: %d\n", _verb.size() / 2);
            switch (_verb.size() / charSize) {
            case 1:
                strPosX = ovalCenterX - ovalWidth / 2 + 74;
                strPosY = ovalCenterY - 16;
                break;
            case 2:
                strPosX = ovalCenterX - ovalWidth / 2 + 90;
                strPosY = ovalCenterY - 16;
                break;
            case 3:
                strPosX = ovalCenterX - ovalWidth / 2 + 74;
                strPosY = ovalCenterY - 16;
                break;
            case 4:
                strPosX = ovalCenterX - ovalWidth / 2 + 74;
                strPosY = ovalCenterY - 16;
                break;
            case 5:
                strPosX = ovalCenterX - ovalWidth / 2 + 74;
                strPosY = ovalCenterY - 16;
                break;
            default:
                strPosX = ovalCenterX - ovalWidth / 2 + 74;
                strPosY = ovalCenterY - 16;
                break;
            }

            //printfDx("%s\n",speechVerb.c_str());
            DrawFormatStringToHandle(strPosX, strPosY, c_black, fontHandleSpeechVerb, "%s", _verb.c_str());
        }
    }
}

Object* generateObject(string _objName, int _posX, int _posY) {
    if (_objName == "" || _objName == "A") return new Air(_posX, _posY);
    else if (_objName == "B")  return new BrokenFlag(_posX, _posY);
    else if (_objName == "C")  return new Coin(_posX, _posY);
    else if (_objName == "D")  return new Door(_posX, _posY);
    else if (_objName == "E")  return new Empty(_posX, _posY);
    else if (_objName == "F")  return new Flag(_posX, _posY);
    else if (_objName == "G")  return new Gear(_posX, _posY);
    else if (_objName == "H")  return new Hole(_posX, _posY);
    else if (_objName == "P")  return new Player(_posX, _posY);
    else if (_objName == "R")  return new Rock(_posX, _posY);
    else if (_objName == "T")  return new Tree(_posX, _posY);
    else if (_objName == "W")  return new Wall(_posX, _posY);
    else if (_objName == "あ") return new kana_A(_posX, _posY);
    else if (_objName == "え") return new kana_E(_posX, _posY);
    else if (_objName == "お") return new kana_O(_posX, _posY);
    else if (_objName == "き") return new kana_KI(_posX, _posY);
    else if (_objName == "く") return new kana_KU(_posX, _posY);
    else if (_objName == "け") return new kana_KE(_posX, _posY);
    else if (_objName == "こ") return new kana_KO(_posX, _posY);
    else if (_objName == "す") return new kana_SU(_posX, _posY);
    else if (_objName == "と") return new kana_TO(_posX, _posY);
    else if (_objName == "な") return new kana_NA(_posX, _posY);
    else if (_objName == "ま") return new kana_MA(_posX, _posY);
    else if (_objName == "る") return new kana_RU(_posX, _posY);
    else if (_objName == "わ") return new kana_WA(_posX, _posY);
    else                       return new Object(_posX, _posY);
}

int getUsableVerbListIndex(string key) {
    for (int i = 0; i < usableVerbList.size(); i++) {
        if (usableVerbList[i] == key) {
            return i;
        }
    }
    return -1;
}

int getVerbListIndex(string key) {
    for (int i = 0; i < verbList.size(); i++) {
        if (verbList[i][0] == key) {
            return i;
        }
    }
    return - 1;
}

bool getVerbListValue(string key, string kind) {
    //kindは　make　か　auto
    int kindIndex = 0;
    if (kind == "make") {
        kindIndex = 1;
    }
    else if(kind == "auto") {
        kindIndex = 2;
    }
    else {
        return false;
    }

    for (int i = 0; i < verbList.size(); i++) {
        if (verbList[i][0] == key) {
            if (verbList[i][kindIndex]=="T") {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

bool isExistKana(vector<kanaUnit> _kanaList, string target) {
    for (int i = 0; i < _kanaList.size(); i++) {
        if (_kanaList[i].kana == target) {
            return true;
        }
    }
    return false;
}

bool isExistInt(vector<int> array, int target) {
    // 2次元vectorの各行をループ
    for (int i = 0; i < array.size(); i++) {
        // 文字列が見つかった場合
        if (array[i] == target) {
            return true;
        }

    }
    // 見つからなかった場合
    //printfDx("false\n");
    return false;
}

bool isExistString(vector<string> array, string target) {
    // 2次元vectorの各行をループ
    for (int i = 0; i < array.size(); i++) {
        // 文字列が見つかった場合
        if (array[i] == target) {
            return true;
        }

    }
    // 見つからなかった場合
    //printfDx("false\n");
    return false;
}

bool isExistString(vector<vector<string>> array, string target) {
    // 2次元vectorの各行をループ
    for (int i = 0; i < array.size(); i++) {
        // 各行の各要素をループ
        for (int j = 0; j < array[i].size(); j++) {
            // 文字列が見つかった場合
            if (array[i][j] == target) {
                return true;
            }
        }
    }
    // 見つからなかった場合
    //printfDx("false\n");
    return false;
}

bool isExistVerb(vector<string> _verb) {
    //kanaListに入力の動詞があるか
    //あ る く
    int exist = 0;
    for (int i = 0; i < _verb.size(); i++) {
        for (int j = 0; j < kanaList.size(); j++) {
            if (_verb[i] == kanaList[j].kana && kanaList[j].numTimes != 0) {
                exist += 1;
                break;
            }
        }
    }
    if (exist == _verb.size()) {
        return true;
    }
    return false;
}

void openVerbMenu() {
    //printfDx("%d\n", smoothMove);
    int delta = 10;//メニュー開閉時の描画位置の変化量
   
    int verbOffset = (int)smoothMove * verbXOffset / maxSmoothMove;

    int fieldOffset = (int)smoothMove * fieldXOffset / maxSmoothMove;
    //player 座標
    int pPosSX = (int)(fieldStartX + fieldOffset + player->posY * squareSize * magRate);
    int pPosSY = (int)(fieldStartY + player->posX * squareSize * magRate);
    int pPosEX = (int)(fieldStartX + fieldOffset + (player->posY + 1) * squareSize * magRate - 1);
    int pPosEY = (int)(fieldStartY + (player->posX + 1) * squareSize * magRate - 1);

    int ovalCenterX = 0;
    int ovalCenterY = 0;
    int ovalWidth = 256;
    int ovalHeight = 96;

    int triTopX = 0;
    int triTopY = 0;

    bool hitKey_Z = false;
    bool hitKey_X = false;
    bool hitKey_UP = false;
    bool hitKey_DOWN = false;
    bool hitKey_LEFT = false;
    bool hitKey_RIGHT = false;
    bool hitKey_SPACE = false;
    bool hitKey_RETURN = false;
    bool hitKey_TAB = false;
    bool hitKey_BACK = false;

    if (CheckHitKeyAll()) {
        if (CheckHitKey(KEY_INPUT_Z))hitKey_Z = true;
        if (CheckHitKey(KEY_INPUT_X))hitKey_X = true;
        if (CheckHitKey(KEY_INPUT_UP))hitKey_UP = true;
        if (CheckHitKey(KEY_INPUT_DOWN))hitKey_DOWN = true;
        if (CheckHitKey(KEY_INPUT_LEFT))hitKey_LEFT = true;
        if (CheckHitKey(KEY_INPUT_RIGHT))hitKey_RIGHT = true;
        if (CheckHitKey(KEY_INPUT_SPACE))hitKey_SPACE = true;
        if (CheckHitKey(KEY_INPUT_RETURN))hitKey_RETURN = true;
        if (CheckHitKey(KEY_INPUT_TAB))hitKey_TAB = true;
        if (CheckHitKey(KEY_INPUT_BACK))hitKey_BACK = true;
    }

    //X メニュー開く
    if (!isOpenVerbMenu && hitKey_X && keyLockCount == 0 && isMovePlayer) {
        keyLockCount = 15;
        isOpenVerbMenu = true;
        isMovePlayer = false;
        isControlVerbMenu = true;
        smoothMove = 0;
        randMsg = createRandomMsg();
        PlaySoundMem(SEHandle_openMenu, DX_PLAYTYPE_BACK, TRUE);
    }
    //Tabメニュー開く
    if (!isOpenVerbMenu && hitKey_TAB && keyLockCount == 0 && isMovePlayer) {
        keyLockCount = 15;
        isOpenVerbMenu = true;
        isMovePlayer = false;
        isControlVerbMenu = false;
        smoothMove = 0;
        randMsg = createRandomMsg();
        PlaySoundMem(SEHandle_openMenu, DX_PLAYTYPE_BACK, TRUE);
    }
    //メニュ閉じる
    if (isOpenVerbMenu && hitKey_X && keyLockCount == 0) {
        keyLockCount = 15;
        isOpenVerbMenu = false;
        isMovePlayer = true;
        smoothMove = maxSmoothMove;
        PlaySoundMem(SEHandle_closeMenu, DX_PLAYTYPE_BACK, TRUE);
    }

    //動詞一覧メニュー描画
    DrawBox(windowSizeX+verbOffset, margin*2+kanaListHeight, windowSizeX-verbXOffset+verbOffset, windowSizeY-margin*2, c_white, TRUE);//mainbox
    DrawBox(windowSizeX-margin+verbOffset, margin*2+kanaListHeight, windowSizeX+verbOffset, margin*2+kanaListHeight+ (int)(squareSize*1.5), c_white, TRUE);//index
    for (int i = 0; i < usableVerbList.size(); i++) {
        //動詞間の下線
        DrawLine(windowSizeX + verbOffset + 12 , margin * 2 + kanaListHeight + (int)(squareSize * 1.5) * (i+1), windowSizeX - verbXOffset + verbOffset - 12, margin * 2 + kanaListHeight + (int)(squareSize * 1.5) * (i + 1), c_black);
    }

    DrawString(windowSizeX - margin + verbOffset + 6, margin * 2 + kanaListHeight + 8, "X", c_black);
    DrawString(windowSizeX - margin + verbOffset + 3, margin * 2 + kanaListHeight + 26, "⇔", c_black);
    if (usableVerbList.size() != 0) {
        int paragraphOffset = 0;
        if (cursorUPPos % usableVerbList.size() >= 12)paragraphOffset = 90;
        DrawString(1280 + verbOffset + 20 + paragraphOffset, 122 + (int)(squareSize * 1.5) * ((cursorUPPos % usableVerbList.size())%12), "⇒", c_black);
    }
    
    //動詞メニューの動詞の描画
    drawUsableVerbList();


    //メニュー開いてるときだけ実行
    if (isOpenVerbMenu) {
        //TABキーで操作するメニューを切り替える
        if (hitKey_TAB && keyLockCount == 0) {
            PlaySoundMem(SEHandle_openMenu, DX_PLAYTYPE_BACK, TRUE);
            isControlVerbMenu = !isControlVerbMenu;
            keyLockCount = 15;
        }
        
        //動詞一覧触ってる時
        //printfDx("%d\n", cursorUPPos);
        if (isControlVerbMenu) {
            //cursorPos更新
            if (hitKey_UP && keyLockCount == 0) {
                keyLockCount = 5;
                PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
                if (usableVerbList.size() > 12 && cursorUPPos%usableVerbList.size() == 0) {
                    cursorUPPos = 11;
                }
                else if (usableVerbList.size() > 12 && cursorUPPos%usableVerbList.size() == 12) {
                    cursorUPPos = usableVerbList.size() - 1;
                }
                else {
                    cursorUPPos += (int)usableVerbList.size() - 1;
                }
            }
            else if (hitKey_DOWN && keyLockCount == 0) {
                keyLockCount = 5;
                PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
                if (usableVerbList.size() > 12 && cursorUPPos%usableVerbList.size() == 11) {
                    cursorUPPos = 0;
                }
                else if (usableVerbList.size() > 12 && cursorUPPos%usableVerbList.size() == usableVerbList.size()-1) {
                    cursorUPPos = 12;
                }
                else {
                    cursorUPPos += 1;
                }
                
            }
            else if (usableVerbList.size()>=12 && hitKey_RIGHT && keyLockCount == 0) {
                keyLockCount = 5;
                PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
                if (cursorUPPos % usableVerbList.size() + 12 <= usableVerbList.size()-1) {
                    //選択動詞の右に動詞がある
                    cursorUPPos += 12;
                }
                
            }
            else if (usableVerbList.size() >= 12 && hitKey_LEFT && keyLockCount == 0) {
                keyLockCount = 5;
                PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
                if (cursorUPPos % usableVerbList.size() >= 12) {
                    //選択動詞の右に動詞がある
                    cursorUPPos -= 12;
                }
            }

            //Zキーで使う動詞を決定
            if (usableVerbList.size() != 0) {
                int useVerbIndex = cursorUPPos % usableVerbList.size();
                if ((hitKey_Z || hitKey_SPACE || hitKey_RETURN) && keyLockCount == 0) {
                    keyLockCount = 10;
                    if (!isExistInt(unusable, useVerbIndex) && !isExistVerb(reshapeStringToVector("とまる"))) {
                        chooseVerb(usableVerbList[useVerbIndex]);
                    }
                    else {
                        PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
                    }
                }
            }
        }
        //kana一覧触ってるとき
        else {
            //選択枠描画
            int SX = margin + (int)(squareSize * kanaImageMag * 1.5) * (cursorLRPos % kanaList.size()) - squareSize * kanaImageMag / 8;
            int SY = margin - squareSize * kanaImageMag / 8;
            int EX = margin + squareSize * kanaImageMag + (int)(squareSize * kanaImageMag * 1.5) * (cursorLRPos % kanaList.size()) + squareSize * kanaImageMag * 3 / 8;
            int EY = margin + squareSize * kanaImageMag + squareSize * kanaImageMag / 8;
            int thick = 5;
            for (int i = 0; i < thick; i++) {
                DrawBox(SX+i, SY+i, EX-i, EY-i, c_green, FALSE);
            }

            //cursorPos更新
            if (hitKey_LEFT && keyLockCount == 0) {
                PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
                keyLockCount = 5;
                cursorLRPos += (int)kanaList.size() - 1;
            }
            else if (hitKey_RIGHT && keyLockCount == 0) {
                PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
                keyLockCount = 5;
                cursorLRPos += 1;
            }

            //動詞一覧網掛け
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, margin * 2 + kanaListHeight, windowSizeX - verbXOffset + verbOffset, windowSizeY - margin * 2, c_shade, TRUE);//mainbox
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
            

            //Zキーで使う文字をstringに追加
            int useKanaIndex = cursorLRPos % kanaList.size();
            if ((hitKey_Z || hitKey_SPACE) && keyLockCount == 0) {
                keyLockCount = 5;
                PlaySoundMem(SEHandle_selectKana, DX_PLAYTYPE_BACK, TRUE);
                makingVerb.push_back(kanaList[useKanaIndex].kana);
            }

            //バックスペースでmakingStringの一番後ろを消す
            if (hitKey_BACK && keyLockCount == 0) {
                keyLockCount = 10;
                if (!makingVerb.empty()) {  // 空ではないか確認
                    PlaySoundMem(SEHandle_deleteKana, DX_PLAYTYPE_BACK, TRUE);
                    makingVerb.pop_back();   // 最後の要素を削除
                }
            }

            //ENTERキーでmakingVerb判定
            if (hitKey_RETURN && keyLockCount == 0) {
                keyLockCount = 10;
                if (isExistString(verbList, reshapeVectorToString(makingVerb))) {
                    if (verbList[getVerbListIndex(reshapeVectorToString(makingVerb))][1] == "F") {
                        PlaySoundMem(SEHandle_makeVerb, DX_PLAYTYPE_BACK, TRUE);
                        //verbList更新
                        verbList[getVerbListIndex(reshapeVectorToString(makingVerb))][1] = "T";
                        //csv書き込み
                        writeVerbList(verbList);
                        isControlVerbMenu = true;
                        //sortUsableVerbList();
                        cursorUPPos = getUsableVerbListIndex(reshapeVectorToString(makingVerb));
                        isUpdateUsableVerbList = true;
                    }
                    else {
                        PlaySoundMem(SEHandle_makeVerbAlready, DX_PLAYTYPE_BACK, TRUE);
                        isControlVerbMenu = true;
                        cursorUPPos = getUsableVerbListIndex(reshapeVectorToString(makingVerb));
                    }
                }
                else {
                    PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
                }
                makingVerb = {};
            }
        }
        //吹き出しの描画
        //マップ上半分？
        if (stageData.mapHeight / 2 >= player->posX) { ovalCenterY = pPosEY + ovalHeight / 2; triTopY = pPosEY; }
        else { ovalCenterY = pPosSY - ovalHeight / 2; triTopY = pPosSY; }
        //マップ左半分？
        if (stageData.mapWidth / 2 >= player->posY) { ovalCenterX = pPosEX + ovalWidth / 2; triTopX = pPosEX; }
        else { ovalCenterX = pPosSX - ovalWidth / 2; triTopX = pPosSX; }
        DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_white, TRUE);
        DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_black, FALSE);
        DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_white, TRUE);
        DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_black, FALSE);
        //makingVerb描画
        string printMakingVerb = reshapeVectorToString(makingVerb);
        if (printMakingVerb.empty()) {
            printMakingVerb = randMsg;
        }
        if (!isControlVerbMenu) {
            DrawString(ovalCenterX - ovalWidth * 3 / 8 + 15, ovalCenterY + ovalHeight / 4 - 10, "Zで入力　Enterで完成", c_black);
        }

        if (fontHandleMsg == 0) {
            fontHandleMsg = CreateFontToHandle(NULL, 20, 8);
        }

        DrawFormatStringToHandle(ovalCenterX - ovalWidth / 2 + 8, ovalCenterY - 8, c_black, fontHandleMsg, "%s", printMakingVerb.c_str());
    }

    

    //メニュー位置の処理
    if (isOpenVerbMenu) {
        if (maxSmoothMove > smoothMove) {
            smoothMove += delta;
            if (maxSmoothMove <= smoothMove)
                smoothMove = maxSmoothMove;
        }
    }
    //メニュー開いてない時
    else {
        if (0 < smoothMove) {
            smoothMove -= delta;
            if (0 >= smoothMove) {
                smoothMove = 0;
                makingVerb = {};
            }
        }
    }

    
}

void printVector(vector<vector<string>> array) {
    //デバッグ用　2次vectorをプリント
    printfDx("------------------------------\nprint\n");
    for (int i = 0; i < array.size(); i++) {
        for (int j = 0; j < array[i].size(); j++) {
            printfDx("%s ",array[i][j].c_str());
        }
        printfDx("\n");
    }
}

vector<string> reshapeStringToVector(string _str) {
    vector<string> result = {};
    size_t charSize = 2;

    for (size_t i = 0; i < _str.size(); i += charSize) {
        string charStr = _str.substr(i, charSize);
        result.push_back(charStr);
    }
    return result;
}

string reshapeVectorToString(vector<string> _str) {
    string result = "";
    for (int i = 0; i < _str.size(); i++) {
        result += _str[i];
    }
    return result;
}

void retryStage() {
    //printfDx("%d\n", retryCount);
    if (CheckHitKey(KEY_INPUT_R)) {
        retryCount++;
    }
    if (!CheckHitKey(KEY_INPUT_R)) {
        retryCount = 0;
    }

    if (retryCount != 0) {
        int thick = 3;
        for (int i = 0; i < thick; i++) {
            DrawBox(1005+i, 687+i, 1005 + 107 * retryCount / 30-i, 710-i, c_red, FALSE);
        }
    }
    if (retryCount >= 30) {
        retryCount = 0;
        PlaySoundMem(SEHandle_restartStage, DX_PLAYTYPE_BACK, TRUE);
        int startTimeBuff = startTime;
        backSelectScene();
        changeScene(sceneGame);
        setStageData(stageData);
        startTime = startTimeBuff;
    }
}

void returnSelect() {
    //printfDx("%d\n", returnCount);
    if (CheckHitKey(KEY_INPUT_ESCAPE)) {
        returnCount++;
    }
    if (!CheckHitKey(KEY_INPUT_ESCAPE)) {
        returnCount = 0;
    }

    if (returnCount != 0) {
        int thick = 3;
        for (int i = 0; i < thick; i++) {
            DrawBox(1115+i, 687+i, 1115 + 160 * returnCount / 30-i, 710-i, c_red, FALSE);
        }
    }
    if (returnCount>=30) {
        returnCount = 0;
        PlaySoundMem(SEHandle_exitStage, DX_PLAYTYPE_BACK, TRUE);
        backSelectScene();
    }

}

void setStageData(Data _stageData) {
    stageData = _stageData;
}

void showDebug(int x, int y) {
    //Game.cpp内でしか使わない
    if (CheckHitKey(KEY_INPUT_D) != 0) { //Dキーが押されていたらデバッグ表示
        int color = c_green;

        DrawBox(x-10, y-10 ,x + 400, y + 400, c_white, TRUE);

        DrawFormatString(x, y,       color, " ステージ名　　　　:%s", stageData.stageName.c_str());
        DrawFormatString(x, y + 20,  color, " マップファイルパス:%s", stageData.stageFile.c_str());
        DrawFormatString(x, y + 40,  color, " マップ高さ　　　　:%d", stageData.mapHeight);
        DrawFormatString(x, y + 60,  color, " マップ幅　　　　　:%d", stageData.mapWidth);
        DrawFormatString(x, y + 80,  color, " 初期∞文字　　　　:%s", stageData.infKana.c_str());
        DrawFormatString(x, y + 100, color, " 初期文字数　　　　:%d", stageData.numKana);

        DrawFormatString(x, y + 130, color, " magRate　　　　   :%.1f", magRate);

        DrawFormatString(x, y + 170, color, " map", stageMap);
        for (int i = 0; i < stageData.mapHeight; i++) {
            for (int j = 0; j < stageData.mapWidth; j++) {
                DrawFormatString(x+10 + 20 * j, y + 190 + 30 * i, color, "%s", stageMap[i][j].c_str());
            }
        }
        


    }
}

void sortUsableVerbList() {
    if (!usableVerbList.empty()) {
        vector<string> useVerb = {};
        vector<string> autoVerb = {};

        sort(usableVerbList.begin(), usableVerbList.end());

        //auto make:T Fで分けてconcat
        for (int i = 0; i < usableVerbList.size(); i++) {
            if (getVerbListValue(usableVerbList[i], "auto")) {
                autoVerb.push_back(usableVerbList[i]);
            }
            else{
                useVerb.push_back(usableVerbList[i]);
            }
        }
        autoVerb.insert(autoVerb.end(), useVerb.begin(), useVerb.end());
        usableVerbList = autoVerb;
    }
}

vector<string> split(const string& str, char delimiter) {
    vector<string> result;
    string::size_type start = 0;
    string::size_type end;

    // delimiterが見つかる位置を探して分割
    while ((end = str.find(delimiter, start)) != string::npos) {
        result.push_back(str.substr(start, end - start));  // 部分文字列を取得
        start = end + 1;  // 次の位置に移動
    }

    // 最後の部分を追加
    result.push_back(str.substr(start));
    return result;
}

vector<string> splitHiragana(const string& verb) {
    vector<string> result;
    for (size_t i = 0; i < verb.size();) {
        unsigned char c = verb[i];  // 現在のバイトを取得
        int charSize;

        // Shift_JISの1バイト目が0x80以上の場合、2バイト文字
        if (c >= 0x80) {
            charSize = 2; // 2バイト文字（ひらがなやカタカナなど）
        }
        else {
            charSize = 1; // 1バイト文字（ASCII文字など）
        }

        // 文字列から1文字（または2バイト）を切り出し、vectorに追加
        result.push_back(verb.substr(i, charSize));
        i += charSize; // バイト数分インデックスを進める
    }
    return result;
}

void updateUndoBuff() {
    isCanUndo = true;
    undoObjectList = objectList;
    undoPlayerPosX = player->posX;
    undoPlayerPosY = player->posY;
    undoPlayerDire = player->dire;
    undoKanaList = kanaList;
    undoIsGetCoin = isGetCoin;
    undoIsSitPlayer = isSitPlayer;
    undoIsSeeThroughPlayer = isSeeThroughPlayer;
    if (isPrintDebug)printfDx("update\n");
}

void useLastVerb() {
    //printfDx("%d\n", keyLockCount);
    if (CheckHitKey(KEY_INPUT_C) && keyLockCount == 0) {
        //printfDx("aaa\n");
        keyLockCount = 10;
        vector<string> buff = usableVerbList;
        for (int i = 0; i < usableVerbList.size(); i++) {
            if (buff[i] == lastVerb) {
                //printfDx("%s\n", lastVerb.c_str());
                chooseVerb(lastVerb);
            }
        }
    } 
}

void writeStageList(vector<Data> stageList) {
    // 書き込むファイル名
    const string fileName = stageListPath;

    // ファイルを開く（上書きモード）
    ofstream file(fileName, ios::trunc); // 上書きのために ios::trunc を使う

    // ファイルが正常に開けたか確認
    if (!file.is_open()) {
        return;
    }

    // ヘッダーを書き込む
    file << "StageName,StageFile,MapHeight,MapWidt,infKana,NumKana,clear,getCoin,StageLevel\n"; // 適宜ヘッダ内容は変更

    // stageList の内容を書き込む
    for (int i = 0; i < stageList.size();i++) {
        string row = "";
        row += stageList[i].stageName + ',';
        row += stageList[i].stageFile + ',';
        row += to_string(stageList[i].mapHeight) + ',';
        row += to_string(stageList[i].mapWidth) + ',';
        row += stageList[i].infKana + ',';
        row += to_string(stageList[i].numKana) + ',';
        if (stageList[i].clear) row += "T,";
        else row += "F,";
        if (stageList[i].getCoin) row += "T,";
        else row += "F,";
        row += to_string(stageList[i].stageLevel);

        file << row;
        file << "\n"; // 行の終わりに改行を挿入
    }

    // ファイルを閉じる
    file.close();
}

void writeVerbList(vector<vector<string>> verbList) {
    // 書き込むファイル名
    const string fileName = verbListPath;

    // ファイルを開く（上書きモード）
    ofstream file(fileName, ios::trunc); // 上書きのために ios::trunc を使う

    // ファイルが正常に開けたか確認
    if (!file.is_open()) {
        return;
    }

    // ヘッダーを書き込む
    file << "verb,make,auto\n"; // 適宜ヘッダ内容は変更

    // verbList の内容を書き込む
    for (const auto& row : verbList) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ","; // 最後のカンマは付けない
            }
        }
        file << "\n"; // 行の終わりに改行を挿入
    }

    // ファイルを閉じる
    file.close();
}

vector<vector<string>> loadObjectList() {
    vector<vector<string>> objectList = {};//全てのstageData

    string line;//ファイル1行分読み込みの置き場
    ifstream ifs(objectFilePath);//読み込むファイル

    getline(ifs, line);//ヘッダーを飛ばす

    //1行読み込み、その1行をカンマで区切ってtempDataに格納
    for (int i = 0; getline(ifs, line); i++) {
        stringstream ss(line);
        vector<string> objectData = {};//1行分のstageData

        for (int j = 0; j < 2; j++) {
            string buff = "";
            getline(ss, buff, ',');
            objectData.push_back(buff);
        }

        objectList.push_back(objectData);//1行分のstageDataを全てのstageDataに足す
    }
    return objectList;
}

vector<vector<string>> loadStageMap(Data _stageData) {
    vector<vector<string>> stageMap = {};
    string line;//ファイル1行分読み込みの置き場
    ifstream ifs(_stageData.stageFile);//読み込むファイル

    for (int i = 0; i < _stageData.mapHeight; i++) {
        vector<string> mapBuf = {};
        getline(ifs, line);
        stringstream ss(line);
        for (int j = 0; j < _stageData.mapWidth; j++) {
            string buff = "";
            getline(ss, buff, ',');
            mapBuf.push_back(buff.c_str());
        }
        stageMap.push_back(mapBuf);//1行分のstageDataを全てのstageDataに足す
        //printfDx("\n\n\n%d:%s %s %s", i, stageMap[i][0].c_str(), stageMap[i][1].c_str(), stageMap[i][2].c_str());
    }

    return stageMap;
}

//オブジェクト同士接触の処理について
void touchCoin() {
    if (!isSeeThroughPlayer) {
        //playerとkanaが接触した時
        kanaUnit buffKana = {};
        string kanaName = objectList[calcObjectListIndex(player->posX, player->posY)]->name;//そのれがCoin

        if (kanaName == "C") {
            doGetCoin(player->posX, player->posY);
        }
        //printfDx("%s\n", isGetCoin ? "T" : "F");
    }
}

void touchFlag() {
    if (!isSeeThroughPlayer) {
        if (objectList[stageData.mapWidth * player->posX + player->posY]->name == "F") {
            if (clearTime == 59) {
                int clearSound = SEHandle_clearStage;
                if (isGetCoin) {
                    clearSound = SEHandle_clearStageCoin;
                }
                PlaySoundMem(clearSound, DX_PLAYTYPE_BACK, TRUE);
            }
            doClearProcess();
        }
        else {
            clearTime = 60;
        }
    }
}

void touchKana() {
    if (!isSeeThroughPlayer) {
        //playerとkanaが接触した時
        string kanaKinds = objectList[calcObjectListIndex(player->posX, player->posY)]->kinds;
        if (kanaKinds == "kana") {
            doGetKana(player->posX, player->posY);
        }
    }
}

//クリア処理
void doClearProcess() {
    //printfDx("clearTime: %d\n", clearTime);
    clearTime -= 1;
    //クリア！
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(0, 0, windowSizeX, windowSizeY, c_white, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

    drawClearScene();

    isMovePlayer = false;
    isCanUndo = false;
    if (clearTime <= 0) {
        if (!stageList[cursorStageList % stageList.size()].clear || (!stageList[cursorStageList % stageList.size()].getCoin) && isGetCoin) {
            //初めてのクリア　または　初めてコイン獲得
            if (!stageList[cursorStageList % stageList.size()].getCoin && isGetCoin) {
                stageList[cursorStageList % stageList.size()].getCoin = TRUE;
            }
            stageList[cursorStageList % stageList.size()].clear = TRUE;
            writeStageList(stageList);
        }
        backSelectScene();
    }
}

void doGetCoin(int _posX, int _posY) {
    isGetCoin = true;
    PlaySoundMem(SEHandle_getCoin, DX_PLAYTYPE_BACK, TRUE);
    //フィールドのkanaオブジェクトを消す
    objectList[calcObjectListIndex(_posX, _posY)] = new Air(_posX, _posY);
}

void doGetKana(int _posX, int _posY) {
    kanaUnit buffKana = {};
    string kanaName = objectList[calcObjectListIndex(_posX, _posY)]->name;//そのkana "kana_A"
    string kanaKinds = objectList[calcObjectListIndex(_posX, _posY)]->kinds;

    PlaySoundMem(SEHandle_getKana, DX_PLAYTYPE_BACK, TRUE);

    //既にkanaを持っているなら
    if (isExistKana(kanaList, kanaName)) {
        //+1する
        for (int i = 0; i < kanaList.size(); i++) {
            if (kanaList[i].kana == kanaName && !(kanaList[i].numTimes < 0)) {
                kanaList[i].numTimes += 1;
            }
        }
    }
    //持っていないなら
    else {
        buffKana.kana = kanaName;
        buffKana.numTimes = 1;
        string handlePath = "./data/image/kana2_" + buffKana.kana + ".png";
        buffKana.handle = LoadGraph(handlePath.c_str());
        kanaList.push_back(buffKana);
        createUsableVerbList();//usableを更新
    }

    //フィールドのkanaオブジェクトを消す
    objectList[calcObjectListIndex(_posX, _posY)] = new Air(_posX, _posY);

    isUpdateUsableVerbList = true;
}

//以下　動詞使用時の処理
void chooseVerb(string _verb) {
    //1文字ずつのvectorに分ける
    vector<string> verb = splitHiragana(_verb);
    string targetName = objectList[calcTargetIndex(player->posX, player->posY, player->dire)]->name;
    string targetKinds = objectList[calcTargetIndex(player->posX, player->posY, player->dire)]->kinds;
    bool targetThrough = objectList[calcTargetIndex(player->posX, player->posY, player->dire)]->through;
    //printfDx("target: %s\n", targetName.c_str());
    //対象オブジェクトと動詞の処理　力業で全部ここに実装　if文から各種関数を実行
    //if      (_verb == "あるく") doWalk(verb);
    
    if (targetThrough) {
        if (_verb == "あるく") { walkPlayer(verb); return; }
    }
    if (!isSeeThroughPlayer) {
        if (_verb == "けす") {
            objectErase(verb); return;
        }
        else if (_verb == "すける") {
            playerSeeThrough(verb); return;
        }
        else if (_verb == "すわる") {
            playerSit(verb); return;
        }

        if (targetName == "A") {

        }
        else if (targetName == "B") {
            if (_verb == "なおす") { flagFix(verb); return; }
        }
        else if (targetName == "C") {
            if (_verb == "える" || _verb == "とる") { coinGet(verb); return; }
        }
        else if (targetName == "D") {
            if (_verb == "あける") { doorOpen(verb); return; }
            if (_verb == "ける") { doorKick(verb); return; }
        }
        else if (targetName == "F") {
            if (_verb == "おる") { flagFold(verb); return; }
        }
        else if (targetName == "G") {
            if (_verb == "まわす") { gearRotate(verb); return; }
        }
        else if (targetName == "R") {
            if (_verb == "わる") { rockCrack(verb); return; }
            if (_verb == "おす" || _verb == "ける") { rockPush(verb); return; }
            if (_verb == "こえる" || _verb == "こす") { rockJump(verb); return; }
        }
        else if (targetName == "T") {
            if (_verb == "きる" || _verb == "こる") { treeCut(verb); return; }
            if (_verb == "おる") { treeFold(verb); return; }
        }
        else if (targetName == "W") {
            if (_verb == "わる") { rockCrack(verb); return; }
        }

        if (targetKinds == "kana") {
            if (_verb == "える" || _verb == "とる") { getKana(verb); return; }
        }


    }
    PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);

}

void useVerb(vector<string> _verb) {
    int charSize = 2;

    //あるく以外は吹き出しを出す
    if (reshapeVectorToString(_verb) != "あるく") {
        //吹き出し内の文字列変更
        speechVerb = reshapeVectorToString(_verb);
        speechBubble = 20;
    }

    lastUseVerb = _verb;
    kanaHighLightTime = 40;

    //使った文字を減らす
    for (int i = 0; i < _verb.size(); i++) {
        for (int j = 0; j < kanaList.size(); j++) {
            if (_verb[i] == kanaList[j].kana && kanaList[j].numTimes > 0) {//有限なら減らす
                kanaList[j].numTimes -= 1;
                break;
            }
        }
    }

    //回数0のkanaUnitを消す
    for (int i = (int)kanaList.size() - 1; i >= 0; i--) {
        if (kanaList[i].numTimes == 0) {
            kanaList.erase(kanaList.begin() + i);  // 要素を削除して間を詰める
        }
    }
    createUsableVerbList();

    //lastVerbに登録するかどうか
    string concatString = "";
    if (!getVerbListValue(reshapeVectorToString(_verb), "auto")) {
        for (int i = 0; i < _verb.size(); i++) {
            concatString += _verb[i];
        }
        lastVerb = concatString;
    }
    
    //メニュー閉じる
    isOpenVerbMenu = false;
    isMovePlayer = true;

    isUpdateUsableVerbList = true;
}


void coinGet(vector<string> _verb) {
    if (isPrintDebug)printfDx("getCoin\n");
    updateUndoBuff();
    doGetCoin(calcTargetPos(player->posX, player->posY, player->dire)[0], calcTargetPos(player->posX, player->posY, player->dire)[1]);
    useVerb(_verb);
}
void doorKick(vector<string> _verb) {
    if (isPrintDebug)printfDx("doorKick\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_kickDoor, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new Air(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void doorOpen(vector<string> _verb) {
    if (isPrintDebug)printfDx("doorOpen\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_openDoor, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new Air(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void flagFix(vector<string> _verb) {
    if (isPrintDebug)printfDx("flagFix\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_fixFlag, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new Flag(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void flagFold(vector<string> _verb) {
    if (isPrintDebug)printfDx("flagFold\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_foldFlag, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new BrokenFlag(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void getKana(vector<string> _verb) {
    if (isPrintDebug)printfDx("getKana\n");
    updateUndoBuff();
    doGetKana(calcTargetPos(player->posX, player->posY, player->dire)[0], calcTargetPos(player->posX, player->posY, player->dire)[1]);
    useVerb(_verb);
}
void objectErase(vector<string> _verb) {
    if (isPrintDebug)printfDx("eraseObject\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_eraseObject, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new Air(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void playerSeeThrough(vector<string> _verb) {
    if (isPrintDebug)printfDx("playerSeeThrough\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_eraseObject, DX_PLAYTYPE_BACK, TRUE);
    isSeeThroughPlayer = true;
    useVerb(_verb);
}
void playerSit(vector<string> _verb) {
    if (isPrintDebug)printfDx("playerSit\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_sitPlayer, DX_PLAYTYPE_BACK, TRUE);
    isSitPlayer = true;
    useVerb(_verb);
}
void rockCrack(vector<string> _verb) {
    if (isPrintDebug)printfDx("rockCrack\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_crackRock, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new Air(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void rockJump(vector<string> _verb) {
    vector<int> rockPos = calcTargetPos(player->posX, player->posY, player->dire);
    vector<int> airPos = calcTargetPos(rockPos[0], rockPos[1], player->dire);
    if (objectList[calcObjectListIndex(airPos[0], airPos[1])]->through) {
        if (isPrintDebug)printfDx("rockJump\n");
        updateUndoBuff();
        PlaySoundMem(SEHandle_jumpRock, DX_PLAYTYPE_BACK, TRUE);
        player->posX = airPos[0];
        player->posY = airPos[1];
        useVerb(_verb);
        return;
    }
    PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
}
void rockPush(vector<string> _verb) {
    vector<int> rockPos = calcTargetPos(player->posX, player->posY, player->dire);
    vector<int> nextPos = calcTargetPos(rockPos[0], rockPos[1], player->dire);
    if (objectList[calcObjectListIndex(nextPos[0], nextPos[1])]->name=="A") {
        if (isPrintDebug)printfDx("rockPush\n");
        updateUndoBuff();
        PlaySoundMem(SEHandle_pushRock, DX_PLAYTYPE_BACK, TRUE);
        objectList[calcObjectListIndex(rockPos[0], rockPos[1])] = new Air(rockPos[0], rockPos[1]);
        objectList[calcObjectListIndex(nextPos[0], nextPos[1])] = new Rock(nextPos[0], nextPos[1]);
        useVerb(_verb);
        return;
    }
    else if (objectList[calcObjectListIndex(nextPos[0], nextPos[1])]->name == "H") {
        if (isPrintDebug)printfDx("rockPushFill\n");
        updateUndoBuff();
        PlaySoundMem(SEHandle_pushRock, DX_PLAYTYPE_BACK, TRUE);
        objectList[calcObjectListIndex(rockPos[0], rockPos[1])] = new Air(rockPos[0], rockPos[1]);
        objectList[calcObjectListIndex(nextPos[0], nextPos[1])] = new Fill(nextPos[0], nextPos[1]);
        useVerb(_verb);
        return;
    }
    PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
}
void rockPushFill(vector<string> _verb) {
    vector<int> rockPos = calcTargetPos(player->posX, player->posY, player->dire);
    vector<int> holePos = calcTargetPos(rockPos[0], rockPos[1], player->dire);
    if (objectList[calcObjectListIndex(holePos[0], holePos[1])]->name == "H") {
        if (isPrintDebug)printfDx("rockPushFill\n");
        updateUndoBuff();
        PlaySoundMem(SEHandle_pushRock, DX_PLAYTYPE_BACK, TRUE);
        objectList[calcObjectListIndex(rockPos[0], rockPos[1])] = new Air(rockPos[0], rockPos[1]);
        objectList[calcObjectListIndex(holePos[0], holePos[1])] = new Fill(holePos[0], holePos[1]);
        useVerb(_verb);
        return;
    }
    PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
}
void treeCut(vector<string> _verb) {
    if (isPrintDebug)printfDx("treeCut\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_cutTree, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new Air(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void treeFold(vector<string> _verb) {
    if (isPrintDebug)printfDx("treeFold\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_foldFlag, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    objectList[calcObjectListIndex(targetPos[0], targetPos[1])] = new Air(targetPos[0], targetPos[1]);
    useVerb(_verb);
}
void gearRotate(vector<string> _verb) {
    //隣接する4つのオブジェクトを時計回りに90度
    int num = 4;
    if (isPrintDebug)printfDx("gearRotate\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_rotateGear, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);

    vector<int> objPosX = { targetPos[0] + 1, targetPos[0], targetPos[0] - 1, targetPos[0] };
    vector<int> objPosY = { targetPos[1], targetPos[1] -1 , targetPos[1], targetPos[1] + 1 };
    vector<string> objStr = {};
    for (int i = 0; i < num; i++) {
        objStr.push_back(objectList[calcObjectListIndex(objPosX[i], objPosY[i])]->name);
    }
    for (int i = 0; i < num; i++) {
        objectList[calcObjectListIndex(objPosX[i], objPosY[i])] = generateObject(objStr[(i + num - 1) % num], objPosX[i], objPosY[i]);
    }

    //playerの位置変更
    if (player->dire == dire_DOWN) {
        //上から
        player->posX = targetPos[0];
        player->posY = targetPos[1] + 1;
        player->dire = dire_LEFT;
    }
    else if (player->dire == dire_LEFT) {
        //右から
        player->posX = targetPos[0] + 1;
        player->posY = targetPos[1];
        player->dire = dire_UP;
    }
    else if (player->dire == dire_UP) {
        //下から
        player->posX = targetPos[0];
        player->posY = targetPos[1] - 1;
        player->dire = dire_RIGHT;
    }
    else if (player->dire == dire_RIGHT) {
        //左から
        player->posX = targetPos[0] - 1;
        player->posY = targetPos[1];
        player->dire = dire_DOWN;
    }
    useVerb(_verb);
}
void walkPlayer(vector<string> _verb) {
    if (isPrintDebug)printfDx("walkPlayer\n");
    updateUndoBuff();
    PlaySoundMem(SEHandle_walkPlayer, DX_PLAYTYPE_BACK, TRUE);
    vector<int> targetPos = calcTargetPos(player->posX, player->posY, player->dire);
    player->posX = targetPos[0];
    player->posY = targetPos[1];
    useVerb(_verb);
}