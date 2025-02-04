#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <tuple>
#include <functional>

// SerializeField���g�p�ł���悤�ɂ��邽�߂̊��N���X
class Serializable {
	// �o�^�p�N���X���炾���ύX�ł���悤�ɂ���
	friend class SerializableRegistrar;
public:
	virtual ~Serializable() = default;

	void Func() {
		for (auto& member : m_member) {
			// �^�̃n�b�V���l�ɉ�����m_procs�ɓo�^�����֐����Ăяo��
			size_t& hash = std::get<0>(member);
			if (m_procs.find(hash) != m_procs.end()) {
				m_procs[hash](std::get<1>(member), std::get<2>(member));
			}
		}
	}

private:
	// �^�̃n�b�V���l�ɑΉ������֐���o�^���郁���o�ϐ�
	static inline std::unordered_map<size_t, std::function<void(std::string&, void*)>> m_procs =
	// �����ł�std::string��int�ɑ΂��ĒP�ɒl���o�͂���R�[�h���������œo�^
	{
		{
			typeid(std::string).hash_code(),
			[](std::string& _name,void* _ptr) {
				// *((std::string*)_ptr) ������̃L���X�g���͂���߂���ɕ|���B
				std::cout << "string" << ":\t" << _name << " = " << *((std::string*)_ptr) << std::endl;
			}
		},
		{
			typeid(int).hash_code(),
			[](std::string& _name,void* _ptr) {
				std::cout << "int" << ":\t" << _name << " = " << *((int*)_ptr) << std::endl;
			}
		}
	};

	// �N���X�̃n�b�V���A�ϐ����A�ϐ��ւ̎Q�Ƃ�ێ�
	std::vector<std::tuple<size_t, std::string, void*>> m_member
		= std::vector<std::tuple<size_t, std::string, void*>>();
};



// Serializable�ɓo�^���邽�߂̃N���X
class SerializableRegistrar {
public:
	// �R���X�g���N�^��m_member�ɓo�^����
	SerializableRegistrar(Serializable* _owner, const type_info& _type, const char* _name, void* _ptr) {
		_owner->m_member.emplace_back(_type.hash_code(), _name, _ptr);
	}
	~SerializableRegistrar() = default;
};



// �}�N��
#define SerializeField(type, name, initial_value) \
type name = initial_value; \
const SerializableRegistrar SR##name##Registrar = SerializableRegistrar(this, typeid(type), #name, (void*)&name);
/*
�W�J����Ƃ���Ȋ���
���̕ϐ��ɉ�����SerializableRegistrar��ǉ��Œ�`����悤�ɂ��邱�Ƃ�m_member�ɓo�^�����悤�ɂ���B

SerializeField(int, m_hoge, 12345);
��
int m_hoge = 12345;
const SerializableRegistrar SRm_hogeRegistrar = SerializableRegistrar(this, typeid(int), "m_hoge", (void*)&m_hoge);
*/
