#pragma once
#include "../IProcess.h"
#include <iostream>

/**
* @class	ログ出力用処理
* @brief	引数を改行しながらcoutに出力する
* @details	書式：	log(var,var,...)
*			引数：	出力する数値
*			戻り値：	0.0
*			例文：	log(1,-1)
*					log(speed,0,1,2,3)
*/
class LogProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) {
		for (auto& arg : _args) {
			std::cout << arg << std::endl;
		}
		return 0.0f;
	}
};

HPE_REGISTER_PROCESS(LogProcess, "log");
