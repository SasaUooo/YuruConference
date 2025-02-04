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
	// SerializeField(int, m_hoge, 0); ※ エラー

	// エラーE0289	コンストラクター "SerializableRegistrar::SerializableRegistrar" のインスタンスが引数リストと一致しません	
	// Serializableを継承していないためマクロ展開時のthisがSerializable*に変換できずにエラー。
};