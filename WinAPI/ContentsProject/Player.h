#pragma once
#include <EngineCore/Actor.h>

// ���� :
class APlayer : public AActor
{
	enum class State
	{
		IDLE,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		MAX
	};

public:
	// constrcuter destructer
	APlayer();
	~APlayer();

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void RunSoundPlay();

	void AnimationSetting();
	void EngineDebug(float _DeltaTime);

	FVector2D Lerp(FVector2D _Start, FVector2D _Dest, float _DeltaTime);

	// �Է� ��� 2 : �̺�Ʈ ������� ó��
	//void LeftMove(float _DeltaTime);
	//void RightMove(float _DeltaTime);
	//void UpMove(float _DeltaTime);
	//void DownMove(float _DeltaTime);

protected:

private:
	float Speed = 150;
	int MySpriteIndex = 0;

	class USpriteRenderer* BodyRenderer = nullptr;
	class USpriteRenderer* HeadRenderer = nullptr;

	State State = State::IDLE;

	class ARoom* CurRoom = nullptr;
};

