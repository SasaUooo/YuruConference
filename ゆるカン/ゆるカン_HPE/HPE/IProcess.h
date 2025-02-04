#pragma once
#include <variant>
#include <string>
#include <vector>
#include <any>
#include <functional>
#include "ProcessProvider.h"

constexpr float HPE_RESULT_ERROR = std::numeric_limits<float>::min();

// プロセスのクラスを作ったらクラスの定義の外にこのマクロをおいて関数として登録する
// 例：HPE_REGISTER_PROCESS(AddProcess, "add")
#define HPE_REGISTER_PROCESS(className, funcName)  \
namespace className##ProcessRegistrar{\
	static ProcessRegistrar<className> m_registrar = ProcessRegistrar<className>(funcName);\
}

// 条件式として扱いたいクラスに書く
// こいつはクラスの中に書く。このマクロの下はpublicになるので注意
#define HPE_THIS_IS_CONDITION(className) \
public:		\
	className(){	\
		m_isCondition = true;	\
	}

using Argument = float;		///< 引数で扱う数値。floatのみサポートしている

class IProcess
{
	// FileProcessからはExecuteが実行出来てほしいのでfriend
	friend class FileProcess;
public:
	/**
	* @brief	仮想デストラクタ
	* @details	default
	*/
	virtual ~IProcess() = default;

protected:
	/**
	 * @brief	実際に実行される処理
	 * @param	_owner この処理が影響を与えるオブジェクトへの参照
	 * @param	_args 処理で使用する引数
	 * @return	処理の戻り値。float型を1つだけ返せる
	 */
	virtual Argument Process(std::any& _owner, const std::vector<Argument>& _args) = 0;

private:
	/**
	 * @brief	継承先のProcess関数を実行するための処理
	 * @param 	_owner この処理が影響を与えるオブジェクトへの参照
	 * @param 	_args この処理で使用する引数
	 * @param 	_return この処理の戻り値を格納するArgumentへのポインタ
	 * @return	この処理の評価。0なら正常終了
	 */
	virtual float Execute(std::any& _owner, const std::vector<Argument>& _args, Argument* _return, float _result = 0.0f) {
		float result = Process(_owner, _args);
		if (_return != nullptr) {
			*_return = result;
		}

		// 条件式じゃないなら0だけ返す
		if (!m_isCondition) {
			result = 0.0f;
		}
		return result;
	}

protected:
	bool m_isCondition = false;	// この処理が条件式かどうかのフラグ
};
