#pragma once
#include "ActorComponent.h"

// 설명 :
class USceneComponent : public UActorComponent
{
public:
	// constrcuter destructer
	USceneComponent();
	~USceneComponent();

	// delete Function
	USceneComponent(const USceneComponent& _Other) = delete;
	USceneComponent(USceneComponent&& _Other) noexcept = delete;
	USceneComponent& operator=(const USceneComponent& _Other) = delete;
	USceneComponent& operator=(USceneComponent&& _Other) noexcept = delete;

	FTransform GetTransform()
	{
		return Transform;
	}
	
	// 액터를 기준으로 한 트랜스폼
	FTransform GetActorTransform();

	FVector2D GetComponentScale()
	{
		return Transform.Scale;
	}

	FVector2D GetComponentLocation()
	{
		return Transform.Location;
	}

	// 컴포넌트는 액터 위치를 기반으로 렌더링 된다는 사실을 절대로 잊으면 안된다.
	// 액터가 (100, 100)에 있고 컴포넌트가 (100, 100)에 있다면 실제 렌더위치는 (200, 200)이다.
	void SetComponentLocation(FVector2D _Location)
	{
		Transform.Location = _Location;
	}

	void SetComponentScale(FVector2D _Scale)
	{
		Transform.Scale = _Scale;
	}

	void AddComponentLocation(FVector2D _Direction)
	{
		Transform.Location += _Direction;
	}

	void AddComponentScale(FVector2D _Scale)
	{
		Transform.Scale += _Scale;
	}

protected:

private:
	FTransform Transform;
};

