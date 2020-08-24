/*----------------------------------------------------------------------------------
�v�f���܂Ƃ߂�Tuple namespace
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_TUPLE_HPP_
#define SGFRAMEWORK_HEADER_TUPLE_HPP_

//Framework namespace
namespace SGFramework
{
	//�v�f���܂Ƃ߂�Tuple namespace
	//���͓̂����N���X��Two ~ Five
	namespace Tuple
	{
		//2�̗v�f���܂Ƃ߂�Two class
		template<class E1, class E2>
		struct Two
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			inline Two(const E1& e1, const E2& e2) : e1(e1), e2(e2) {}

			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index) 
			{
				switch (index)
				{
				case 1:
					return e2;
				default:
					return e1;
				}
			}

			//size
			static constexpr uint cSize = 2;

			//default
			Two(Two&& move) = default;
			//default
			Two& operator =(const Two& copy) = default;
			//default
			Two& operator =(Two&& move) = default;

			E1 e1;	//�G�������g1
			E2 e2;	//�G�������g2
		};
		//2�̗v�f���܂Ƃ߂�Two class [Reference]
		template<class E1, class E2>
		struct Two<E1&, E2&>
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			inline Two(E1& e1r, E2& e2r) : e1r(e1r), e2r(e2r) {}
			
			//size
			static constexpr uint cSize = 2;

			//default
			Two(Two<E1&, E2&>&& move) = default;
			//default
			Two<E1&, E2&>& operator =(const Two<E1&, E2&>& copy) = default;
			//default
			Two<E1&, E2&>& operator =(Two<E1&, E2&>&& move) = default;

			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index)
			{
				switch (index)
				{
				case 1:
					return e2r;
				default:
					return e1r;
				}
			}

			E1& e1r;	//�G�������g1
			E2& e2r;	//�G�������g2
		};

		//3�̗v�f���܂Ƃ߂�Three class
		template<class E1, class E2, class E3>
		struct Three
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			//����3: �G�������g3
			inline Three(const E1& e1, const E2& e2, const E3& e3)
				: e1(e1), e2(e2), e3(e3) {}

			//size
			static constexpr uint cSize = 3;

			//default
			Three(Three&& move) = default;
			//default
			Three& operator =(const Three& copy) = default;
			//default
			Three& operator =(Three&& move) = default;

			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index)
			{
				switch (index)
				{
				case 1:
					return e2;
				case 2:
					return e3;
				default:
					return e1;
				}
			}

			E1 e1;	//�G�������g1
			E2 e2;	//�G�������g2
			E3 e3;	//�G�������g3
		};
		//3�̗v�f���܂Ƃ߂�Three class
		template<class E1, class E2, class E3>
		struct Three<E1&, E2&, E3&>
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			//����3: �G�������g3
			inline Three(E1& e1r, E2& e2r, E3& e3r)
				: e1r(e1r), e2r(e2r), e3r(e3r) {}

			//size
			static constexpr uint cSize = 3;

			//default
			Three(Three<E1&, E2&, E3&>&& move) = default;
			//default
			Three<E1&, E2&, E3&>& operator =(const Three<E1&, E2&, E3&>& copy) = default;
			//default
			Three<E1&, E2&, E3&>& operator =(Three<E1&, E2&, E3&>&& move) = default;

			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index)
			{
				switch (index)
				{
				case 1:
					return e2r;
				case 2:
					return e3r;
				default:
					return e1r;
				}
			}

			E1& e1r;	//�G�������g1
			E2& e2r;	//�G�������g2
			E3& e3r;	//�G�������g3
		};

		//4�̗v�f���܂Ƃ߂�For class
		template<class E1, class E2, class E3, class E4>
		struct For
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			//����3: �G�������g3
			//����4: �G�������g4
			inline For(const E1& e1, const E2& e2, const E3& e3, const E4& e4)
				: e1(e1), e2(e2), e3(e3), e4(e4) {}

			//size
			static constexpr uint cSize = 4;

			//default
			For(For&& move) = default;
			//default
			For& operator =(const For& copy) = default;
			//default
			For& operator =(For&& move) = default;

			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index)
			{
				switch (index)
				{
				case 1:
					return e2;
				case 2:
					return e3;
				case 3:
					return e4;
				default:
					return e1;
				}
			}

			E1 e1;	//�G�������g1
			E2 e2;	//�G�������g2
			E3 e3;	//�G�������g3
			E4 e4;	//�G�������g4
		};
		//4�̗v�f���܂Ƃ߂�For class
		template<class E1, class E2, class E3, class E4>
		struct For<E1&, E2&, E3&, E4&>
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			//����3: �G�������g3
			//����4: �G�������g4
			inline For(E1& e1r, E2& e2r, E3& e3r, E4& e4r)
				: e1r(e1r), e2r(e2r), e3r(e3r), e4r(e4r) {}

			//size
			static constexpr uint cSize = 4;

			//default
			For(For<E1&, E2&, E3&, E4&>&& move) = default;
			//default
			For<E1&, E2&, E3&, E4&>& operator =(const For<E1&, E2&, E3&, E4&>& copy) = default;
			//default
			For<E1&, E2&, E3&, E4&>& operator =(For<E1&, E2&, E3&, E4&>&& move) = default;

			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index)
			{
				switch (index)
				{
				case 1:
					return e2r;
				case 2:
					return e3r;
				case 3:
					return e4r;
				default:
					return e1r;
				}
			}

			E1& e1r;	//�G�������g1
			E2& e2r;	//�G�������g2
			E3& e3r;	//�G�������g3
			E4& e4r;	//�G�������g4
		};

		//5�̗v�f���܂Ƃ߂�Five class
		template<class E1, class E2, class E3, class E4, class E5>
		struct Five
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			//����3: �G�������g3
			//����4: �G�������g4
			//����5: �G�������g5
			inline Five(const E1& e1, const E2& e2, const E3& e3, const E4& e4, const E5& e5)
				: e1(e1), e2(e2), e3(e3), e4(e4), e5(e5) {}

			//size
			static constexpr uint cSize = 5;

			//default
			Five(Five&& move) = default;
			//default
			Five& operator =(const Five& copy) = default;
			//default
			Five& operator =(Five&& move) = default;

			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index)
			{
				switch (index)
				{
				case 1:
					return e2;
				case 2:
					return e3;
				case 3:
					return e4;
				case 4:
					return e5;
				default:
					return e1;
				}
			}

			E1 e1;	//�G�������g1
			E2 e2;	//�G�������g2
			E3 e3;	//�G�������g3
			E4 e4;	//�G�������g4
			E5 e5;	//�G�������g5
		};
		//5�̗v�f���܂Ƃ߂�Five class
		template<class E1, class E2, class E3, class E4, class E5>
		struct Five<E1&, E2&, E3&, E4&, E5&>
		{
			//----------------------------------------------------------------------------------
			//[�R���X�g���N�^]
			//������������s��
			//����1: �G�������g1
			//����2: �G�������g2
			//����3: �G�������g3
			//����4: �G�������g4
			//����5: �G�������g5
			inline Five(E1& e1r, E2& e2r, E3& e3r, E4& e4r, E5& e5r)
				: e1r(e1r), e2r(e2r), e3r(e3r), e4r(e4r), e5r(e5r) {}

			//size
			static constexpr uint cSize = 5;

			//default
			Five(Five<E1&, E2&, E3&, E4&, E5&>&& move) = default;
			//default
			Five<E1&, E2&, E3&, E4&, E5&>& operator =(const Five<E1&, E2&, E3&, E4&, E5&>& copy) = default;
			//default
			Five<E1&, E2&, E3&, E4&, E5&>& operator =(Five<E1&, E2&, E3&, E4&, E5&>&& move) = default;
			
			//[] operator return->element, index = out of range-> e1
			template <typename ElementType>
			ElementType& operator [] (uint index)
			{
				switch (index)
				{
				case 1:
					return e2r;
				case 2:
					return e3r;
				case 3:
					return e4r;
				case 4:
					return e5r;
				default:
					return e1r;
				}
			}

			E1& e1r;	//�G�������g1
			E2& e2r;	//�G�������g2
			E3& e3r;	//�G�������g3
			E4& e4r;	//�G�������g4
			E5& e5r;	//�G�������g5
		};

		//----------------------------------------------------------------------------------
		//[MakeTuple]
		//Tuple::Two���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		template<class E1, class E2>
		inline static Tuple::Two<E1, E2> MakeTuple(const E1& e1, const E2& e2)
		{
			return std::move(Tuple::Two<E1, E2>(e1, e2));
		}
		//----------------------------------------------------------------------------------
		//[MakeTuple]
		//Tuple::Three���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		//����3: �G�������g3
		template<class E1, class E2, class E3>
		inline static Tuple::Three<E1, E2, E3> MakeTuple(const E1& e1, const E2& e2, const E3& e3)
		{
			return std::move(Tuple::Three<E1, E2, E3>(e1, e2, e3));
		}
		//----------------------------------------------------------------------------------
		//[MakeTuple]
		//Tuple::For���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		//����3: �G�������g3
		//����4: �G�������g4
		template<class E1, class E2, class E3, class E4>
		inline static Tuple::For<E1, E2, E3, E4> MakeTuple(const E1& e1, const E2& e2, const E3& e3, const E4& e4)
		{
			return std::move(Tuple::For<E1, E2, E3, E4>(e1, e2, e3, e4));
		}
		//----------------------------------------------------------------------------------
		//[MakeTuple]
		//Tuple::Five���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		//����3: �G�������g3
		//����4: �G�������g4
		//����5: �G�������g5
		template<class E1, class E2, class E3, class E4, class E5>
		inline static Tuple::Five<E1, E2, E3, E4, E5> MakeTuple(const E1& e1, const E2& e2, const E3& e3, const E4& e4, const E5& e5)
		{
			return std::move(Tuple::Five<E1, E2, E3, E4, E5>(e1, e2, e3, e4, e5));
		}

		//----------------------------------------------------------------------------------
		//[MakeReferTuple]
		//�v�f���S�ĎQ�ƌ^��Tuple::Two���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		template<class E1, class E2>
		inline static Tuple::Two<E1&, E2&> MakeReferTuple(E1& e1r, E2& e2r)
		{
			return std::move(Tuple::Two<E1&, E2&>(e1r, e2r));
		}
		//----------------------------------------------------------------------------------
		//[MakeReferTuple]
		//�v�f���S�ĎQ�ƌ^��Tuple::Three���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		//����3: �G�������g3
		template<class E1, class E2, class E3>
		inline static Tuple::Three<E1&, E2&, E3&> MakeReferTuple(E1& e1r, E2& e2r, E3& e3r)
		{
			return std::move(Tuple::Three<E1&, E2&, E3&>(e1r, e2r, e3r));
		}
		//----------------------------------------------------------------------------------
		//[MakeReferTuple]
		//�v�f���S�ĎQ�ƌ^��Tuple::For���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		//����3: �G�������g3
		//����4: �G�������g4
		template<class E1, class E2, class E3, class E4>
		inline static Tuple::For<E1&, E2&, E3&, E4&> MakeReferTuple(E1& e1r, E2& e2r, E3& e3r, E4& e4r)
		{
			return std::move(Tuple::For<E1&, E2&, E3&, E4&>(e1r, e2r, e3r, e4r));
		}
		//----------------------------------------------------------------------------------
		//[MakeReferTuple]
		//�v�f���S�ĎQ�ƌ^��Tuple::Five���쐬����
		//return: �C���X�^���X
		//����1: �G�������g1
		//����2: �G�������g2
		//����3: �G�������g3
		//����4: �G�������g4
		//����5: �G�������g5
		template<class E1, class E2, class E3, class E4, class E5>
		inline static Tuple::Five<E1&, E2&, E3&, E4&, E5&> MakeReferTuple(E1& e1r, E2& e2r, E3& e3r, E4& e4r, E5& e5r)
		{
			return std::move(Tuple::Five<E1&, E2&, E3&, E4&, E5&>(e1r, e2r, e3r, e4r, e5r));
		}
	}
}
#endif // !SGFRAMEWORK_HEADER_TUPLE_HPP_
