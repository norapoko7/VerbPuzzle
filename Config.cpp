#include "Config.h"
#include "SceneMgr.h"
#include "DxLib.h"

//�X�V
void updateConfig() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //ESC�L�[��������Ă�����
        changeScene(sceneSelect);//�V�[�����Z���N�g�ɕύX
    }
}

//�`��
void drawConfig() {
    DrawString(0, 0, "�ݒ��ʂł��B", GetColor(255, 255, 255));
    DrawString(0, 20, "ESC�L�[�������ƃZ���N�g��ʂցB", GetColor(255, 255, 255));
}