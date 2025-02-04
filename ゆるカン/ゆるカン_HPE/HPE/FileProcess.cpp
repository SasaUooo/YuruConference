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

		// 外部の引数を設定する
		auto size = _input.size();
		for (int i = 0; i < size; i++) {
			std::string key = "#input"s + std::to_string(i + 1);
			*m_variableMap[key] = _input[i];
		}
	}
	catch (process_text_parse_error& e) {
		std::cout << "解釈失敗" << std::endl;
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

	// メンバー変数を全てクリア
	m_processDataList.clear();
	m_variableMap.clear();
	m_onceRemoved = false;
}

void FileProcess::StartProcess(std::any& _owner)
{
	static std::vector<Argument> args = std::vector<Argument>();	// 使うことはないので高速化のためにstaticにする
	float ret = 0.0f;
	this->Execute(_owner, args, &ret);
}

void FileProcess::AddProcessData(const std::string& _funcName, const std::vector<FileArgument>& _fileArgs, const std::string& _result, int _ifSkipAmount, int _iterationAmount, bool _once)
{
	ProcessData proc;
	proc.pFunction = ProcessProvider::GetInstance(_funcName);	// 関数名を追加

	// ファイルで読み取ったデータからこのインスタンスで使うポインタに変換する
	for (auto& fileArg : _fileArgs) {
		std::pair<Argument*, bool>	pair;

		// 変数だったら変数マップの値のポインタ
		// 定数だったらnewしてそのポインタを使う
		// フラグは定数の時にtrueにする
		std::visit([&](const auto& arg) {
			using T = std::decay_t<decltype(arg)>;
			if constexpr (std::is_same_v<T, std::string>) {
				// std::stringの場合
				pair.first = m_variableMap[arg];
				pair.second = false;
			}
			else if constexpr (std::is_same_v<T, float>) {
				// floatの場合
				pair.first = new float(arg);
				pair.second = true;
			}
			}, fileArg);

		// 処理実行時の引数データに追加
		proc.preArguments.emplace_back(pair);
	}
	proc.pResult = m_variableMap[_result];	// 変数名から変数マップの値のポインタにする
	proc.ifSkipAmount = _ifSkipAmount;
	proc.iterationAmount = _iterationAmount;
	proc.once = _once;
	m_processDataList.emplace_back(proc);
}

// FileProcessの_argsは空
Argument FileProcess::Process(std::any& _owner, const std::vector<Argument>& _args)
{
	// Execute関数に渡す引数のリスト
	//static std::vector<Argument> m_tempArguments = std::vector<Argument>();
	std::string oldFileName = m_loadingFileName;
	m_processChanged = false;

	// m_processDataListを前から順番に処理
	auto dataListSize = m_processDataList.size();
	for (int itr = 0; itr < dataListSize; itr++) {
		auto& procData = m_processDataList[itr];

		// 必要なサイズ分にresize
		auto size = procData.preArguments.size();
		m_tempArguments.resize(size);

		// PreArgumentsからArgumentsに変換する処理
		// 値渡しのために数値をコピーする
		for (int i = 0; i < size; i++) {
			m_tempArguments[i] = *procData.preArguments[i].first;
		}

		bool evaluation = static_cast<bool>(procData.pFunction->Execute(_owner, m_tempArguments, procData.pResult));
		// 条件式かつfalseだったら#ifの中の処理を飛ばす
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

	// #once用
	if (!m_onceRemoved && !m_processChanged) {
		// 定数を解放する
		auto enditr = m_processDataList.end();
		for (auto& proc : m_processDataList) {
			if (proc.once) {
				proc.Uninit();
			}
		}

		// 処理リストから削除
		m_processDataList.erase(std::remove_if(m_processDataList.begin(), m_processDataList.end(), [](ProcessData& data) {
			return data.once;
			}), m_processDataList.end());

		// フラグを立てて何回もこの処理を実行しないようにする
		m_onceRemoved = true;
	}
	return 0.0f;
}
