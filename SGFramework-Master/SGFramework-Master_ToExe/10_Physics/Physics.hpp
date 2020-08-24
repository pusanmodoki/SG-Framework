/*----------------------------------------------------------------------------------
物理エンジン(PhysX)を管理するPhysics class
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PHYSICS_HPP_
#define SGFRAMEWORK_HEADER_PHYSICS_HPP_
#include "PhysXInclude.hpp"
#include "PhysicsMaterial.hpp"
#include "../02_CustomVariable/CustomVariable.hpp"
#include "../05_Graphics/TransformUsing.hpp"
#include "Detail/PhysicsMessage.hpp"

//Framework namespace
namespace SGFramework
{
	//Base Classes
	namespace BaseClass
	{
		class BaseRigidBody;
		class BaseCollider;
	}

	struct RaycastHit;
	struct OverlapHit;
	struct SweepHit;

	//物理エンジン(PhysX)を管理するPhysics class
	class Physics final
	{
	private:
		//prototype
		struct HitPair;

	public:
		//friend
		friend class Application;
		friend struct ContactInfo;
		friend class BaseClass::BaseRigidBody;
		friend class BaseClass::BaseCollider;
		friend class RigidBodyStatic;
		friend class RigidBodyDynamic;
		friend class BoxCollider;
		friend class CapsuleCollider;
		friend class MeshCollider;
		friend class PlaneCollider;
		friend class SphereCollider;

		DELETE_ALL_DEFAULT_CLASS_FUNCTIONS(Physics);

		//LockGuard Physics Scene
		class LockGuardPhysics
		{
		public:
			inline LockGuardPhysics(bool isWrite) : isWrite(isWrite)
			{
				if (IS_TRUE(isWrite)) Physics::m_scene->lockWrite();
				else Physics::m_scene->lockRead();
			}
			inline ~LockGuardPhysics()
			{
				if (IS_TRUE(isWrite)) Physics::m_scene->unlockWrite();
				else Physics::m_scene->unlockRead();
			}

		private:
			bool isWrite;
		};


		//----------------------------------------------------------------------------------
		//[Raycast]
		//Rayで当たり判定を行う
		//引数1: 原点
		//引数2: 方向
		//引数3: 距離, default = infinity
		//引数4: RaycastHit, nullptrでない場合はHit情報を代入する, default = nullptr
		//引数4: LayerMask, default = all layer mask
		//引数5: true = rayに当たった情報を全て返却
		static bool Raycast(const Vector3& origin, const Vector3& direction, float distance = MathAF::cInfinity,
			RaycastHit* raycastHit = nullptr, const LayerMask& layerMask = LayerMask::getAllLayersMask(), bool isMultiHit = false);

		//----------------------------------------------------------------------------------
		//[BoxCast]
		//BoxRayで当たり判定を行う
		//引数1: 原点
		//引数2: 方向
		//引数3: サイズ
		//引数4: 回転
		//引数5: 距離, default = infinity
		//引数6: SweepHit, nullptrでない場合はHit情報を代入する, default = nullptr
		//引数7: LayerMask, default = all layer mask
		static bool BoxCast(const Vector3& origin, const Vector3& direction, const Vector3& size, const Quaternion& rotation,
			float distance = MathAF::cInfinity, SweepHit* hit = nullptr, const LayerMask& layerMask = LayerMask::getAllLayersMask());
		//----------------------------------------------------------------------------------
		//[SphereCast]
		//SphereRayで当たり判定を行う
		//引数1: 原点
		//引数2: 方向
		//引数3: 半径
		//引数4: 距離, default = infinity
		//引数5: SweepHit, nullptrでない場合はHit情報を代入する, default = nullptr
		//引数6: LayerMask, default = all layer mask
		static bool SphereCast(const Vector3& origin, const Vector3& direction, float radius,
			float distance = MathAF::cInfinity, SweepHit* hit = nullptr, const LayerMask& layerMask = LayerMask::getAllLayersMask());
		//----------------------------------------------------------------------------------
		//[CapsuleCast]
		//CapsuleRayで当たり判定を行う
		//引数1: 原点
		//引数2: 方向
		//引数3: 半径
		//引数4: 高さ
		//引数5: 回転
		//引数6: 距離, default = infinity
		//引数7: SweepHit, nullptrでない場合はHit情報を代入する, default = nullptr
		//引数8: LayerMask, default = all layer mask
		static bool CapsuleCast(const Vector3& origin, const Vector3& direction, float radius, float height, const Quaternion& rotation,
			float distance = MathAF::cInfinity, SweepHit* hit = nullptr, const LayerMask& layerMask = LayerMask::getAllLayersMask());

		//----------------------------------------------------------------------------------
		//[OverlapBox]
		//Boxにオブジェクトが接触しているか確認する
		//引数1: 座標
		//引数2: 回転
		//引数3: サイズ
		//引数4: OverlapHit, nullptrでない場合はHit情報を代入する, default = nullptr
		//引数5: LayerMask, default = all layer mask
		static bool OverlapBox(const Vector3& position, const Quaternion& rotation, 
			const Vector3& size, OverlapHit* hit = nullptr, const LayerMask& layerMask = LayerMask::getAllLayersMask());
		//----------------------------------------------------------------------------------
		//[OverlapSphere]
		//Sphereにオブジェクトが接触しているか確認する
		//引数1: 座標
		//引数2: radius
		//引数3: OverlapHit, nullptrでない場合はHit情報を代入する, default = nullptr
		//引数4: LayerMask, default = all layer mask
		static bool OverlapSphere(const Vector3& position, float radius, 
			OverlapHit* hit = nullptr, const LayerMask& layerMask = LayerMask::getAllLayersMask());
		//----------------------------------------------------------------------------------
		//[OverlapCapsule]
		//Capsuleにオブジェクトが接触しているか確認する
		//引数1: 座標
		//引数2: 回転
		//引数3: radius
		//引数4: height
		//引数5: OverlapHit, nullptrでない場合はHit情報を代入する, default = nullptr
		//引数6: LayerMask, default = all layer mask
		static bool OverlapCapsule(const Vector3& position, const Quaternion& rotation,
			float radius, float height, OverlapHit* hit = nullptr, const LayerMask& layerMask = LayerMask::getAllLayersMask());

		//physics gravity (read function property)				
		SGF_FUNCTION_PROPERTY static Vector3 getGravity() { Physics::LockGuardPhysics guard(false); m_scene->getGravity(); }
		//physics gravity (set function property)
		//argument 1: set value 
		SGF_FUNCTION_PROPERTY static Vector3 setGravity(const Vector3& set) { Physics::LockGuardPhysics guard(true); m_scene->setGravity(set.ToPhysX()); }
		

		//All LayerMask Value
		static constexpr uint cAllLayers = VariableLimit::cUintMax;
		//PhysX->PxPhysics
		static PointerProperty<physx::PxPhysics*> physicsFactory;
		//PhysX->PxScene
		static PointerProperty<physx::PxScene*> physicsScene;
		//PhysX->PxControllerManager
		static PointerProperty<physx::PxControllerManager*> controllerManager;
		//Default PhysicsMaterial (staticFriction = 0.5f, dynamicFriction = 0.5f, restitution(反発係数) = 0.1f)
		static GetOnlyProperty<PhysicsMaterial> defaultPhysicsMaterial;

	private:
		//Collision Filter Data
		//world0 = this layer
		//world1 = layerMask
		//world2 = used callback = 1
		//world3 = transform instance id
		//Query Filter Data
		//world0 = layerMask
		//world1 = exclude instance id

		//Actor情報を保存するActorInfo
		struct ActorInfo
		{
			inline ActorInfo(const WeakPointer<Transform>& transform, const WeakPointer<BaseClass::BaseRigidBody>& rigidBody,
				bool isDynamic) : transform(transform), rigidBody(rigidBody), isDynamic(isDynamic) {}
		
			inline ActorInfo(const WeakPointer<Transform>& transform, const WeakPointer<BaseClass::BaseRigidBody>& rigidBody,
				physx::PxRigidActor* actor, physx::PxRigidDynamic* actorDynamic, bool isDynamic) : transform(transform), 
				rigidBody(rigidBody), actor(actor),  actorDynamic(actorDynamic), isDynamic(isDynamic) {}

			//transform
			WeakPointer<Transform> transform;
			//rigid body
			WeakPointer<BaseClass::BaseRigidBody> rigidBody;
			//actor pointer
			physx::PxRigidActor* actor;
			//actor pointer (dynamic)
			physx::PxRigidDynamic* actorDynamic;
			//is dynamic rigid body?
			bool isDynamic;
		};

		//Collider情報を保存するColliderInfo
		struct ColliderInfo
		{
			inline ColliderInfo(const WeakPointer<BaseClass::BaseCollider>& collider, physx::PxShape* shape) 
				: collider(collider), shape(shape) {}

			//collider
			WeakPointer<BaseClass::BaseCollider> collider;
			//actor pointer
			physx::PxShape* shape;
		};

		//collider hit pairを保存するHitPair
		struct HitPair
		{
			inline HitPair(uint64 hashID, 
				void* userData0, void* userData1,
				uint instanceID0, uint instanceID1,	bool isTrigger0, 
				bool isTrigger1, bool isEnabledCallback0, bool isEnabledCallback1) 
				: hashID(hashID), collider0(), collider1(),
				m_colliderInstanceID0(static_cast<uint>(reinterpret_cast<size_t>(userData0))), 
				m_colliderInstanceID1(static_cast<uint>(reinterpret_cast<size_t>(userData1))),
				pairInstanceID0(instanceID0), pairInstanceID1(instanceID1),
				isTrigger0(isTrigger0), isTrigger1(isTrigger1) ,
				isEnabledCallback0(isEnabledCallback0), isEnabledCallback1(isEnabledCallback1) {}

			//Hash用uint64生成
			inline static uint64 CalculateHashSeed(uint instanceID0, uint instanceID1)
			{
				return instanceID0 >= instanceID1 ? ((static_cast<uint64>(instanceID0) << 32) | static_cast<uint64>(instanceID1))
					: ((static_cast<uint64>(instanceID1) << 32) | static_cast<uint64>(instanceID0));
			}

			//Copy
			inline const HitPair& operator = (const HitPair& copy)
			{
				//const cast
				*const_cast<uint64*>(&this->hashID) = copy.hashID;
				
				this->collider0 = copy.collider0;
				this->collider1 = copy.collider1;
				this->pairInstanceID0 = copy.pairInstanceID0;
				this->pairInstanceID1 = copy.pairInstanceID1;
				this->isTrigger0 = copy.isTrigger0;
				this->isTrigger1 = copy.isTrigger1;
				this->isEnabledCallback0 = copy.isEnabledCallback0;
				this->isEnabledCallback1 = copy.isEnabledCallback1;
				return *this;
			}

			//Found weaks
			inline void FindWeaks()
			{
				auto find = Physics::m_colliders.find(m_colliderInstanceID0);
				if (find != Physics::m_colliders.end())
					collider0 = find->second.collider;
				
				find = Physics::m_colliders.find(m_colliderInstanceID1);
				if (find != Physics::m_colliders.end())
					collider1 = find->second.collider;
			}

			//hash id
			const uint64 hashID;
			//collider namber0
			WeakPointer<BaseClass::BaseCollider> collider0;
			//collider namber1
			WeakPointer<BaseClass::BaseCollider> collider1;
			//instance id namber0
			uint pairInstanceID0;
			//instance id namber1
			uint pairInstanceID1;
			//isTrigger namber0
			bool isTrigger0;
			//isTrigger namber1
			bool isTrigger1;
			//isEnabledCallback namber0
			bool isEnabledCallback0;
			//isEnabledCallback namber1
			bool isEnabledCallback1;

		private:
			//collider instance id namber0
			uint m_colliderInstanceID0;
			//collider instance id namber1
			uint m_colliderInstanceID1;
		};

		//collider lost pairを保存するLostPair
		struct LostPair
		{
			inline LostPair(const HitPair& pair, bool isObjectDestroy, bool isRemovedHitPairs)
				: pair(pair), isObjectDestroy(isObjectDestroy), isRemovedHitPairs(isRemovedHitPairs) {}

			//Hit Pair
			HitPair pair;
			//object destroy?
			bool isObjectDestroy;
			//remove m_hitPairs?
			bool isRemovedHitPairs;
		};

		//SimulationFilterCallback
		class SimulationCallback : public physx::PxSimulationFilterCallback
		{
			//pair found callback
			inline physx::PxFilterFlags pairFound(physx::PxU32 pairID,
				physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, const physx::PxActor* a0, const physx::PxShape* s0,
				physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, const physx::PxActor* a1, const physx::PxShape* s1,
				physx::PxPairFlags& pairFlags);
			//pair lost callback
			inline void pairLost(physx::PxU32 pairID,
				physx::PxFilterObjectAttributes attributes0,
				physx::PxFilterData filterData0,
				physx::PxFilterObjectAttributes attributes1,
				physx::PxFilterData filterData1,
				bool objectRemoved);
			//not use
			inline bool statusChange(physx::PxU32& pairID, physx::PxPairFlags& pairFlags, physx::PxFilterFlags& filterFlags) { return false; }
		};

		//SimulationEventCallback
		class SimulationEventCallback : public physx::PxSimulationEventCallback
		{
			//空実装
			inline void onConstraintBreak(physx::PxConstraintInfo *constraints, physx::PxU32 count) override {}
			inline void onWake(physx::PxActor **actors, physx::PxU32 count) override {}
			inline void onSleep(physx::PxActor **actors, physx::PxU32 count) override {}
			inline void onContact(const physx::PxContactPairHeader &pairHeader, const physx::PxContactPair *pairs, physx::PxU32 nbPairs) override {}
			inline void 	onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count) override;
			inline void 	onAdvance(const physx::PxRigidBody *const *bodyBuffer, const physx::PxTransform *poseBuffer, const physx::PxU32 count) override {}

			//pair lost callback
			inline void pairLost(physx::PxU32 pairID,
				physx::PxFilterObjectAttributes attributes0,
				physx::PxFilterData filterData0,
				physx::PxFilterObjectAttributes attributes1,
				physx::PxFilterData filterData1,
				bool objectRemoved) {}
			//not use
			inline bool statusChange(physx::PxU32& pairID, physx::PxPairFlags& pairFlags, physx::PxFilterFlags& filterFlags) { return false; }
		};

		//QueryFilterCallback
		class QueryCallback : public physx::PxQueryFilterCallback
		{
			//pre filter
			inline physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData &filterData,
				const physx::PxShape *shape, const physx::PxRigidActor *actor, physx::PxHitFlags &queryFlags);
			//post filter
			inline physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData &filterData, const physx::PxQueryHit &hit);
		};

		
		//----------------------------------------------------------------------------------
		//[StartUp]
		//throw: failed start up
		//引数1: gravity
		//引数2: HitPair->Init Num Reserve
		static void StartUp(const ReadElement::Pack& gravity, const ReadElement::Pack& hitPairVectorReserves);
		//----------------------------------------------------------------------------------
		//[ShutDown]
		static void ShutDown();
		
		static void PhysicsThread();

		static void SyncPhysicsBegin();
		static void UpdatePhysics();
		static void SyncPhysicsEnd();



		static void BeginPhysics();

		//----------------------------------------------------------------------------------
		//[ProcessingMessage]
		//メッセージを処理する
		//引数1: message
		inline static void ProcessingMessage(const Detail::Physics::PhysicsMessage& message);
		//----------------------------------------------------------------------------------
		//[ProcessingRigidBodyDynamicMessage]
		//メッセージを処理する (RigidBodyDynamic)
		//引数1: message
		//引数1: message type
		inline static void ProcessingRigidBodyDynamicMessage(const Detail::Physics::PhysicsMessage& message, uint messageType);
		//----------------------------------------------------------------------------------
		//[ProcessingPointerMessage]
		//メッセージを処理する (Pointer)
		//引数1: message
		//引数1: message type
		inline static void ProcessingPointerMessage(const Detail::Physics::PhysicsMessage& message, uint messageType);
		//----------------------------------------------------------------------------------
		//[ProcessingOtherMessage]
		//メッセージを処理する (Other)
		//引数1: message
		//引数1: message type
		inline static void ProcessingOtherMessage(const Detail::Physics::PhysicsMessage& message, uint messageType);

		//----------------------------------------------------------------------------------
		//[FilterShader]
		//Collision Filter
		inline static physx::PxFilterFlags FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
			physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
			physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);


		inline static void SendPhysicsMessage(const Detail::Physics::PhysicsMessage& message)
		{
			auto type = message.type;
			
			AtomicOperation::LockAtomic(m_lockMessages);

			if (type & Detail::Physics::MessageType::PointerMessageFlag)
			{
				type ^= Detail::Physics::MessageType::PointerMessageFlag;

				if (type & Detail::Physics::MessageType::UnregisterCollider
					|| type & Detail::Physics::MessageType::UnregisterRigidBody)
				{
					if (m_preferentRegisterInstanceIDs.find(message.instanceID)
						!= m_preferentRegisterInstanceIDs.end())
					{
						m_preferentRegisterInstanceIDs.erase(message.instanceID);

						for (size_t i = 0, max = m_preferentMessages.size(); i < max; ++i)
						{
							if (m_preferentMessages[i].instanceID == message.instanceID)
							{
								m_preferentMessages[i].Release();
								m_preferentMessages.erase(m_preferentMessages.begin() + i);
								break;
							}
						}
					}
					else
					{
						m_preferentMessages.emplace_back(message);
						m_preferentRegisterInstanceIDs.emplace(message.instanceID);
					}
				}
				else if (type & Detail::Physics::MessageType::RegisterColliderCL
					|| type & Detail::Physics::MessageType::RegisterRigidBodyRG)
				{
					m_preferentMessages.emplace_back(message);
					m_preferentRegisterInstanceIDs.emplace(message.instanceID);
				}
			}
			else
				m_messages.emplace_back(message);

			AtomicOperation::UnlockAtomic(m_lockMessages);
		}

		
		//dynamic actor map
		static std::unordered_map<uint, WeakPointer<BaseClass::BaseRigidBody>&> m_dynamicActors1;
		//static actor map
		static std::unordered_map<uint, WeakPointer<BaseClass::BaseRigidBody>&> m_staticActors;
		//shape map
		static std::unordered_set<BaseClass::BaseCollider*> m_shapes;
		//remove vector
		static std::vector<physx::PxRigidActor*> m_removeActors;
		//remove vector
		static std::vector<WeakPointer<BaseClass::BaseCollider>> m_removeColliders;
		//remove vector
		static std::vector<physx::PxShape*> m_removeShapes;




		//actors map
		static std::unordered_map<uint, ActorInfo> m_actors;
		//dynamic actor map
		static std::unordered_map<uint, WeakPointer<BaseClass::BaseRigidBody>> m_dynamicActors;
		//actors map
		static std::unordered_map<uint, ColliderInfo> m_colliders;
		//hit pair map
		static std::unordered_map<uint64, HitPair> m_hitPairs;
		//lost pair vector
		static std::vector<LostPair> m_lostPairs;
		//found pair vector
		static std::vector<HitPair> m_foundPairs;
		//優先message
		static std::vector<Detail::Physics::PhysicsMessage> m_preferentMessages;
		//優先message (use thread)
		static std::unordered_set<uint> m_preferentRegisterInstanceIDs;
		//message
		static std::vector<Detail::Physics::PhysicsMessage> m_messages;
		//message (use thread)
		static std::vector<Detail::Physics::PhysicsMessage> m_useThreadMessages;
		//physics thread
		static std::thread m_physicsThread;
		//condition(start)
		static ConditionVariable::Auto m_startUpdateCondition;
		//condition(end)
		static ConditionVariable::Auto m_endUpdateCondition;
		//PxDefaultAllocator
		static physx::PxDefaultAllocator m_allocator;
		//PxDefaultErrorCallback
		static physx::PxDefaultErrorCallback m_errorCallback;
		//PxFoundation
		static physx::PxFoundation* m_foundation;
		//PxPhysics
		static physx::PxPhysics* m_physics;
		//PxCudaContextManager
		static physx::PxCudaContextManager* m_cudaManager;
		//PxControllerManager
		static physx::PxControllerManager* m_controllerManager;
		//PxDefaultCpuDispatcher
		static physx::PxDefaultCpuDispatcher* m_cpuDispatcher;
		//PxCooking
		static physx::PxCooking* m_cooking;
		//PxPvdTransport
		static physx::PxPvdTransport* m_transport;
		//PxPvd
		static physx::PxPvd* m_visualDebugger;
		//PxScene
		static physx::PxScene* m_scene;
		//SimulationCallback
		static SimulationCallback m_simulationCallback;
		//SimulationEventCallback
		static SimulationEventCallback m_simulationEventCallback;
		//QueryCallback
		static QueryCallback m_queryCallback;
		//PhysicsMaterial
		static PhysicsMaterial m_defaultMaterial;
		//lock atomic
		static std::atomic_bool m_lock;
		//lock atomic (message)
		static std::atomic_bool m_lockMessages;
		//my class is shut down
		static std::atomic_bool m_isShutDown;
		//complete fast update
		static bool m_isFirstUpdate;
	};

	//pair found callback
	inline physx::PxFilterFlags Physics::SimulationCallback::pairFound(physx::PxU32 pairID,
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, 
		const physx::PxActor * a0, const physx::PxShape * s0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		const physx::PxActor * a1, const physx::PxShape * s1, physx::PxPairFlags & pairFlags)
	{	
		//used callback & not equal transforms->register found pair
		if ((filterData0.word2 | filterData1.word2) && filterData0.word3 != filterData1.word3)
		{
			m_foundPairs.emplace_back(HitPair::CalculateHashSeed(filterData0.word3, filterData1.word3),
				s0->userData, s1->userData, filterData0.word3, filterData1.word3,
				s0->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE),
				s1->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE),
				filterData0.word2 != 0, filterData1.word2 != 0);
			return physx::PxFilterFlag::eNOTIFY;
		}
		else
			return physx::PxFilterFlag::eSUPPRESS;
	}
	//pair lost callback
	inline void Physics::SimulationCallback::pairLost(physx::PxU32 pairID,
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, 
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, bool objectRemoved)
	{
		//ハッシュシード生成
		uint64 id = HitPair::CalculateHashSeed(filterData0.word3, filterData1.word3);
		//Found
		auto find = m_hitPairs.find(id);
		
		//Pair発見->Lost追加
		if (find != m_hitPairs.end())
			m_lostPairs.emplace_back(find->second, objectRemoved, true);
		else
		{
			//FoundPairからも検索, なければ追加
			for(auto& e : m_foundPairs)
				if (e.hashID == id)
				{
					m_lostPairs.emplace_back(e, objectRemoved, false);
					return;
				}
		}
	}

	//pre filter
	inline physx::PxQueryHitType::Enum Physics::QueryCallback::preFilter(const physx::PxFilterData & filterData,
		const physx::PxShape * shape, const physx::PxRigidActor * actor, physx::PxHitFlags & queryFlags)
	{
		//FilterData取得
		auto shapeFilterData = shape->getSimulationFilterData();
		
		//ヒット判定, Multi TouchでなければBlock, そうでなければTouch返却
		if (filterData.word0 & shapeFilterData.word0)
			return filterData.word1 ? physx::PxQueryHitType::eTOUCH : physx::PxQueryHitType::eBLOCK;
		else
			return physx::PxQueryHitType::eNONE;
	}

	//post filter
	inline physx::PxQueryHitType::Enum Physics::QueryCallback::postFilter(
		const physx::PxFilterData & filterData, const physx::PxQueryHit & hit)
	{
		//FilterData取得
		auto shapeFilterData = hit.shape->getSimulationFilterData();

		//ヒット判定, Multi TouchでなければBlock, そうでなければTouch返却
		if (filterData.word0 & shapeFilterData.word0)
			return filterData.word1 ? physx::PxQueryHitType::eTOUCH : physx::PxQueryHitType::eBLOCK;
		else
			return physx::PxQueryHitType::eNONE;
	}

	//on trigger event
	inline void 	Physics::SimulationEventCallback::onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count) 
	{
		//Trigger側FilterData
		auto triggerFilterData = pairs->triggerShape->getSimulationFilterData();
		//相手のFilterData
		auto otherFilterData = pairs->otherShape->getSimulationFilterData();

		//TriggerEnter & レイヤー判定 & !同一オブジェクト判定->Found登録
		if ((pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND) &&
			(triggerFilterData.word2 | otherFilterData.word2) && 
			triggerFilterData.word3 != otherFilterData.word3)
		{
			m_foundPairs.emplace_back(HitPair::CalculateHashSeed(triggerFilterData.word3, otherFilterData.word3),
				pairs->triggerShape->userData, pairs->otherShape->userData, triggerFilterData.word3, otherFilterData.word3,
				pairs->triggerShape->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE),
				pairs->otherShape->getFlags().isSet(physx::PxShapeFlag::eTRIGGER_SHAPE),
				triggerFilterData.word2 != 0, otherFilterData.word2 != 0);
		}
		//TriggerExit
		else if ((pairs->status & physx::PxPairFlag::eNOTIFY_TOUCH_LOST))
		{
			//ハッシュシード生成
			uint64 id = HitPair::CalculateHashSeed(pairs->triggerShape->getSimulationFilterData().word3, 
				pairs->otherShape->getSimulationFilterData().word3);

			//Found
			auto find = m_hitPairs.find(id);
			//オブジェクト削除によるExitか判定
			bool isObjectRemoved = (pairs->flags &
				(physx::PxTriggerPairFlag::eREMOVED_SHAPE_OTHER |
					physx::PxTriggerPairFlag::eREMOVED_SHAPE_TRIGGER)) ? true : false;

			//Pair存在ならLost追加
			if (find != m_hitPairs.end())
				m_lostPairs.emplace_back(find->second, isObjectRemoved, true);
			else
			{
				//Foundからも検索->見つければLost追加
				for (auto& e : m_foundPairs)
					if (e.hashID == id)
					{
						m_lostPairs.emplace_back(e, isObjectRemoved, false);
						return;
					}
			}
		}
	}

}
#endif // !SGFRAMEWORK_HEADER_PHYSICS_HPP_