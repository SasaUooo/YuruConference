#pragma once
#include "../IProcess.h"

class SinProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 1) return 1.17549435E-38f;
		return sin(_args[0]);
	}
};

HPE_REGISTER_PROCESS(SinProcess, "sin");




class CosProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 1) return 1.17549435E-38f;
		return cos(_args[0]);
	}
};

HPE_REGISTER_PROCESS(CosProcess, "cos");




class TanProcess : public IProcess
{
private:
	Argument Process(std::any& _owner, const std::vector<Argument>& _args) override {
		if (_args.size() < 1) return 1.17549435E-38f;
		return tan(_args[0]);
	}
};

HPE_REGISTER_PROCESS(TanProcess, "tan");


