#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEXA> UEngineWindow::WindowClasss;

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
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
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

    hInstance = _Instance; // 이게 위로 올라가야 하지 않나? 
}

int UEngineWindow::WindowMessageLoop()
{
    // 윈도우 단축키
    // HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));
    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        // 윈도우 단축키는 사용하지 않을 것이므로 TranslateAccelerator 함수는 항상 false임
        if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
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
        // std::string ErrorText = "같은 이름의 윈도우 클래스를 2번 등록했습니다" + std::string(_Class.lpszClassName);
        // std::string 내부에 들고 있는 맴버변수 => std::string => std::vector<char>

        // std::vector<char> char* = new char[100];
        // ErrorText const char* 리턴해주는 함수가 c_str()
        // const char* Text = ErrorText.c_str();
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

void UEngineWindow::Create(std::string_view _ClassName /*= "Default"*/)
{
    Create("Window", _ClassName);
}

void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    // C++ 17 이상 맵에서 지원하는 함수 contains
    // 맵에 저장 여부를 bool로 반환
    if (false == WindowClasss.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + " 등록하지 않은 클래스로 윈도우창을 만들려고 했습니다");
        return;
    }

    // 윈도우 창 하나를 생성해준다.
    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + " 윈도우 생성에 실패했습니다.");
        return;
    }

}

void UEngineWindow::Open(std::string_view _TitleName /*= "Window"*/)
{
    // 처음에는 윈도우 핸들이 nullptr이므로
    if (nullptr == WindowHandle)
    {
        // 윈도우 창 만들어.
        Create();
    }

    // ShowWindow 함수가 호출되면 이제서야 창이 뜬다.
    ShowWindow(WindowHandle, SW_SHOW);
    // ShowWindow(WindowHandle, SW_HIDE);

    UpdateWindow(WindowHandle);
}
