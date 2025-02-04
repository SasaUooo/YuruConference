#pragma once
#include "../IProcess.h"
#include <iostream>

/**
* @class	���O�o�͗p����
* @brief	���������s���Ȃ���cout�ɏo�͂���
* @details	�����F	log(var,var,...)
*			�����F	�o�͂��鐔�l
*			�߂�l�F	0.0
*			�ᕶ�F	log(1,-1)
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
