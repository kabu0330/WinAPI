#pragma once

// 설명 : 게임 엔진 로직 저장 및 실행
class EngineDelegate
{
public:
	EngineDelegate();
	
	// 생성자 호출하면서 바로 함수 포인터를 넣을 수 있다.
	EngineDelegate(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	~EngineDelegate();


	void operator()()
	{
		std::list<std::function<void()>>::iterator StartIter = Functions.begin();
		std::list<std::function<void()>>::iterator EndIter = Functions.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			std::function<void()> Function = *StartIter;
			Function();
		}
	}

	void operator+=(std::function<void()> _Function)
	{
		Functions.push_back(_Function);
	}

	// 함수 포인터가 하나라도 있으면 true, 없으면 false 반환
	bool IsBind()
	{
		return false == Functions.empty();
	}

protected:

private:
	std::list<std::function<void()>> Functions;
};

