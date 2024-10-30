#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineBase/Object.h>

#include "EngineSprite.h"
#include <string>

// 설명 :
class AActor : public UObject
{
public:
	typedef AActor Super;

	friend class ULevel;

	// constrcuter destructer
	AActor();
	virtual ~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	virtual void BeginPlay() {}
	virtual void Tick(float _DeltaTime) {}
	virtual void Render();

	class ULevel* GetWorld()
	{
		return World;
	}

	void SetActorLocation(FVector2D _Location)
	{
		Transform.Location = _Location;
	}

	void AddActorLocation(FVector2D _Direction)
	{
		Transform.Location += _Direction;
	}

	void SetActorScale(FVector2D _Scale)
	{
		Transform.Scale = _Scale;
	}

	FVector2D GetActorLocation()
	{
		return Transform.Location;
	}

	// 추가
	FVector2D GetActorScale()
	{
		return Transform.Scale;
	}

protected:

private:
	class ULevel* World = nullptr;

	FTransform Transform;

public:
	class UEngineSprite* Sprite = nullptr;
	int CurIndex = 0;

	void SetSprite(std::string_view _Name, int _CurIndex = 0);
};


