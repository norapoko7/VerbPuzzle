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

//�Ȃ����w�b�_���ł͓����Ȃ������̂Ŏd���Ȃ��c�����ȊO�ł͎g��Ȃ��̂�
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
vector<vector<string>> stageMap = {}; //�X�e�[�W�̃}�b�v���i�[
vector<vector<string>> stageObject = {}; //�X�e�[�W�ɏo�Ă���I�u�W�F�N�g�Ƃ��̉摜�p�X�̔z����i�[
vector<Object*> objectList = {};//�X�e�[�W���̂��ׂẴI�u�W�F�N�g
vector<Object*> undoObjectList = {};//1��߂��p
Object* player = {};
int undoPlayerPosX = 0;//1��߂��p
int undoPlayerPosY = 0;
int undoPlayerDire = 0;
vector<kanaUnit> kanaList = {};
vector<kanaUnit> undoKanaList = {};
vector<string> usableVerbList = {};
string lastVerb = {};//���邭�ȊO�A�Ō�Ɏg�p���������@C�L�[�Ďg�p�̂���
vector<string> lastUseVerb = {};//�Ō�Ɏg�p��������
vector<string> showVerbList = {};//�������j���[�̈ꗗ
vector<int> unusable = {};//�����ꗗ�ŁH�H�ɂȂ���̂̃C���f�b�N�X�ꗗ
vector<int> autoVerb = {};


double magRate = 1;//�}�b�v�g��\���̌W��
int fieldStartX = 0;//�}�b�v�\������̍��W
int fieldStartY = 0;

int clearTime = 0;
int startTime = 0;
int elapseTime = 0;//�o�ߎ���
int startTimeBuff;// restart�p
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
int maxSmoothMove = 60; //�������قǑ����J��
int colorClear = 0;
int speechBubble = 0;
string speechVerb = "";
bool isPrintDebug = false;
int kanaHighLightTime = 0;
bool isUpdateUsableVerbList = true;//true�̎�usableVerbList���X�V�A���������T�A�X�V������F�ɂ���
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
//�V�X�e��
extern int SEHandle_closeMenu;
extern int SEHandle_exitStage;
extern int SEHandle_intoStage;
extern int SEHandle_moveCursor;
extern int SEHandle_openMenu;
extern int SEHandle_restartStage;
//�X�e�[�W��
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

//�X�V
void updateGame() {
    if (CheckHitKey(KEY_INPUT_2) != 0) { //ESC�L�[��������Ă�����
        backSelectScene();
    }
}

//�`��
void drawGame() {
    //����̂ݎ��s
    if (stageMap.empty()) {
        stageMap = loadStageMap(stageData); //map�f�[�^�擾
        //stageObject = createStageObject(stageMap);

        //kanaList�쐬
        createKanaList(split(stageData.infKana.c_str(), '-'));

        //�I�u�W�F�N�g�̍쐬
        createObjectList(&objectList);
        
        //usableVerbList�쐬
        createUsableVerbList();
        
        //magRate�ȂǕ`��ɕK�v�Ȑݒ� magRate: �g�嗦 �`��G���A���łł��邾���傫��
        magRate = floor(min(squareHeight/(double)stageData.mapHeight, squareWidth/(double)stageData.mapWidth) * 2)/2;//0.5���ݐ؂�̂�
        fieldStartX = (int)(fieldCenterX - squareSize * stageData.mapWidth / 2.0 * magRate);
        fieldStartY = (int)(fieldCenterY - squareSize * stageData.mapHeight / 2.0 * magRate);

        //undo�p�����l
        undoObjectList = objectList;
        undoPlayerPosX = player->posX;
        undoPlayerPosY = player->posY;
        undoPlayerDire = player->dire;
        undoKanaList = kanaList;
        undoIsGetCoin = isGetCoin;
        undoIsSitPlayer = isSitPlayer;
        undoIsSeeThroughPlayer = isSeeThroughPlayer;
    }

    //�I�u�W�F�N�g�̕`��
    for (int i = 0; i < objectList.size(); i++) {
        objectList[i]->drawObject(magRate, fieldStartX, fieldStartY);
    }
    player->drawObject(magRate, fieldStartX, fieldStartY);

    //undo�p�̏���
    //updateUndoBuff();
    doUndo();
    //printfDx("%d\n", startTime);
    //printfDx("%d, %d\n\n", undoPlayerPosX, undoPlayerPosY);

    //��ʕ`��
    drawKanaList();
    openVerbMenu();
    
    drawHelp();
    drawUseVerb(speechVerb);

    //�����t���I�u�W�F�N�g�X�V
    player->movePlayer();
    useLastVerb();

    //�}�C�t���[���X�V
    //createUsableVerbLit(); �}�C�t���[���͏d������
    if (keyLockCount > 0)keyLockCount--;

    //�I�u�W�F�N�g�Ԃ̏���
    touchCoin();
    touchKana();

    touchFlag();//�S�[����������̂ň�Ԍ���
    returnSelect();
    retryStage();

    //DrawString(0, 0, "Game��ʂł��B", GetColor(255, 255, 255));
    //DrawString(0, 20, "2�L�[�������ƃZ���N�g��ʂցB", GetColor(255, 255, 255));

    //�f�o�b�O�A�Ō�ɏ���
    //showDebug(0,60);
}

void backSelectScene() {
    //������������
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

    changeScene(sceneSelect);//�V�[�����Z���N�g�ɕύX
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
        {"TAB�œ����쐬���[�h�I",100},
        {"�������͎����Ŏg����",30},
        {"�m��Ȃ�������??�ŕ\��",20},
        {"?�̌��͕������̃q���g",20},
        {"�V�t�g�Ō��������ς��",20},
        {"C�Œ��O�̓���������1�x",20},
        {"�R�C��,��ꂽ�炤�ꂵ��",20},
        {"�������g���Ăǂ����悤",5},
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
    return "���̃��b�Z�[�W�͏o�Ȃ��͂�";
}

vector<vector<string>> createStageObject(vector<vector<string>> _stageMap) {
    vector<vector<string>> stageObject = {};
    vector<vector<string>> objectList = loadObjectList();
    for (int i = 0; i < _stageMap.size(); i++) {
        for (int j = 0; j < _stageMap[0].size(); j++) {
            vector<string> object = {};//1�s��
            if (!isExistString(stageObject, _stageMap[i][j].c_str()) && _stageMap[i][j]!="") {
                //�͂��߂ďo�Ă�������&""�łȂ����̂����X�g�ɒǉ�
                object.push_back(_stageMap[i][j].c_str());

                //���̕����̃C���[�W�t�@�C���p�X��ǉ�
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

    // �r�b�g�}�X�N���g���ĕ����W���𐶐�
    for (int i = 1; i < (1 << n); ++i) {
        vector<string> subset;

        // i�̊e�r�b�g�ɑΉ�����kanaList�̗v�f�𕔕��W���ɒǉ�
        for (int j = 0; j < n; ++j) {
            if (i & (1 << j)) {
                subset.push_back(kanaList[j].kana);
            }
        }

        sort(subset.begin(), subset.end());

        // �����W���̏���𐶐�
        do {
            // �����printfDx�ŕ\��
            string result;
            for (const auto& kana : subset) {
                result += kana;
            }
            makableVerbList.push_back(result);
            //printfDx("%s\n", result.c_str());
        } while (next_permutation(subset.begin(), subset.end()));
    }

    //makable��verbList�̐ϏW����usable�ɂ���
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
    DrawFormatStringToHandle(windowSizeX / 2 - 265, windowSizeY / 2 + 50, colorClear, fontHandleDetail, "Clear time: %.1f �b", (double)elapseTime/FPS);
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
        //�I�𒆂̓����Ŏg���������n�C���C�g
        if (isOpenVerbMenu && isControlVerbMenu && getVerbListValue(usableVerbList[cursorUPPos % usableVerbList.size()], "make")) {
            drawHighlightKana(reshapeStringToVector(usableVerbList[cursorUPPos % usableVerbList.size()]));
        }
        //�g�����������n�C���C�g����
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
        DrawFormatString(EX-squareSize* kanaImageMag /8, EY-squareSize* kanaImageMag *3/8, color2, "%s", kanaList[i].numTimes>=0 ? to_string(kanaList[i].numTimes).c_str() : "��");
    }

    
}

void drawHelp() {
    if (fontHandleNormal == 0) {
        fontHandleNormal = CreateFontToHandle(NULL, -1, -1);
        fontHandleHelp = CreateFontToHandle(NULL, 20, 4);
        fontHandleUndo = CreateFontToHandle(NULL, 25, 10);
    }

    vector<string> helpMsg = { "B:1��߂�",
                               "Z:����",
                               "X:���j���[�J��",
                               "Shift+�����L�[:�����ύX",
                               "C:�Ō�Ɏg�p��������\"" + lastVerb +"\"",
                               "Tab:���j���[�؂�ւ�",
                               "Enter:�����쐬",
                               "backspace:1��������",
                               "R:���X�^�[�g",
                               "ESC:�X�e�[�W�I����"};
    
    DrawFormatStringToHandle(20, 680, isCanUndo? c_white :c_shade2 , fontHandleUndo, "%s", helpMsg[0].c_str());
    DrawFormatStringToHandle(170, 685, c_white, fontHandleHelp, "%s", helpMsg[1].c_str());
    DrawFormatStringToHandle(250, 685, c_white, fontHandleHelp, "%s", helpMsg[2].c_str());

    DrawFormatStringToHandle(1010, 690, c_white, fontHandleNormal, "%s", helpMsg[8].c_str());
    DrawFormatStringToHandle(1120, 690, c_white, fontHandleNormal, "%s", helpMsg[9].c_str());

    if (!isOpenVerbMenu) {
        //���j���[���J���Ă��Ȃ�
        DrawFormatStringToHandle(410, 685, c_white, fontHandleHelp, "%s", helpMsg[3].c_str());
        DrawFormatStringToHandle(660, 685, c_white, fontHandleHelp, "%s", helpMsg[4].c_str());
    }
    else if (isControlVerbMenu) {
        //���j���[�J���Ă��ā@�����ꗗ
        DrawFormatStringToHandle(410, 685, c_white, fontHandleHelp, "%s", helpMsg[5].c_str());
    }
    else {
        //���j���[�J���Ă��ā@�����ꗗ
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
    vector<string> showUseVerb = {};//�g���铮�����X�g
    vector<string> showNotUseVerb = {};//�g���Ȃ��������X�g�@�S���H�H
    string showVerb = "";

    int paragraphOffset = 0;
    int c_verb = c_black;
    
    if (isUpdateUsableVerbList) {
        showVerbList = {};
        unusable = {};
        autoVerb = {};
        //?�������\�����邩������
        for (int i = 0; i < usableVerbList.size(); i++) {
            showVerb = usableVerbList[i];
            if (getVerbListValue(usableVerbList[i], "make")) {//��������Ƃ��铮��
                if (getVerbListValue(usableVerbList[i], "auto")) {
                    //auto�̓����̃C���f�b�N�X
                    autoVerb.push_back(i);
                }
                showUseVerb.push_back(showVerb);
            }
            else {
                //�Ȃ������當�������̂܂܁H�H�H�ɂȂ�
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
            //�I�[�g�����Ȃ�
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
    //player ���W
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
        //speechBubble��������
        speechBubble--;

        if (!isOpenVerbMenu) {
            //�����o���̕`��
            //�}�b�v�㔼���H
            if (stageData.mapHeight / 2 >= player->posX) { ovalCenterY = pPosEY + ovalHeight / 2; triTopY = pPosEY; }
            else { ovalCenterY = pPosSY - ovalHeight / 2; triTopY = pPosSY; }
            //�}�b�v�������H
            if (stageData.mapWidth / 2 >= player->posY) { ovalCenterX = pPosEX + ovalWidth / 2; triTopX = pPosEX; }
            else { ovalCenterX = pPosSX - ovalWidth / 2; triTopX = pPosSX; }
            DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_white, TRUE);
            DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_black, FALSE);
            DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_white, TRUE);
            DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_black, FALSE);

            //�����傫���ݒ�
            if (fontHandleSpeechVerb == 0) {
                fontHandleSpeechVerb = CreateFontToHandle(NULL, 36, 20);
            }

            //�����ʒu�ݒ�(���������Ƃ�)
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
    else if (_objName == "��") return new kana_A(_posX, _posY);
    else if (_objName == "��") return new kana_E(_posX, _posY);
    else if (_objName == "��") return new kana_O(_posX, _posY);
    else if (_objName == "��") return new kana_KI(_posX, _posY);
    else if (_objName == "��") return new kana_KU(_posX, _posY);
    else if (_objName == "��") return new kana_KE(_posX, _posY);
    else if (_objName == "��") return new kana_KO(_posX, _posY);
    else if (_objName == "��") return new kana_SU(_posX, _posY);
    else if (_objName == "��") return new kana_TO(_posX, _posY);
    else if (_objName == "��") return new kana_NA(_posX, _posY);
    else if (_objName == "��") return new kana_MA(_posX, _posY);
    else if (_objName == "��") return new kana_RU(_posX, _posY);
    else if (_objName == "��") return new kana_WA(_posX, _posY);
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
    //kind�́@make�@���@auto
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
    // 2����vector�̊e�s�����[�v
    for (int i = 0; i < array.size(); i++) {
        // �����񂪌��������ꍇ
        if (array[i] == target) {
            return true;
        }

    }
    // ������Ȃ������ꍇ
    //printfDx("false\n");
    return false;
}

bool isExistString(vector<string> array, string target) {
    // 2����vector�̊e�s�����[�v
    for (int i = 0; i < array.size(); i++) {
        // �����񂪌��������ꍇ
        if (array[i] == target) {
            return true;
        }

    }
    // ������Ȃ������ꍇ
    //printfDx("false\n");
    return false;
}

bool isExistString(vector<vector<string>> array, string target) {
    // 2����vector�̊e�s�����[�v
    for (int i = 0; i < array.size(); i++) {
        // �e�s�̊e�v�f�����[�v
        for (int j = 0; j < array[i].size(); j++) {
            // �����񂪌��������ꍇ
            if (array[i][j] == target) {
                return true;
            }
        }
    }
    // ������Ȃ������ꍇ
    //printfDx("false\n");
    return false;
}

bool isExistVerb(vector<string> _verb) {
    //kanaList�ɓ��͂̓��������邩
    //�� �� ��
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
    int delta = 10;//���j���[�J���̕`��ʒu�̕ω���
   
    int verbOffset = (int)smoothMove * verbXOffset / maxSmoothMove;

    int fieldOffset = (int)smoothMove * fieldXOffset / maxSmoothMove;
    //player ���W
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

    //X ���j���[�J��
    if (!isOpenVerbMenu && hitKey_X && keyLockCount == 0 && isMovePlayer) {
        keyLockCount = 15;
        isOpenVerbMenu = true;
        isMovePlayer = false;
        isControlVerbMenu = true;
        smoothMove = 0;
        randMsg = createRandomMsg();
        PlaySoundMem(SEHandle_openMenu, DX_PLAYTYPE_BACK, TRUE);
    }
    //Tab���j���[�J��
    if (!isOpenVerbMenu && hitKey_TAB && keyLockCount == 0 && isMovePlayer) {
        keyLockCount = 15;
        isOpenVerbMenu = true;
        isMovePlayer = false;
        isControlVerbMenu = false;
        smoothMove = 0;
        randMsg = createRandomMsg();
        PlaySoundMem(SEHandle_openMenu, DX_PLAYTYPE_BACK, TRUE);
    }
    //���j������
    if (isOpenVerbMenu && hitKey_X && keyLockCount == 0) {
        keyLockCount = 15;
        isOpenVerbMenu = false;
        isMovePlayer = true;
        smoothMove = maxSmoothMove;
        PlaySoundMem(SEHandle_closeMenu, DX_PLAYTYPE_BACK, TRUE);
    }

    //�����ꗗ���j���[�`��
    DrawBox(windowSizeX+verbOffset, margin*2+kanaListHeight, windowSizeX-verbXOffset+verbOffset, windowSizeY-margin*2, c_white, TRUE);//mainbox
    DrawBox(windowSizeX-margin+verbOffset, margin*2+kanaListHeight, windowSizeX+verbOffset, margin*2+kanaListHeight+ (int)(squareSize*1.5), c_white, TRUE);//index
    for (int i = 0; i < usableVerbList.size(); i++) {
        //�����Ԃ̉���
        DrawLine(windowSizeX + verbOffset + 12 , margin * 2 + kanaListHeight + (int)(squareSize * 1.5) * (i+1), windowSizeX - verbXOffset + verbOffset - 12, margin * 2 + kanaListHeight + (int)(squareSize * 1.5) * (i + 1), c_black);
    }

    DrawString(windowSizeX - margin + verbOffset + 6, margin * 2 + kanaListHeight + 8, "X", c_black);
    DrawString(windowSizeX - margin + verbOffset + 3, margin * 2 + kanaListHeight + 26, "��", c_black);
    if (usableVerbList.size() != 0) {
        int paragraphOffset = 0;
        if (cursorUPPos % usableVerbList.size() >= 12)paragraphOffset = 90;
        DrawString(1280 + verbOffset + 20 + paragraphOffset, 122 + (int)(squareSize * 1.5) * ((cursorUPPos % usableVerbList.size())%12), "��", c_black);
    }
    
    //�������j���[�̓����̕`��
    drawUsableVerbList();


    //���j���[�J���Ă�Ƃ��������s
    if (isOpenVerbMenu) {
        //TAB�L�[�ő��삷�郁�j���[��؂�ւ���
        if (hitKey_TAB && keyLockCount == 0) {
            PlaySoundMem(SEHandle_openMenu, DX_PLAYTYPE_BACK, TRUE);
            isControlVerbMenu = !isControlVerbMenu;
            keyLockCount = 15;
        }
        
        //�����ꗗ�G���Ă鎞
        //printfDx("%d\n", cursorUPPos);
        if (isControlVerbMenu) {
            //cursorPos�X�V
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
                    //�I�𓮎��̉E�ɓ���������
                    cursorUPPos += 12;
                }
                
            }
            else if (usableVerbList.size() >= 12 && hitKey_LEFT && keyLockCount == 0) {
                keyLockCount = 5;
                PlaySoundMem(SEHandle_moveCursor, DX_PLAYTYPE_BACK, TRUE);
                if (cursorUPPos % usableVerbList.size() >= 12) {
                    //�I�𓮎��̉E�ɓ���������
                    cursorUPPos -= 12;
                }
            }

            //Z�L�[�Ŏg������������
            if (usableVerbList.size() != 0) {
                int useVerbIndex = cursorUPPos % usableVerbList.size();
                if ((hitKey_Z || hitKey_SPACE || hitKey_RETURN) && keyLockCount == 0) {
                    keyLockCount = 10;
                    if (!isExistInt(unusable, useVerbIndex) && !isExistVerb(reshapeStringToVector("�Ƃ܂�"))) {
                        chooseVerb(usableVerbList[useVerbIndex]);
                    }
                    else {
                        PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);
                    }
                }
            }
        }
        //kana�ꗗ�G���Ă�Ƃ�
        else {
            //�I��g�`��
            int SX = margin + (int)(squareSize * kanaImageMag * 1.5) * (cursorLRPos % kanaList.size()) - squareSize * kanaImageMag / 8;
            int SY = margin - squareSize * kanaImageMag / 8;
            int EX = margin + squareSize * kanaImageMag + (int)(squareSize * kanaImageMag * 1.5) * (cursorLRPos % kanaList.size()) + squareSize * kanaImageMag * 3 / 8;
            int EY = margin + squareSize * kanaImageMag + squareSize * kanaImageMag / 8;
            int thick = 5;
            for (int i = 0; i < thick; i++) {
                DrawBox(SX+i, SY+i, EX-i, EY-i, c_green, FALSE);
            }

            //cursorPos�X�V
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

            //�����ꗗ�Ԋ|��
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
            DrawBox(0, margin * 2 + kanaListHeight, windowSizeX - verbXOffset + verbOffset, windowSizeY - margin * 2, c_shade, TRUE);//mainbox
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
            

            //Z�L�[�Ŏg��������string�ɒǉ�
            int useKanaIndex = cursorLRPos % kanaList.size();
            if ((hitKey_Z || hitKey_SPACE) && keyLockCount == 0) {
                keyLockCount = 5;
                PlaySoundMem(SEHandle_selectKana, DX_PLAYTYPE_BACK, TRUE);
                makingVerb.push_back(kanaList[useKanaIndex].kana);
            }

            //�o�b�N�X�y�[�X��makingString�̈�Ԍ�������
            if (hitKey_BACK && keyLockCount == 0) {
                keyLockCount = 10;
                if (!makingVerb.empty()) {  // ��ł͂Ȃ����m�F
                    PlaySoundMem(SEHandle_deleteKana, DX_PLAYTYPE_BACK, TRUE);
                    makingVerb.pop_back();   // �Ō�̗v�f���폜
                }
            }

            //ENTER�L�[��makingVerb����
            if (hitKey_RETURN && keyLockCount == 0) {
                keyLockCount = 10;
                if (isExistString(verbList, reshapeVectorToString(makingVerb))) {
                    if (verbList[getVerbListIndex(reshapeVectorToString(makingVerb))][1] == "F") {
                        PlaySoundMem(SEHandle_makeVerb, DX_PLAYTYPE_BACK, TRUE);
                        //verbList�X�V
                        verbList[getVerbListIndex(reshapeVectorToString(makingVerb))][1] = "T";
                        //csv��������
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
        //�����o���̕`��
        //�}�b�v�㔼���H
        if (stageData.mapHeight / 2 >= player->posX) { ovalCenterY = pPosEY + ovalHeight / 2; triTopY = pPosEY; }
        else { ovalCenterY = pPosSY - ovalHeight / 2; triTopY = pPosSY; }
        //�}�b�v�������H
        if (stageData.mapWidth / 2 >= player->posY) { ovalCenterX = pPosEX + ovalWidth / 2; triTopX = pPosEX; }
        else { ovalCenterX = pPosSX - ovalWidth / 2; triTopX = pPosSX; }
        DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_white, TRUE);
        DrawTriangle(triTopX, triTopY, ovalCenterX - ovalWidth / 4, ovalCenterY, ovalCenterX + ovalWidth / 4, ovalCenterY, c_black, FALSE);
        DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_white, TRUE);
        DrawOval(ovalCenterX, ovalCenterY, ovalWidth / 2, ovalHeight / 2, c_black, FALSE);
        //makingVerb�`��
        string printMakingVerb = reshapeVectorToString(makingVerb);
        if (printMakingVerb.empty()) {
            printMakingVerb = randMsg;
        }
        if (!isControlVerbMenu) {
            DrawString(ovalCenterX - ovalWidth * 3 / 8 + 15, ovalCenterY + ovalHeight / 4 - 10, "Z�œ��́@Enter�Ŋ���", c_black);
        }

        if (fontHandleMsg == 0) {
            fontHandleMsg = CreateFontToHandle(NULL, 20, 8);
        }

        DrawFormatStringToHandle(ovalCenterX - ovalWidth / 2 + 8, ovalCenterY - 8, c_black, fontHandleMsg, "%s", printMakingVerb.c_str());
    }

    

    //���j���[�ʒu�̏���
    if (isOpenVerbMenu) {
        if (maxSmoothMove > smoothMove) {
            smoothMove += delta;
            if (maxSmoothMove <= smoothMove)
                smoothMove = maxSmoothMove;
        }
    }
    //���j���[�J���ĂȂ���
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
    //�f�o�b�O�p�@2��vector���v�����g
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
    //Game.cpp���ł����g��Ȃ�
    if (CheckHitKey(KEY_INPUT_D) != 0) { //D�L�[��������Ă�����f�o�b�O�\��
        int color = c_green;

        DrawBox(x-10, y-10 ,x + 400, y + 400, c_white, TRUE);

        DrawFormatString(x, y,       color, " �X�e�[�W���@�@�@�@:%s", stageData.stageName.c_str());
        DrawFormatString(x, y + 20,  color, " �}�b�v�t�@�C���p�X:%s", stageData.stageFile.c_str());
        DrawFormatString(x, y + 40,  color, " �}�b�v�����@�@�@�@:%d", stageData.mapHeight);
        DrawFormatString(x, y + 60,  color, " �}�b�v���@�@�@�@�@:%d", stageData.mapWidth);
        DrawFormatString(x, y + 80,  color, " �����������@�@�@�@:%s", stageData.infKana.c_str());
        DrawFormatString(x, y + 100, color, " �����������@�@�@�@:%d", stageData.numKana);

        DrawFormatString(x, y + 130, color, " magRate�@�@�@�@   :%.1f", magRate);

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

        //auto make:T F�ŕ�����concat
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

    // delimiter��������ʒu��T���ĕ���
    while ((end = str.find(delimiter, start)) != string::npos) {
        result.push_back(str.substr(start, end - start));  // ������������擾
        start = end + 1;  // ���̈ʒu�Ɉړ�
    }

    // �Ō�̕�����ǉ�
    result.push_back(str.substr(start));
    return result;
}

vector<string> splitHiragana(const string& verb) {
    vector<string> result;
    for (size_t i = 0; i < verb.size();) {
        unsigned char c = verb[i];  // ���݂̃o�C�g���擾
        int charSize;

        // Shift_JIS��1�o�C�g�ڂ�0x80�ȏ�̏ꍇ�A2�o�C�g����
        if (c >= 0x80) {
            charSize = 2; // 2�o�C�g�����i�Ђ炪�Ȃ�J�^�J�i�Ȃǁj
        }
        else {
            charSize = 1; // 1�o�C�g�����iASCII�����Ȃǁj
        }

        // �����񂩂�1�����i�܂���2�o�C�g�j��؂�o���Avector�ɒǉ�
        result.push_back(verb.substr(i, charSize));
        i += charSize; // �o�C�g�����C���f�b�N�X��i�߂�
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
    // �������ރt�@�C����
    const string fileName = stageListPath;

    // �t�@�C�����J���i�㏑�����[�h�j
    ofstream file(fileName, ios::trunc); // �㏑���̂��߂� ios::trunc ���g��

    // �t�@�C��������ɊJ�������m�F
    if (!file.is_open()) {
        return;
    }

    // �w�b�_�[����������
    file << "StageName,StageFile,MapHeight,MapWidt,infKana,NumKana,clear,getCoin,StageLevel\n"; // �K�X�w�b�_���e�͕ύX

    // stageList �̓��e����������
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
        file << "\n"; // �s�̏I���ɉ��s��}��
    }

    // �t�@�C�������
    file.close();
}

void writeVerbList(vector<vector<string>> verbList) {
    // �������ރt�@�C����
    const string fileName = verbListPath;

    // �t�@�C�����J���i�㏑�����[�h�j
    ofstream file(fileName, ios::trunc); // �㏑���̂��߂� ios::trunc ���g��

    // �t�@�C��������ɊJ�������m�F
    if (!file.is_open()) {
        return;
    }

    // �w�b�_�[����������
    file << "verb,make,auto\n"; // �K�X�w�b�_���e�͕ύX

    // verbList �̓��e����������
    for (const auto& row : verbList) {
        for (size_t i = 0; i < row.size(); ++i) {
            file << row[i];
            if (i < row.size() - 1) {
                file << ","; // �Ō�̃J���}�͕t���Ȃ�
            }
        }
        file << "\n"; // �s�̏I���ɉ��s��}��
    }

    // �t�@�C�������
    file.close();
}

vector<vector<string>> loadObjectList() {
    vector<vector<string>> objectList = {};//�S�Ă�stageData

    string line;//�t�@�C��1�s���ǂݍ��݂̒u����
    ifstream ifs(objectFilePath);//�ǂݍ��ރt�@�C��

    getline(ifs, line);//�w�b�_�[���΂�

    //1�s�ǂݍ��݁A����1�s���J���}�ŋ�؂���tempData�Ɋi�[
    for (int i = 0; getline(ifs, line); i++) {
        stringstream ss(line);
        vector<string> objectData = {};//1�s����stageData

        for (int j = 0; j < 2; j++) {
            string buff = "";
            getline(ss, buff, ',');
            objectData.push_back(buff);
        }

        objectList.push_back(objectData);//1�s����stageData��S�Ă�stageData�ɑ���
    }
    return objectList;
}

vector<vector<string>> loadStageMap(Data _stageData) {
    vector<vector<string>> stageMap = {};
    string line;//�t�@�C��1�s���ǂݍ��݂̒u����
    ifstream ifs(_stageData.stageFile);//�ǂݍ��ރt�@�C��

    for (int i = 0; i < _stageData.mapHeight; i++) {
        vector<string> mapBuf = {};
        getline(ifs, line);
        stringstream ss(line);
        for (int j = 0; j < _stageData.mapWidth; j++) {
            string buff = "";
            getline(ss, buff, ',');
            mapBuf.push_back(buff.c_str());
        }
        stageMap.push_back(mapBuf);//1�s����stageData��S�Ă�stageData�ɑ���
        //printfDx("\n\n\n%d:%s %s %s", i, stageMap[i][0].c_str(), stageMap[i][1].c_str(), stageMap[i][2].c_str());
    }

    return stageMap;
}

//�I�u�W�F�N�g���m�ڐG�̏����ɂ���
void touchCoin() {
    if (!isSeeThroughPlayer) {
        //player��kana���ڐG������
        kanaUnit buffKana = {};
        string kanaName = objectList[calcObjectListIndex(player->posX, player->posY)]->name;//���̂ꂪCoin

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
        //player��kana���ڐG������
        string kanaKinds = objectList[calcObjectListIndex(player->posX, player->posY)]->kinds;
        if (kanaKinds == "kana") {
            doGetKana(player->posX, player->posY);
        }
    }
}

//�N���A����
void doClearProcess() {
    //printfDx("clearTime: %d\n", clearTime);
    clearTime -= 1;
    //�N���A�I
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
    DrawBox(0, 0, windowSizeX, windowSizeY, c_white, TRUE);
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

    drawClearScene();

    isMovePlayer = false;
    isCanUndo = false;
    if (clearTime <= 0) {
        if (!stageList[cursorStageList % stageList.size()].clear || (!stageList[cursorStageList % stageList.size()].getCoin) && isGetCoin) {
            //���߂ẴN���A�@�܂��́@���߂ăR�C���l��
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
    //�t�B�[���h��kana�I�u�W�F�N�g������
    objectList[calcObjectListIndex(_posX, _posY)] = new Air(_posX, _posY);
}

void doGetKana(int _posX, int _posY) {
    kanaUnit buffKana = {};
    string kanaName = objectList[calcObjectListIndex(_posX, _posY)]->name;//����kana "kana_A"
    string kanaKinds = objectList[calcObjectListIndex(_posX, _posY)]->kinds;

    PlaySoundMem(SEHandle_getKana, DX_PLAYTYPE_BACK, TRUE);

    //����kana�������Ă���Ȃ�
    if (isExistKana(kanaList, kanaName)) {
        //+1����
        for (int i = 0; i < kanaList.size(); i++) {
            if (kanaList[i].kana == kanaName && !(kanaList[i].numTimes < 0)) {
                kanaList[i].numTimes += 1;
            }
        }
    }
    //�����Ă��Ȃ��Ȃ�
    else {
        buffKana.kana = kanaName;
        buffKana.numTimes = 1;
        string handlePath = "./data/image/kana2_" + buffKana.kana + ".png";
        buffKana.handle = LoadGraph(handlePath.c_str());
        kanaList.push_back(buffKana);
        createUsableVerbList();//usable���X�V
    }

    //�t�B�[���h��kana�I�u�W�F�N�g������
    objectList[calcObjectListIndex(_posX, _posY)] = new Air(_posX, _posY);

    isUpdateUsableVerbList = true;
}

//�ȉ��@�����g�p���̏���
void chooseVerb(string _verb) {
    //1��������vector�ɕ�����
    vector<string> verb = splitHiragana(_verb);
    string targetName = objectList[calcTargetIndex(player->posX, player->posY, player->dire)]->name;
    string targetKinds = objectList[calcTargetIndex(player->posX, player->posY, player->dire)]->kinds;
    bool targetThrough = objectList[calcTargetIndex(player->posX, player->posY, player->dire)]->through;
    //printfDx("target: %s\n", targetName.c_str());
    //�ΏۃI�u�W�F�N�g�Ɠ����̏����@�͋ƂőS�������Ɏ����@if������e��֐������s
    //if      (_verb == "���邭") doWalk(verb);
    
    if (targetThrough) {
        if (_verb == "���邭") { walkPlayer(verb); return; }
    }
    if (!isSeeThroughPlayer) {
        if (_verb == "����") {
            objectErase(verb); return;
        }
        else if (_verb == "������") {
            playerSeeThrough(verb); return;
        }
        else if (_verb == "�����") {
            playerSit(verb); return;
        }

        if (targetName == "A") {

        }
        else if (targetName == "B") {
            if (_verb == "�Ȃ���") { flagFix(verb); return; }
        }
        else if (targetName == "C") {
            if (_verb == "����" || _verb == "�Ƃ�") { coinGet(verb); return; }
        }
        else if (targetName == "D") {
            if (_verb == "������") { doorOpen(verb); return; }
            if (_verb == "����") { doorKick(verb); return; }
        }
        else if (targetName == "F") {
            if (_verb == "����") { flagFold(verb); return; }
        }
        else if (targetName == "G") {
            if (_verb == "�܂킷") { gearRotate(verb); return; }
        }
        else if (targetName == "R") {
            if (_verb == "���") { rockCrack(verb); return; }
            if (_verb == "����" || _verb == "����") { rockPush(verb); return; }
            if (_verb == "������" || _verb == "����") { rockJump(verb); return; }
        }
        else if (targetName == "T") {
            if (_verb == "����" || _verb == "����") { treeCut(verb); return; }
            if (_verb == "����") { treeFold(verb); return; }
        }
        else if (targetName == "W") {
            if (_verb == "���") { rockCrack(verb); return; }
        }

        if (targetKinds == "kana") {
            if (_verb == "����" || _verb == "�Ƃ�") { getKana(verb); return; }
        }


    }
    PlaySoundMem(SEHandle_cantUse, DX_PLAYTYPE_BACK, TRUE);

}

void useVerb(vector<string> _verb) {
    int charSize = 2;

    //���邭�ȊO�͐����o�����o��
    if (reshapeVectorToString(_verb) != "���邭") {
        //�����o�����̕�����ύX
        speechVerb = reshapeVectorToString(_verb);
        speechBubble = 20;
    }

    lastUseVerb = _verb;
    kanaHighLightTime = 40;

    //�g�������������炷
    for (int i = 0; i < _verb.size(); i++) {
        for (int j = 0; j < kanaList.size(); j++) {
            if (_verb[i] == kanaList[j].kana && kanaList[j].numTimes > 0) {//�L���Ȃ猸�炷
                kanaList[j].numTimes -= 1;
                break;
            }
        }
    }

    //��0��kanaUnit������
    for (int i = (int)kanaList.size() - 1; i >= 0; i--) {
        if (kanaList[i].numTimes == 0) {
            kanaList.erase(kanaList.begin() + i);  // �v�f���폜���ĊԂ��l�߂�
        }
    }
    createUsableVerbList();

    //lastVerb�ɓo�^���邩�ǂ���
    string concatString = "";
    if (!getVerbListValue(reshapeVectorToString(_verb), "auto")) {
        for (int i = 0; i < _verb.size(); i++) {
            concatString += _verb[i];
        }
        lastVerb = concatString;
    }
    
    //���j���[����
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
    //�אڂ���4�̃I�u�W�F�N�g�����v����90�x
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

    //player�̈ʒu�ύX
    if (player->dire == dire_DOWN) {
        //�ォ��
        player->posX = targetPos[0];
        player->posY = targetPos[1] + 1;
        player->dire = dire_LEFT;
    }
    else if (player->dire == dire_LEFT) {
        //�E����
        player->posX = targetPos[0] + 1;
        player->posY = targetPos[1];
        player->dire = dire_UP;
    }
    else if (player->dire == dire_UP) {
        //������
        player->posX = targetPos[0];
        player->posY = targetPos[1] - 1;
        player->dire = dire_RIGHT;
    }
    else if (player->dire == dire_RIGHT) {
        //������
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