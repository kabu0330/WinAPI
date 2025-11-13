# 아이작의 번제(The Binding of Isaac) 모작

## 플레이 영상
<p align="center">
 <img alt="이미지" src="readme\Play.webp">
</p>


Youtube : https://youtu.be/Uf7M80sdum0?si=-n27QgLValSNjPO6

## 프로젝트 소개
* 로그라이크 슈팅 액션 게임
* 몬스터 13종(보스 : 파리공작)
* 맵 13개(입장 방 1, 몬스터 방 7, 보스 방 1, 아이템 방 4 / 단일 스테이지)
* UI
* 타이틀 화면, ESC 화면, 보스 인트로 화면, 데스 리포트 화면

   ![Imgae](https://github.com/user-attachments/assets/3db1956b-8548-427d-9c69-c3942d66c3c3)

<details>
<summary> 입력 키 (클릭) </summary><p>
#### 조작

#### 타이틀
- 타이틀 화면 스킵 : F4
- 화면 넘기기 : Space Bar
- 뒤로가기 : ESC


#### 플레이
- 이동 : W, A, S, D
- 공격 : →, ←, ↑, ↓
- 폭탄 : E
- 일시 정지 : ESC

##### 치트키  
- 무적 / 무적 해제 : F1
- HP Max : F2
- HP -1 : F3
- 게임 리셋 / 다시 시작 : F4
- 폭탄 5개 획득 : J
- 플레이어 즉사 : I
- 현재 맵 몬스터 즉사 : G
- 디버그 모드 : B
- (디버그) 일시 정지 : Z
- 패시브 아이템 생성 : F7 ('Inner Eye' 고정)

</details>

___

## 개요
1. 씬 이동
2. 아이템
3. 떨림 효과
4. 보스 중심으로 회전

## 1. 씬 이동
<p align="center">
 <img alt="이미지" src="readme\Scene2.webp">
</p>

<p align="center">
 <img alt="이미지" src="readme\Scence.png">
</p>

원작의 타이틀 화면 느낌을 구성하기 위해서 타이틀 레벨에 카메라가 한 화면에 보여줄 수 있는 크기를 기준으로 이미지를 이어 붙였고, 각 씬별로 자신과 이어진 다른 씬의 포인터를 저장하고, 키 입력에 따라 연결된 씬의 위치로 카메라를 이동시켰습니다.

<p>
<details>
<summary>코드 보기 (클릭)</summary><p>  

```cpp
//header

enum class TitleSceneDir
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	MAX
};

class ATitleScene : public AActor
{
public:
	// constrcuter destructer
	ATitleScene();
	~ATitleScene();

	// delete Function
	ATitleScene(const ATitleScene& _Other) = delete;
	ATitleScene(ATitleScene&& _Other) noexcept = delete;
	ATitleScene& operator=(const ATitleScene& _Other) = delete;
	ATitleScene& operator=(ATitleScene&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime);

   // 다른 씬과 연결
	bool InterLinkScene(ATitleScene* _Scene, TitleSceneDir _Dir);
	ATitleScene* LinkScene(ATitleScene* _Scene, TitleSceneDir _Dir);
	bool IsLinking(ATitleScene* _Scene);

	TitleSceneDir SetDirection(TitleSceneDir _Dir);
	void SetLocationForLink(ATitleScene* _Scene, TitleSceneDir _Dir);

	void CheckInput();

	void SetFirstScene();
	bool IsLastScene();

	bool MoveToScene(TitleSceneDir _Dir);
	void MoveToCameraInitSetting(ATitleScene*  _Scene, TitleSceneDir _Dir);
	void MoveToCamera(float _DeltaTime);
	bool IsAtDestination();
	void ChangeCurrentScene();

	void ChangeAnimationScene();

protected:

private:
	class USpriteRenderer* TitleRenderer = nullptr;

	std::map<TitleSceneDir, ATitleScene*> Scenes;
	
	static ATitleScene* CurrentScene;
	static ATitleScene* FirstScene;

	FVector2D Force = FVector2D::ZERO;
	FVector2D CameraMoveForce = FVector2D::ZERO;
	FVector2D TargetScenePos = FVector2D::ZERO;
	bool IsCameraMove = false;
	bool IsNextScene = true;

	USoundPlayer Sound;
};
```

```cpp
//cpp
bool ATitleScene::IsLinking(ATitleScene* _Scene)
{
	std::map<TitleSceneDir, ATitleScene*>::iterator StartIter = Scenes.begin();
	std::map<TitleSceneDir, ATitleScene*>::iterator EndIter = Scenes.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		ATitleScene* Scene = StartIter->second;
		if (_Scene == Scene)
		{
			return true;
		}
	}

	return false;
}

bool ATitleScene::InterLinkScene(ATitleScene* _Scene, TitleSceneDir _Dir)
{
	TitleSceneDir Dir = SetDirection(_Dir);

	  this->LinkScene(_Scene, _Dir);
	_Scene->LinkScene(  this,  Dir);

	SetLocationForLink(_Scene, _Dir);

	return true;
}

ATitleScene* ATitleScene::LinkScene(ATitleScene* _Scene, TitleSceneDir _Dir)
{
	if (this == _Scene)
	{
		MSGASSERT("연결하려는 타이틀 씬이 자기 자신입니다. 자기 자신과 연결할 수는 없습니다.");
		return nullptr;
	}
	if (true == IsLinking(_Scene))
	{
		MSGASSERT("이미 연결된 타이틀 씬입니다. 중복하여 연결할 수 없습니다.");
		return nullptr;
	}

	switch (_Dir)
	{
	case TitleSceneDir::LEFT:
	{
		Scenes.insert({ TitleSceneDir::LEFT, _Scene });
		return Scenes.find(TitleSceneDir::LEFT)->second;
	}
	case TitleSceneDir::RIGHT:
	{
		Scenes.insert({ TitleSceneDir::RIGHT, _Scene });
		return Scenes.find(TitleSceneDir::RIGHT)->second;
	}
	case TitleSceneDir::UP:
	{
		Scenes.insert({ TitleSceneDir::UP, _Scene });
		return Scenes.find(TitleSceneDir::UP)->second;
	}
	case TitleSceneDir::DOWN:
	{
		Scenes.insert({ TitleSceneDir::DOWN, _Scene });
		return Scenes.find(TitleSceneDir::DOWN)->second;
	}
	default:
		break;
	}

	return nullptr;
}

void ATitleScene::CheckInput()
{
   // ...
	if (UEngineInput::GetInst().IsDown((VK_SPACE)))
	{
		IsNextScene = true;

		if (true == MoveToScene(TitleSceneDir::RIGHT))
		{
			Sound = UEngineSound::Play("book_page_turn.wav");
			return;
		}
		else if (true == MoveToScene(TitleSceneDir::DOWN))
		{
			Sound = UEngineSound::Play("book_page_turn.wav");
			return;
		}
		
		if(true == IsLastScene())
		{
			bool Result = IsLastScene();
			// Space를 눌러 이동한 씬이 마지막 씬인데 여기서 또 스페이스를 눌렀다면
			// PlayGameMode로 레벨 체인지
			UEngineAPICore::GetCore()->ResetLevel<APlayGameMode, APlayer>("Play");
			UEngineAPICore::GetCore()->OpenLevel("Play");

			UEngineSound::AllSoundStop();
			Sound = UEngineSound::Play("title_screen_jingle_v1_01.ogg");
		}
	}
}

bool ATitleScene::MoveToScene(TitleSceneDir _Dir)
{
	std::map<TitleSceneDir, ATitleScene*>::iterator FindScene = Scenes.find(_Dir);
	if (Scenes.end() == FindScene)
	{
		return false;
	}
	 
	ATitleScene* NextScene = FindScene->second;
	if (nullptr == NextScene)
	{
		return false;
	}
	if (CurrentScene == NextScene)
	{
		return false;
	}
	
	// 카메라 이동값 세팅, Tick에서 카메라 이동시킬 것임
	MoveToCameraInitSetting(NextScene, _Dir);

	return true;
}

void ATitleScene::MoveToCameraInitSetting(ATitleScene* _Scene, TitleSceneDir _Dir)
{
	IsCameraMove = true;

	FVector2D ThisPos = GetActorLocation();
	FVector2D TargetPos = _Scene->GetActorLocation();
	TargetScenePos = TargetPos;
}

void ATitleScene::MoveToCamera(float _DeltaTime)
{
	if (false == IsCameraMove)
	{
		return;
	}

	if (true == IsAtDestination())
	{
		return;
	}
	
	// 속도
	FVector2D WorldPos = GetWorld()->GetCameraPos() + Global::WindowHalfScale;
	FVector2D Direction = TargetScenePos - WorldPos;

	float Speed = 1500.0f;

 	Direction.Normalize();
	CameraMoveForce = Direction * Speed * _DeltaTime;

 	GetWorld()->AddCameraPos(CameraMoveForce);
	FVector2D CameraPos = GetWorld()->GetCameraPos() + Global::WindowHalfScale;
}
```

</details>
<p>

___

## 2. 아이템
<p align="center">
 <img alt="이미지" src="readme\Item00.webp">
</p>

<p align="center">
 <img alt="이미지" src="readme\Item01.webp">
</p>

아이템에 따라 로직 처리를 달리하고, 공격 효과도 변경되도록 구현했습니다.
1. 습득 즉시 아이템 사용
2. 아이템 저장
3. 습득 실패
4. 패시브 효과 적용

아이템 습득 실패 시, 아이템은 반사 벡터에 의해 튕겨져나갑니다.


<p align="center">
 <img alt="이미지" src="readme\ItemPPT.png">
</p>


<p>
<details>
<summary>코드 보기 (클릭)</summary><p>  

1. 아이템
```cpp
//header
enum class EItemType
{
	NONE,
	PASSIVE, // 눈물 효과를 바꿀 아이템
	USE,     // 사용과 함께 즉시 소멸
	MAX
};

// 설명 : 모든 아이템의 속성을 정의하는 클래스
class AItem : public AActor
{
public:
	// constrcuter destructer
	AItem();
	~AItem();

	// delete Function
	AItem(const AItem& _Other) = delete;
	AItem(AItem&& _Other) noexcept = delete;
	AItem& operator=(const AItem& _Other) = delete;
	AItem& operator=(AItem&& _Other) noexcept = delete;

	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
	
   virtual bool EatFunction(APlayer* _Player) = 0;  // 아이템 습득 즉시
	virtual void UseItem(APlayer* _Player) {}       // 아이템 사용
	
	virtual void RemoveRoomData(); // 아이템 습득 시 룸에서 렌더러 삭제

   /** 아이템 습득 로직*/
	void TryPickup(AActor* _Player);
	void PickupSuccess();
	void FailToPickup(class AActor* _Player);

	// Tear를 바꾸는 함수
	virtual void TearFire(APlayer* _Player, FVector2D _TearPos, FVector2D _TearDir, float _PlayerSpeed) {}; // 자식클래스에서 재정의

	void CollisionSetting();
	void Move(float _DeltaTIme);
	void ItemDestroy();

protected:
	EItemType ItemType = EItemType::NONE;
	int ItemCount = 0;
	int Att = 0;

	float DropAnimationDuration = 3.0f;

	float TimeElapsed = 0.0f;
	class ARoom* ParentRoom = nullptr;

	USpriteRenderer* DropRenderer = nullptr;
	USpriteRenderer* DropEffectRenderer = nullptr;
	USpriteRenderer* BodyRenderer = nullptr;

	USpriteRenderer* HeadRenderer = nullptr; // 얼굴 바뀌는 아이템

	U2DCollision* PlayerCollision = nullptr; // 플레이어와 습득 상호작용을 하려면 반드시 만들어야 함
	U2DCollision* MonsterCollision = nullptr;
	U2DCollision* UniversalCollision = nullptr;

	bool IsUseEnd = false;
	bool IsPickedUp = false; // 맵에서 자신의 정보를 삭제
	bool IsDropEffect = false; // 플레이어 위로 번쩍 들어올리는 아이템만 해당
	bool IsPushback = true; // 플레이어 아이템 리스트에 들어갈 것인지.
	bool IsMove = false; // 맵에 등장했을 때 위 아래로 움직일거냐
	bool IsOwnedByPlayer = false; // 위치 플레이어한테 부착


}

```


```cpp
//cpp
void AItem::TryPickup(AActor* _Player)
{
	APlayer* Player = dynamic_cast<APlayer*>(_Player);
	if (nullptr == Player)
	{
		return;
	}
	
	if (false == Player->TryPickupItem(this, ItemCount))
	{
		FailToPickup(Player);
	}
}

void AItem::PickupSuccess()
{
	IsPickedUp = true;
	DropEffect();
	RemoveRoomData();
}

void AItem::FailToPickup(AActor* _Player)
{
 	FVector2D Dir = GetActorLocation() - _Player->GetActorLocation();
	Dir.Normalize(); // 방향벡터

	Force = Dir * 200.0f;
}

void AItem::ReverseForce(float _DeltaTime)
{
	if (FVector2D::ZERO == Force)
	{
		return;
	}

	FVector2D ReverseForce = -Force;
	ReverseForce.Normalize();

	Force += ReverseForce * _DeltaTime * 150.0f;
	
	if (20.0f >= Force.Length())
	{
		Force = FVector2D::ZERO;	
	}

	if (true == IsAtBoundary) // 반사
	{
		FVector2D BoundaryNormal = FVector2D::ZERO;

		if (Force.X > 0) // 오른쪽 경계
		{
			BoundaryNormal = FVector2D(-1.0f, 0.0f);
		}
		else if (Force.X < 0) // 왼쪽 경계
		{
			BoundaryNormal = FVector2D(1.0f, 0.0f);
		}
		else if (Force.Y > 0) // 아래 경계
		{
			BoundaryNormal = FVector2D(0.0f, 1.0f);
		}
		else // 위 경계
		{
			BoundaryNormal = FVector2D(0.0f, -1.0f);
		}

		// 반사 처리
		FVector2D Result =  Reflect(BoundaryNormal);
		Force *= Result.GetNormal();

		IsAtBoundary = false;
	}
	
	FVector2D Result = Force;
	AddActorLocation(Force * _DeltaTime);
}

FVector2D AItem::Reflect(FVector2D _Dir)
{
	FVector2D ReflectedForce = Force - (_Dir * (2.0f * Force.Dot(_Dir)));
	return ReflectedForce;
}
```

2. 캐릭터
```cpp
//cpp
bool APlayer::TryPickupItem(AItem* _Item, const int& _Count)
{
	if (false == _Item->EatFunction(this))
	{
		return false;
	}

	_Item->PickupSuccess();

	if (false == _Item->IsStored()) 
	{
		return true;
	}

	AddItem(_Item, _Count);

	return true;
}

void APlayer::AddItem(AItem* _Item, const int& _Count)
{
	EItemType ItemType = _Item->GetItemType();
	if (EItemType::PASSIVE == ItemType)
	{
		PassiveItem = _Item;
	}

	std::string ItemName = _Item->GetName();
	for (int i = 0; i < _Count; i++)
	{
		Items.push_back(_Item);
		ItemCounts[ItemName]++;
	}
}

void APlayer::InputItem()
{
	AItem* Item = nullptr;
	if (UEngineInput::GetInst().IsDown('E'))
	{
		if (BombCooldown < BombDuration)
		{
			return;
		}

		Item = ReturnItem("Bomb");
		if (nullptr != Item)
		{
			Item->UseItem(this);

			RemveItem(Item);
		}

		BombCooldown = 0.0f;
	}
}
```

</details>
<p>

___


## 3. 떨림 효과

<p align="center">
 <img alt="이미지" src="readme\SineWave.webp">
</p>

사인파를 이용해서 캐릭터 이미지의 Scale 값을 조절해 떨고 있는 듯한 느낌을 주었습니다.


```cpp
//cpp
void ABossIntroScene::Oscillation(float _DeltaTime)
{
	// 진동 속도와 강도 설정
	const float OscillationSpeed = 1000.0f; // 진동 속도
	const float OscillationMagnitude = 5.0f; // 진동 크기

	// 사인 함수를 이용해 진동 계산
	float Oscillation = std::sin(_DeltaTime * OscillationSpeed) * OscillationMagnitude;

	// 스케일 업데이트
	FVector2D NewScale = { 224.0f + Oscillation, 156.0f + Oscillation };
	PlayerRenderer->SetComponentScale(NewScale);
}
```

<p>
<details>
<summary>코드 보기 (클릭)</summary><p>  

</details>
<p>

___



## 4. 보스 중심으로 회전
<p align="center">
 <img alt="이미지" src="readme\Rotator.webp">
</p>

<p align="center">
 <img alt="이미지" src="readme\Rotator.png">
</p>

보스 몬스터가 스폰한 하위 몬스터는 플레이어를 인식하기 전까지 보스 주변을 원을 그리며 이동합니다.




<p>
<details>
<summary>코드 보기 (클릭)</summary><p>  

1. 보스
```cpp
void ATheDukeOfFlies::BeginSummonFliesLogic()
{
	ARoom* ParentRoom = ARoom::GetCurRoom();
	if (nullptr == ParentRoom)
	{
		return;
	}
	if (true == IsDeath())
	{
		return;
	}

	// 스폰 위치
	FVector2D SetFliesPos = this->GetActorLocation() - ParentRoom->GetActorLocation() + FVector2D(0, 100);

	float OrbitRadius = 150.0f;
	const int Count = 3;
	float Angles[Count] = { // 스폰 위치를 각도로 지정
	static_cast<float>(50.0f * std::numbers::pi / 180.0f), 
	static_cast<float>(90.0f * std::numbers::pi / 180.0f), 
	static_cast<float>(130.0f * std::numbers::pi / 180.0f), 
	};

	AAttackFly* ChildFly[Count] = { nullptr, };
	for (int i = 0; i < Count; i++)
	{
		// 부하의 상대 위치 계산
		FVector2D Offset;
		Offset.X = OrbitRadius * cos(Angles[i]);
		Offset.Y = OrbitRadius * sin(Angles[i]);

		// 부하의 최종 위치: SetFliesPos를 기준으로 Offset 추가
		FVector2D SpawnPosition = SetFliesPos + Offset;

		// 부하 생성
		ChildFly[i] = dynamic_cast<AAttackFly*>(ParentRoom->CreateMonster<AAttackFly>(SpawnPosition));
		if (ChildFly[i] == nullptr)
		{
			MSGASSERT("AttackFly 생성 실패");
			return;
		}

		// 부하가 보스를 따라다니도록 설정
		ChildFly[i]->SetIsFollowBoss(this);
		ChildFly[i]->SetInitialAngle(Angles[i]);

		Flies.push_back(ChildFly[i]);
	}

	TimeEventer.PushEvent(SkillPostActionTime, [this]() {
		IsAttacking = false; 
		CooldownElapsed = 0.0f; });
}
```

2. 부하
```cpp
void AAttackFly::FollowBoss(float _DeltaTime)
{
	if (nullptr == ParentBoss)
	{
		IsFollowBoss = false;
		return;
	}

	FVector2D BossPosition = ParentBoss->GetActorLocation();

	// 각도 업데이트
	Angle -= OrbitSpeed * _DeltaTime;

	if (Angle >= static_cast<float>(2 * std::numbers::pi))
	{
		Angle -= static_cast<float>(2 * std::numbers::pi);
	}

	// 목표 위치 계산 (보스 중심의 원형 궤적)
	FVector2D TargetPosition;
	TargetPosition.X = BossPosition.X + OrbitRadius * cos(Angle);
	TargetPosition.Y = BossPosition.Y + OrbitRadius * sin(Angle);

	// 현재 위치 계산
	FVector2D CurrentPosition = GetActorLocation();
	FVector2D Direction = TargetPosition - CurrentPosition;
	float LengthSquared = Direction.X * Direction.X + Direction.Y * Direction.Y;

	// 목표 위치 근처라면 정확히 위치 고정
	if (LengthSquared <= (GetMoveSpeed() * _DeltaTime) * (GetMoveSpeed() * _DeltaTime))
	{
		SetActorLocation(TargetPosition);
		return; // 위치 고정 후 추가 이동 방지
	}

	// 방향 정규화 후 이동
	if (LengthSquared > 0.0f)
	{
		Direction.X /= sqrt(LengthSquared);
		Direction.Y /= sqrt(LengthSquared);
	}

	FVector2D MoveDelta = Direction * GetMoveSpeed() * _DeltaTime;
	AddActorLocation(MoveDelta);
}
```
</details>
<p>

___

