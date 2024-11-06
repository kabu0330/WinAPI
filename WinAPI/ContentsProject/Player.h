#pragma once
#include <EngineCore/Actor.h>
#include "Tear.h"

// 설명 : 아이작
class APlayer : public AActor
{
	enum class LowerState
	{
		IDLE,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		MAX
	};

	enum class UpperState
	{
		IDLE,
		LEFT,
		RIGHT,
		UP,
		DOWN,
		ATTACK_LEFT,
		ATTACK_RIGHT,
		ATTACK_UP,
		ATTACK_DOWN,
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

	//void RunSoundPlay();

	void Move(float _DeltaTime);
	void CameraPosMove(float _DeltaTime);

	void InputAttack(float _DeltaTime);
	void Attack(float _DeltaTime);
	bool IsAttack() const
	{
		return true == TearFire;
	}

	void SetAttackDir(UpperState _HeadState);

	int GetAttackDir() const
	{
		return CurAttackHeadDir;
	}

	void CurStateAnimation(float _DeltaTime);

	void SpriteSetting();


	// Heart
	static int GetPlayerHeart()
	{
		return Heart;
	}
	static void SetPlayerHeart(int _Value)
	{
		Heart = _Value;
	}
	static void ChangePlayerHeart(int _Value)
	{
		Heart += _Value;
	}
	static int GetHeartMax()
	{
		return HeartMax;
	}

protected:

private:
	// Heart
	static int Heart;
	static int HeartMax;
	//static int SoulHeart;
	//static int AllHeartMax;

	// Stat
	float Speed = 350;
	int MySpriteIndex = 0;

	// Animation State
	UpperState HeadState = UpperState::IDLE;
	LowerState BodyState = LowerState::IDLE;

	// Renderer
	class USpriteRenderer* BodyRenderer = nullptr;
	class USpriteRenderer* HeadRenderer = nullptr;
	float StateElapesd = 0.0f;
	float StateTime = 0.10f;
	int CurAttackHeadDir = 0;

	//Bullet
	ATear* Tear = nullptr;
	float Cooldown = 0.4f;
	float CoolDownElapsed = 0.0f;
	FVector2D TearDir = FVector2D::ZERO;
	bool TearFire = false;
	bool LeftFire = true;

	// 카메라 이동관련 멤버
	float CameraMoveTime = 0.0f;
	float LerpAlpha = 0.0f;
	bool  CameraMove = false;
	FVector2D CameraMoveDir = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos = FVector2D::ZERO;


	// 입력 방법 2 : 이벤트 방식으로 처리
	//void LeftMove(float _DeltaTime);
	//void RightMove(float _DeltaTime);
	//void UpMove(float _DeltaTime);
	//void DownMove(float _DeltaTime);
};

