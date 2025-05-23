#pragma once
#include "../../general.h"
#include "../../Driver/driver.hpp"

//
//
//namespace UWorld {
//	constexpr auto PersistentLevel = 0x38; // ULevel*
//	constexpr auto TempStaticLevel = 0x40; // ULevel*
//	constexpr auto NetDriver = 0x48; // UNetDriver*
//	constexpr auto LineBatcher = 0x50; // ULineBatchComponent*
//	constexpr auto PersistentLineBatcher = 0x58; // ULineBatchComponent*
//	constexpr auto ForegroundLineBatcher = 0x60; // ULineBatchComponent*
//	constexpr auto NetworkManager = 0x68; // AGameNetworkManager*
//	constexpr auto PhysicsCollisionHandler = 0x70; // UPhysicsCollisionHandler*
//	constexpr auto ExtraReferencedObjects = 0x78; // TArray<UObject*>
//	constexpr auto PerModuleDataObjects = 0x88; // TArray<UObject*>
//	constexpr auto StreamingLevels = 0x98; // TArray<ULevelStreaming*>
//	constexpr auto StreamingLevelsToConsider = 0xa8; // FStreamingLevelsToConsider
//	constexpr auto ServerStreamingLevelsVisibility = 0xd0; // AServerStreamingLevelsVisibility*
//	constexpr auto StreamingLevelsPrefix = 0xd8; // FString
//	constexpr auto CurrentLevelPendingVisibility = 0xf0; // ULevel*
//	constexpr auto CurrentLevelPendingInvisibility = 0xf8; // ULevel*
//	constexpr auto DemoNetDriver = 0x100; // UDemoNetDriver*
//	constexpr auto GeometryCollectionISMCLoader = 0x108; // UGeometryCollectionISMCLoader*
//	constexpr auto MyParticleEventManager = 0x110; // AParticleEventManager*
//	constexpr auto DefaultPhysicsVolume = 0x118; // APhysicsVolume*
//	constexpr auto bAreConstraintsDirty = 0x17f; // uint8
//	constexpr auto PostLoadActorNum = 0x184; // uint32
//	constexpr auto AssetTags = 0x188; // FGameplayTagContainer
//	constexpr auto NavigationSystem = 0x200; // UNavigationSystemBase*
//	constexpr auto AuthorityGameMode = 0x208; // AGameModeBase*
//	constexpr auto GameState = 0x210; // AGameStateBase*
//	constexpr auto AISystem = 0x218; // UAISystemBase*
//	constexpr auto AvoidanceManager = 0x220; // UAvoidanceManager*
//	constexpr auto Levels = 0x228; // TArray<ULevel*>
//	constexpr auto LevelCollections = 0x238; // TArray<FLevelCollection>
//	constexpr auto OwningGameInstance = 0x270; // UGameInstance*
//	constexpr auto ParameterCollectionInstances = 0x278; // TArray<UMaterialParameterCollectionInstance*>
//	constexpr auto CanvasForRenderingToTarget = 0x288; // UCanvas*
//	constexpr auto CanvasForDrawMaterialToRenderTarget = 0x290; // UCanvas*
//	constexpr auto PhysicsField = 0x338; // UPhysicsFieldComponent*
//	constexpr auto LWILastAssignedUID = 0x358; // uint32
//	constexpr auto ComponentsThatNeedPreEndOfFrameSync = 0x360; // TArray<UActorComponent*>
//	constexpr auto ComponentsThatNeedEndOfFrameUpdate = 0x370; // TArray<UActorComponent*>
//	constexpr auto ComponentsThatNeedEndOfFrameUpdate_OnGameThread = 0x380; // TArray<UActorComponent*>
//	constexpr auto WorldComposition = 0x8f8; // UWorldComposition*
//	constexpr auto ContentBundleManager = 0x900; // UContentBundleManager*
//	constexpr auto PSCPool = 0x9c0; // FWorldPSCPool
//	constexpr auto PhysicsObjectLoadHelper = 0xb08; // UPhysicsObjectLoadHelper*
//	constexpr auto DecalPool = 0xb10; // USimpleDecalPool*
//}

class UWorld
{
public:
	struct World {
		uintptr_t PersistentLevel;
		uintptr_t WorldComposition;
		uintptr_t GameState;
		uintptr_t OwningGameInstance;
	};

	UWorld* GetWorld(uintptr_t base);
private:
	uintptr_t WorldSize = sizeof(uintptr_t);
};


class ULevel
{
public:
	ULevel* GetLevel(uintptr_t Level);
	ULevel* GetActorsFromLevel(uintptr_t Level);
};

