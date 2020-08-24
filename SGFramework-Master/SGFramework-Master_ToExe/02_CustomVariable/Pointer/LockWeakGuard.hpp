/*----------------------------------------------------------------------------------
�R���X�g���N�^��LockShared, �f�X�g���N�^��UnlockShared����
LockWeakGuard class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_LOCK_WEAK_GUARD_HPP_
#define SGFRAMEWORK_HEADER_LOCK_WEAK_GUARD_HPP_

//Framework namespace
namespace SGFramework
{
	//�R���X�g���N�^��LockShared, �f�X�g���N�^��UnlockShared����
	//LockWeakGuard class
	template <class Weak>
	class LockWeakGuard
	{
	public:
		//delete
		LockWeakGuard() = delete;
		LockWeakGuard(const LockWeakGuard& copy) = delete;
		LockWeakGuard(const LockWeakGuard&& move) = delete;
		LockWeakGuard& operator=(const LockWeakGuard& copy) = delete;
		LockWeakGuard& operator=(LockWeakGuard&& move) = delete;

		//----------------------------------------------------------------------------------
		//[�R���X�g���N�^]
		//LockShared���s��
		//����1: WeakPointer
		inline explicit LockWeakGuard(Weak& weak) : m_weak(weak)
		{
			m_isResult = m_weak.getIsLocked();
			if (IS_FALSE(m_isResult))
				m_isResult = m_weak.LockShared();
		}

		//----------------------------------------------------------------------------------
		//[�f�X�g���N�^]
		//UnlockShared���s��
		inline ~LockWeakGuard()
		{
			m_weak.UnlockShared();
		}

		//return is Locked
		inline bool getIsLocked() { return m_isResult; }
		//return is Locked
		inline operator bool() { return m_isResult; }
		//return is Not Locked
		inline bool operator !() { return !m_isResult; }

	private:
		Weak& m_weak;	//WeakPointer�̎Q��
		bool m_isResult;		//result
	};
}
#endif // !SGFRAMEWORK_HEADER_LOCK_WEAK_GUARD_HPP_