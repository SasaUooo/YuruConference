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
* @class	ProcessCompilerで検知したエラーを吐くためのexceptionクラス
* @details	特に特別なことはない普通のexception
*/
class process_text_parse_error : public std::exception {
public:
	explicit process_text_parse_error(const std::string& _what) : std::exception(_what.c_str()) {}
};

/**
* @class	FileProcessのコンパイラ
* @brief	テキストファイルからFileProcessを作成する役割
*/
class ProcessTextParser
{
public:
	/**
	* @brief	テキストファイルのロード処理
	* @details	テキストファイルを読み取って、FileProcessに
	*
	* @param	_filePath	 読み込むテキストファイルのパス
	* @param	_fileProcess 処理を入れるFileProcessインスタンス
	* @param	_input		 処理に外部から与える引数
	* @throw	テキストファイルの読み込みに失敗するとstd::runtime_errorがthrowされる
	* @throw	テキストファイルの解釈に失敗するとprocess_compile_errorがthrowされる
	*
	* @memo		翻訳に失敗していた場合は空の変数マップや空の処理リストがキャッシュに保存されます
	*/
	static inline void Load(const char* _filePath, FileProcess& _fileProcess);	// ファイルから読み込む処理

private:
	/**
	* @brief	ロード処理のうち変数部の処理
	* @details	文字列キューの中から関数の記述が行われる箇所まで変数を読み込み、
	*			FileProcessの変数マップに登録する
	*			初期値が設定されない場合は0.0が初期値になる
	*
	* @param	_procStr テキストファイルを行ごとに読み込んだstring型のdeque配列
	* @param	_tempID キャッシュに使う文字列。
	* @return	関数の実行結果。解釈に失敗するとfalse。
	*
	* @memo		_procStrで解釈が終わった文字列はpopしているので、実行前と結果が変わります
	*			1文でも解釈に失敗した場合はキャッシュには変数が1つも保存されません
	*/
	static inline bool TranslateVariables(std::deque<std::string>& _procStr, const std::string& _tempID);	// Loadのサブ関数 変数部分と処理部分を分ける

	/**
	* @brief	ロード処理のうち関数部の処理
	* @details	文字列キューの中から関数の記述を読み込み、
	*			関数名、引数リスト、戻り値を入れる変数の情報を作成し、
	*			処理データキャッシュに保存する
	*
	* @param	_procStr テキストファイルを行ごとに読み込んだstring型のdeque配列
	* @param	_tempID キャッシュに使う文字列。
	* @return	関数の実行結果。解釈に失敗するとfalse。
	*
	* @memo		_procStrで解釈が終わった文字列はpopしているので、実行前と結果が変わります
	*			1文でも解釈に失敗した場合はキャッシュには命令が1つも保存されません
	*/
	static inline bool TranslateFunctions(std::deque<std::string>& _procStr, const std::string& _tempID);		// Loadのサブ関数 処理部分を解釈する

	/**
	* @brief	キャッシュに使う文字列を取得する関数
	* @details	ファイルパスを引数で受け取って、
				ファイルパスと最終変更日時を繋げた文字列を生成して返す
	* @param	_filePath 文字列生成に使用するファイルパス
	* @return	生成された文字列
	*
	* @memo		最終変更日時はエクスプローラーで見れるあれ。
	*			中身を変更したら変わるのでプログラム実行中の更新をチェックできる。
	*/
	[[nodiscard]] static inline std::string GetTemporaryFileID(const char* _filePath);

	/**
	* @brief	#_keywordに対応する#end_keywordを探す
	* @details	残りの部分のテキストを全て受け取って、
	*			対応するネストの#endifが出てくるまでの処理の数を第2引数に入れる
	* @param	_procStr	残りの部分のテキスト
	* @param	_skipAmountResult	#ifから#endifまでどれだけ処理があるか（falseの時に何行飛ばすか）
	* @param	_keyword	ifやforなどのキーワード
	* @return	対応する#endが見つかればtrue、見つからなければfalse
	*/
	[[nodiscard]] static inline bool GetPairEnd(std::deque<std::string> _procStr, int* _skipAmountResult, const std::string& _keyword);

	/**
	* @brief	FileProcessにデータを渡して初期化してもらう
	* @details	tempIDを使ってキャッシュからファイルの読み込みデータを取得し
	*			FileProcessに渡して中身を生成してもらう
	* @param	_tempID キャッシュからの読み込みに使う文字列
	* @param	_fileProcess 読み込んだデータを入れるFileProcessのインスタンス
	*/
	static inline void ApplyData(const std::string& _tempID, FileProcess& _fileProcess);


	///< テキストファイルから読み取ったデータをキャッシュしておくためのコンテナ
	///< tempIDをキーとして、変数マップと処理データ配列をペアにしている
	static inline std::unordered_map<std::string, std::pair<std::map<std::string, Argument>, std::vector<FunctionData>>> m_parsedDataMap
		= std::unordered_map<std::string, std::pair<std::map<std::string, Argument>, std::vector<FunctionData>>>();
};



/* ----------------------------------------------------------------- */
/* INLINE */
/* ----------------------------------------------------------------- */
void ProcessTextParser::Load(const char* _filePath, FileProcess& _fileProcess)
{
	// ファイルプロセスをまっさらな状態にする
	_fileProcess.Reset();

	// ファイル名と変更日時からキャッシュに使う文字列を生成
	auto tempID = GetTemporaryFileID(_filePath);

	// キャッシュになかったら翻訳する
	if (!m_parsedDataMap.contains(tempID)) {
		std::ifstream file(_filePath); // 読み込むファイルの名前
		if (!file) {
			throw std::runtime_error("ファイルを開けませんでした: "s + _filePath);
		}

		// ファイルから1行ずつ読み込む
		// 不要な行は無視する
		std::deque<std::string> procStr; // 行を格納するキュー
		std::string line;
		while (std::getline(file, line)) {
			if (line.empty()) continue;				// 空の行なら無視する	
			if (line.substr(0, 2) == "//") continue;// 行頭が "//" で始まる場合はコメントなので無視する
			procStr.push_back(line);
		}

		file.close();

		// 先にキャッシュに要素を作る
		m_parsedDataMap.emplace(tempID, std::pair<std::map<std::string, Argument>, std::vector<FunctionData>>());

		bool ret = true;
		// 変数部を読み込み
		ret = TranslateVariables(procStr, tempID);
		if (!ret) {
			throw process_text_parse_error("変数部の解釈に失敗しました");
		}

		// 関数部を読み込み
		std::vector<FunctionData> functionDataList = std::vector<FunctionData>();
		ret = TranslateFunctions(procStr, tempID);
		if (!ret) {
			throw process_text_parse_error("関数部の解釈に失敗しました");
		}
	}

	// キャッシュからとってきてFileProcessを初期化
	ApplyData(tempID, _fileProcess);
}

bool ProcessTextParser::TranslateVariables(std::deque<std::string>& _procStr, const std::string& _tempID)
{
	//--------------------
	// 変数部分の処理
	//--------------------
	bool ret = true;
	std::map<std::string, Argument> variableMap = std::map<std::string, Argument>();	// 一時的な変数マップ
	std::vector<std::string> varInitProcs = std::vector<std::string>();	// 外部変数での初期化時に関数部に追加する処理の配列

	// 翻訳する文字列のキューが空になるまでループ
	while (!_procStr.empty()) {
		std::string str = _procStr.front();

		// 変数宣言じゃない文字列が出てきたら処理終了
		if (str.substr(0, 4) != "var ") break;

		//------------------
		// 変数の書式チェック
		//------------------
		std::istringstream ss(str);
		try {
			std::string varKeyword, varName, initValue;
			ss >> varKeyword >> varName >> initValue;

			// 変数名チェック
			if (varName.empty() || !std::isalpha(static_cast<unsigned char>(varName[0]))) {
				throw process_text_parse_error("ERROR: "s + str + "\n無効な変数名です: " + varName);
			}
			// デフォルトの初期値は0.0
			if (initValue.empty()) initValue = "0.0";	// 初期値が指定されていなければ0

			/* 外部からの引数を利用して初期化する場合の処理 */
			if (initValue.substr(0, 6) == "#input") {
				variableMap.emplace(initValue, std::numeric_limits<float>::min());	// 引数を受け取る箱を用意
				varInitProcs.emplace_back("#once set(" + initValue + ")>>" + varName);	// 初期化用の処理

				initValue = std::to_string(std::numeric_limits<float>::min());						// 一旦異常値で初期化して貰う
			}

			// 変数名と初期値で一時的なマップに要素を構築
			variableMap.emplace(varName, std::stof(initValue));
		}
		//-----------------
		//エラーハンドリング
		//-----------------
		catch (std::invalid_argument& e) {
			// 185行目のstofの例外
			std::cerr << "変数の初期値が無効です" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		catch (std::out_of_range& e) {
			// 185行目のstofの例外
			std::cerr << "初期値が表現できる範囲を超えています" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		catch (process_text_parse_error& e) {
			// 179行目の変数名の例外
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		catch (std::exception& e) {
			// ここは出ないはず
			std::cerr << "変数宣言において未知の例外が発生しました" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		_procStr.pop_front();
	}

	// 1つでも読み込めてなかったら空にする
	if (!ret) variableMap.clear();

	// キャッシュに登録
	m_parsedDataMap[_tempID].first = variableMap;

	for (auto& proc : varInitProcs) {
		_procStr.push_front(proc);
	}

	return ret;
}

bool ProcessTextParser::TranslateFunctions(std::deque<std::string>& _procStr, const std::string& _tempID)
{
	//--------------------
	// 関数部分の処理
	//--------------------
	std::vector<FunctionData> functionDataList = std::vector<FunctionData>();
	functionDataList.reserve(_procStr.size());
	std::stack<std::pair<int, int>> forloopStack = std::stack<std::pair<int, int>>();	// ループの最後の処理とどれだけ戻るか
	int forloopLine = -1;
	int line = 0;
	bool ret = true;

	while (!_procStr.empty()) {
		try {
			FunctionData data;

			auto& str = _procStr.front();
			// 半角スペースをすべて削除
			str.erase(std::remove(str.begin(), str.end(), ' '), str.end());

			// 最初に#onceと書かれていたら一回だけ実行するフラグをtrueにする
			if (str.substr(0, 5) == "#once") {
				data.once = true;
				str = str.substr(5);
			}
			if (str.substr(0, 3) == "#if") {
				if (!GetPairEnd(_procStr, &data.ifSkipAmount, "if")) {
					throw process_text_parse_error("ERROR: "s + str + "\n#endifが足りません");
				}
				str = str.substr(3);
			}
			if (str.substr(0, 6) == "#while") {
				int skipAmount = 0;
				if (!GetPairEnd(_procStr, &skipAmount, "while")) {
					throw process_text_parse_error("ERROR: "s + str + "\n#endwhileが足りません");
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

			// ()を探して中身を文字列として取り出す
			auto argBegin = str.find("(", 0);	// (の位置
			auto argEnd = str.find(")", 0);		// )の位置

			// "(" か ")" がなかったら関数としておかしいのでエラー
			if (argBegin == std::string::npos || argEnd == std::string::npos) {
				throw process_text_parse_error("ERROR: "s + str + "\n関数の()が不明です");
			}

			//--------↓関数を取得
			auto funcName = str.substr(0, argBegin);	// 最初から"("の手前までを関数名として取得

			// 関数名がなかったらエラー
			if (funcName.empty()) {
				throw process_text_parse_error("ERROR: "s + str + "\n関数名が不明です");
			}
			// 関数が登録されてなかったらエラー
			if (!ProcessProvider::IsRegistered(funcName)) {
				throw process_text_parse_error("ERROR: "s + str + "\n未知の関数です");
			}
			data.functionName = funcName;

			//--------↓引数を取得
			auto& varMap = m_parsedDataMap[_tempID].first;	// 変数マップ
			auto argStr = str.substr(argBegin + 1, argEnd - (argBegin + 1));	// "("の次の文字から")"の手前までを引数とする
			// 引数ごとに区切る
			std::vector<std::string> splittedArgumentStr = std::vector<std::string>();
			for (auto&& part : std::ranges::views::split(argStr, ',')) {
				splittedArgumentStr.emplace_back(part.begin(), part.end());
			}

			// stofを使用して変数と定数の違いを付ける
			std::vector<FileArgument> fileArguments = std::vector<FileArgument>();
			for (auto& splittedArg : splittedArgumentStr) {
				try {
					// 数値に出来るならそのまま配列に追加
					auto value = std::stof(splittedArg);
					fileArguments.emplace_back(value);
				}
				catch ([[maybe_unused]] std::exception& e) {
					// 278行目のstofのエラー、splittedArgは数値じゃなくて変数名
					if (varMap.contains(splittedArg)) {
						// 変数名が存在するかだけチェック。
						// FileProcessのインスタンス側で変数マップの値へのポインタに変換する
						fileArguments.emplace_back(splittedArg);
					}
					else {
						throw process_text_parse_error("ERROR: "s + str + "\n存在しない変数です: " + splittedArg);
					}
				}
			}
			data.fileArguments = std::move(fileArguments);

			//--------戻り値を入れる変数を取得
			std::string returnArg = "";

			// )の次から2文字が>>なら戻り値の指定がある
			if (str.substr(argEnd + 1, 2) == ">>") {
				returnArg = str.substr(argEnd + 3);	// 戻り値を入れる変数名を取得

				// 変数マップに名前が存在しないならエラー
				if (!varMap.contains(returnArg)) {
					throw process_text_parse_error("ERROR: "s + str + "\n存在しない変数です: " + returnArg);
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
			// 関数の書式か変数名のミスとか。
			std::cout << e.what() << std::endl;
			ret = false;	// 1文でも解釈できなかったら結果をfalseにする
		}
		catch (std::exception& e) {
			// ここは実行されないはず
			std::cerr << "未知のエラーです" << std::endl;
			std::cerr << e.what() << std::endl;
			ret = false;
		}
		line++;
		_procStr.pop_front();
	}
	if (!ret) functionDataList.clear();	// 解釈できてなかったら全部消す

	// 次に向けてキャッシュ
	m_parsedDataMap[_tempID].second = functionDataList;

	return ret;
}

inline std::string ProcessTextParser::GetTemporaryFileID(const char* _filePath)
{
	std::string timeString = "";
	try {
		// ファイルの最終更新時刻を取得
		auto ftime = std::filesystem::last_write_time(_filePath);


		// `ftime` を時間点に変換
		auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			ftime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());

		// 時刻を `std::time_t` 型に変換
		std::time_t cftime = std::chrono::system_clock::to_time_t(sctp);

		// 安全なlocaltime_sを使用
		std::tm localTime;
		localtime_s(&localTime, &cftime);

		// 時刻を文字列としてフォーマット
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
	int level = 1;	// ネストの深さ
	auto begin = "#" + _keyword;
	auto end = "#end" + _keyword;
	auto beginSize = begin.size();
	auto endSize = end.size();
	_procStr.pop_front();	// #ifの分をのぞく
	while (level != 0 && !_procStr.empty()) {
		auto& funcName = _procStr.front();

		// ネストしたら深さ+1
		if (funcName.substr(0, beginSize) == begin) {
			level++;
		}
		if (funcName.substr(0, 4) == "#end") {
			(*_skipAmountResult)--;	// endは処理ではないのでリストに残らない→カウントを増やしたくないので相殺
			// endifが出てきたら深さ-1
			if (funcName.substr(0, endSize) == end) {
				level--;
			}
		}
		// if文の中の処理関数のカウント+1
		(*_skipAmountResult)++;
		_procStr.pop_front();
	}
	// 全部見終わったのにネストが残ってるならendifが足りてないので実行失敗
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

	// fileProcessに反映
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
