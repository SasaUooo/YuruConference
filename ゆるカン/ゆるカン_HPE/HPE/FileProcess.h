#pragma once
#include "IProcess.h"
#include "ProcessProvider.h"
#include <unordered_map>
#include <memory>
#include <any>
#include <map>

///< Argumentへのポインタと定数値フラグ（trueだったらnewされてるのでdeleteが必要）
using PreArgument = std::pair<Argument*, bool>;

///< ファイルから読み込んできた引数の要素。変数名あるいは定数
using FileArgument = std::variant<std::string, Argument>;

///< 変数マップの型
using VariableMap = std::map<std::string, Argument*>;

/**
* @brief	関数を実行に必要な要素をまとめた構造体
*/
struct FunctionData {
	std::string functionName = "";		///< 関数名
	std::vector<FileArgument> fileArguments = std::vector<FileArgument>();	///< 関数が使う引数のデータ
	std::string returnArgumentName = "";	///< 戻り値を渡す変数の名前
	int ifSkipAmount = 0;
	int iterationAmount = 0;
	bool once = false;
};

/**
* @class	テキストファイルを読み込んだ処理
*/
class FileProcess : public IProcess
{
	/**
	* @brief	実際に実行するときに使うデータ
	* @details	function	実行する関数のunique_ptrのポインタ。開放の必要なし
	*			preArguments 実行時に第1要素のfloat*をfloatにして引数として使う
	*			result		実行結果を受け取る変数へのポインタ。開放の必要なし。nullptrでもいい
	*/
	struct ProcessData {
		IProcess* pFunction = nullptr;
		std::vector<PreArgument> preArguments = std::vector<PreArgument>();
		Argument* pResult = nullptr;
		int ifSkipAmount = 0;
		int iterationAmount = 0;
		bool once = false;

		// プロセス内に含まれる定数をすべて破棄
		// (初期化時にnewで生成されているため。)
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
	* @brief	コンストラクタ
	* @details	default
	*/
	FileProcess() = default;

	/**
	* @brief	デストラクタ
	* @details	Reset関数を実行
	*/
	~FileProcess();

	/**
	* @brief	テキストファイルをロードする
	* @details	ProcessCompilerにpathと自分のインスタンスを渡すだけ
	* @param	_filepath	ロードするテキストファイルのパス
	* @param	_input		処理に外から渡す引数
	*/
	void Load(const char* _filepath, const std::vector<Argument>& _input);

	/**
	* @brief	読み込まれている処理を破棄する
	* @details	初期化時にnewされた定数をdelete
	*			処理のリストと変数マップをクリア
	*/
	void Reset();

	/**
	* @brief	読み込まれている処理を実行する
	* @details	Execute関数を引数なし、戻り値を受け取らない状態で実行する
	* @param	_owner	この処理が影響を与えるインスタンスへのポインタ
	*
	* @memo		大体Update処理で実行することを想定している
	*/
	void StartProcess(std::any& _owner);

	/**
	* @brief	処理データを追加する
	* @param	_funcName	関数の名前
	* @param	_preArgs	ファイルから読み取った引数データのリスト。変数名かfloat値が入ってる
	* @param	_result		戻り値を受け取る変数の名前
	* @param	_once		一回だけ実行するフラグ
	*/
	void AddProcessData(const std::string& _funcName, const std::vector<FileArgument>& _fileArgs,
		const std::string& _result, int _ifSkipAmount, int _iterationAmount, bool _once);

	/**
	* @brief	変数データを追加する
	* @param	_name	追加する変数の名前
	* @param	_value	変数の初期値
	*/
	inline void AddVariable(const std::string& _name, Argument _value);

	/**
	* @brief	変数マップを取得する
	* @return	変数マップをconstで返す
	*/
	inline const VariableMap& GetVariableMap() const;
private:
	/**
	* @brief	IProcess::Executeで実行されるプロセス
	* @details	m_processListに格納された処理を実行する
				処理クラスインスタンス、引数の値、戻り値のポインタを取得して順次実行する
	* @param	_owner	影響を与えるクラスのインスタンスへのポインタ
	* @param	_args	実行する関数に与える引数のリスト
	* @return	関数の戻り値。処理によってはない。
	*/
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override;

private:
	///< 変数の名前と値を結びつけるマップ。変数マップ
	VariableMap m_variableMap = VariableMap();

	///< 実行される処理の配列
	std::vector<ProcessData> m_processDataList = std::vector<ProcessData>();

	///< 引数を渡すときに一時的に入れておくための配列
	std::vector<Argument> m_tempArguments = std::vector<Argument>();

	///< #onceフラグの処理を実行して削除したかのフラグ
	bool m_onceRemoved = false;

	///< setmoveなどでプロセスが途中で変更されたときにtrueにするフラグ
	bool m_processChanged = false;

	///< 変更を検知するために今持ってるファイルの名前を持っておく
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