#pragma once
#include "FileProcess.h"
#include <deque>
#include <stack>
#include <fstream>
#include <sstream>
#include <iostream>
#include <ranges>
#include <filesystem>
using namespace std::string_literals;


/**
* @class	ProcessCompiler�Ō��m�����G���[��f�����߂�exception�N���X
* @details	���ɓ��ʂȂ��Ƃ͂Ȃ����ʂ�exception
*/
class process_text_parse_error : public std::exception {
public:
	explicit process_text_parse_error(const std::string& _what) : std::exception(_what.c_str()) {}
};

/**
* @class	FileProcess�̃R���p�C��
* @brief	�e�L�X�g�t�@�C������FileProcess���쐬�������
*/
class ProcessTextParser
{
public:
	/**
	* @brief	�e�L�X�g�t�@�C���̃��[�h����
	* @details	�e�L�X�g�t�@�C����ǂݎ���āAFileProcess��
	*
	* @param	_filePath	 �ǂݍ��ރe�L�X�g�t�@�C���̃p�X
	* @param	_fileProcess ����������FileProcess�C���X�^���X
	* @param	_input		 �����ɊO������^�������
	* @throw	�e�L�X�g�t�@�C���̓ǂݍ��݂Ɏ��s�����std::runtime_error��throw�����
	* @throw	�e�L�X�g�t�@�C���̉��߂Ɏ��s�����process_compile_error��throw�����
	*
	* @memo		�|��Ɏ��s���Ă����ꍇ�͋�̕ϐ��}�b�v���̏������X�g���L���b�V���ɕۑ�����܂�
	*/
	static inline void Load(const char* _filePath, FileProcess& _fileProcess);	// �t�@�C������ǂݍ��ޏ���

private:
	/**
	* @brief	���[�h�����̂����ϐ����̏���
	* @details	������L���[�̒�����֐��̋L�q���s����ӏ��܂ŕϐ���ǂݍ��݁A
	*			FileProcess�̕ϐ��}�b�v�ɓo�^����
	*			�����l���ݒ肳��Ȃ��ꍇ��0.0�������l�ɂȂ�
	*
	* @param	_procStr �e�L�X�g�t�@�C�����s���Ƃɓǂݍ���string�^��deque�z��
	* @param	_tempID �L���b�V���Ɏg��������B
	* @return	�֐��̎��s���ʁB���߂Ɏ��s�����false�B
	*
	* @memo		_procStr�ŉ��߂��I������������pop���Ă���̂ŁA���s�O�ƌ��ʂ��ς��܂�
	*			1���ł����߂Ɏ��s�����ꍇ�̓L���b�V���ɂ͕ϐ���1���ۑ�����܂���
	*/
	static inline bool TranslateVariables(std::deque<std::string>& _procStr, const std::string& _tempID);	// Load�̃T�u�֐� �ϐ������Ə��������𕪂���

	/**
	* @brief	���[�h�����̂����֐����̏���
	* @details	������L���[�̒�����֐��̋L�q��ǂݍ��݁A
	*			�֐����A�������X�g�A�߂�l������ϐ��̏����쐬���A
	*			�����f�[�^�L���b�V���ɕۑ�����
	*
	* @param	_procStr �e�L�X�g�t�@�C�����s���Ƃɓǂݍ���string�^��deque�z��
	* @param	_tempID �L���b�V���Ɏg��������B
	* @return	�֐��̎��s���ʁB���߂Ɏ��s�����false�B
	*
	* @memo		_procStr�ŉ��߂��I������������pop���Ă���̂ŁA���s�O�ƌ��ʂ��ς��܂�
	*			1���ł����߂Ɏ��s�����ꍇ�̓L���b�V���ɂ͖��߂�1���ۑ�����܂���
	*/
	static inline bool TranslateFunctions(std::deque<std::string>& _procStr, const std::string& _tempID);		// Load�̃T�u�֐� �������������߂���

	/**
	* @brief	�L���b�V���Ɏg����������擾����֐�
	* @details	�t�@�C���p�X�������Ŏ󂯎���āA
				�t�@�C���p�X�ƍŏI�ύX�������q����������𐶐����ĕԂ�
	* @param	_filePath �����񐶐��Ɏg�p����t�@�C���p�X
	* @return	�������ꂽ������
	*
	* @memo		�ŏI�ύX�����̓G�N�X�v���[���[�Ō���邠��B
	*			���g��ύX������ς��̂Ńv���O�������s���̍X�V���`�F�b�N�ł���B
	*/
	[[nodiscard]] static inline std::string GetTemporaryFileID(const char* _filePath);

	/**
	* @brief	#_keyword�ɑΉ�����#end_keyword��T��
	* @details	�c��̕����̃e�L�X�g��S�Ď󂯎���āA
	*			�Ή�����l�X�g��#endif���o�Ă���܂ł̏����̐����2�����ɓ����
	* @param	_procStr	�c��̕����̃e�L�X�g
	* @param	_skipAmountResult	#if����#endif�܂łǂꂾ�����������邩�ifalse�̎��ɉ��s��΂����j
	* @param	_keyword	if��for�Ȃǂ̃L�[���[�h
	* @return	�Ή�����#end���������true�A������Ȃ����false
	*/
	[[nodiscard]] static inline bool GetPairEnd(std::deque<std::string> _procStr, int* _skipAmountResult, const std::string& _keyword);

	/**
	* @brief	FileProcess�Ƀf�[�^��n���ď��������Ă��炤
	* @details	tempID���g���ăL���b�V������t�@�C���̓ǂݍ��݃f�[�^���擾��
	*			FileProcess�ɓn���Ē��g�𐶐����Ă��炤
	* @param	_tempID �L���b�V������̓ǂݍ��݂Ɏg��������
	* @param	_fileProcess �ǂݍ��񂾃f�[�^������FileProcess�̃C���X�^���X
	*/
	static inline void ApplyData(const std::string& _tempID, FileProcess& _fileProcess);


	///< �e�L�X�g�t�@�C������ǂݎ�����f�[�^���L���b�V�����Ă������߂̃R���e�i
	///< tempID���L�[�Ƃ��āA�ϐ��}�b�v�Ə����f�[�^�z����y�A�ɂ��Ă���
	static inline std::unordered_map<std::string, std::pair<std::map<std::string, Argument>, std::vector<FunctionData>>> m_parsedDataMap
		= std::unordered_map<std::string, std::pair<std::map<std::string, Argument>, std::vector<FunctionData>>>();
};



/* ----------------------------------------------------------------- */
/* INLINE */
/* ----------------------------------------------------------------- */
void ProcessTextParser::Load(const char* _filePath, FileProcess& _fileProcess)
{
	// �t�@�C���v���Z�X���܂�����ȏ�Ԃɂ���
	_fileProcess.Reset();

	// �t�@�C�����ƕύX��������L���b�V���Ɏg��������𐶐�
	auto tempID = GetTemporaryFileID(_filePath);

	// �L���b�V���ɂȂ�������|�󂷂�
	if (!m_parsedDataMap.contains(tempID)) {
		std::ifstream file(_filePath); // �ǂݍ��ރt�@�C���̖��O
		if (!file) {
			throw std::runtime_error("�t�@�C�����J���܂���ł���: "s + _filePath);
		}

		// �t�@�C������1�s���ǂݍ���
		// �s�v�ȍs�͖�������
		std::deque<std::string> procStr; // �s���i�[����L���[
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty()) continue;				// ��̍s�Ȃ疳������	
			if (line.substr(0, 2) == "//") continue;// �s���� "//" �Ŏn�܂�ꍇ�̓R�����g�Ȃ̂Ŗ�������
			procStr.push_back(line);
		}

		file.close();

		// ��ɃL���b�V���ɗv�f�����
		m_parsedDataMap.emplace(tempID, std::pair<std::map<std::string, Argument>, std::vector<FunctionData>>());

		bool ret = true;
		// �ϐ�����ǂݍ���
		ret = TranslateVariables(procStr, tempID);
		if (!ret) {
			throw process_text_parse_error("�ϐ����̉��߂Ɏ��s���܂���");
		}

		// �֐�����ǂݍ���
		std::vector<FunctionData> functionDataList = std::vector<FunctionData>();
		ret = TranslateFunctions(procStr, tempID);
		if (!ret) {
			throw process_text_parse_error("�֐����̉��߂Ɏ��s���܂���");
		}
	}

	// �L���b�V������Ƃ��Ă���FileProcess��������
	ApplyData(tempID, _fileProcess);
}

bool ProcessTextParser::TranslateVariables(std::deque<std::string>& _procStr, const std::string& _tempID)
{
	//--------------------
	// �ϐ������̏���
	//--------------------
	bool ret = true;
	std::map<std::string, Argument> variableMap = std::map<std::string, Argument>();	// �ꎞ�I�ȕϐ��}�b�v
	std::vector<std::string> varInitProcs = std::vector<std::string>();	// �O���ϐ��ł̏��������Ɋ֐����ɒǉ����鏈���̔z��

	// �|�󂷂镶����̃L���[����ɂȂ�܂Ń��[�v
	while (!_procStr.empty()) {
		std::string str = _procStr.front();

		// �ϐ��錾����Ȃ������񂪏o�Ă����珈���I��
		if (str.substr(0, 4) != "var ") break;

		//------------------
		// �ϐ��̏����`�F�b�N
		//------------------
		std::istringstream ss(str);
		try {
			std::string varKeyword, varName, initValue;
			ss >> varKeyword >> varName >> initValue;

			// �ϐ����`�F�b�N
			if (varName.empty() || !std::isalpha(static_cast<unsigned char>(varName[0]))) {
				throw process_text_parse_error("ERROR: "s + str + "\n�����ȕϐ����ł�: " + varName);
			}
			// �f�t�H���g�̏����l��0.0
			if (initValue.empty()) initValue = "0.0";	// �����l���w�肳��Ă��Ȃ����0

			/* �O������̈����𗘗p���ď���������ꍇ�̏��� */
			if (initValue.substr(0, 6) == "#input") {
				variableMap.emplace(initValue, std::numeric_limits<float>::min());	// �������󂯎�锠��p��
				varInitProcs.emplace_back("#once set(" + initValue + ")>>" + varName);	// �������p�̏���

				initValue = std::to_string(std::numeric_limits<float>::min());						// ��U�ُ�l�ŏ��������ĖႤ
			}

			// �ϐ����Ə����l�ňꎞ�I�ȃ}�b�v�ɗv�f���\�z
			variableMap.emplace(varName, std::stof(initValue));
		}
		//-----------------
		//�G���[�n���h�����O
		//-----------------
		catch (std::invalid_argument& e) {
			// 185�s�ڂ�stof�̗�O
			std::cerr << "�ϐ��̏����l�������ł�" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		catch (std::out_of_range& e) {
			// 185�s�ڂ�stof�̗�O
			std::cerr << "�����l���\���ł���͈͂𒴂��Ă��܂�" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		catch (process_text_parse_error& e) {
			// 179�s�ڂ̕ϐ����̗�O
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		catch (std::exception& e) {
			// �����͏o�Ȃ��͂�
			std::cerr << "�ϐ��錾�ɂ����Ė��m�̗�O���������܂���" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		_procStr.pop_front();
	}

	// 1�ł��ǂݍ��߂ĂȂ��������ɂ���
	if (!ret) variableMap.clear();

	// �L���b�V���ɓo�^
	m_parsedDataMap[_tempID].first = variableMap;

	for (auto& proc : varInitProcs) {
		_procStr.push_front(proc);
	}

	return ret;
}

bool ProcessTextParser::TranslateFunctions(std::deque<std::string>& _procStr, const std::string& _tempID)
{
	//--------------------
	// �֐������̏���
	//--------------------
	std::vector<FunctionData> functionDataList = std::vector<FunctionData>();
	functionDataList.reserve(_procStr.size());
	std::stack<std::pair<int, int>> forloopStack = std::stack<std::pair<int, int>>();	// ���[�v�̍Ō�̏����Ƃǂꂾ���߂邩
	int forloopLine = -1;
	int line = 0;
	bool ret = true;

	while (!_procStr.empty()) {
		try {
			FunctionData data;

			auto& str = _procStr.front();
			// ���p�X�y�[�X�����ׂč폜
			str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

			// �ŏ���#once�Ə�����Ă������񂾂����s����t���O��true�ɂ���
			if (str.substr(0, 5) == "#once") {
				data.once = true;
				str = str.substr(5);
			}
			if (str.substr(0, 3) == "#if") {
				if (!GetPairEnd(_procStr, &data.ifSkipAmount, "if")) {
					throw process_text_parse_error("ERROR: "s + str + "\n#endif������܂���");
				}
				str = str.substr(3);
			}
			if (str.substr(0, 6) == "#while") {
				int skipAmount = 0;
				if (!GetPairEnd(_procStr, &skipAmount, "while")) {
					throw process_text_parse_error("ERROR: "s + str + "\n#endwhile������܂���");
				}
				data.ifSkipAmount = skipAmount;
				forloopStack.push(std::pair(line + skipAmount, -1 * (skipAmount + 1)));
				forloopLine = forloopStack.top().first;
				str = str.substr(6);
			}
			if (str.substr(0, 4) == "#end") {
				_procStr.pop_front();
				continue;
			}

			// ()��T���Ē��g�𕶎���Ƃ��Ď��o��
			auto argBegin = str.find("(", 0);	// (�̈ʒu
			auto argEnd = str.find(")", 0);		// )�̈ʒu

			// "(" �� ")" ���Ȃ�������֐��Ƃ��Ă��������̂ŃG���[
			if (argBegin == std::string::npos || argEnd == std::string::npos) {
				throw process_text_parse_error("ERROR: "s + str + "\n�֐���()���s���ł�");
			}

			//--------���֐����擾
			auto funcName = str.substr(0, argBegin);	// �ŏ�����"("�̎�O�܂ł��֐����Ƃ��Ď擾

			// �֐������Ȃ�������G���[
			if (funcName.empty()) {
				throw process_text_parse_error("ERROR: "s + str + "\n�֐������s���ł�");
			}
			// �֐����o�^����ĂȂ�������G���[
			if (!ProcessProvider::IsRegistered(funcName)) {
				throw process_text_parse_error("ERROR: "s + str + "\n���m�̊֐��ł�");
			}
			data.functionName = funcName;

			//--------���������擾
			auto& varMap = m_parsedDataMap[_tempID].first;	// �ϐ��}�b�v
			auto argStr = str.substr(argBegin + 1, argEnd - (argBegin + 1));	// "("�̎��̕�������")"�̎�O�܂ł������Ƃ���
			// �������Ƃɋ�؂�
			std::vector<std::string> splittedArgumentStr = std::vector<std::string>();
			for (auto&& part : std::ranges::views::split(argStr, ',')) {
				splittedArgumentStr.emplace_back(part.begin(), part.end());
			}

			// stof���g�p���ĕϐ��ƒ萔�̈Ⴂ��t����
			std::vector<FileArgument> fileArguments = std::vector<FileArgument>();
			for (auto& splittedArg : splittedArgumentStr) {
				try {
					// ���l�ɏo����Ȃ炻�̂܂ܔz��ɒǉ�
					auto value = std::stof(splittedArg);
					fileArguments.emplace_back(value);
				}
				catch ([[maybe_unused]] std::exception& e) {
					// 278�s�ڂ�stof�̃G���[�AsplittedArg�͐��l����Ȃ��ĕϐ���
					if (varMap.contains(splittedArg)) {
						// �ϐ��������݂��邩�����`�F�b�N�B
						// FileProcess�̃C���X�^���X���ŕϐ��}�b�v�̒l�ւ̃|�C���^�ɕϊ�����
						fileArguments.emplace_back(splittedArg);
					}
					else {
						throw process_text_parse_error("ERROR: "s + str + "\n���݂��Ȃ��ϐ��ł�: " + splittedArg);
					}
				}
			}
			data.fileArguments = std::move(fileArguments);

			//--------�߂�l������ϐ����擾
			std::string returnArg = "";

			// )�̎�����2������>>�Ȃ�߂�l�̎w�肪����
			if (str.substr(argEnd + 1, 2) == ">>") {
				returnArg = str.substr(argEnd + 3);	// �߂�l������ϐ������擾

				// �ϐ��}�b�v�ɖ��O�����݂��Ȃ��Ȃ�G���[
				if (!varMap.contains(returnArg)) {
					throw process_text_parse_error("ERROR: "s + str + "\n���݂��Ȃ��ϐ��ł�: " + returnArg);
				}
			}
			data.returnArgumentName = returnArg;

			if (line == forloopLine) {
				data.iterationAmount = forloopStack.top().second;
				forloopStack.pop();
				if (!forloopStack.empty()) {
					forloopLine = forloopStack.top().first;
				}
				else {
					forloopLine = -1;
				}
			}

			functionDataList.emplace_back(data);
		}
		catch (process_text_parse_error& e) {
			// �֐��̏������ϐ����̃~�X�Ƃ��B
			std::cout << e.what() << std::endl;
			ret = false;	// 1���ł����߂ł��Ȃ������猋�ʂ�false�ɂ���
		}
		catch (std::exception& e) {
			// �����͎��s����Ȃ��͂�
			std::cerr << "���m�̃G���[�ł�" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		line++;
		_procStr.pop_front();
	}
	if (!ret) functionDataList.clear();	// ���߂ł��ĂȂ�������S������

	// ���Ɍ����ăL���b�V��
	m_parsedDataMap[_tempID].second = functionDataList;

	return ret;
}

inline std::string ProcessTextParser::GetTemporaryFileID(const char* _filePath)
{
	std::string timeString = "";
	try {
		// �t�@�C���̍ŏI�X�V�������擾
		auto ftime = std::filesystem::last_write_time(_filePath);


		// `ftime` �����ԓ_�ɕϊ�
		auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

		// ������ `std::time_t` �^�ɕϊ�
		std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

		// ���S��localtime_s���g�p
		std::tm localTime;
		localtime_s(&localTime, &cftime);

		// �����𕶎���Ƃ��ăt�H�[�}�b�g
		std::ostringstream oss;
		oss << std::put_time(&localTime, "%Y-%m-%d_%H-%M-%S");
		timeString = oss.str();
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error: " + std::string(e.what()) << std::endl;
	}

	return std::string(_filePath) + "_" + timeString;
}

inline bool ProcessTextParser::GetPairEnd(std::deque<std::string> _procStr, int* _skipAmountResult, const std::string& _keyword)
{
	int level = 1;	// �l�X�g�̐[��
	auto begin = "#" + _keyword;
	auto end = "#end" + _keyword;
	auto beginSize = begin.size();
	auto endSize = end.size();
	_procStr.pop_front();	// #if�̕����̂���
	while (level != 0 && !_procStr.empty()) {
		auto& funcName = _procStr.front();

		// �l�X�g������[��+1
		if (funcName.substr(0, beginSize) == begin) {
			level++;
		}
		if (funcName.substr(0, 4) == "#end") {
			(*_skipAmountResult)--;	// end�͏����ł͂Ȃ��̂Ń��X�g�Ɏc��Ȃ����J�E���g�𑝂₵�����Ȃ��̂ő��E
			// endif���o�Ă�����[��-1
			if (funcName.substr(0, endSize) == end) {
				level--;
			}
		}
		// if���̒��̏����֐��̃J�E���g+1
		(*_skipAmountResult)++;
		_procStr.pop_front();
	}
	// �S�����I������̂Ƀl�X�g���c���Ă�Ȃ�endif������ĂȂ��̂Ŏ��s���s
	if (level != 0) {
		return false;
	}
	return true;
}

inline void ProcessTextParser::ApplyData(const std::string& _tempID, FileProcess& _fileProcess)
{
	for (auto& pair : m_parsedDataMap[_tempID].first) {
		_fileProcess.AddVariable(pair.first, pair.second);
	}

	// fileProcess�ɔ��f
	for (auto& data : m_parsedDataMap[_tempID].second) {
		_fileProcess.AddProcessData(
			data.functionName,
			data.fileArguments,
			data.returnArgumentName,
			data.ifSkipAmount,
			data.iterationAmount,
			data.once
		);
	}
}
