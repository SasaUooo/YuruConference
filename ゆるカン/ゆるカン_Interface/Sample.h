#pragma once
#include <vector>
#include <string>
#include <iostream>

class Interface;

// システム側に渡したいデータ。
struct SomeData {
	std::string text;
};

// このコードだと1つじゃないとInterface側でバグるのでシングルトン推奨
// （Interfaceクラス内でちょっと補足）
class System {
public:
	void Init();
	void Func();
	void AddInterface(Interface* _ptr);		// 追加用
	void RemoveInterface(Interface* _ptr);	// 削除用

private:
	std::vector<Interface*> m_pList;	// 本当は追加キュー、削除キューがほしい（Update中に追加されたときに面倒）
};

class Interface {
public:
	Interface();
	virtual ~Interface();

	// 使用するデータを取得する関数
	virtual SomeData GetSomeData() = 0;

	// SystemのInitで使用する
	// m_pSystemを配列で複数保持出来るようにしたらSystemが複数あっても対応できるはず
	static void SetSystem(System* _system);
private:
	// staticでSystemへのポインタを持っておくことで、継承先が知らなくて良くなる
	// Systemが途中で消えるならUninitとかでここを消す
	static inline System* m_pSystem = nullptr;
};
