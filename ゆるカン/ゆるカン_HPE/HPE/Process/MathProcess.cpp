#pragma once
#include "../IProcess.h"
#include "../../Util.h"

/**
* @class	�ϐ��ɑ�����邽�߂̏���
* @brief	���������̂܂ܕԂ�
* @details	�����F	set(var)>>var
*			����1�F	���l1
*			�߂�l�F	����1
*					����������Ȃ��ꍇ�̂�0.0
*			�ᕶ�F	set(1)>>var
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
* @class	�ϐ��̑����Z�p����
* @brief	����2�̘a��Ԃ�
* @details	�����F	add(var,var)>>var
*			����1�F	���l1
*			����2�F	���l2
*			�߂�l�F	���l1�Ɛ��l2�̘a
*					����������Ȃ��ꍇ�̂�0.0
*			�ᕶ�F	add(1,-1)>>sum
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
* @class	�ϐ��̈����Z�p����
* @brief	����1-����2��Ԃ�
* @details	�����F	subtract(var,var)>>var
*			����1�F	���l1
*			����2�F	���l2
*			�߂�l�F	���l1�Ɛ��l2�̍�
*			�ᕶ�F	subtract(1,-1)>>hoge
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
* @class	�ϐ��̊|���Z�p����
* @brief	����2�̐ς�Ԃ�
* @details	�����F	multiple(var,var)>>var
*			����1�F	���l1
*			����2�F	���l2
*			�߂�l�F	���l1�Ɛ��l2�̐�
*					����������Ȃ��ꍇ�̂�0.0
*			�ᕶ�F	multiple(1,-1)>>result
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
* @class	�ϐ��̊���Z�p����
* @brief	����2�̏���Ԃ�
* @details	�����F	divide(var,var)>>var
*			����1�F	���l1
*			����2�F	���l2
*			�߂�l�F	���l1/���l2
*					����������Ȃ��ꍇ��0.0
*					0���Z����ƈُ�l��Ԃ�
*			�ᕶ�F	divide(2,1)>>result
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
* @class	�ϐ��̏�]�Z�p����
* @brief	����2�̊��������܂��Ԃ��B
*			�l�͎l�̌ܓ����������l�Ƃ��Ĉ�����
* @details	�����F	surplus(var,var)>>var
*			����1�F	���l1
*			����2�F	���l2
*			�߂�l�F	���l1%���l2
*					����������Ȃ��ꍇ��0.0
*					0���Z����ƈُ�l��Ԃ�
*			�ᕶ�F	surplus(3,2)>>result
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
* @class	������������
* @brief	����1�`����2�̊Ԃ̗�����Ԃ�
* @details	�����F	random(var,var)>>var
*			����1�F	�Œ�l
*			����2�F	�͈�
*			�߂�l�F �͈͓��̐����̂��������_���Ȑ��l
*					����������Ȃ��ꍇ�̂�0.0
*			�ᕶ�F	random(0,3)>>result
*/
class RandomProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;

		std::uniform_real_distribution<float> dist(_args[0], _args[1]);

		// �����𐶐�
		return dist(Util::GetRandomGenerator());
	}
};

HPE_REGISTER_PROCESS(RandomProcess, "random");



/**
* @class	������������
* @brief	����1�`����2�̊Ԃ̐����̗�����Ԃ�
*			�����_�ȉ��͐؂�̂�
* @details	�����F	randomInt(var,var)>>var
*			����1�F	�Œ�l
*			����2�F	�ő�l
*			�߂�l�F �͈͓��̐����̂��������_���Ȑ��l
*					����������Ȃ��ꍇ�̂�0.0
*			�ᕶ�F	randomInt(0,3)>>result
*/
class RandomIntProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 2) return 0.0f;

		std::uniform_int_distribution<int> dist(static_cast<int>(std::ceilf(_args[0])), static_cast<int>(std::ceilf(_args[1])));

		// �����𐶐�
		return static_cast<float>(dist(Util::GetRandomGenerator()));
	}
};

HPE_REGISTER_PROCESS(RandomIntProcess, "randomInt");




/**
* @class	�������擾����
* @brief	����1�̕����ɂ����1�܂���-1��Ԃ�
*			0�̏ꍇ��1
* @details	�����F	sign(var)>>var
*			����1�F	�]������l
*			�߂�l�F �����̕����ɉ����� 1 ���邢�� -1
*					����������Ȃ��ꍇ�̂�0.0
*			�ᕶ�F	sign(5)>>result
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




