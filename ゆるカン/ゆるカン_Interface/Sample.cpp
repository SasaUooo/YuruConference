#include "Sample.h"

void System::Init()
{
	Interface::SetSystem(this);
}

void System::Func()
{
	// �����Ă���Interface�S�Ăɑ΂��ď��������s
	for (auto& elem : m_pList) {
		std::cout << elem->GetSomeData().text << std::endl;;
	}
}

void System::AddInterface(Interface* _ptr)
{
	// �ǉ�
	m_pList.emplace_back(_ptr);
}

void System::RemoveInterface(Interface* _ptr)
{
	// �폜
	std::erase(m_pList, _ptr);
}

Interface::Interface()
{
	// �R���X�g���N�^��System�ɒǉ�
	m_pSystem->AddInterface(this);
}

Interface::~Interface()
{
	// �f�X�g���N�^�ō폜
	m_pSystem->RemoveInterface(this);
}

void Interface::SetSystem(System* _system)
{
	m_pSystem = _system;
}
