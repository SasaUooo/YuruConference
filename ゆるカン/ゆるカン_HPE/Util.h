// ���������p�̃t�@�C���B�{�̂ɂ͊֌W�Ȃ�
#pragma once
#include <random>

namespace Util {
	inline std::mt19937& GetRandomGenerator();
}

std::mt19937& Util::GetRandomGenerator()
{
	static std::random_device rd;
	static std::mt19937 gen(rd());
	return gen;
}