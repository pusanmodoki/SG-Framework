/*----------------------------------------------------------------------------------
コンストラクタでLockShared, デストラクタでUnlockSharedする
LockWeakGuard class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_LOCK_WEAK_GUARD_HPP_
#define SGFRAMEWORK_HEADER_LOCK_WEAK_GUARD_HPP_

//Framework namespace
namespace SGFramework
{
	//コンストラクタでLockShared, デストラクタでUnlockSharedする
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
		//[コンストラクタ]
		//LockSharedを行う
		//引数1: WeakPointer
		inline explicit LockWeakGuard(Weak& weak) : m_weak(weak)
		{
			m_isResult = m_weak.getIsLocked();
			if (IS_FALSE(m_isResult))
				m_isResult = m_weak.LockShared();
		}

		//----------------------------------------------------------------------------------
		//[デストラクタ]
		//UnlockSharedを行う
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
		Weak& m_weak;	//WeakPointerの参照
		bool m_isResult;		//result
	};
}
#endif // !SGFRAMEWORK_HEADER_LOCK_WEAK_GUARD_HPP_