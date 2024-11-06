#pragma once
#include <EngineCore/GameMode.h>
#include <vector>

// ���� : Play �������� ���� ���� ������ �͵��� �Է��ϴ� Ŭ����
class APlayGameMode : public AGameMode
{
public:
	// constrcuter destructer
	APlayGameMode();
	~APlayGameMode();

	// delete Function
	APlayGameMode(const APlayGameMode& _Other) = delete;
	APlayGameMode(APlayGameMode&& _Other) noexcept = delete;
	APlayGameMode& operator=(const APlayGameMode& _Other) = delete;
	APlayGameMode& operator=(APlayGameMode&& _Other) noexcept = delete;

	void BeginPlay();

	void Tick(float _DeltaTime);

	void UISetting();

	static void SetCurRoom(class ARoom* _Room)
	{
		CurRoom = _Room;
	}
	static ARoom* GetCurRoom()
	{
		return CurRoom;
	}

	void EngineDebug(float _DeltaTime);

	void PlayerCurRoom();

protected:

private:
	static class ARoom* CurRoom;
	std::vector<class ARoom*> AllRooms;
};


