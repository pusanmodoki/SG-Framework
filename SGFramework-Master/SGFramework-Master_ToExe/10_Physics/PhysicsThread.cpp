#include "Physics.hpp"
#include "PhysicsFunction/PhysicsProcessingMessage.hpp"
#include "ContactInfo.hpp"
#include "RigidBody/RigidBody.hpp"
#include "RaycastHit/RaycastHit.hpp"

//Framework namespace
namespace SGFramework
{
	void Physics::PhysicsThread()
	{
		while (true)
		{
			m_startUpdateCondition.Wait();

			if (IS_TRUE(m_isShutDown.load())) return;

			AtomicOperation::LockAtomic(m_lockMessages);
			std::swap(m_messages, m_useThreadMessages);
			m_preferentRegisterInstanceIDs.clear();
			AtomicOperation::UnlockAtomic(m_lockMessages);

			////completed first update
			//if (IS_TRUE(m_isFirstUpdate))
			//{
			//	m_scene->fetchResults(true);
			//	BeginPhysics();
			//}

			for (auto& e : m_useThreadMessages)
			{
				ProcessingMessage(e);
				e.Release();
			}

			//Update
			for (auto& e : m_dynamicActors)
			{
				if (e.second.LockShared())
				{
					if (e.second->transform.LockShared())
					{
						e.second->UpdatePhysics();
						e.second->transform.UnlockShared();
					}
					e.second.UnlockShared();
				}
			}
			for (auto& e : m_colliders)
			{
				if (e.second.collider.LockShared())
				{
					if (e.second.collider->transform.LockShared())
					{
						e.second.collider->UpdatePhysics();
						e.second.collider->transform.UnlockShared();
					}
					e.second.collider.UnlockShared();
				}
			}

			m_useThreadMessages.clear();

			//start simulation
			m_scene->simulate(0.02f);
			//completed first update
			m_isFirstUpdate = true;
		
			m_endUpdateCondition.NotifyOne();
		}
	}

	

	void Physics::BeginPhysics()
	{		
		//lost pairs loop
		for (auto& e : m_lostPairs)
		{
			//lost pair->not destroy objects
			if (IS_FALSE(e.isObjectDestroy))
			{
				auto find0 = m_actors.find(e.pair.pairInstanceID0);
				auto find1 = m_actors.find(e.pair.pairInstanceID1);

				//calllback 0
				if (find0 != m_actors.end() && (e.pair.isEnabledCallback0 & find0->second.rigidBody.LockShared()))
				{
					ContactInfo info(&m_actors.at(e.pair.pairInstanceID0),
						&m_hitPairs.at(e.pair.hashID), true);

					m_actors.at(e.pair.pairInstanceID0).rigidBody->InvokeCallback(
						Component::CallbackFunctions::OnCollisionExit << (static_cast<uint>(e.pair.isTrigger0) * 3), info);

					find0->second.rigidBody.UnlockShared();
				}
				//callback 1
				if (find1 != m_actors.end() && (e.pair.isEnabledCallback1 & find1->second.rigidBody.LockShared()))
				{
					ContactInfo info(&m_actors.at(e.pair.pairInstanceID1),
						&m_hitPairs.at(e.pair.hashID), false);

					m_actors.at(e.pair.pairInstanceID1).rigidBody->InvokeCallback(
						Component::CallbackFunctions::OnCollisionExit << (static_cast<uint>(e.pair.isTrigger1) * 3), info);

					find1->second.rigidBody.UnlockShared();
				}
			}
			//lost pair->destroy objects
			else
			{
				auto find0 = m_actors.find(e.pair.pairInstanceID0);
				auto find1 = m_actors.find(e.pair.pairInstanceID1);

				//calllback 0
				if (find0 != m_actors.end() && (e.pair.isEnabledCallback0 & find0->second.rigidBody.LockShared()))
				{
					//find hit pair
					bool isFindPair = m_hitPairs.find(e.pair.hashID) != m_hitPairs.end();
					//create info
					ContactInfo info(nullptr,
						IS_TRUE(isFindPair) ? &m_hitPairs.at(e.pair.hashID) : nullptr, true,
						IS_TRUE(isFindPair) ? m_hitPairs.at(e.pair.hashID).collider0 : WeakPointer<BaseClass::BaseCollider>());

					m_actors.at(e.pair.pairInstanceID0).rigidBody->InvokeCallback(
						Component::CallbackFunctions::OnCollisionExit << (static_cast<uint>(e.pair.isTrigger0) * 3), info);

					find0->second.rigidBody.UnlockShared();
				}
				//callback 1
				if (find1 != m_actors.end() && (e.pair.isEnabledCallback1 & find1->second.rigidBody.LockShared()))
				{
					//find hit pair
					bool isFindPair = m_hitPairs.find(e.pair.hashID) != m_hitPairs.end();
					//create info
					ContactInfo info(nullptr,
						IS_TRUE(isFindPair) ? &m_hitPairs.at(e.pair.hashID) : nullptr, false,
						IS_TRUE(isFindPair) ? m_hitPairs.at(e.pair.hashID).collider1 : WeakPointer<BaseClass::BaseCollider>());

					m_actors.at(e.pair.pairInstanceID1).rigidBody->InvokeCallback(
						Component::CallbackFunctions::OnCollisionExit << (static_cast<uint>(e.pair.isTrigger1) * 3), info);

					find1->second.rigidBody.UnlockShared();
				}
			}

			//removed hit pair? -> remove
			if (IS_TRUE(e.isRemovedHitPairs))
				m_hitPairs.erase(e.pair.hashID);
			//find found pairs -> remove
			else
				for (uint i = 0, max = static_cast<uint>(m_foundPairs.size()); i < max; ++i)
				{
					if (m_foundPairs[i].hashID == e.pair.hashID)
					{
						m_foundPairs.erase(m_foundPairs.begin() + i);
						break;
					}
				}
		}

		//hit pairs loop
		for (auto& e : m_hitPairs)
		{
			e.second.FindWeaks();

			//callback 0
			if (e.second.isEnabledCallback0 & m_actors.at(e.second.pairInstanceID0).rigidBody.LockShared())
			{
				ContactInfo info(&m_actors.at(e.second.pairInstanceID0), &e.second, true);

				m_actors.at(e.second.pairInstanceID0).rigidBody->InvokeCallback(
					Component::CallbackFunctions::OnCollisionStay << (static_cast<uint>(e.second.isTrigger0) * 3), info);
				
				m_actors.at(e.second.pairInstanceID0).rigidBody.UnlockShared();
			}

			//callback 1
			if (e.second.isEnabledCallback1 & m_actors.at(e.second.pairInstanceID1).rigidBody.LockShared())
			{
				ContactInfo info(&m_actors.at(e.second.pairInstanceID1), &e.second, false);

				m_actors.at(e.second.pairInstanceID1).rigidBody->InvokeCallback(
					Component::CallbackFunctions::OnCollisionStay << (static_cast<uint>(e.second.isTrigger1) * 3), info);
			
				m_actors.at(e.second.pairInstanceID1).rigidBody.UnlockShared();
			}
		}

		//found pairs loop
		for (auto& e : m_foundPairs)
		{
			auto find0 = m_actors.find(e.pairInstanceID0);
			auto find1 = m_actors.find(e.pairInstanceID1);

			if (find0 == m_actors.end() || find1 == m_actors.end())
				continue;

			//emplace hit pair
			m_hitPairs.try_emplace(e.hashID, e);

			//callback 0
			if (e.isEnabledCallback0 & find0->second.rigidBody.LockShared())
			{
				ContactInfo info(&find0->second, &e, true);

				find0->second.rigidBody->InvokeCallback(
					Component::CallbackFunctions::OnCollisionEnter << (static_cast<uint>(e.isTrigger0) * 3), info);
			
				find0->second.rigidBody.UnlockShared();
			}

			//callback 1
			if (e.isEnabledCallback1 & find1->second.rigidBody.LockShared())
			{
				ContactInfo info(&find1->second, &e, false);

				find1->second.rigidBody->InvokeCallback(
					Component::CallbackFunctions::OnCollisionEnter << (static_cast<uint>(e.isTrigger1) * 3), info);
			
				find1->second.rigidBody.UnlockShared();
			}
		}


		//call back
		Transform::InvokeCallbackForSingleThread(Component::CallbackFunctions::FixedUpdate);
		//Thread::
		//Thread::Invok(Component::CallbackFunctions::FixedUpdate);


		//clear vectors
		m_foundPairs.clear();
		m_lostPairs.clear();
	}
}