#include "FileProcess.h"
#include "ProcessTextParser.h"
#include <algorithm>
#include <execution>

FileProcess::~FileProcess()
{
	Reset();
}

void FileProcess::Load(const char* _filepath, const std::vector<Argument>& _input)
{
	try {
		m_loadingFileName = _filepath;
		ProcessTextParser::Load(_filepath, *this);

		// �O���̈�����ݒ肷��
		auto size = _input.size();
		for (int i = 0; i < size; i++) {
			std::string key = "#input"s + std::to_string(i + 1);
			*m_variableMap[key] = _input[i];
		}
	}
	catch (process_text_parse_error& e) {
		std::cout << "���ߎ��s" << std::endl;
		std::cout << e.what() << std::endl;
	}
}

void FileProcess::Reset()
{
	for (auto& proc : m_processDataList) {
		proc.Uninit();
	}
	for (auto& var : m_variableMap) {
		delete var.second;
		var.second = nullptr;
	}

	// �����o�[�ϐ���S�ăN���A
	m_processDataList.clear();
	m_variableMap.clear();
	m_onceRemoved = false;
}

void FileProcess::StartProcess(std::any& _owner)
{
	static std::vector<Argument> args = std::vector<Argument>();	// �g�����Ƃ͂Ȃ��̂ō������̂��߂�static�ɂ���
	float ret = 0.0f;
	this->Execute(_owner, args, &ret);
}

void FileProcess::AddProcessData(const std::string& _funcName, const std::vector<FileArgument>& _fileArgs, const std::string& _result, int _ifSkipAmount, int _iterationAmount, bool _once)
{
	ProcessData proc;
	proc.pFunction = ProcessProvider::GetInstance(_funcName);	// �֐�����ǉ�

	// �t�@�C���œǂݎ�����f�[�^���炱�̃C���X�^���X�Ŏg���|�C���^�ɕϊ�����
	for (auto& fileArg : _fileArgs) {
		std::pair<Argument*, bool>	pair;

		// �ϐ���������ϐ��}�b�v�̒l�̃|�C���^
		// �萔��������new���Ă��̃|�C���^���g��
		// �t���O�͒萔�̎���true�ɂ���
		std::visit([&](const auto& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::string>) {
				// std::string�̏ꍇ
				pair.first = m_variableMap[arg];
				pair.second = false;
			}
			else if constexpr (std::is_same_v<T, float>) {
				// float�̏ꍇ
				pair.first = new float(arg);
				pair.second = true;
			}
			}, fileArg);

		// �������s���̈����f�[�^�ɒǉ�
		proc.preArguments.emplace_back(pair);
	}
	proc.pResult = m_variableMap[_result];	// �ϐ�������ϐ��}�b�v�̒l�̃|�C���^�ɂ���
	proc.ifSkipAmount = _ifSkipAmount;
	proc.iterationAmount = _iterationAmount;
	proc.once = _once;
	m_processDataList.emplace_back(proc);
}

// FileProcess��_args�͋�
Argument FileProcess::Process(std::any& _owner, const std::vector<Argument>& _args)
{
	// Execute�֐��ɓn�������̃��X�g
	//static std::vector<Argument> m_tempArguments = std::vector<Argument>();
	std::string oldFileName = m_loadingFileName;
	m_processChanged = false;

	// m_processDataList��O���珇�Ԃɏ���
	auto dataListSize = m_processDataList.size();
	for (int itr = 0; itr < dataListSize; itr++) {
		auto& procData = m_processDataList[itr];

		// �K�v�ȃT�C�Y����resize
		auto size = procData.preArguments.size();
		m_tempArguments.resize(size);

		// PreArguments����Arguments�ɕϊ����鏈��
		// �l�n���̂��߂ɐ��l���R�s�[����
		for (int i = 0; i < size; i++) {
			m_tempArguments[i] = *procData.preArguments[i].first;
		}

		bool evaluation = static_cast<bool>(procData.pFunction->Execute(_owner, m_tempArguments, procData.pResult));
		// ����������false��������#if�̒��̏������΂�
		if (!evaluation) {
			itr += procData.ifSkipAmount;
		}
		if (procData.iterationAmount != 0) {
			itr += procData.iterationAmount;
		}
		if (m_loadingFileName != oldFileName) {
			m_processChanged = true;
			break;
		}
	}

	// #once�p
	if (!m_onceRemoved && !m_processChanged) {
		// �萔���������
		auto enditr = m_processDataList.end();
		for (auto& proc : m_processDataList) {
			if (proc.once) {
				proc.Uninit();
			}
		}

		// �������X�g����폜
		m_processDataList.erase(std::remove_if(m_processDataList.begin(), m_processDataList.end(), [](ProcessData& data) {
			return data.once;
			}), m_processDataList.end());

		// �t���O�𗧂Ăĉ�������̏��������s���Ȃ��悤�ɂ���
		m_onceRemoved = true;
	}
	return 0.0f;
}
