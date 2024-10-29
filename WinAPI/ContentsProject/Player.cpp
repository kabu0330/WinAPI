#include "PreCompile.h"
#include "Player.h"
#include <EnginePlatform/EngineInput.h>
#include "Tear.h"

APlayer::APlayer()
{
	SetActorLocation({ 300, 300 });
	SetActorScale({ 100, 100 });
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	// 주의사항 : 자기가 자기 자신을 SpawnActor하면 무한 스폰에 빠져 스택 오버플로우
	// 입력 방법 2 : 이벤트 방식으로 처리
	//UEngineInput::GetInst().BindAction('A', KeyEvent::PRESS, std::bind(&APlayer::LeftMove , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('D', KeyEvent::PRESS, std::bind(&APlayer::RightMove, this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('W', KeyEvent::PRESS, std::bind(&APlayer::UpMove   , this, std::placeholders::_1));
	//UEngineInput::GetInst().BindAction('S', KeyEvent::PRESS, std::bind(&APlayer::DownMove , this, std::placeholders::_1));

}

void APlayer::Tick(float _DeltaTime)
{
	//this;
	// 입력 방법 1
	//if (true == UEngineInput::GetInst().IsPress('A'))
	//{
	//	AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
	//}
	//if (true == UEngineInput::GetInst().IsPress('D'))
	//{
	//	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
	//}
	//if (true == UEngineInput::GetInst().IsPress('W'))
	//{
	//	AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
	//}
	//if (true == UEngineInput::GetInst().IsPress('S'))
	//{
	//	AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
	//}

	if (1.0f < UEngineInput::GetInst().IsPressTime(VK_SPACE))
	{
		// 플레이어가 속한 레벨에 Bullet을 생성한다.
        ATear* Ptr = GetWorld()->SpawnActor<ATear>();
		Ptr->SetActorLocation(GetActorLocation());
		return;
	}
}

// 입력 방법 2 : 이벤트 방식으로 처리
void APlayer::LeftMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::LEFT * _DeltaTime * Speed);
}

void APlayer::RightMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::RIGHT * _DeltaTime * Speed);
}

void APlayer::UpMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::UP * _DeltaTime * Speed);
}

void APlayer::DownMove(float _DeltaTime)
{
	AddActorLocation(FVector2D::DOWN * _DeltaTime * Speed);
}

