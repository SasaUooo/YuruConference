/*
�X�N���v�g��������B
�ڂ����̓t�@�C�����ɂ���HPE����d�l.txt�����Ăق����ł��B
�A�s�[���|�C���g�Ƃ��ẮA
	FileProcess.h��include����Ύg�p�ł��邱�ƁA
	1�t�@�C���ŊȒP�ɏ�����ǉ��ł��邱�ƁA
	���s���ɒ��g��ύX���Ă��A�ēxLoad�֐����Ăяo���Α����ɔ��f����邱�ƁA
	�����Ȃǂ̃~�X�������Ă�������x�\���G���[���O�������Ă���邱�ƁA
	�ϐ��A�����A��������A���[�v�Ȃǂ̊�{�I�ȏ�������ʂ�����ł��Ă��邱�ƁB

���_�́A
	���@���I���W�i���Ȃ��ƁA
	float�l���������Ȃ����ƁA
	���s���G���[�̐��䏈���������o���ĂȂ����ƁA
	��ʂ̏������s�����Ƃ���Ƃ������ɏd�����ƁB

�e���V���[�e�B���O�̓G��e�̋����𐧌䂷�邽�߂ɍ�������̂�������s���x�͂�����x����͂��ł����A
�ȒP�ȏ������ׂ����ύX���邽�߂ɍ�������̂Ȃ̂ŁA�傫�ȏ����ɂ͌����Ă܂���B
��������float�l�����g���Ȃ����_�Ō������ł��B
�t�ɁA�ׂ��������������ɂ͌����Ă���͂��ł��B
���s���Ƀ��[�h����̂ŁA���s�t�@�C���ɕύX���Ȃ��A�r���h���Ԃ�Z�k�ł��܂��B
���s���̕ύX�ɂ��Ή����Ă���̂ŁA�ׂ����p�����[�^�𒲐�����g�����Ȃǂ����ӂł��B

���̃T���v���ł́AX��Y���w�肵����A10,-10�Ɍ�����lerp������̂�ݒ肵�Ă���B
*/

#include "HPE/FileProcess.h"
#include <Windows.h>
#include <iostream>

int main(void) {
	FileProcess fp;

	float x, y;
	std::cout << "�X�^�[�g��X�����:" << std::endl;
	std::cin >> x;
	std::cout << "�X�^�[�g��Y�����:" << std::endl;
	std::cin >> y;

	// ����������lerp.txt�����[�h
	fp.Load("asset/HPEScript/lerp.txt", { x,y });

	std::any owner;

	for (int i = 0; i < 110; i++) {
		fp.StartProcess(owner);
		Sleep(10);
	}

	getchar();
	getchar();
	return 0;
}