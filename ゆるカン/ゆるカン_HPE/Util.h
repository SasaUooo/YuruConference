// 乱数生成用のファイル。本体には関係なし
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