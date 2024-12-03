#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include "Tear.h"
#include "Global.h"
#include "Item.h"

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
		DEATH,
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
		DEATH,
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

	// 주요 설정 함수
	void CollisionSetting();
	void CollisionFuctionSetting();
	void SpriteSetting();
	void CurStateAnimation(const float& _DeltaTime);
	void UISetting();
	void UITick(const float& _DeltaTime);
	void ResetDebug();
	void PlayerLimit();

	// Debug
	void PlayerDebugSetting(const float& _DeltaTime);
	void CheatKey(const float& _DeltaTime);


	// 이동관련
	void Move(const float& _DeltaTime);
	bool ProcessMovementInput();
	void UpdateHeadState();
	bool HasMovementInput();
	void SetMovement(bool _IsMovementStopped)
	{
		IsMovementStopped = _IsMovementStopped;
	}

	bool PlayerIsMove() // Tear
	{
		return IsMove;
	}

	void ClampPositionToRoom();
	void IsCameraMove();
	void CameraPosMove(const float& _DeltaTime);
	U2DCollision* GetWarpCollision()
	{
		return WarpCollision;
	}


	// 공격 및 피격
	void InputAttack(const float& _DeltaTime); // 입력 함수
	void Attack(const float& _DeltaTime); // 실제 공격정보를 Tear로 넘기는 함수

	bool IsAttack() const
	{
		return true == TearFire;
	}
	int  GetAttackDir() const
	{
		return CurAttackHeadDir;
	}
	void SetAttackDir(const UpperState& _HeadState);

	int ApplyDamaged(AActor* _Player, const int& _Att, const FVector2D& _Dir);
	void ShowHitAnimation(AActor* _Other);
	void RestoreDefaultMotion();
	void BeginBlinkEffect();
	void StayBlinkEffect();

	void SetInvincible(const bool& _OnOff)
	{
		Invincibility = _OnOff;
	}
	void SwitchInvincibility()
	{
		Invincibility = !Invincibility;
	}

	bool IsInvincible()
	{
		return Invincibility;
	}
	void EnterInvincibility()
	{
		BodyCollision->SetActive(false);
	}

	template<typename EnumType>
	void SetRendererDir(const EnumType& _Dir)
	{
		int Direction = static_cast<int>(_Dir);
		HeadState = static_cast<UpperState>(Direction);
		BodyState = static_cast<LowerState>(Direction);
	}


	// 사망, 일시정지, 재시작 관련
	bool IsDeath();
	void Death(const float& _DeltaTime);
	void DeathAnimation();
	void SpiritAnimation();
	void ShowDeathReport();
	void Reset();



	// Stat
	static int GetHptMax() 
	{
		return HeartMax;
	}
	static void AddHpMax(const int& _Value)
	{
		HeartMax += _Value;
	}

	int GetAtt() const
	{
		return Att;
	}

	int AddAtt(const int& _Value)
	{
		return Att += _Value;
	}

	int GetHp() const
	{
		return Heart;
	}
	void AddHp(const int& _Value)
	{
		if (Heart >= HeartMax)
		{
			Heart = HeartMax;
		}
		Heart += _Value;
	}

	float GetSpeed() const
	{
		return MaxSpeed;
	}
	void AddMaxSpeed(const float& _Value)
	{
		MaxSpeed += _Value;
	}
	FVector2D GetFinalSpeed() const
	{
		return FinalSpeed;
	}
	void SetFinalSpeed(FVector2D _FinalSpeed)
	{
		FinalSpeed = _FinalSpeed;
	}

	void SetDir(const FVector2D& _Dir)
	{
		Direction = _Dir;
	}

	void SwitchIsHit()
	{
		IsHit = !IsHit;
	}

	FVector2D& GetForce()
	{
		return Force;
	}

	void SetForce(const FVector2D& _Force)
	{
		Force = _Force;
	}

	void ReverseForce(const float& _DeltaTime);


	// 아이템
	bool TryPickupItem(class AItem* _Item, const int& _Count);
	void AddItem(class AItem* _Item, const int& _Count);
	int CheckPickupItemCount(std::string_view _ItemName)
	{
		return ItemCounts[_ItemName.data()];
	}
	AItem* ReturnItem(std::string_view _ItemName);

	void InputItem();

	// 아이템 습득
	void RemveItem(AItem* _Item);

	int GetItemCount(std::string_view _GetName)
	{
		return CheckPickupItemCount(_GetName);
	}

	void RemovePassiveItem();


	void ChangePlayerAnimation(float _Time, std::string_view _Name)
	{
		FullRenderer->ChangeAnimation(_Name);
		FullRenderer->SetActive(true);	

		BodyRenderer->SetActive(false);
		HeadRenderer->SetActive(false);

		IsMovementStopped = true;
		FinalSpeed = FVector2D::ZERO;
		CanFire = false;

		TimeEventer.PushEvent(_Time - 1.5f, [this]() {
			FullRenderer->SetActive(false);
			BodyRenderer->SetActive(true);
			HeadRenderer->SetActive(true);
			IsMovementStopped = false;
			CanFire = true;
			});
	}
	void ChangeHeadRenderer(USpriteRenderer* _HeadRenderer)
	{
		HeadRenderer = _HeadRenderer;
	}
	void UpdateItemPos();
	 
	static int GetTotalHpMax()
	{
		return TotalHeartMax;
	}
	
	// 아이템 눈물 효과
	void InitTear()
	{
		if (nullptr != PassiveItem)
		{
			RemovePassiveItem();
		}
		TearDuration = 0.0f;
		TearScale = FVector2D::ZERO;
		TearCooldown = 0.5f;
		ChaseTearDir = FVector2D::ZERO;
		DetectCollision->SetComponentScale(FVector2D::ZERO);
	}
	float GetTearSpeed() const
	{
		return TearSpeed;
	}
	void AddTearSpeed(const float& _TearSpeed)
	{
		TearSpeed += _TearSpeed;
	}
	float GetTearDuration() const
	{
		return TearDuration;
	}
	void AddTearDuration(const float& _TearDuration)
	{
		TearDuration += _TearDuration;
	}
	
	FVector2D GetTearScale() const
	{
		return TearScale;
	}
	void AddTearScale(const FVector2D& _Scale)
	{
		TearScale = _Scale;
	}

	float GetTearCooldown() const
	{
		return TearCooldown;
	}
	void AddTearCooldown(const float& _TearCooldown)
	{
		TearCooldown -= _TearCooldown;
	}

	// 윤겔라
	void SetDetectCollision()
	{
		DetectCollision->SetComponentScale({ 400, 350 });
		float OffsetX = DetectCollision->GetComponentScale().Half().X;

		DetectCollision->SetComponentLocation({OffsetX, 0.0f});
	}
	void ChaseDirection(AActor* _Monster);
	void ChangeDetectCollisionDirection();

protected:

private:
	FVector2D Force = FVector2D::ZERO;

	// Heart
	static int Heart;
	static int HeartMax;
	static int TotalHeartMax;

	// Stat
	// Speed : MoveAcc
	FVector2D Direction = FVector2D::ZERO;
	FVector2D FinalSpeed = FVector2D::ZERO;
	FVector2D TargetSpeed = FVector2D::ZERO;
	float MoveAcc = 5.0f;
	float Deceleration = 15.0f;
	float MaxSpeed = 370.0f; // Speed
	bool IsMove = false; // 가속도 노멀라이즈를 제어할 변수
	bool IsHit = false;
	float TimeElapsed = 0.0f;
	int Att = 3;

	float InvincibilityDuration = 0.5f; // 무적 지속시간

	FVector2D KnockbackDistance = FVector2D::ZERO;
	FVector2D KnockbackStartPos = FVector2D::ZERO;
	float KnockbackDuration = 0.2f; // 넉백으로 인한 이동 불가 시간
	FVector2D UpPos = FVector2D::ZERO;

	// Death And GameSetting
	FVector2D InitPos = FVector2D::ZERO;
	FVector2D DeathPos = FVector2D::ZERO;
	bool IsDead = false;
	bool Invincibility = false;
	bool IsResetReady = false;
	float FadeValue = 0.0f;
	float FadeWeight = 0.5f;
	float FadeDir = 1.0f;
	float SpiritMoveElapsed = 0.0f;
	int FadeCount = 0;

	void FadeChange();
	void FadeIn();
	void FadeOut();
	void SpiritFadeOut();


	// Item
	std::list<class AItem*> Items;
	std::map<std::string, int> ItemCounts;

	bool IsBombCheat = false;
	float BombCooldown = 0.0f;
	float BombDuration = 1.0f;

	// 아이템으로 눈물 효과 변경을 위해서
	AItem* PassiveItem = nullptr;
	float TearSpeed = 0.0f;
	float TearDuration = 0.0f;
	FVector2D TearScale = FVector2D::ZERO;
	class U2DCollision* DetectCollision = nullptr;

	// TearFire
	float TearCooldown = 0.5f; // 쿨타임
	float TearCoolDownElapsed = 0.0f;
	FVector2D TearDir = FVector2D::ZERO;
	FVector2D ChaseTearDir = FVector2D::ZERO;
	bool TearFire = false;
	bool LeftFire = true;
	bool CanFire = true;

	// Collision
	U2DCollision* BodyCollision = nullptr;
	U2DCollision* WarpCollision = nullptr;

	// Renderer
	class USpriteRenderer* BodyRenderer = nullptr;
	class USpriteRenderer* HeadRenderer = nullptr;
	class USpriteRenderer* FullRenderer = nullptr;
	float StateElapsed = 0.0f;

	// Animation State
	UpperState HeadState = UpperState::IDLE;
	LowerState BodyState = LowerState::IDLE;
	int CurAttackHeadDir = 0;
	float StateElapesd = 0.0f;
	float StateTime = 0.10f;
	int MySpriteIndex = 0;

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
	bool  IsMovementStopped  = false;
	FVector2D CameraMoveDir  = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos   = FVector2D::ZERO;

	USoundPlayer Sound;

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
