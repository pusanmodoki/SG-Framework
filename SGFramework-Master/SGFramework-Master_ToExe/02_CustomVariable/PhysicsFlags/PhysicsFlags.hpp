/*----------------------------------------------------------------------------------
Physics‚ÅŽg—p‚·‚éEnum‚ðW‚ß‚½PhysicsFlags.hpp
------------------------------------------------------------------------------------*/
#ifndef SGFRAMEWORK_HEADER_PHYSICS_FLAGS_HPP_
#define SGFRAMEWORK_HEADER_PHYSICS_FLAGS_HPP_

//Framework namespace
namespace SGFramework
{
	//Using RigidBody->AddForce, AddTorque
	struct ForceMode
	{
		enum Enum
		{
			Force,				//!< parameter has unit of mass * distance/ time^2, i.e. a force
			Impluse,			//!< parameter has unit of mass * distance /time
			VelocityChange,	//!< parameter has unit of distance / time, i.e. the effect is mass independent: a velocity change.
			Acceleration		//!< parameter has unit of distance/ time^2, i.e. an acceleration. It gets treated just like a force except the mass is not divided out before integration.
		};
	};
	//Using RigidBody->setLockFlag, setLockFlags
	struct RigidBodyLock
	{
		enum Enum
		{
			VelocityX = (1 << 0),
			VelocityY = (1 << 1),
			VelocityZ = (1 << 2),
			VelocityXZ = VelocityX | VelocityZ,
			VelocityXY = VelocityX | VelocityY,
			VelocityAll = VelocityX | VelocityY | VelocityZ,

			RotationX = (1 << 3),
			RotationY = (1 << 4),
			RotationZ = (1 << 5),
			RotationAll = RotationX | RotationY | RotationZ,
			VelocityAndRotationAll = VelocityAll | RotationAll,
		};
	};
	//Colldier type
	enum class ColliderType
	{
		Sphere,
		Plane,
		Capsule,
		Box,
		ConvexMesh,
		TriangleMesh,
		HeightField,
		Invalid = -1
	};
}

#endif //!SGFRAMEWORK_HEADER_PHYSICS_FLAGS_HPP_