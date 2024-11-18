#pragma once
#include <EngineCore/Actor.h>

// Ό³Έν :
class AMapObject : public AActor
{
public:
	// constrcuter destructer
	AMapObject();
	~AMapObject();

	// delete Function
	AMapObject(const AMapObject& _Other) = delete;
	AMapObject(AMapObject&& _Other) noexcept = delete;
	AMapObject& operator=(const AMapObject& _Other) = delete;
	AMapObject& operator=(AMapObject&& _Other) noexcept = delete;

	void BeginPlay();
	void Tick(float _DeltaTime);

protected:

private:

};

