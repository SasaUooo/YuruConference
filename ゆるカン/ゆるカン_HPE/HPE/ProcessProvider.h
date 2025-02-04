#pragma once
#include <unordered_map>
#include <functional>
#include <memory>
#include <string>
class IProcess;

class ProcessProvider
{
public:
	static inline void Register(const std::string& _funcName, std::unique_ptr<IProcess> _uniqueInstance);
	static inline bool IsRegistered(const std::string& _funcName);

	/**
	* @memo	返すポインタは受け取り側が解放すること。
	*/
	[[nodiscard]] static inline IProcess* GetInstance(const std::string& _funcName);

private:
	static inline std::unordered_map<std::string, std::unique_ptr<IProcess>> m_processInstanceMap =
		std::unordered_map<std::string, std::unique_ptr<IProcess>>();
};


template<typename T>
concept Process = std::is_base_of_v<IProcess, T>() && std::is_default_constructible_v<T>();

template <typename Process>
class ProcessRegistrar {
public:
	ProcessRegistrar(const std::string& _funcName) {
		auto ptr = std::unique_ptr<Process>(new Process);
		ProcessProvider::Register(_funcName, std::move(ptr));
	}

	~ProcessRegistrar() = default;
};



/* ----------------------------------------------------------------- */
/* INLINE */
/* ----------------------------------------------------------------- */

void ProcessProvider::Register(const std::string& _funcName, std::unique_ptr<IProcess> _uniqueInstance)
{
	m_processInstanceMap.emplace(_funcName, std::move(_uniqueInstance));
}

bool ProcessProvider::IsRegistered(const std::string& _funcName)
{
	return m_processInstanceMap.contains(_funcName);
}

IProcess* ProcessProvider::GetInstance(const std::string& _funcName)
{
	return m_processInstanceMap[_funcName].get();
}

