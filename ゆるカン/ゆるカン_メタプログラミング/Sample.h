#pragma once
#include "Serializable.h"
#include <string>

class Hoge : public Serializable {
public:
	Hoge() = default;
	~Hoge() override = default;

private:
	SerializeField(int, m_hoge, 123);
	SerializeField(std::string, m_text, "hoge");
};

class Fuga {
private:
	// SerializeField(int, m_hoge, 0); �� �G���[

	// �G���[E0289	�R���X�g���N�^�[ "SerializableRegistrar::SerializableRegistrar" �̃C���X�^���X���������X�g�ƈ�v���܂���	
	// Serializable���p�����Ă��Ȃ����߃}�N���W�J����this��Serializable*�ɕϊ��ł����ɃG���[�B
};