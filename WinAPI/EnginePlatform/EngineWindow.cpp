#include "PreCompile.h"
#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>
#include "EngineWinImage.h"

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
        // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
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

// (UEngineWindow) EngineWindowInit �� CreateWindowClass �� RegisterClassExA �� WindowClasss �� UEngineWindow �� Open �� Create �� ShowWindow �� UpdateWindow
// ������ â Ŭ������ ������ ����
void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    // �� ���α׷��� �ּҸ� ����
    hInstance = _Instance; 

    // ������ â�� �����ϱ� ���� ����ü(Ŭ����)
    WNDCLASSEXA wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;

    // â Ŭ�������� �߻��ϴ� �޽����� ó���� �Լ�
    wcex.lpfnWndProc = WndProc; // �Լ� ������
    // C��Ÿ�Ͽ����� ��ü������ ����� �� �����Ƿ� �Լ� �����ͷ� ȣ������ �� �ۿ� ����. 

    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;                      // ? NULL ����
    wcex.hIcon = nullptr;                            // .rc���Ͽ� ��ϵ� ������ �ε�
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);   // Ŀ�� ���(ȭ��ǥ)
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ������
    wcex.lpszMenuName = nullptr;                     // ������ â�� �޴� �ٸ� ����� �� ������ ����. UI�� ����
    wcex.lpszClassName = "Default";
    wcex.hIconSm = nullptr;                          // â �������� ���Ŀ�

    CreateWindowClass(wcex);

    // â 2�� ���� ����
    //CreateWindowClass(wcex);
}

// ������ �޽��� ����(PeekMessage)
int UEngineWindow::WindowMessageLoop(EngineDelegate _StartFunction, EngineDelegate _FrameFunction)
{
    // ������ ����Ű���� ����
    // HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINDOWSPROJECT2));
    MSG msg = MSG();

    // ���α׷��� �����ϰ� �� �ѹ� �ε�Ǿ�� �� �� : �̹���
    if (true == _StartFunction.IsBind())
    {
        _StartFunction();   
    }

    // �⺻ �޽��� �����Դϴ�:
    while (0 != WindowCount)
    {
        // ������ ����Ű�� ������� ���� ���̹Ƿ� TranslateAccelerator �Լ��� �׻� false��
        // if (!TranslateAccelerator(msg.hwnd, nullptr, &msg))

        if(0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // PM_REMOVE : PeekMessage�� ó���ϸ� �޽��� ť�� ���� �޽��� ����
            // ���� ó������ ���� �޽����� �����ص� ������ ���� : CPU�� 1�ʿ� �� ���� �Ѵ� �޽����� ó���ϱ� ����
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // 1. �޽����� ���� �ð��� �� ���� ������ ������.
        // 2. �޽��� ó���ϰ� ���� �� ���� ������ ������.
        if (true == _FrameFunction.IsBind())
        {
            _FrameFunction(); // �� ���� ����
        }
    }

    return (int)msg.wParam;
}

// â Ŭ������ ����ϰ� WindowClass�� ����
void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& _Class)
{
    // �Ϲ����� map�� ���� 
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasss.find(std::string(_Class.lpszClassName));
    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasss.end();

    // C++ 17���� �̻󿡼��� bool�� ��ȯ���ִ� contains �Լ��� �ִ�.
    // find �Լ��� ã�ٰ� ������ end�� ��ȯ�Ѵ�.
    if (EndIter != FindIter)
    {
        MSGASSERT(std::string(_Class.lpszClassName) + " ���� �̸��� ������ Ŭ������ 2�� ����߽��ϴ�");
        return;
    }

    // â Ŭ���� ���
    RegisterClassExA(&_Class);

    // �̹� ��ϵ� �̸��� â Ŭ������ �ƴ϶��, map�� ����
    WindowClasss.insert(std::pair{ _Class.lpszClassName, _Class });
}

UEngineWindow::UEngineWindow()
{

}

UEngineWindow::~UEngineWindow()
{
    if (nullptr != WindowImage)
    {
        delete WindowImage;
        WindowImage = nullptr;
    }
    if (nullptr != BackBufferImage)
    {
        delete BackBufferImage;
        BackBufferImage = nullptr;
    }
}

void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    // C++ 17 �̻� �ʿ��� �����ϴ� �Լ� contains
    // �ʿ� ���� ���θ� bool�� ��ȯ
    //bool a = WindowClasss.contains(_ClassName.data());
    if (false == WindowClasss.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + " ������� ���� Ŭ������ ������â�� ������� �߽��ϴ�");
        return;
    }

    // ������ â �ϳ��� �������ش�.
    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + " ������ ������ �����߽��ϴ�.");
        return;
    }

    // GetDC�� ������ ȭ�鿡 HDC�� �����츦 �����Ѵ�.
    // ������ ȭ�鿡 �׸� HDC�� ���⼭ �����Ѵ�.
    //BackBuffer = GetDC(WindowHandle);
    HDC WindowMainDC = GetDC(WindowHandle);

    // ��¥ ������ ���۰� �����Ǵ� ����
    WindowImage = new UEngineWinImage();
    WindowImage->Create(WindowMainDC);

}

void UEngineWindow::Open(std::string_view _TitleName /*= "Window"*/)
{
    // ó������ ������ �ڵ��� nullptr�̹Ƿ�
    if (nullptr == WindowHandle)
    {
        // ������ â �����.
        Create(_TitleName);
    }

    if (0 == WindowHandle)
    {
        return;
    }

    // ShowWindow �Լ��� ȣ��Ǹ� �������� â�� ���.
    ShowWindow(WindowHandle, SW_SHOW);
    // ShowWindow(WindowHandle, SW_HIDE);

    UpdateWindow(WindowHandle);
    ++WindowCount;
}

void UEngineWindow::SetWindowPosAndScale(FVector2D _Pos, FVector2D _Scale)
{
    // ������ ����� �ٲ�� ������� ũ�⵵ �ٲ��.
    if (false == WindowSize.EqualToInt(_Scale))
    {
        // ���� ����۸� �����
        if (nullptr != BackBufferImage)
        {
            delete BackBufferImage;
            BackBufferImage = nullptr;
        }

        // ������ ũ��� ������ ���ο� ����۸� �����.
        BackBufferImage = new UEngineWinImage();
        BackBufferImage->Create(WindowImage, _Scale);
    }

    WindowSize = _Scale;

    // Ÿ��Ʋ �޴��� ũ�⸦ ȭ�� ũ�⿡�� ���ܽ�Ű�� ���� �۾��� �Ѵ�.
    RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };

    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);

    ::SetWindowPos(WindowHandle, nullptr, _Pos.iX(), _Pos.iY(), Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER);

}
