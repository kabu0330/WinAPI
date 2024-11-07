#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
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
	~APlayer() {}

	// delete Function
	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	//void RunSoundPlay();

	// 이동관련
	void Move(float _DeltaTime);
	void CameraPosMove(float _DeltaTime);

	// 공격관련
	void InputAttack(float _DeltaTime);

	void Attack(float _DeltaTime);
	bool IsAttack() const
	{
		return true == TearFire;
	}

	void SetAttackDir(UpperState _HeadState);
	int  GetAttackDir() const
	{
		return CurAttackHeadDir;
	}

	void Collision();

	bool DeathCheck();

	// 애니메이션
	void CurStateAnimation(float _DeltaTime);
	void SpriteSetting();

	// UI
	void UISetting();
	void UITick(float _DeltaTime);

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
	float Speed = 250;
	int MySpriteIndex = 0;

	// MoveAcc
	FVector2D FinalSpeed = FVector2D::ZERO;
	float MoveAcc = 1000.0f;
	float SpeedMax = 350.0f;
	bool IsMove = false;
	FVector2D InverseDir = FVector2D::ZERO;
	FVector2D Deceleration = FVector2D::ZERO;
	float MoveDec = 0.0f;

	// Item
	int PennyCount = 0;
	int BombCount  = 1;
	int KeyCount   = 1;

	// Collision
	U2DCollision* BodyCollision = nullptr;

	// Animation State
	UpperState HeadState = UpperState::IDLE;
	LowerState BodyState = LowerState::IDLE;
	int CurAttackHeadDir = 0;
	float StateElapesd   = 0.0f;
	float StateTime      = 0.10f;

	// Renderer
	class USpriteRenderer* BodyRenderer = nullptr;
	class USpriteRenderer* HeadRenderer = nullptr;

	//Bullet
	ATear* Tear = nullptr;
	float Cooldown = 0.4f;
	float CoolDownElapsed = 0.0f;
	FVector2D TearDir = FVector2D::ZERO;
	bool TearFire = false;
	bool LeftFire = true;

	// UI
	class AHeartUI*        PlayerHpToHeart;

	class APickupItemUI*   PennyUI;
	class APickupNumberUI* PennyPickupNumber;

	class APickupItemUI*   BombUI;
	class APickupNumberUI* BombPickupNumber;

	class APickupItemUI*   KeyUI;
	class APickupNumberUI* KeyPickupNumber;

	// 카메라 이동관련 멤버
	float CameraMoveTime = 0.0f;
	float LerpAlpha      = 0.0f;
	bool  CameraMove     = false;
	FVector2D CameraMoveDir  = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos   = FVector2D::ZERO;



};
// 입력 방법 2 : 이벤트 방식으로 처리
//void LeftMove(float _DeltaTime);
//void RightMove(float _DeltaTime);
//void UpMove(float _DeltaTime);
//void DownMove(float _DeltaTime);

// 주의사항 : 자기가 자기 자신을 SpawnActor하면 무한 스폰에 빠져 스택 오버플로우
// 입력 방법 2 : 이벤트 방식으로 처리
//UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
//UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
//UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
//UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));
