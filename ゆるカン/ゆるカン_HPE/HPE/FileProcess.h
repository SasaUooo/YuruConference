#pragma once
#include "IProcess.h"
#include "ProcessProvider.h"
#include <unordered_map>
#include <memory>
#include <any>
#include <map>

///< Argument�ւ̃|�C���^�ƒ萔�l�t���O�itrue��������new����Ă�̂�delete���K�v�j
using PreArgument = std::pair<Argument*, bool>;

///< �t�@�C������ǂݍ���ł��������̗v�f�B�ϐ������邢�͒萔
using FileArgument = std::variant<std::string, Argument>;

///< �ϐ��}�b�v�̌^
using VariableMap = std::map<std::string, Argument*>;

/**
* @brief	�֐������s�ɕK�v�ȗv�f���܂Ƃ߂��\����
*/
struct FunctionData {
	std::string functionName = "";		///< �֐���
	std::vector<FileArgument> fileArguments = std::vector<FileArgument>();	///< �֐����g�������̃f�[�^
	std::string returnArgumentName = "";	///< �߂�l��n���ϐ��̖��O
	int ifSkipAmount = 0;
	int iterationAmount = 0;
	bool once = false;
};

/**
* @class	�e�L�X�g�t�@�C����ǂݍ��񂾏���
*/
class FileProcess : public IProcess
{
	/**
	* @brief	���ۂɎ��s����Ƃ��Ɏg���f�[�^
	* @details	function	���s����֐���unique_ptr�̃|�C���^�B�J���̕K�v�Ȃ�
	*			preArguments ���s���ɑ�1�v�f��float*��float�ɂ��Ĉ����Ƃ��Ďg��
	*			result		���s���ʂ��󂯎��ϐ��ւ̃|�C���^�B�J���̕K�v�Ȃ��Bnullptr�ł�����
	*/
	struct ProcessData {
		IProcess* pFunction = nullptr;
		std::vector<PreArgument> preArguments = std::vector<PreArgument>();
		Argument* pResult = nullptr;
		int ifSkipAmount = 0;
		int iterationAmount = 0;
		bool once = false;

		// �v���Z�X���Ɋ܂܂��萔�����ׂĔj��
		// (����������new�Ő�������Ă��邽�߁B)
		void Uninit() {
			for (auto& preArg : preArguments) {
				if (preArg.second) {
					delete preArg.first;
					preArg.first = nullptr;
				}
			}
		}
	};
public:
	/**
	* @brief	�R���X�g���N�^
	* @details	default
	*/
	FileProcess() = default;

	/**
	* @brief	�f�X�g���N�^
	* @details	Reset�֐������s
	*/
	~FileProcess();

	/**
	* @brief	�e�L�X�g�t�@�C�������[�h����
	* @details	ProcessCompiler��path�Ǝ����̃C���X�^���X��n������
	* @param	_filepath	���[�h����e�L�X�g�t�@�C���̃p�X
	* @param	_input		�����ɊO����n������
	*/
	void Load(const char* _filepath, const std::vector<Argument>& _input);

	/**
	* @brief	�ǂݍ��܂�Ă��鏈����j������
	* @details	����������new���ꂽ�萔��delete
	*			�����̃��X�g�ƕϐ��}�b�v���N���A
	*/
	void Reset();

	/**
	* @brief	�ǂݍ��܂�Ă��鏈�������s����
	* @details	Execute�֐��������Ȃ��A�߂�l���󂯎��Ȃ���ԂŎ��s����
	* @param	_owner	���̏������e����^����C���X�^���X�ւ̃|�C���^
	*
	* @memo		���Update�����Ŏ��s���邱�Ƃ�z�肵�Ă���
	*/
	void StartProcess(std::any& _owner);

	/**
	* @brief	�����f�[�^��ǉ�����
	* @param	_funcName	�֐��̖��O
	* @param	_preArgs	�t�@�C������ǂݎ���������f�[�^�̃��X�g�B�ϐ�����float�l�������Ă�
	* @param	_result		�߂�l���󂯎��ϐ��̖��O
	* @param	_once		��񂾂����s����t���O
	*/
	void AddProcessData(const std::string& _funcName, const std::vector<FileArgument>& _fileArgs,
		const std::string& _result, int _ifSkipAmount, int _iterationAmount, bool _once);

	/**
	* @brief	�ϐ��f�[�^��ǉ�����
	* @param	_name	�ǉ�����ϐ��̖��O
	* @param	_value	�ϐ��̏����l
	*/
	inline void AddVariable(const std::string& _name, Argument _value);

	/**
	* @brief	�ϐ��}�b�v���擾����
	* @return	�ϐ��}�b�v��const�ŕԂ�
	*/
	inline const VariableMap& GetVariableMap() const;
private:
	/**
	* @brief	IProcess::Execute�Ŏ��s�����v���Z�X
	* @details	m_processList�Ɋi�[���ꂽ���������s����
				�����N���X�C���X�^���X�A�����̒l�A�߂�l�̃|�C���^���擾���ď������s����
	* @param	_owner	�e����^����N���X�̃C���X�^���X�ւ̃|�C���^
	* @param	_args	���s����֐��ɗ^��������̃��X�g
	* @return	�֐��̖߂�l�B�����ɂ���Ă͂Ȃ��B
	*/
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override;

private:
	///< �ϐ��̖��O�ƒl�����т���}�b�v�B�ϐ��}�b�v
	VariableMap m_variableMap = VariableMap();

	///< ���s����鏈���̔z��
	std::vector<ProcessData> m_processDataList = std::vector<ProcessData>();

	///< ������n���Ƃ��Ɉꎞ�I�ɓ���Ă������߂̔z��
	std::vector<Argument> m_tempArguments = std::vector<Argument>();

	///< #once�t���O�̏��������s���č폜�������̃t���O
	bool m_onceRemoved = false;

	///< setmove�ȂǂŃv���Z�X���r���ŕύX���ꂽ�Ƃ���true�ɂ���t���O
	bool m_processChanged = false;

	///< �ύX�����m���邽�߂ɍ������Ă�t�@�C���̖��O�������Ă���
	std::string m_loadingFileName = "";
};






inline void FileProcess::AddVariable(const std::string& _name, Argument _value)
{
	m_variableMap.emplace(_name, new float(_value));
}

inline const VariableMap& FileProcess::GetVariableMap() const
{
	return m_variableMap;
}