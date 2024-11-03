#pragma once
#include <EngineCore/Actor.h>
#include <string>
#include "Room.h"

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
	
	void Initialize(const FVector2D& _Location, RoomDir _Direction, ARoom* _ConnectedRoom);
	void SpriteSetting();

	ARoom* GetConnectedRoom() const
	{
		return ConnectedRoom;
	}

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

protected:

private:
	class USpriteRenderer* DoorRendererX = nullptr;
	class USpriteRenderer* DoorRendererY = nullptr;
	RoomDir Direction = RoomDir::NONE;
	ARoom* ConnectedRoom = nullptr;
};

