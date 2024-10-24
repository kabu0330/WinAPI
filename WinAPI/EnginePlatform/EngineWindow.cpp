#include "PreCompile.h"
#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEXA> UEngineWindow::WindowClasss;
int WindowCount = 0;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        //PostQuitMessage(0);
        --WindowCount;
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// (UEngineWindow) EngineWindowInit → CreateWindowClass → RegisterClassExA → WindowClasss → UEngineWindow → Open → Create → ShowWindow → UpdateWindow
void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    hInstance = _Instance; 

    // 윈도우 창 클래스의 구성을 정의
    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc; // 함수 포인터
    // C스타일에서는 객체지향을 사용할 수 없으므로 함수 포인터로 호출해줄 수 밖에 없다. 
    // 창 클래스에서 발생하는 메시지를 처리할 함수

    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;                      // ? NULL 상태
    wcex.hIcon = nullptr;                            // .rc파일에 등록된 정보를 로드
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);   // 커서 모양(화살표)
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 배경색상
    wcex.lpszMenuName = nullptr;                     // 윈도우 창에 메뉴 바를 만들어 둔 게임은 없다. UI를 쓰지
    wcex.lpszClassName = "Default";
    wcex.hIconSm = nullptr;                          // 창 아이콘은 추후에

    CreateWindowClass(wcex);

    // 창 2개 띄우면 에러
    //CreateWindowClass(wcex);
}

int UEngineWindow::WindowMessageLoop(EngineDelegate _StartFunction, EngineDelegate _FrameFunction)
{
    // 윈도우 단축키
    // HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));
    MSG msg = MSG();

    // 프로그램이 시작하고 딱 한번 로드되어야 할 것 : 이미지
    if (true == _StartFunction.IsBind())
    {
        _StartFunction();   
    }

    // 기본 메시지 루프입니다:
    while (0 != WindowCount)
    {
        // 윈도우 단축키는 사용하지 않을 것이므로 TranslateAccelerator 함수는 항상 false임
        // if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))

        if(0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // PM_REMOVE : PeekMessage가 처리하면 메시지 큐에 쌓인 메시지 삭제
            // 아직 처리하지 못한 메시지를 삭제해도 괜찮은 이유 : CPU가 1초에 만 개가 넘는 메시지를 처리하기 때문
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 1. 메시지가 없는 시간에 내 게임 엔진을 돌린다.
        // 2. 메시지 처리하고 나서 내 게임 엔진을 돌린다.
        if (true == _FrameFunction.IsBind())
        {
            _FrameFunction(); // 내 게임 엔진
        }
    }

    return (int)msg.wParam;
}

void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& _Class)
{
    // 일반적인 map의 사용법 
    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasss.end();
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasss.find(std::string(_Class.lpszClassName));

    // C++ 17버전 이상에서는 bool로 반환해주는 contains 함수가 있다.
    // find 함수로 찾다가 없으면 end를 반환한다.
    if (EndIter != FindIter)
    {
        MSGASSERT(std::string(_Class.lpszClassName) + " 같은 이름의 윈도우 클래스를 2번 등록했습니다");
        return;
    }

    // 창 클래스 등록
    RegisterClassExA(&_Class);

    // 이미 등록된 이름의 창 클래스가 아니라면, map에 저장
    WindowClasss.insert(std::pair{ _Class.lpszClassName, _Class });
}

UEngineWindow::UEngineWindow()
{

}

UEngineWindow::~UEngineWindow()
{
}

void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    // C++ 17 이상 맵에서 지원하는 함수 contains
    // 맵에 저장 여부를 bool로 반환
    //bool a = WindowClasss.contains(_ClassName.data());
    if (false == WindowClasss.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + " 등록하지 않은 클래스로 윈도우창을 만들려고 했습니다");
        return;
    }

    // 윈도우 창 하나를 생성해준다.
    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + " 윈도우 생성에 실패했습니다.");
        return;
    }

    // GetDC는 윈도우 화면에 HDC와 윈도우를 연결한다.
    // 윈도우 화면에 그릴 HDC는 여기서 생성한다.
    BackBuffer = GetDC(WindowHandle);
}

void UEngineWindow::Open(std::string_view _TitleName /*= "Window"*/)
{
    // 처음에는 윈도우 핸들이 nullptr이므로
    if (nullptr == WindowHandle)
    {
        // 윈도우 창 만들어.
        Create("Window");
    }

    if (0 == WindowHandle)
    {
        return;
    }

    // ShowWindow 함수가 호출되면 이제서야 창이 뜬다.
    ShowWindow(WindowHandle, SW_SHOW);
    // ShowWindow(WindowHandle, SW_HIDE);

    UpdateWindow(WindowHandle);
    ++WindowCount;
}
