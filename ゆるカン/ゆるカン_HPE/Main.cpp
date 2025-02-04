/*
スクリプトを作った。
詳しくはファイル内にあるHPE言語仕様.txtを見てほしいです。
アピールポイントとしては、
	FileProcess.hをincludeすれば使用できること、
	1ファイルで簡単に処理を追加できること、
	実行中に中身を変更しても、再度Load関数を呼び出せば即座に反映されること、
	書式などのミスがあってもある程度構文エラーログをだしてくれること、
	変数、引数、条件分岐、ループなどの基本的な処理を一通り実装できていること。

欠点は、
	文法がオリジナルなこと、
	float値しか扱えないこと、
	実行時エラーの制御処理が実装出来てないこと、
	大量の処理を行おうとするとさすがに重いこと。

弾幕シューティングの敵や弾の挙動を制御するために作ったものだから実行速度はある程度あるはずですが、
簡単な処理を細かく変更するために作ったものなので、大きな処理には向いてません。
そもそもfloat値しか使えない時点で厳しいです。
逆に、細かい処理を書くには向いているはずです。
実行中にロードするので、実行ファイルに変更がなく、ビルド時間を短縮できます。
実行中の変更にも対応しているので、細かくパラメータを調整する使い方などが得意です。

このサンプルでは、XとYを指定したら、10,-10に向けてlerpするものを設定している。
*/

#include "HPE/FileProcess.h"
#include <Windows.h>
#include <iostream>

int main(void) {
	FileProcess fp;

	float x, y;
	std::cout << "スタートのXを入力:" << std::endl;
	std::cin >> x;
	std::cout << "スタートのYを入力:" << std::endl;
	std::cin >> y;

	// 引数無しでlerp.txtをロード
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