/*----------------------------------------------------------------------------------
std namespace��string�𑀍삷��֐���ǉ�����StdStringFunction.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_STD_STRING_FUNCTION_HPP_
#define SGFRAMEWORK_HEADER_STD_STRING_FUNCTION_HPP_
#include <vector>
#include <string>
#include <iterator>
#include <ostream>
#include <sstream>

//std�N���X�g��
namespace std_expansion
{
	//----------------------------------------------------------------------------------
	//[split]
	//string�ϐ����畡���̗v�f�𕪊�����
	//�����������ʂ�Ԃ�l�ŕԋp,�Q�Ɠn���ŕύX����I�[�o�[���[�h����
	//����1: ��������string
	//����2: ���������؂蕶��
	//����3: ���s���폜���邩
	inline std::vector<std::string> split(const std::string& splitString, char delimiter, bool isNewLineDelete)
	{
		std::vector<std::string> ret;					//�Ԃ�l
		std::string getString = splitString;			//��������string
		std::string buf;									//getline�p�̃o�b�t�@
		std::stringstream sStream(getString);	//getline�p��stream

		//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
		while (std::getline(sStream, buf, delimiter))
		{
			ret.emplace_back(buf);
		}

		return ret;
	}
	//----------------------------------------------------------------------------------
	//[split]
	//string�ϐ����畡���̗v�f�𕪊�����
	//�Q�Ɠn���̈���2�ɏ�������,�Ԃ�l�ɂ���I�[�o�[���[�h����
	//����1: ��������string
	//����2: ���ʂ���������string vector
	//����2: ���������؂蕶��
	//����3: ���s���폜���邩
	inline void split(const std::string& splitString, std::vector<std::string>& compString, char delimiter, bool isNewLineDelete)
	{
		std::string getString = splitString;		//��������string
		std::string buf;									//getline�p�̃o�b�t�@
		std::stringstream sStream(getString);	//getline�p��stream

		//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
		while (std::getline(sStream, buf, delimiter))
		{
			compString.emplace_back(buf);
		}
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vector�̕����̗v�f��1��string�A������
	//�����������ʂ�Ԃ�l�ŕԋp,�Q�Ɠn���ŕύX����I�[�o�[���[�h����
	//����1: �A������string vector
	//����2: �v�f���Ƃ̋�؂蕶��
	//����3: ���ʂ̍Ō�ɉ��s��ǉ����邩
	inline std::string join(std::vector<std::string>& joinString, char delimiter, bool isNewLine)
	{
		std::ostringstream oString;		//getline�p��stream
		//stream�ɗv�f + ��؂蕶���ŘA��
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::string>(oString, &delimiter));
		std::string ret = oString.str();	//�Ԃ�l�ɑ��

		//ret���v�f����Ȃ�Ō�̋�؂蕶���폜
		if (ret.size())		ret.erase(ret.size() - std::char_traits<char>::length(&delimiter));
		//ret���v�f���� & ���ʂ̍Ō�ɉ��s = true�Ȃ�Ō�ɉ��s�ǉ�
		if (ret.size() && isNewLine) ret.push_back('\n');
		return ret;
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vector�̕����̗v�f��1��string�A������
	//�Q�Ɠn���̈���2�ɏ�������,�Ԃ�l�ɂ���I�[�o�[���[�h����
	//����1: �A������string vector
	//����2: ���ʂ���������string
	//����3: �v�f���Ƃ̋�؂蕶��
	//����4: ���ʂ̍Ō�ɉ��s��ǉ����邩
	inline void join(std::vector<std::string>& joinString, std::string & compString,
		char delimiter, bool isNewLine)
	{
		std::ostringstream oString;		//getline�p��stream
		//stream�ɗv�f + ��؂蕶���ŘA��
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::string>(oString, &delimiter));
		//���ʑ���O�ɃT�C�Y����Ȃ�N���A
		if (compString.size())	compString.clear();
		compString = oString.str();		//���ʂ���

		//���ʂ��v�f����Ȃ�Ō�̋�؂蕶���폜
		if (compString.size())	compString.erase(compString.size() - std::char_traits<char>::length(&delimiter));
		//ret���v�f���� & ���ʂ̍Ō�ɉ��s = true�Ȃ�Ō�ɉ��s�ǉ�	
		if (compString.size() && isNewLine) compString.push_back('\n');
	}


	//----------------------------------------------------------------------------------
	//[split]
	//string�ϐ����畡���̗v�f�𕪊�����
	//�����������ʂ�Ԃ�l�ŕԋp,�Q�Ɠn���ŕύX����I�[�o�[���[�h����
	//����1: ��������string
	//����2: ���������؂蕶��
	//����3: ���s���폜���邩
	inline std::vector<std::wstring> split(const std::wstring& splitString, wchar_t delimiter, bool isNewLineDelete)
	{
		std::vector<std::wstring> ret;				//�Ԃ�l
		std::wstring getString = splitString;			//��������string
		std::wstring buf;									//getline�p�̃o�b�t�@
		std::wstringstream sStream(getString);	//getline�p��stream

		//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
		while (std::getline(sStream, buf, delimiter))
		{
			ret.emplace_back(buf);
		}

		return ret;
	}
	//----------------------------------------------------------------------------------
	//[split]
	//string�ϐ����畡���̗v�f�𕪊�����
	//�Q�Ɠn���̈���2�ɏ�������,�Ԃ�l�ɂ���I�[�o�[���[�h����
	//����1: ��������string
	//����2: ���ʂ���������string vector
	//����2: ���������؂蕶��
	//����3: ���s���폜���邩
	inline void split(const std::wstring& splitString, std::vector<std::wstring>& compString, wchar_t delimiter, bool isNewLineDelete)
	{
		std::wstring getString = splitString;			//��������string
		std::wstring buf;									//getline�p�̃o�b�t�@
		std::wstringstream sStream(getString);	//getline�p��stream

		//���s�폜 = true �ōŌ�̕��������s�Ȃ�폜
		if (isNewLineDelete && getString[getString.size() - 1] == '\n')
			getString[getString.size() - 1] = '\0';

		//��؂蕶�������Ƃɗv�f�擾���s���v�f��string vector�ɒǉ�
		while (std::getline(sStream, buf, delimiter))
		{
			compString.emplace_back(buf);
		}
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vector�̕����̗v�f��1��string�A������
	//�����������ʂ�Ԃ�l�ŕԋp,�Q�Ɠn���ŕύX����I�[�o�[���[�h����
	//����1: �A������string vector
	//����2: �v�f���Ƃ̋�؂蕶��
	//����3: ���ʂ̍Ō�ɉ��s��ǉ����邩
	inline std::wstring join(std::vector<std::wstring>& joinString, wchar_t delimiter, bool isNewLine)
	{
		std::wostringstream oString;		//getline�p��stream
		//stream�ɗv�f + ��؂蕶���ŘA��
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::wstring, wchar_t>(oString, &delimiter));
		std::wstring ret = oString.str();	//�Ԃ�l�ɑ��

		//ret���v�f����Ȃ�Ō�̋�؂蕶���폜
		if (ret.size())		ret.erase(ret.size() - std::char_traits<wchar_t>::length(&delimiter));
		//ret���v�f���� & ���ʂ̍Ō�ɉ��s = true�Ȃ�Ō�ɉ��s�ǉ�
		if (ret.size() && isNewLine) ret.push_back('\n');
		return ret;
	}
	//----------------------------------------------------------------------------------
	//[join]
	//string vector�̕����̗v�f��1��string�A������
	//�Q�Ɠn���̈���2�ɏ�������,�Ԃ�l�ɂ���I�[�o�[���[�h����
	//����1: �A������string vector
	//����2: ���ʂ���������string
	//����3: �v�f���Ƃ̋�؂蕶��
	//����4: ���ʂ̍Ō�ɉ��s��ǉ����邩
	inline void join(std::vector<std::wstring>& joinString, std::wstring & compString,
		wchar_t delimiter, bool isNewLine)
	{
		std::wostringstream oString;		//getline�p��stream
		//stream�ɗv�f + ��؂蕶���ŘA��
		std::copy(joinString.begin(), joinString.end(), std::ostream_iterator<std::wstring, wchar_t>(oString, &delimiter));
		//���ʑ���O�ɃT�C�Y����Ȃ�N���A
		if (compString.size())	compString.clear();
		compString = oString.str();		//���ʂ���

		//���ʂ��v�f����Ȃ�Ō�̋�؂蕶���폜
		if (compString.size())	compString.erase(compString.size() - std::char_traits<wchar_t>::length(&delimiter));
		//ret���v�f���� & ���ʂ̍Ō�ɉ��s = true�Ȃ�Ō�ɉ��s�ǉ�	
		if (compString.size() && isNewLine) compString.push_back('\n');
	}
}

#endif // !SGFRAMEWORK_HEADER_STD_STRING_TION_HPP_
