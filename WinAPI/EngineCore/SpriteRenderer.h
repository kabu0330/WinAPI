#pragma once
#include <map>
#include <EngineBase/EngineDelegate.h>
#include "SceneComponent.h"
#include "EngineSprite.h"

// 설명 : 화면에 이미지를 띄우기 위한 모든 기능을 수행
// 이미지, 스프라이트, 애니메이션 모두 구분없이 렌더러 하나로 통합한다.
class USpriteRenderer : public USceneComponent
{
public:
	class FrameAnimation
	{
	public:
		UEngineSprite* Sprite = nullptr;
		//int Start = 0;
		//int End = 0;

		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;
		std::map<int, EngineDelegate> Events;

		int CurIndex = 0;
		int ResultIndex = 0;
		float CurTime = 0.0f;
		bool Loop = true;

		void Reset()
		{
			CurIndex = 0;
			CurTime = 0;
			ResultIndex = 0;
		}
	};

public:
	// constrcuter destructer
	USpriteRenderer();
	~USpriteRenderer();

	// delete Function
	USpriteRenderer(const USpriteRenderer& _Other) = delete;
	USpriteRenderer(USpriteRenderer&& _Other) noexcept = delete;
	USpriteRenderer& operator=(const USpriteRenderer& _Other) = delete;
	USpriteRenderer& operator=(USpriteRenderer&& _Other) noexcept = delete;

	void Render(float _DeltaTime);
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order);

	int GetOrder()
	{
		return Order;
	}

	FVector2D SetSpriteScale(float _Ratio = 1.0f, int _CurIndex = 0);

	void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, int _Start, int _End, float Time = 0.1f, bool _Loop = true);

	// _AnimationName 이름의 애니메이션을 만들어라.
	// _SpriteName 이 이름의 스프라이트로
	// _Indexs 프레임은 이녀석들을 사용해서   0     1      2     3     4     5 
	// _Frame 이 시간을 드려서                 0.1    0.1    0.1   0.1  0.1   0.1
	// _Loop = true면 반복 false면 마지막 프레임에서 정지
	void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, std::vector<float> _Frame, bool _Loop = true);

	void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, float _Frame, bool _Loop = true);

	// 내가 Idle인데 Idle 바꾸라고 했다. 
	void ChangeAnimation(std::string_view _AnimationName, bool _Force = false);
	
	void SetAnimationEvent(std::string_view _AnimationName, int _Frame, std::function<void()> _Function);


protected:

public:
	int Order = 0;
	int CurIndex = 0;

	class UEngineSprite* Sprite = nullptr;

	void SetSprite(std::string_view _Name, int _CurIndex = 0);

	std::map<std::string, FrameAnimation> FrameAnimations;
	FrameAnimation* CurAnimation = nullptr;

};

