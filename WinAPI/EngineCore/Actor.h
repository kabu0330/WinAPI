#pragma once
#include <EnginePlatform/EngineSound.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/Object.h>
#include <EngineBase/TimeEvent.h>

#include "EngineSprite.h"
#include <string>

// 설명 :
class AActor : public UObject
{
public:

	friend class ULevel;

	AActor();
	virtual ~AActor();

	// 객체가 레벨에서 처음 Tick을 돌리기 직전에 실행된다.
	virtual void BeginPlay() {}


	virtual void Tick(float _DeltaTime);
	
	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	class ULevel* GetWorld()
	{
		return World;
	}

	void SetActorLocation(FVector2D _Location)
	{
		PrevLocation = GetActorLocation();
		Transform.Location = _Location;
	}

	void AddActorLocation(FVector2D _Direction)
	{
		PrevLocation = GetActorLocation();
		Transform.Location += _Direction;
	}

	void SetActorScale(FVector2D _Scale)
	{
		Transform.Scale = _Scale;
	}

	FTransform GetTransform()
	{
		return Transform;
	}

	FVector2D GetActorLocation()
	{
		return Transform.Location;
	}
	FVector2D GetPrevActorLocation()
	{
		return PrevLocation;
	}

	FVector2D GetActorScale()
	{
		return Transform.Scale;
	}

	// 컴포넌트의 소유자는 액터이다. 삭제도 액터가 해야한다.
	// 액터가 가진 컴포넌트를 다른 액터에 넘겨주거나 실행 중에 렌더를 삭제할 수 없다.
	template<typename ComponentType> // SpriteRenderer
	ComponentType* CreateDefaultSubObject()
	{
		ComponentType* NewComponent = new ComponentType();

		UActorComponent* ComponentPtr = dynamic_cast<UActorComponent*>(NewComponent);

		// 내가 널 만든 액터야.
		ComponentPtr->ParentActor = this;

		// 호출 지연
		// NewComponent->BeginPlay();
		// 만들기만 하고 실행 안한 상태가 된것.
		Components.push_back(NewComponent);

		// BeginPlay가 실행안된 컴포넌트들을 다 자료구조에 담는다.
		ComponentList.push_back(NewComponent);
		return NewComponent;
	}

protected:
	UTimeEvent TimeEventer;

private:
	static void ComponentBeginPlay();

	static bool IsNewActorCreate;
	static std::list<class UActorComponent*> ComponentList;

	void ReleaseCheck(float _DeltaTime) override;
	void ReleaseTimeCheck(float _DeltaTime) override;

	class ULevel* World = nullptr;

	FTransform Transform;
	FVector2D PrevLocation = FVector2D::ZERO;

	std::list<class UActorComponent*> Components;

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;
};

typedef AActor Super;
