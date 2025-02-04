#pragma once
#include <variant>
#include <string>
#include <vector>
#include <any>
#include <functional>
#include "ProcessProvider.h"

constexpr float HPE_RESULT_ERROR = std::numeric_limits<float>::min();

// �v���Z�X�̃N���X���������N���X�̒�`�̊O�ɂ��̃}�N���������Ċ֐��Ƃ��ēo�^����
// ��FHPE_REGISTER_PROCESS(AddProcess, "add")
#define HPE_REGISTER_PROCESS(className, funcName)  \
namespace className##ProcessRegistrar{\
	static ProcessRegistrar<className> m_registrar = ProcessRegistrar<className>(funcName);\
}

// �������Ƃ��Ĉ��������N���X�ɏ���
// �����̓N���X�̒��ɏ����B���̃}�N���̉���public�ɂȂ�̂Œ���
#define HPE_THIS_IS_CONDITION(className) \
public:		\
	className(){	\
		m_isCondition = true;	\
	}

using Argument = float;		///< �����ň������l�Bfloat�̂݃T�|�[�g���Ă���

class IProcess
{
	// FileProcess�����Execute�����s�o���Ăق����̂�friend
	friend class FileProcess;
public:
	/**
	* @brief	���z�f�X�g���N�^
	* @details	default
	*/
	virtual ~IProcess() = default;

protected:
	/**
	 * @brief	���ۂɎ��s����鏈��
	 * @param	_owner ���̏������e����^����I�u�W�F�N�g�ւ̎Q��
	 * @param	_args �����Ŏg�p�������
	 * @return	�����̖߂�l�Bfloat�^��1�����Ԃ���
	 */
	virtual Argument Process(std::any& _owner, const std::vector<Argument>& _args) = 0;

private:
	/**
	 * @brief	�p�����Process�֐������s���邽�߂̏���
	 * @param 	_owner ���̏������e����^����I�u�W�F�N�g�ւ̎Q��
	 * @param 	_args ���̏����Ŏg�p�������
	 * @param 	_return ���̏����̖߂�l���i�[����Argument�ւ̃|�C���^
	 * @return	���̏����̕]���B0�Ȃ琳��I��
	 */
	virtual float Execute(std::any& _owner, const std::vector<Argument>& _args, Argument* _return, float _result = 0.0f) {
		float result = Process(_owner, _args);
		if (_return != nullptr) {
			*_return = result;
		}

		// ����������Ȃ��Ȃ�0�����Ԃ�
		if (!m_isCondition) {
			result = 0.0f;
		}
		return result;
	}

protected:
	bool m_isCondition = false;	// ���̏��������������ǂ����̃t���O
};
