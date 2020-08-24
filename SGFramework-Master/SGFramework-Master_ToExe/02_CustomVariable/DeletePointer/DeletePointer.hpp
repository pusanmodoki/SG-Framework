/*----------------------------------------------------------------------------------
�|�C���^��delete���邽�߂����ɍ쐬����DeletePointer.h
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_DELETE_POINTER_HPP_
#define SGFRAMEWORK_HEADER_DELETE_POINTER_HPP_
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <Windows.h>
#include <algorithm>

//Framework namespace
namespace SGFramework
{
	//�폜���邽�߂̊֐����i�[����namespace Deleting
	namespace Deleteing
	{
		//�v���g�^�C�v�錾
		//�P�̂��z�񂩑I��
		template<class T>
		inline void DeletePointer(T *& pt, const bool isArray);
		//�P�̔�
		template<class T>
		inline void DeletePointer(T *& pt);
		//�z���
		template<class T>
		inline void DeleteArrayPointer(T *& pt);
		//vector�P�̔�
		template<class T>
		inline void DeletePointer(std::vector<T*>& pt);
		//deque�P�̔�
		template<class T>
		inline void DeletePointer(std::deque<T*>& pt);
		//list�P�̔�
		template<class T>
		inline void DeletePointer(std::list<T*>& pt);
		//map�P�̔�
		template<class T>
		inline void DeletePointer(std::map<T, T*>& pt);

		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//�|�C���^�[��delete���ANULL��������
		//����1: �폜����|�C���^(�Q��)
		//����2: �z�񂩂ǂ���
		template<class T>
		inline void DeletePointer(T *& pt, const bool isArray)
		{
			//delete
			if (IS_FALSE(isArray))	delete pt;
			else							delete[] pt;
			pt = nullptr;		//NULL���
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//�|�C���^�[��delete���ANULL��������
		//���z��łȂ��ꍇ�݂̂̑Ή�, �z��̓I�[�o�[���[�h��DeleteArrayPointer
		//����1: �폜����|�C���^(�Q��)
		template<class T>
		inline void DeletePointer(T *& pt)
		{
			delete pt;			//delete
			pt = nullptr;		//NULL���
		}
		//----------------------------------------------------------------------------------
		//[DeleteArrayPointer]
		//�|�C���^�[��delete���ANULL��������
		//���z��̏ꍇ�݂̂̑Ή�, �z��łȂ��ꍇ��DeletePointer
		//����1: �폜����|�C���^(�Q��)
		template<class T>
		inline void DeleteArrayPointer(T *& pt)
		{
			delete[] pt;		//delete
			pt = nullptr;		//NULL���
		}

		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//�|�C���^�[��delete���A���X�g���N���A����
		//���v�f��new�����|�C���^�͒P�̂ł��邱��
		//����1: �폜����|�C���^���X�g(�Q��)
		template<class T>
		inline void DeletePointer(std::vector<T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//�|�C���^�[��delete���A���X�g���N���A����
		//���v�f��new�����|�C���^�͒P�̂ł��邱��
		//����1: �폜����|�C���^���X�g(�Q��)
		template<class T>
		void DeletePointer(std::deque<T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//�|�C���^�[��delete���A���X�g���N���A����
		//���v�f��new�����|�C���^�͒P�̂ł��邱��
		//����1: �폜����|�C���^���X�g(�Q��)
		template<class T>
		inline void DeletePointer(std::list<T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
		//----------------------------------------------------------------------------------
		//[DeletePointer]
		//�|�C���^�[��delete���A���X�g���N���A����
		//���v�f��new�����|�C���^�͒P�̂ł��邱��
		//����1: �폜����|�C���^���X�g(�Q��)
		template<class T>
		inline void DeletePointer(std::map<T, T*>& pt)
		{
			for (T*& t : pt) delete t;
			pt.clear();
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_DELETE_POINTER_HPP_