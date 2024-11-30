#include "PreCompile.h"
#include "TitleScene.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include "Global.h"
#include "ContentsEnum.h"
#include "Player.h"
#include "PlayGameMode.h"

ATitleScene* ATitleScene::FirstScene  = nullptr;
ATitleScene* ATitleScene::CurrentScene  = nullptr;

ATitleScene::ATitleScene()
{
	SetActorLocation(Global::WindowSize.Half());

	TitleRenderer = CreateDefaultSubObject<USpriteRenderer>();
	TitleRenderer->SetOrder(ERenderOrder::Scene);
	TitleRenderer->SetComponentScale(Global::WindowSize);
	TitleRenderer->CreateAnimation("MainTitleScene", "MainTitle", 0, 7, 0.2f);
	TitleRenderer->CreateAnimation("SaveFileScene", "SaveFile", 0, 1, 0.2f);
	TitleRenderer->CreateAnimation("SelectScene", "InGameSelectScene.png", 0, 0, 0.2f, false);
	TitleRenderer->CreateAnimation("CharacterSelectScene", "InGameCharacterSelectScene.png", 0, 0, 0.2f, false);


	TitleRenderer->ChangeAnimation("MainTitleScene");
}

void ATitleScene::ChangeAnimationScene()
{
	std::string ThisName = GetName();
	if ("SaveFileScene" == ThisName)
	{
		TitleRenderer->ChangeAnimation("SaveFileScene");
	}
	else if ("SelectScene" == ThisName)
	{
		TitleRenderer->ChangeAnimation("SelectScene");
	}
	else if ("CharacterSelectScene" == ThisName)
	{
		TitleRenderer->ChangeAnimation("CharacterSelectScene");
	}
}

void ATitleScene::BeginPlay()
{
	Super::BeginPlay();
	SetFirstScene();

	ChangeAnimationScene();
}

void ATitleScene::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	CheckInput();
	MoveToCamera(_DeltaTime);
}

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
	// 서로 연결될 방향은 반대방향
	TitleSceneDir Dir = SwitchTitleSceneDir(_Dir);

	  this->LinkScene(_Scene, _Dir);
	_Scene->LinkScene(  this,  Dir);


	// 연결된 씬을 this를 기준으로 위치를 조정한다.
	SetSceneLocation(_Scene, _Dir);

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

TitleSceneDir ATitleScene::SwitchTitleSceneDir(TitleSceneDir _Dir)
{
	TitleSceneDir Dir = TitleSceneDir::NONE;
	switch (_Dir)
	{
	case TitleSceneDir::LEFT:
		Dir = TitleSceneDir::RIGHT;
		break;

	case TitleSceneDir::RIGHT:
		Dir = TitleSceneDir::LEFT;
		break;

	case TitleSceneDir::UP:
		Dir = TitleSceneDir::DOWN;
		break;
	case TitleSceneDir::DOWN:
		Dir = TitleSceneDir::UP;
		break;

	default:
		break;
	}
	return Dir;
}

void ATitleScene::SetSceneLocation(ATitleScene* _Scene, TitleSceneDir _Dir)
{
	ATitleScene* Scene = _Scene;
	FVector2D Anchor = GetActorLocation();
	FVector2D Scale = TitleRenderer->GetComponentScale();

	TitleSceneDir Dir = _Dir;	
	switch (Dir)
	{
	case TitleSceneDir::LEFT:
	{
		Scene->SetActorLocation({ Anchor.X - Scale.X, Anchor.Y });
		break;
	}
	case TitleSceneDir::RIGHT:
	{
		Scene->SetActorLocation({ Anchor.X + Scale.X, Anchor.Y });
		break;
	}
	case TitleSceneDir::UP:
	{
		Scene->SetActorLocation({ Anchor.X, Anchor.Y - Scale.Y });
		break;
	}
	case TitleSceneDir::DOWN:
	{
		Scene->SetActorLocation({ Anchor.X, Anchor.Y + Scale.Y });
		break;
	}
	default:
		break;
	}
}

void ATitleScene::CheckInput()
{
	if (this != CurrentScene)
	{
		return;
	}
	if (true == IsCameraMove)
	{
		return;
	}

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
	if (UEngineInput::GetInst().IsDown((VK_ESCAPE)))
	{
		// 첫번째 타이틀 화면
		if (FirstScene == this)
		{
			return;
		}

		IsNextScene = false;

		if (true == MoveToScene(TitleSceneDir::LEFT))
		{
			Sound = UEngineSound::Play("book_page_turn.wav");
			return;
		}
		else if (true == MoveToScene(TitleSceneDir::UP))
		{
			Sound = UEngineSound::Play("book_page_turn.wav");
			return;
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
	float Distance = Direction.Length();

	float Deceleration = 1000.0f;
	float Speed = UEngineMath::Max(Distance / Deceleration, 3.0f);

 	Direction.Normalize();
	CameraMoveForce = Direction * Speed;

	// 이동 로직
 	GetWorld()->AddCameraPos(CameraMoveForce);
	FVector2D CameraPos = GetWorld()->GetCameraPos() + Global::WindowHalfScale;
	int a = 0;
}

bool ATitleScene::IsAtDestination()
{
	FVector2D WorldPos = GetWorld()->GetCameraPos() + Global::WindowHalfScale;

	float Distance = FVector2D::Distance(TargetScenePos, WorldPos);

	// 목적지에 도착했어?
	if (Distance <= 10.0f)
	{
		// 키입력 받을 현재 씬 바꿔.
		ChangeCurrentScene();

		GetWorld()->SetCameraPos(TargetScenePos - Global::WindowHalfScale);

		// 초기화
		IsCameraMove = false;
		CameraMoveForce = FVector2D::ZERO;
		TargetScenePos = FVector2D::ZERO; 
		return true;
	}

	return false;
}

void ATitleScene::ChangeCurrentScene()
{
	if (true == IsNextScene)
	{
		std::map<TitleSceneDir, ATitleScene*>::iterator RightScene = Scenes.find(TitleSceneDir::RIGHT);
		std::map<TitleSceneDir, ATitleScene*>::iterator DownScene = Scenes.find(TitleSceneDir::DOWN);
		std::map<TitleSceneDir, ATitleScene*>::iterator EndIter = Scenes.end();

		if (EndIter != RightScene)
		{
			CurrentScene = RightScene->second;
		}
		else if (EndIter != DownScene)
		{
			CurrentScene = DownScene->second;
		}
	}
	else if (false == IsNextScene)
	{
		std::map<TitleSceneDir, ATitleScene*>::iterator LeftScene = Scenes.find(TitleSceneDir::LEFT);
		std::map<TitleSceneDir, ATitleScene*>::iterator UpScene = Scenes.find(TitleSceneDir::UP);
		std::map<TitleSceneDir, ATitleScene*>::iterator EndIter = Scenes.end();

		if (EndIter != LeftScene)
		{
			CurrentScene = LeftScene->second;
		}
		else if (EndIter != UpScene)
		{
			CurrentScene = UpScene->second;
		}
	}

}

void ATitleScene::SetFirstScene()
{
	if (nullptr == CurrentScene)
	{
		FirstScene = this;
		CurrentScene = this;
	}
}

bool ATitleScene::IsLastScene()
{
	ATitleScene* CurScene = this;

	if (CurrentScene != CurScene)
	{
		return false;
	}
	// 내가 현재 씬이라면, 내 다음이 있느냐
	// 오른쪽과 아래쪽 씬 둘 다 없다면 마지막 씬으로 판단

	std::map<TitleSceneDir, ATitleScene*>::iterator RightScene = CurScene->Scenes.find(TitleSceneDir::RIGHT);
	std::map<TitleSceneDir, ATitleScene*>::iterator DownScene = CurScene->Scenes.find(TitleSceneDir::DOWN);
	std::map<TitleSceneDir, ATitleScene*>::iterator EndIter = CurScene->Scenes.end();
		
	// 오른쪽에도, 아래쪽에도 연결된 포인터가 없다면 CurrentScene인 나는 마지막 씬
	if (EndIter == RightScene && EndIter == DownScene)
	{
		return true;
	}
	else // 내 오른쪽이든 아래쪽이든 연결된 포인터가 있다면 나는 마지막 씬이 아님
	{
		return false;
	}
	
}

ATitleScene::~ATitleScene()
{

}
