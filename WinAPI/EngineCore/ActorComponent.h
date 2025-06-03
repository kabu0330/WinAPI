#pragma once
#include <map>
#include <EngineBase/Object.h>
#include <EngineBase/EngineDelegate.h>
#include "EngineCoreDebug.h"
#include "EngineSprite.h"

// "플레이어의 위치를 기준으로 어디에 있다."를 표현하는 기능들을 언리얼에서는 SceneComponent라고 한다. (HP Bar, 무기 등)
// 위치를 가지지 않는 ActorComponent가 있다. (길찾기 기능, 점수 계산, 퀘스트, 대화 등)
// Component								: Actor의 구성 요소
// ActorComponent : public Object			: 위치는 필요 없는		
// SceneComponent : public ActorComponent	: 위치도 필요한
// Renderer		  : public SceneComponent	: 이미지 렌더
// 2DCollision	  : public SceneComponent	: 이미지 렌더
// 이로써 Actor는 수십 개의 이미지를 가질 수 있게 된다.

// 설명 : Actor의 구성 요소(이미지, 충돌체)
class UActorComponent : public UObject
{
public:
	// 액터만이 ParentActor 세팅할수 있게 제한하기 위해서 아래와 같이 짠다.
	friend class AActor;
	typedef UActorComponent Super;

	UActorComponent();
	virtual ~UActorComponent() = 0; // 객체화 못시키게 하는법 중 하나

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	virtual void BeginPlay() {}

	virtual void ComponentTick(float _DeltaTime) {}

	class AActor* GetActor()
	{
		return ParentActor;
	}

	bool IsActive() override
	{
		// 랜더러는 자신을 가진 액터에게 종속된다.
		// 부모도        true            true
		return UObject::IsActive() && GetActor()->IsActive();
	}

	bool IsDestroy() override
	{
		// 부모도        true            true
		return UObject::IsDestroy() || GetActor()->IsDestroy();
	}

protected:

private:
	class AActor* ParentActor;
};

