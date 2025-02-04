#include "../IProcess.h"

/**
* @class	�l���r����
* @brief	�l1���l2���傫������Ԃ�
* @details	�����F	#if greater(var,var)
*			����1�F	�l1
*			����2�F	�l2
*			�߂�l�F �l1���l2���傫�����true�A����ȊO��false
*					����������Ȃ��ꍇ�͏��false
*			�ᕶ�F	greater(5,0)	// ����
*/
class GreaterProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(GreaterProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//���s

		return (_args[0] > _args[1]);
	}
};

HPE_REGISTER_PROCESS(GreaterProcess, "greater");


/**
* @class	�l���r����
* @brief	�l1���l2��菬��������Ԃ�
* @details	�����F	#if less(var,var)
*			����1�F	�l1
*			����2�F	�l2
*			�߂�l�F �l1���l2��菬�������true�A����ȊO��false
*					����������Ȃ��ꍇ�͏��false
*			�ᕶ�F	less(0,5)	// ����
*/
class LessProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(LessProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//���s

		return (_args[0] < _args[1]);
	}
};

HPE_REGISTER_PROCESS(LessProcess, "less");


/**
* @class	�l���r����
* @brief	�l1���l2�ȏォ��Ԃ�
* @details	�����F	#if greaterEqual(var,var)
*			����1�F	�l1
*			����2�F	�l2
*			�߂�l�F �l1���l2�ȏ�Ȃ�true�A����ȊO��false
*					����������Ȃ��ꍇ�͏��false
*			�ᕶ�F	greaterEqual(1,1)	// ����
*/
class GreaterEqualProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(GreaterEqualProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//���s

		return (_args[0] >= _args[1]);
	}
};

HPE_REGISTER_PROCESS(GreaterEqualProcess, "greaterEqual");


/**
* @class	�l���r����
* @brief	�l1���l2�ȉ�����Ԃ�
* @details	�����F	#if less(var,var)
*			����1�F	�l1
*			����2�F	�l2
*			�߂�l�F �l1���l2�ȉ��Ȃ�true�A����ȊO��false
*					����������Ȃ��ꍇ�͏��false
*			�ᕶ�F	lessEqual(0,0)	// ����
*/
class LessEqualProcess : public IProcess
{
	HPE_THIS_IS_CONDITION(LessEqualProcess)
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;	//���s

		return (_args[0] <= _args[1]);
	}
};

HPE_REGISTER_PROCESS(LessEqualProcess, "lessEqual");
