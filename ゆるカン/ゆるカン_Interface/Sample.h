#pragma once
#include <vector>
#include <string>
#include <iostream>

class Interface;

// �V�X�e�����ɓn�������f�[�^�B
struct SomeData {
	std::string text;
};

// ���̃R�[�h����1����Ȃ���Interface���Ńo�O��̂ŃV���O���g������
// �iInterface�N���X���ł�����ƕ⑫�j
class System {
public:
	void Init();
	void Func();
	void AddInterface(Interface* _ptr);		// �ǉ��p
	void RemoveInterface(Interface* _ptr);	// �폜�p

private:
	std::vector<Interface*> m_pList;	// �{���͒ǉ��L���[�A�폜�L���[���ق����iUpdate���ɒǉ����ꂽ�Ƃ��ɖʓ|�j
};

class Interface {
public:
	Interface();
	virtual ~Interface();

	// �g�p����f�[�^���擾����֐�
	virtual SomeData GetSomeData() = 0;

	// System��Init�Ŏg�p����
	// m_pSystem��z��ŕ����ێ��o����悤�ɂ�����System�����������Ă��Ή��ł���͂�
	static void SetSystem(System* _system);
private:
	// static��System�ւ̃|�C���^�������Ă������ƂŁA�p���悪�m��Ȃ��ėǂ��Ȃ�
	// System���r���ŏ�����Ȃ�Uninit�Ƃ��ł���������
	static inline System* m_pSystem = nullptr;
};
