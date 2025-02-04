#include "Sample.h"

void System::Init()
{
	Interface::SetSystem(this);
}

void System::Func()
{
	// 持っているInterface全てに対して処理を実行
	for (auto& elem : m_pList) {
		std::cout << elem->GetSomeData().text << std::endl;;
	}
}

void System::AddInterface(Interface* _ptr)
{
	// 追加
	m_pList.emplace_back(_ptr);
}

void System::RemoveInterface(Interface* _ptr)
{
	// 削除
	std::erase(m_pList, _ptr);
}

Interface::Interface()
{
	// コンストラクタでSystemに追加
	m_pSystem->AddInterface(this);
}

Interface::~Interface()
{
	// デストラクタで削除
	m_pSystem->RemoveInterface(this);
}

void Interface::SetSystem(System* _system)
{
	m_pSystem = _system;
}
