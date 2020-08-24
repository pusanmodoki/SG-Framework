#include "GarbageCollector.hpp"

//Framework namespace
namespace SGFramework
{
	//Unique Pointers, Tuple1: Deallocator, Tuple2: isAutoGabageCollection
	std::unordered_map<void*, Tuple::Two<Detail::Pointer::BaseDeallocator*, bool>> GabageCollector::m_uniques;
	//Shared Pointers, Tuple1: Deallocator, Tuple2: Counter, Tuple3: isAutoGabageCollection
	std::unordered_map<void*, Tuple::Three<Detail::Pointer::BaseDeallocator*, Detail::Pointer::BaseCounter*, bool>> GabageCollector::m_shareds;
	//Clone Shared Pointers
	std::unordered_map<void*, GabageCollector::CloneSharedInfos> GabageCollector::m_cloneShareds;
	//Clome References
	std::unordered_map<void*, std::unordered_set<void*>> GabageCollector::m_cloneSharedReferences;

	//Delete AutoCollection Uniques
	std::unordered_set<void*> GabageCollector::m_deleteAutoUniques;
	//Delete ManualCollection Uniques
	std::unordered_set<void*> GabageCollector::m_deleteManualUniques;

	//Delete AutoCollection Shareds
	std::unordered_set<void*> GabageCollector::m_deleteAutoShareds;
	//Delete ManualCollection Shareds
	std::unordered_set<void*> GabageCollector::m_deleteManualShareds;
	
	//Delete AutoCollection Shareds
	std::unordered_set<void*> GabageCollector::m_deleteAutoCloneShareds;
	//Delete ManualCollection Shareds
	std::unordered_set<void*> GabageCollector::m_deleteManualCloneShareds;
	
	//Delete Weaks
	std::unordered_set<Detail::Pointer::BaseCounter*> GabageCollector::m_deleteWeaks;

	//delete capacity (Objects)
	uint GabageCollector::m_numDeleteCapacity = 0;
	//num delete object (one frame)
	uint GabageCollector::m_numDeleteOfOneFlame = 0;
	//num delete stack objects
	uint GabageCollector::m_numDeleteObjects = 0;
	//lock
	std::atomic_bool GabageCollector::m_lock = false;
	//shut down flag	
	std::atomic_bool GabageCollector::m_isShutDown = false;
	//delete type flags
	byte GabageCollector::m_deleteFlags = 0;
	//manual collection reserve next frame
	bool GabageCollector::m_isManualCollectionUsedNextFrame = false;
	//now gabage collection flag
	bool GabageCollector::m_isNowGabageCollection = false;
}