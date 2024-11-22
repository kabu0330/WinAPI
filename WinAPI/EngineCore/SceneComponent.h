#pragma once
#include "ActorComponent.h"

// ���� :
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
	
	// ���͸� �������� �� Ʈ������
	FTransform GetActorTransform();

	FVector2D GetComponentScale()
	{
		return Transform.Scale;
	}

	FVector2D GetComponentLocation()
	{
		return Transform.Location;
	}

	// ������Ʈ�� ���� ��ġ�� ������� ������ �ȴٴ� ����� ����� ������ �ȵȴ�.
	// ���Ͱ� (100, 100)�� �ְ� ������Ʈ�� (100, 100)�� �ִٸ� ���� ������ġ�� (200, 200)�̴�.
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

