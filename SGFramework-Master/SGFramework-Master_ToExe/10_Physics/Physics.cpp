/*----------------------------------------------------------------------------------
物理エンジン(PhysX)を管理するPhysics class
------------------------------------------------------------------------------------*/
#include "../07_Thread/Thread.hpp"
#include "Physics.hpp"
#include "PhysicsFunction/PhysicsFilterShader.hpp"
#include "PhysicsFunction/PhysicsProcessingMessage.hpp"
#include "RigidBody/RigidBody.hpp"
#include "ContactInfo.hpp"
#include "RaycastHit/RaycastHit.hpp"

//Framework namespace
namespace SGFramework
{
	//PhysX->PxPhysics
	PointerProperty<physx::PxPhysics*> Physics::physicsFactory(m_physics);
	//PhysX->PxScene
	PointerProperty<physx::PxScene*> Physics::physicsScene(m_scene);
	//PhysX->PxControllerManager
	PointerProperty<physx::PxControllerManager*> Physics::controllerManager(m_controllerManager);
	//Default PhysicsMaterial (staticFriction = 0.5f, dynamicFriction = 0.5f, restitution(反発係数) = 0.1f)
	GetOnlyProperty<PhysicsMaterial> Physics::defaultPhysicsMaterial(m_defaultMaterial);

	//actors map
	std::unordered_map<uint, Physics::ActorInfo> Physics::m_actors;
	//dynamic actor map
	std::unordered_map<uint, WeakPointer<BaseClass::BaseRigidBody>> Physics::m_dynamicActors;
	//actors map
	std::unordered_map<uint, Physics::ColliderInfo> Physics::m_colliders;
	//hit pair map
	std::unordered_map<uint64, Physics::HitPair> Physics::m_hitPairs;
	//dynamic actor map
	std::unordered_map<uint, WeakPointer<BaseClass::BaseRigidBody>&> Physics::m_dynamicActors1;
	//static actor map
	std::unordered_map<uint, WeakPointer<BaseClass::BaseRigidBody>&> Physics::m_staticActors;
	//shape map
	std::unordered_set<BaseClass::BaseCollider*> Physics::m_shapes;
	//lost pair vector
	std::vector<Physics::LostPair> Physics::m_lostPairs;
	//found pair vector
	std::vector<Physics::HitPair> Physics::m_foundPairs;
	//優先message
	std::vector<Detail::Physics::PhysicsMessage> Physics::m_preferentMessages;
	//優先message (use thread)
	std::unordered_set<uint> Physics::m_preferentRegisterInstanceIDs;
	//message
	std::vector<Detail::Physics::PhysicsMessage> Physics::m_messages;
	//message (use thread)
	std::vector<Detail::Physics::PhysicsMessage> Physics::m_useThreadMessages;
	//physics thread
	std::thread Physics::m_physicsThread;
	//condition(start)
	ConditionVariable::Auto Physics::m_startUpdateCondition;
	//condition(end)
	ConditionVariable::Auto Physics::m_endUpdateCondition;
	//remove vector
	std::vector<physx::PxRigidActor*> Physics::m_removeActors;
	//remove vector
	std::vector<WeakPointer<BaseClass::BaseCollider>> Physics::m_removeColliders;
	//remove vector
	std::vector<physx::PxShape*> Physics::m_removeShapes;
	//PxDefaultAllocator
	physx::PxDefaultAllocator Physics::m_allocator = physx::PxDefaultAllocator();
	//PxDefaultErrorCallback
	physx::PxDefaultErrorCallback Physics::m_errorCallback = physx::PxDefaultErrorCallback();
	//PxFoundation
	physx::PxFoundation* Physics::m_foundation = nullptr;
	//PxPhysics
	physx::PxPhysics* Physics::m_physics = nullptr;
	//PxCudaContextManager
	physx::PxCudaContextManager* Physics::m_cudaManager = nullptr;
	//PxControllerManager
	physx::PxControllerManager* Physics::m_controllerManager = nullptr;
	//PxDefaultCpuDispatcher
	physx::PxDefaultCpuDispatcher* Physics::m_cpuDispatcher = nullptr;
	//PxCooking
	physx::PxCooking* Physics::m_cooking = nullptr;
	//PxPvdTransport
	physx::PxPvdTransport* Physics::m_transport = nullptr;
	//PxPvd
	physx::PxPvd* Physics::m_visualDebugger = nullptr;
	//PxScene
	physx::PxScene* Physics::m_scene = nullptr;
	//SimulationCallback
	Physics::SimulationCallback Physics::m_simulationCallback;
	//SimulationEventCallback
	Physics::SimulationEventCallback Physics::m_simulationEventCallback;
	//QueryCallback
	Physics::QueryCallback Physics::m_queryCallback;
	//PhysicsMaterial
	PhysicsMaterial Physics::m_defaultMaterial;
	//lock atomic
	std::atomic_bool Physics::m_lock = false;
	//lock atomic (message)
	std::atomic_bool Physics::m_lockMessages = false;
	//my class is shut down
	std::atomic_bool Physics::m_isShutDown = false;
	//complete fast update
	bool Physics::m_isFirstUpdate = false;

	//----------------------------------------------------------------------------------
	//[StartUp]
	//throw: failed start up
	//引数1: gravity
	//引数2: HitPair->Init Num Reserve
	void Physics::StartUp(const ReadElement::Pack& gravity,
		const ReadElement::Pack& hitPairVectorReserves)
	{
		//Create Foundation
		m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);
		if (m_foundation == nullptr)
			throw InvalidArgument(L"Error!! Physics->StartUp", L"PxCreateFoundation failed");

		//Support GPU->Create cuda context manager
#if PX_SUPPORT_GPU_PHYSX
		physx::PxCudaContextManagerDesc cudaContextDesc = {};

		m_cudaManager = PxCreateCudaContextManager(*m_foundation, cudaContextDesc, PxGetProfilerCallback());
		if (m_cudaManager != nullptr && IS_FALSE(m_cudaManager->contextIsValid()))
		{
			m_cudaManager->release();
			m_cudaManager = nullptr;
		}
#endif

		//create pvd
		m_visualDebugger = PxCreatePvd(*m_foundation);
		if (m_foundation == nullptr)
			throw InvalidArgument(L"Error!! Physics->StartUp", L"PxCreatePvd failed");

		//pvd transport
		m_transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		if (m_transport == nullptr)
			throw InvalidArgument(L"Error!! Physics->StartUp", L"PxDefaultPvdSocketTransportCreate failed");
		//pvd connect
		m_visualDebugger->connect(*m_transport, physx::PxPvdInstrumentationFlag::ePROFILE);

		//?
		physx::PxTolerancesScale tolerancesScale = physx::PxTolerancesScale();

		//create physics engine
		m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, tolerancesScale, false, m_visualDebugger);
		if (m_physics == nullptr)
			throw InvalidArgument(L"Error!! Physics->StartUp", L"PxCreatePhysics failed");

		//create cooking
		physx::PxCookingParams cookingParams(tolerancesScale);
		cookingParams.meshWeldTolerance = 0.001f;
		cookingParams.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES);
		cookingParams.buildGPUData = m_cudaManager != nullptr ? true : false;
		m_cooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_foundation, cookingParams);
		if (m_cooking == nullptr)
			throw InvalidArgument(L"Error!! Physics->StartUp", L"PxCreateCooking failed");

		//create cpu dispatcher
		uint affinity = Thread::getAffinityPhysX();
		m_cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1, &affinity);
		if (m_cpuDispatcher == nullptr)
			throw InvalidArgument(L"Error!! Physics->StartUp", L"PxDefaultCpuDispatcherCreate failed");

		//scene setting
		physx::PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(gravity[0].valueFloat, gravity[1].valueFloat, gravity[2].valueFloat);
		sceneDesc.cpuDispatcher = m_cpuDispatcher;
		sceneDesc.filterShader = FilterShader;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;
		sceneDesc.simulationEventCallback;
		sceneDesc.ccdContactModifyCallback;
		sceneDesc.contactModifyCallback;
		sceneDesc.filterCallback = &m_simulationCallback;
		sceneDesc.simulationEventCallback = &m_simulationEventCallback;

		//created cuda manager->setting scene
		if (m_cudaManager != nullptr)
		{
			sceneDesc.cudaContextManager = m_cudaManager;

			sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
			sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;
			sceneDesc.gpuMaxNumPartitions = 8;//2のべき乗だけっぽい
			sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
		}

		//create scene
		m_scene = m_physics->createScene(sceneDesc);
		//create controller manager
		m_controllerManager = PxCreateControllerManager(*m_scene);

		//get pvd client
		physx::PxPvdSceneClient* visualDebuggerClient = m_scene->getScenePvdClient();
		if (visualDebuggerClient != nullptr)
		{
			visualDebuggerClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, false);
			visualDebuggerClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, false);
			visualDebuggerClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, false);
		}

		//setting physics
		PhysicsMaterial::Physics() = m_physics;

		//create default material
		m_defaultMaterial = PhysicsMaterial(0.5f, 0.5f, 0.1f);
		if (m_defaultMaterial == nullptr)
			throw InvalidArgument(L"Error!! Physics->StartUp", L"createMaterial failed");

		//reserve
		m_foundPairs.reserve(hitPairVectorReserves[0].valueInt);
		m_lostPairs.reserve(hitPairVectorReserves[0].valueInt);

		//thread起動
		m_physicsThread = std::thread(PhysicsThread);
		Thread::setAffinityPhysicsUpdate(m_physicsThread.native_handle());
	}

	//----------------------------------------------------------------------------------
	//[ShutDown]
	void Physics::ShutDown()
	{
		AtomicOperation::Init(m_isShutDown, true);
		m_startUpdateCondition.NotifyOne();

		if (m_physicsThread.joinable())
			m_physicsThread.join();

		//fetch results
		if (IS_TRUE(m_isFirstUpdate))
			m_scene->fetchResults(true);

		//simulation
		m_scene->simulate(static_cast<float>(Time::fixedInterval()));
		m_scene->fetchResults(true);

		//prosessing message
		for (auto& e : m_messages)
		{
			ProcessingMessage(e);
			e.Release();
		}
		for (auto& e : m_useThreadMessages)
		{
			ProcessingMessage(e);
			e.Release();
		}
		for (auto& e : m_preferentMessages)
		{
			ProcessingMessage(e);
			e.Release();
		}
		//clear vector
		m_removeActors.clear();
		m_removeShapes.clear();
		m_removeColliders.clear();
		m_hitPairs.clear();
		m_actors.clear();

		//release pointers
		m_defaultMaterial.Release();
		PHYSX_RELEASE(m_controllerManager);
		PHYSX_RELEASE(m_scene);
		PHYSX_RELEASE(m_cpuDispatcher);
		PHYSX_RELEASE(m_cooking);
		PHYSX_RELEASE(m_physics);
		PHYSX_RELEASE(m_cudaManager);
		PHYSX_RELEASE(m_transport);
		PHYSX_RELEASE(m_visualDebugger);
		PHYSX_RELEASE(m_foundation);
	}

	void Physics::SyncPhysicsBegin()
	{
		if (IS_TRUE(m_isFirstUpdate))
		{
			m_endUpdateCondition.Wait();

			//fetch results
			m_scene->fetchResults(true);
		}
		for (auto& e : m_preferentMessages)
		{
			ProcessingMessage(e);
			e.Release();
		}
		m_preferentMessages.clear();
	
	}

	void Physics::UpdatePhysics()
	{
		//completed first update
		if (IS_TRUE(m_isFirstUpdate))
		{
			Physics::BeginPhysics();
		}
	}

	void Physics::SyncPhysicsEnd()
	{
		m_startUpdateCondition.NotifyOne();
	}
}