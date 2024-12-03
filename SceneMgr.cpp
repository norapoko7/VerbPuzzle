#include "DxLib.h"

#include "Title.h"
#include "Select.h"
#include "Game.h"
#include "Menu.h"
#include "Config.h"

#include "SceneMgr.h"

static sceneName scene = sceneTitle;    //�V�[���Ǘ��ϐ�

//�X�V
void updateScene() {
    switch (scene) {       //�V�[���ɂ���ď����𕪊�
    case sceneTitle:    //���݂̉�ʂ����j���[�Ȃ�
        updateTitle();   //���j���[��ʂ̍X�V����������
        break;//�ȉ���
    case sceneSelect:
        updateSelect();
        break;
    case sceneGame:
        updateGame();
        break;
    case sceneMenu:
        updateMenu();
        break;
    case sceneConfig:
        updateConfig();
        break;
    }
}

//�`��
void drawScene() {
    switch (scene) {      //�V�[���ɂ���ď����𕪊�
    case sceneTitle:   //���݂̉�ʂ����j���[��ʂȂ�
        drawTitle();    //���j���[��ʂ̕`�揈��������
        break;//�ȉ���
    case sceneSelect:
        drawSelect();
        break;
    case sceneGame:
        drawGame();
        break;
    case sceneMenu:
        drawMenu();
        break;
    case sceneConfig:
        drawConfig();
        break;
    }
}

// ���� nextScene �ɃV�[����ύX����
void changeScene(sceneName nextScene) {
    scene = nextScene;
}