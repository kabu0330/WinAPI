#pragma once
#include <EngineCore/Actor.h>
#include "Tear.h"

// ���� : ������
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

	void Move(float _DeltaTime);
	void CameraPosMove(float _DeltaTime);
	void SpriteSetting();



	// �Է� ��� 2 : �̺�Ʈ ������� ó��
	//void LeftMove(float _DeltaTime);
	//void RightMove(float _DeltaTime);
	//void UpMove(float _DeltaTime);
	//void DownMove(float _DeltaTime);

protected:

private:
	float Speed = 350;
	int MySpriteIndex = 0;

	class USpriteRenderer* BodyRenderer = nullptr;
	class USpriteRenderer* HeadRenderer = nullptr;

	State State = State::IDLE;

	//Bullet
	ATear* Tear = nullptr;


	// ī�޶� �̵����� ���
	float CameraMoveTime = 0.0f;
	float LerpAlpha = 0.0f;
	bool CameraMove = false;
	FVector2D CameraMoveDir  = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos   = FVector2D::ZERO;
};

