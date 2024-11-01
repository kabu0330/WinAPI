#include "PreCompile.h"
#include "Player.h"
#include <string>
#include <vector>

#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include "Tear.h"
#include "Global.h"
#include "ContentsEnum.h"

void APlayer::RunSoundPlay()
{
	UEngineDebug::OutPutString("SoundPlay");
}

APlayer::APlayer()
{
	// 1. Actor의 위치는 의미가 있어도 크기는 의미가 없다.
	SetActorLocation(Global::WindowSize.Half());

	// 2. 상태에 따른 애니메이션 동작을 정의한다.
	AnimationSetting();

	// 3. 캐릭터의 이동영역을 지정할 충돌체를 생성한다.
 
	// 4. 캐릭터의 히트박스를 설정할 충돌체를 생성한다.
	// CreateDefaultSubObject<U2DCollision>();
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	Super::BeginPlay();

	// 직접 카메라 피봇을 설정해줘야 한다.
	//FVector2D Size = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	//GetWorld()->SetCameraPivot(Size.Half() * -1.0f);

	// 주의사항 : 자기가 자기 자신을 SpawnActor하면 무한 스폰에 빠져 스택 오버플로우
	// 입력 방법 2 : 이벤트 방식으로 처리
	//UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));

}

void APlayer::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	EngineDebug();

	// 입력 방법 1
	if (true == UEngineInput::GetInst().IsPress('A'))
	{
		BodyRenderer->ChangeAnimation("Body_Left");
		HeadRenderer->ChangeAnimation("Head_Left");
		AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('D'))
	{
		BodyRenderer->ChangeAnimation("Body_Right");
		HeadRenderer->ChangeAnimation("Head_RIght");
		AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('W'))
	{
		BodyRenderer->ChangeAnimation("Body_Up");
		HeadRenderer->ChangeAnimation("Head_Up");
		AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
	}
	if (true == UEngineInput::GetInst().IsPress('S'))
	{
		BodyRenderer->ChangeAnimation("Body_Down");
		HeadRenderer->ChangeAnimation("Head_Down");
		AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	}

	if (false == UEngineInput::GetInst().IsPress('A') &&
		false == UEngineInput::GetInst().IsPress('D') &&
		false == UEngineInput::GetInst().IsPress('W') &&
		false == UEngineInput::GetInst().IsPress('S'))
	{
		BodyRenderer->ChangeAnimation("Body_Idle");
		HeadRenderer->ChangeAnimation("Head_Down");
	}

	// 대각선 이동은 어떻게 할 것인가?

	if (1.0f < UEngineInput::GetInst().IsPressTime(VK_SPACE))
	{
		// 플레이어가 속한 레벨에 Bullet을 생성한다.
		ATear* Ptr = GetWorld()->SpawnActor<ATear>();
		Ptr->SetActorLocation(GetActorLocation());
		return;
	}

	//if (true == UEngineInput::GetInst().IsDown('R'))
	//{
	//	SetSprite("icon.png", MySpriteIndex);
	//	++MySpriteIndex;
	//}
}

void APlayer::AnimationSetting()
{
	// 1. 헤더에 랜더러를 하나 만든다.
	// 2. CreateDefualtSubObject 함수를 사용하여 렌더러 컴포넌트를 만든다.
	// 3. CreateAnimation 함수를 사용하여 ("동작 이름", "이미지.png", 동작순서, 프레임)을 설정한다.
	// 4. SetComponentScale 함수를 사용하여 렌더러 컴포넌트의 크기를 정의한다.
	// 5. ChanageAnimation 함수를 사용하여 기본 동작을 정의한다.
	// 6. SetOrder로 정렬 순서를 정할 수 있다.

	BodyRenderer = CreateDefaultSubObject<USpriteRenderer>();
	BodyRenderer->CreateAnimation("Body_Left", "Body.png", 1, 9, 0.1f);
	BodyRenderer->CreateAnimation("Body_Right", "Body.png", 10, 19, 0.1f);
	BodyRenderer->CreateAnimation("Body_Down", "Body.png", 20, 29, 0.1f);
	BodyRenderer->CreateAnimation("Body_Up", "Body.png", { 29, 28, 27, 26, 25, 24, 23, 22, 21, 20 }, { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f });
	BodyRenderer->CreateAnimation("Body_Idle", "Body.png", 29, 29, 0.1f);
	BodyRenderer->SetComponentScale({ 45, 45 });
	BodyRenderer->ChangeAnimation("Body_Idle");

	HeadRenderer = CreateDefaultSubObject<USpriteRenderer>();
	HeadRenderer->CreateAnimation("Head_Left", "Head.png", 0, 1, 0.5f);
	HeadRenderer->CreateAnimation("Head_Right", "Head.png", 2, 3, 0.5f);
	HeadRenderer->CreateAnimation("Head_Down", "Head.png", 6, 7, 0.5f);
	HeadRenderer->CreateAnimation("Head_Up", "Head.png", 4, 5, 0.5f);
	HeadRenderer->SetComponentLocation({ 0, -BodyRenderer->GetComponentScale().Half().iY() + 3 });
	HeadRenderer->SetComponentScale({ 45, 45 });
	HeadRenderer->ChangeAnimation("Head_Down");


	BodyRenderer->SetOrder(ERenderOrder::PLAYER);
	HeadRenderer->SetOrder(ERenderOrder::PLAYER);
	//SpriteRenderer->SetComponentLocation({ -100, 0 });


	//SpriteRenderer->CreateAnimation("Run_Right", "Player_Right.png", { 2, 3, 4 }, { 0.1f, 0.1f, 0.1f });
	//SpriteRenderer->ChangeAnimation("Idle_Right");
	//SpriteRenderer->ChangeAnimation("Idle_Right");
}

void APlayer::EngineDebug()
{
	UEngineDebug::CoreOutPutString("Player : " + GetActorLocation().ToString());

	if (true == UEngineInput::GetInst().IsDown('B'))
	{
		UEngineDebug::SwitchIsDebug();
	}
}

// 입력 방법 2 : 이벤트 방식으로 처리
//void APlayer::LeftMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
//}
//
//void APlayer::RightMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
//}
//
//void APlayer::UpMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
//}
//
//void APlayer::DownMove(float _DeltaTime)
//{
//	AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
//}

