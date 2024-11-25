#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/2DCollision.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineSound.h>
#include "Tear.h"
#include "Global.h"
#include "Item.h"

// ���� : ������
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

	// �ֿ� ���� �Լ�
	void CollisionSetting();
	void CollisionFuctionSetting();
	void SpriteSetting();
	void CurStateAnimation(float _DeltaTime);
	void UISetting();
	void UITick(float _DeltaTime);
	void ResetDebug();
	void PlayerLimit();

	// Debug
	void PlayerDebugSetting(float _DeltaTime);
	void UIDebug(float _DeltaTime);


	// �̵�����
	void Move(float _DeltaTime);
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
	void CameraPosMove(float _DeltaTime);
	U2DCollision* GetWarpCollision()
	{
		return WarpCollision;
	}


	// ���� �� �ǰ�
	void InputAttack(float _DeltaTime); // �Է� �Լ�
	void Attack(float _DeltaTime); // ���� ���������� Tear�� �ѱ�� �Լ�

	bool IsAttack() const
	{
		return true == TearFire;
	}
	int  GetAttackDir() const
	{
		return CurAttackHeadDir;
	}
	void SetAttackDir(UpperState _HeadState);

	int ApplyDamaged(AActor* _Player, int _Att, FVector2D _Dir);
	void ShowHitAnimation(AActor* _Other);
	void RestoreDefaultMotion();
	void BeginBlinkEffect();
	void StayBlinkEffect();

	void SetInvincible(bool _OnOff)
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
	void SetRendererDir(EnumType _Dir)
	{
		int Direction = static_cast<int>(_Dir);
		HeadState = static_cast<UpperState>(Direction);
		BodyState = static_cast<LowerState>(Direction);
	}


	// ���, �Ͻ�����, ����� ����
	bool IsDeath();
	void Death(float _DeltaTime);
	void DeathAnimation();
	void SpiritAnimation();
	void ShowDeathReport();
	void Reset();



	// Stat
	static int GetHptMax() 
	{
		return HeartMax;
	}
	static void AddHpMax(int _Value)
	{
		HeartMax += _Value;
	}

	int GetAtt() const
	{
		return Att;
	}

	int AddAtt(int _Value)
	{
		return Att += _Value;
	}

	int GetHp() const
	{
		return Heart;
	}
	void AddHp(int _Value)
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
	void AddMaxSpeed(float _Value)
	{
		MaxSpeed += _Value;
	}
	void SetDir(FVector2D _Dir)
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

	void SetForce(FVector2D _Force)
	{
		Force = _Force;
	}

	void ReverseForce(float _DeltaTime);


	// ������
	bool Drop(class AItem* _Item, int _Count);
	int CheckPickupItemCount(std::string_view _ItemName);
	AItem* ReturnItem(std::string_view _ItemName);

	void InputItem();

	// ������ ����
	int GetItemCount(std::string_view _GetName)
	{
		return CheckPickupItemCount(_GetName);
	}
	void ChangePlayerAnimation(float _Time, std::string_view _Name)
	{
		FullRenderer->ChangeAnimation(_Name);
		FullRenderer->SetActive(true);	

		BodyRenderer->SetActive(false);
		HeadRenderer->SetActive(false);

		IsMovementStopped = true;
		FinalSpeed = FVector2D::ZERO;

		TimeEventer.PushEvent(_Time - 1.5f, [this]() {
			FullRenderer->SetActive(false);
			BodyRenderer->SetActive(true);
			HeadRenderer->SetActive(true);
			IsMovementStopped = false;
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
	
	// ������ ���� ȿ��
	void InitTear()
	{
		TearDuration = 0.0f;
		TearScale = FVector2D::ZERO;
		TearCooldown = 0.5f;
		ChaseTearDir = FVector2D::ZERO;
		DetectCollision->SetComponentScale(FVector2D::ZERO);
	}
	
	void AddTearSpeed(float _TearSpeed)
	{
		TearSpeed += _TearSpeed;
	}
	void AddTearDuration(float _TearDuration)
	{
		TearDuration += _TearDuration;
	}
	void AddTearScale(FVector2D _Scale)
	{
		TearScale = _Scale;
	}

	void AddTearCooldown(float _TearCooldown)
	{
		TearCooldown -= _TearCooldown;
	}

	// ���ֶ�
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
	bool IsMove = false; // ���ӵ� ��ֶ���� ������ ����
	bool IsHit = false;
	float TimeElapsed = 0.0f;
	int Att = 3;

	float InvincibilityDuration = 0.5f; // ���� ���ӽð�

	FVector2D KnockbackDistance = FVector2D::ZERO;
	FVector2D KnockbackStartPos = FVector2D::ZERO;
	float KnockbackDuration = 0.2f; // �˹����� ���� �̵� �Ұ� �ð�
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

	//std::map<EItemType, std::list<class AItem*>> Items;

	// ���������� ���� ȿ�� ������ ���ؼ�
	float TearSpeed = 0.0f;
	float TearDuration = 0.0f;
	FVector2D TearScale = FVector2D::ZERO;
	class U2DCollision* DetectCollision = nullptr;

	// TearFire
	float TearCooldown = 0.5f; // ��Ÿ��
	float TearCoolDownElapsed = 0.0f;
	FVector2D TearDir = FVector2D::ZERO;
	FVector2D ChaseTearDir = FVector2D::ZERO;
	bool TearFire = false;
	bool LeftFire = true;

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

	// ī�޶� �̵����� ���
	float CameraMoveTime = 0.0f;
	float LerpAlpha      = 0.0f;
	bool  IsMovementStopped  = false;
	FVector2D CameraMoveDir  = FVector2D::ZERO;
	FVector2D StartCameraPos = FVector2D::ZERO;
	FVector2D EndCameraPos   = FVector2D::ZERO;

	//BGM
	USoundPlayer BGM;

};
// �Է� ��� 2 : �̺�Ʈ ������� ó��
//void LeftMove(float _DeltaTime);
//void RightMove(float _DeltaTime);
//void UpMove(float _DeltaTime);
//void DownMove(float _DeltaTime);

// ���ǻ��� : �ڱⰡ �ڱ� �ڽ��� SpawnActor�ϸ� ���� ������ ���� ���� �����÷ο�
// �Է� ��� 2 : �̺�Ʈ ������� ó��
//UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
//UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
//UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
//UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));
