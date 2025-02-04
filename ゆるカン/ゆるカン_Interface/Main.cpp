/*
C++�̃C���^�[�t�F�[�X�Ƃ́H
C++�ɂ�����C���^�[�t�F�[�X�Ƃ́A�h���N���X�ɓ���̋@�\��񋟂��邽�߂̒��ۓI�Ȋ��N���X�̂��Ƃ��w���܂��B
��� �������z�֐��ipure virtual function�j ���g���Ď�������A�I�u�W�F�N�g�̃|�����[�t�B�Y���i���Ԑ��j���������܂��B

�C���^�[�t�F�[�X�̓���
	�������z�֐��݂̂�����
		virtual �L�[���[�h�� = 0 ���g���Ē�`����B
	�C���X�^���X���ł��Ȃ�
		�C���^�[�t�F�[�X���̂͋�̓I�Ȏ����������Ȃ����߁A���ڃI�u�W�F�N�g�����Ȃ��B
	�|�����[�t�B�Y���������ł���
		�قȂ�N���X�������C���^�[�t�F�[�X���������邱�ƂŁA��т����g�������ł���B
By ChatGPT

���̃T���v���ł́A�ŋ߂��C�ɓ���́A
�V�X�e���̃T�|�[�g��񋟂��Ă����Interface�̃N���X�݌v���ȒP�ɏ������R�[�h�B
*/


#include "Sample.h"

// public�p�����ċ@�\���T�|�[�g����
class Object : public Interface {
public:
	Object() = default;
	Object(const char* _text) : m_text(_text) {};
	~Object() override = default;

	// Interface�ɂ�����������
	virtual SomeData GetSomeData() override {
		// ������SomeData�ɉ������ēn�����̓N���X���ƂɎ��R
		// �����ł̓����o�ϐ��Ŏ����Ă���m_text��n�����Ƃɂ���
		SomeData data = {};
		data.text = m_text;
		return data;
	}

private:
	std::string m_text;
};


int main(void) {
	System sys;
	sys.Init();

	// Obj1,2��ǉ�
	std::cout << "A��B��ǉ�" << std::endl;
	Object* obj1 = new Object("A");
	Object* obj2 = new Object("B");
	sys.Func();
	getchar();

	// Obj3��ǉ�
	std::cout << "C��ǉ�" << std::endl;
	Object* obj3 = new Object("C");
	sys.Func();
	getchar();

	// Obj2���폜
	std::cout << "B���폜" << std::endl;
	delete obj2;
	sys.Func();
	getchar();

	// Obj3���폜
	std::cout << "C���폜" << std::endl;
	delete obj3;
	sys.Func();
	getchar();

	delete obj1;

	return 0;
}