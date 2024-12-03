#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"

//�X�V
void updateMenu() {
    if (CheckHitKey(KEY_INPUT_2) != 0) { //Enter�L�[��������Ă�����
        changeScene(sceneSelect);               //�V�[�����Z���N�g�ɕύX
    }
    else if (CheckHitKey(KEY_INPUT_3) != 0) { //Esc�L�[��������Ă�����
        changeScene(sceneGame);               //�V�[�����Q�[���ɕύX
    }
}

//�`��
void drawMenu() {
    DrawString(0, 0, "�Q�[����ʂł��B", GetColor(255, 255, 255));
    DrawString(0, 20, "2�L�[�������ƃZ���N�g��ʂցB", GetColor(255, 255, 255));
    DrawString(0, 40, "3�L�[�������ƃQ�[����ʂցB", GetColor(255, 255, 255));
}