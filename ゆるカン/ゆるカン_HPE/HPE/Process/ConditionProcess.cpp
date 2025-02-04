#include "../IProcess.h"

/**
* @class	値を比較する
* @brief	値1が値2より大きいかを返す
* @details	書式：	#if greater(var,var)
*			引数1：	値1
*			引数2：	値2
*			戻り値： 値1が値2より大きければtrue、それ以外はfalse
*					引数が足りない場合は常にfalse
*			例文：	greater(5,0)	// 成功
*/
class GreaterProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(GreaterProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//失敗

		return (_args[0] > _args[1]);
	}
};

HPE_REGISTER_PROCESS(GreaterProcess, "greater");


/**
* @class	値を比較する
* @brief	値1が値2より小さいかを返す
* @details	書式：	#if less(var,var)
*			引数1：	値1
*			引数2：	値2
*			戻り値： 値1が値2より小さければtrue、それ以外はfalse
*					引数が足りない場合は常にfalse
*			例文：	less(0,5)	// 成功
*/
class LessProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(LessProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//失敗

		return (_args[0] < _args[1]);
	}
};

HPE_REGISTER_PROCESS(LessProcess, "less");


/**
* @class	値を比較する
* @brief	値1が値2以上かを返す
* @details	書式：	#if greaterEqual(var,var)
*			引数1：	値1
*			引数2：	値2
*			戻り値： 値1が値2以上ならtrue、それ以外はfalse
*					引数が足りない場合は常にfalse
*			例文：	greaterEqual(1,1)	// 成功
*/
class GreaterEqualProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(GreaterEqualProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//失敗

		return (_args[0] >= _args[1]);
	}
};

HPE_REGISTER_PROCESS(GreaterEqualProcess, "greaterEqual");


/**
* @class	値を比較する
* @brief	値1が値2以下かを返す
* @details	書式：	#if less(var,var)
*			引数1：	値1
*			引数2：	値2
*			戻り値： 値1が値2以下ならtrue、それ以外はfalse
*					引数が足りない場合は常にfalse
*			例文：	lessEqual(0,0)	// 成功
*/
class LessEqualProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(LessEqualProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//失敗

		return (_args[0] <= _args[1]);
	}
};

HPE_REGISTER_PROCESS(LessEqualProcess, "lessEqual");
