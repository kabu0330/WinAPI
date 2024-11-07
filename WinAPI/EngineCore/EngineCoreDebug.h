#pragma once

// 네임스페이스는 프로젝트별로 똑같은 이름이 있어도 
namespace UEngineDebug 	// MainWindow에도 동일한 이름의 네임스페이스가 있다. 
{
	enum class EDebugPosType
	{
		Rect,
		Circle,
	};


	void SetIsDebug(bool _IsDebug);

	void SwitchIsDebug();

	void CoreOutPutString(std::string_view _Text);

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos);

	void CoreDebugRender(FTransform _Trans, EDebugPosType _Type);

	void PrintEngineDebugRender();
}