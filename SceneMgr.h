#pragma once

typedef enum {
    sceneTitle,    //�^�C�g�����
    sceneSelect,   //�X�e�[�W�Z���N�g���
    sceneGame,     //�X�e�[�W���
    sceneMenu,     //���j���[���
    sceneConfig,     //�ݒ���
} sceneName;

//�X�V
void updateScene();

//�`��
void drawScene();

// ���� nextScene �ɃV�[����ύX����
void changeScene(sceneName nextScene);