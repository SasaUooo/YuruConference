/*
C++のインターフェースとは？
C++におけるインターフェースとは、派生クラスに特定の機能を提供するための抽象的な基底クラスのことを指します。
主に 純粋仮想関数（pure virtual function） を使って実装され、オブジェクトのポリモーフィズム（多態性）を実現します。

インターフェースの特徴
	純粋仮想関数のみを持つ
		virtual キーワードと = 0 を使って定義する。
	インスタンス化できない
		インターフェース自体は具体的な実装を持たないため、直接オブジェクトを作れない。
	ポリモーフィズムを実現できる
		異なるクラスが同じインターフェースを実装することで、一貫した使い方ができる。
By ChatGPT

このサンプルでは、最近お気に入りの、
システムのサポートを提供してくれるInterfaceのクラス設計を簡単に書いたコード。
*/


#include "Sample.h"

// public継承して機能をサポートする
class Object : public Interface {
public:
	Object() = default;
	Object(const char* _text) : m_text(_text) {};
	~Object() override = default;

	// Interfaceにより実装を強制
	virtual SomeData GetSomeData() override {
		// ここでSomeDataに何を入れて渡すかはクラスごとに自由
		// ここではメンバ変数で持っているm_textを渡すことにする
		SomeData data = {};
		data.text = m_text;
		return data;
	}

private:
	std::string m_text;
};


int main(void) {
	System sys;
	sys.Init();

	// Obj1,2を追加
	std::cout << "AとBを追加" << std::endl;
	Object* obj1 = new Object("A");
	Object* obj2 = new Object("B");
	sys.Func();
	getchar();

	// Obj3を追加
	std::cout << "Cを追加" << std::endl;
	Object* obj3 = new Object("C");
	sys.Func();
	getchar();

	// Obj2を削除
	std::cout << "Bを削除" << std::endl;
	delete obj2;
	sys.Func();
	getchar();

	// Obj3を削除
	std::cout << "Cを削除" << std::endl;
	delete obj3;
	sys.Func();
	getchar();

	delete obj1;

	return 0;
}