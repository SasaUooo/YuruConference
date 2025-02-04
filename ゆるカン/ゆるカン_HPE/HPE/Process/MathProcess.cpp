#pragma once
#include "../IProcess.h"
#include "../../Util.h"

/**
* @class	変数に代入するための処理
* @brief	引数をそのまま返す
* @details	書式：	set(var)>>var
*			引数1：	数値1
*			戻り値：	引数1
*					引数が足りない場合のみ0.0
*			例文：	set(1)>>var
*/
class SetProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 1) return 0.0f;
		return _args[0];
	}
};

HPE_REGISTER_PROCESS(SetProcess, "set");


/**
* @class	変数の足し算用処理
* @brief	引数2つの和を返す
* @details	書式：	add(var,var)>>var
*			引数1：	数値1
*			引数2：	数値2
*			戻り値：	数値1と数値2の和
*					引数が足りない場合のみ0.0
*			例文：	add(1,-1)>>sum
*					add(speed,1.0)>>speed
*/
class AddProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;
		return _args[0] + _args[1];
	}
};

HPE_REGISTER_PROCESS(AddProcess, "add");

/**
* @class	変数の引き算用処理
* @brief	引数1-引数2を返す
* @details	書式：	subtract(var,var)>>var
*			引数1：	数値1
*			引数2：	数値2
*			戻り値：	数値1と数値2の差
*			例文：	subtract(1,-1)>>hoge
*/
class SubtractProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return HPE_RESULT_ERROR;
		return _args[0] - _args[1];
	}
};

HPE_REGISTER_PROCESS(SubtractProcess, "subtract");


/**
* @class	変数の掛け算用処理
* @brief	引数2つの積を返す
* @details	書式：	multiple(var,var)>>var
*			引数1：	数値1
*			引数2：	数値2
*			戻り値：	数値1と数値2の積
*					引数が足りない場合のみ0.0
*			例文：	multiple(1,-1)>>result
*/
class MultipleProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;
		return _args[0] * _args[1];
	}
};

HPE_REGISTER_PROCESS(MultipleProcess, "multiple");


/**
* @class	変数の割り算用処理
* @brief	引数2つの商を返す
* @details	書式：	divide(var,var)>>var
*			引数1：	数値1
*			引数2：	数値2
*			戻り値：	数値1/数値2
*					引数が足りない場合は0.0
*					0除算すると異常値を返す
*			例文：	divide(2,1)>>result
*/
class DivideProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;
		if (_args[1] == 0.0f) return 1.17549435E-38f;
		return _args[0] / _args[1];
	}
};

HPE_REGISTER_PROCESS(DivideProcess, "divide");



/**
* @class	変数の剰余算用処理
* @brief	引数2つの割ったあまりを返す。
*			値は四捨五入した整数値として扱われる
* @details	書式：	surplus(var,var)>>var
*			引数1：	数値1
*			引数2：	数値2
*			戻り値：	数値1%数値2
*					引数が足りない場合は0.0
*					0除算すると異常値を返す
*			例文：	surplus(3,2)>>result
*/
class SurplusProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;
		if (_args[1] == 0.0f) return 1.17549435E-38f;
		return static_cast<int>(std::round(_args[0])) % static_cast<int>(std::round(_args[1]));
	}
};

HPE_REGISTER_PROCESS(SurplusProcess, "surplus");



/**
* @class	乱数生成処理
* @brief	引数1～引数2の間の乱数を返す
* @details	書式：	random(var,var)>>var
*			引数1：	最低値
*			引数2：	範囲
*			戻り値： 範囲内の整数のうちランダムな数値
*					引数が足りない場合のみ0.0
*			例文：	random(0,3)>>result
*/
class RandomProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;

		std::uniform_real_distribution<float> dist(_args[0], _args[1]);

		// 乱数を生成
		return dist(Util::GetRandomGenerator());
	}
};

HPE_REGISTER_PROCESS(RandomProcess, "random");



/**
* @class	乱数生成処理
* @brief	引数1～引数2の間の整数の乱数を返す
*			小数点以下は切り捨て
* @details	書式：	randomInt(var,var)>>var
*			引数1：	最低値
*			引数2：	最大値
*			戻り値： 範囲内の整数のうちランダムな数値
*					引数が足りない場合のみ0.0
*			例文：	randomInt(0,3)>>result
*/
class RandomIntProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;

		std::uniform_int_distribution<int> dist(static_cast<int>(std::ceilf(_args[0])), static_cast<int>(std::ceilf(_args[1])));

		// 乱数を生成
		return static_cast<float>(dist(Util::GetRandomGenerator()));
	}
};

HPE_REGISTER_PROCESS(RandomIntProcess, "randomInt");




/**
* @class	符号を取得する
* @brief	引数1の符号によって1または-1を返す
*			0の場合は1
* @details	書式：	sign(var)>>var
*			引数1：	評価する値
*			戻り値： 引数の符号に応じて 1 あるいは -1
*					引数が足りない場合のみ0.0
*			例文：	sign(5)>>result
*/
class SignProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 1) return 0.0f;

		return std::copysignf(1.0f, _args[0]);
	}
};

HPE_REGISTER_PROCESS(SignProcess, "sign");




