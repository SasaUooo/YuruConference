#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>
#include <functional>

// SerializeFieldを使用できるようにするための基底クラス
class Serializable {
	// 登録用クラスからだけ変更できるようにする
	friend class SerializableRegistrar;
public:
	virtual ~Serializable() = default;

	void Func() {
		for (auto& member : m_member) {
			// 型のハッシュ値に応じてm_procsに登録した関数を呼び出し
			size_t& hash = std::get<0>(member);
			if (m_procs.find(hash) != m_procs.end()) {
				m_procs[hash](std::get<1>(member), std::get<2>(member));
			}
		}
	}

private:
	// 型のハッシュ値に対応した関数を登録するメンバ変数
	static inline std::unordered_map<size_t, std::function<void(std::string&, void*)>> m_procs =
	// ここではstd::stringとintに対して単に値を出力するコードを初期化で登録
	{
		{
			typeid(std::string).hash_code(),
			[](std::string& _name,void* _ptr) {
				// *((std::string*)_ptr) あたりのキャストがはちゃめちゃに怖い。
				std::cout << "string" << ":\t" << _name << " = " << *((std::string*)_ptr) << std::endl;
			}
		},
		{
			typeid(int).hash_code(),
			[](std::string& _name,void* _ptr) {
				std::cout << "int" << ":\t" << _name << " = " << *((int*)_ptr) << std::endl;
			}
		}
	};

	// クラスのハッシュ、変数名、変数への参照を保持
	std::vector<std::tuple<size_t, std::string, void*>> m_member
		= std::vector<std::tuple<size_t, std::string, void*>>();
};



// Serializableに登録するためのクラス
class SerializableRegistrar {
public:
	// コンストラクタでm_memberに登録する
	SerializableRegistrar(Serializable* _owner, const type_info& _type, const char* _name, void* _ptr) {
		_owner->m_member.emplace_back(_type.hash_code(), _name, _ptr);
	}
	~SerializableRegistrar() = default;
};



// マクロ
#define SerializeField(type, name, initial_value) \
type name = initial_value; \
const SerializableRegistrar SR##name##Registrar = SerializableRegistrar(this, typeid(type), #name, (void*)&name);
/*
展開するとこんな感じ
その変数に応じたSerializableRegistrarを追加で定義するようにすることでm_memberに登録されるようにする。

SerializeField(int, m_hoge, 12345);
↓
int m_hoge = 12345;
const SerializableRegistrar SRm_hogeRegistrar = SerializableRegistrar(this, typeid(int), "m_hoge", (void*)&m_hoge);
*/
