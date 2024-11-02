#pragma once
#include <EngineCore/Actor.h>
#include <string>

// Ό³Έν :
class ADoor : public AActor
{
public:
	// constrcuter destructer
	ADoor();
	~ADoor();

	// delete Function
	ADoor(const ADoor& _Other) = delete;
	ADoor(ADoor&& _Other) noexcept = delete;
	ADoor& operator=(const ADoor& _Other) = delete;
	ADoor& operator=(ADoor&& _Other) noexcept = delete;
	
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void NormalDoorSetting();

	void SetDoorLock(std::string_view _Dir)
	{

	}

protected:

private:
	class USpriteRenderer* DoorRenderer = nullptr;
};

