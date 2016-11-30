#include <windows.h>

#define WM_INIT     (WM_USER+1000)

HWND Przycisk1;

int x = 250, y = 250, //po³o¿enie...
vx = 1, vy = 1, //prêdkoœæ...
dx = 5, dy = 5; //rozmiary...
const int wie = 200; //wielkoœæ tablicy (snake)
int tab[wie][2] = {0,0};    //tablica z wspó³rzêdnymi po³o¿enia snake

unsigned char R = 255, G = 255, B = 255; //kolor wype³nienia

void paint(HDC hdc, int x, int y, int g)
{
    HPEN hpen = CreatePen(PS_SOLID, 8, RGB(54, g, 150));
    HBRUSH hbrush = CreateSolidBrush(RGB(G, B, B));
    SelectObject(hdc, hpen);
    SelectObject(hdc, hbrush);
    Ellipse(hdc, x, y, x + dx, y + dy);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    DeleteObject(hpen);
    DeleteObject(hbrush);
}



//NOWE
//metoda dodaj¹ca do tablicy wszpó³rzêdne snake
void tabAdd(HDC hdc, int x, int y, HWND hwnd)
{
    for (int i = 1; i < wie-1; i++)
            {
                paint(hdc, tab[i][0], tab[i][1], 1);
            }


    for (int i = wie-1; i > 0; i--) // pêtla po ca³ej tablicy
    {

        tab[i][0] = tab[i-1][0];    //przesuniêcie wspó³rzêdnych snake o jeden w dó³ w tablic
        tab[i][1] = tab[i-1][1];
    }

    tab[0][0] = x;      //przypisanie wspó³rzêdnych g³owy snake do 1 elementu tablicy
    tab[0][1] = y;


}

//FOK: Funkcja Obs³ugi Komunikatów (Okienkowych)
LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
    switch (message)
    {
    case WM_CREATE:
        Przycisk1 = CreateWindowEx(0, "BUTTON", "EXIT", WS_CHILD | WS_VISIBLE, 1, 1, 150, 30, hwnd, NULL, NULL, NULL);
        break;

    case WM_KEYDOWN:
    {

        switch ((int)wp)
        {
        case VK_DOWN:
            vy = 1;
            vx = 0;
            break;
        case VK_UP:
            vy = -1;
            vx = 0;
            break;
        case VK_LEFT:
            vx = -1;
            vy = 0;
            break;
        case VK_RIGHT:
            vx = 1;
            vy = 0;
            break;
        case VK_ESCAPE:
            DestroyWindow(hwnd);
            break;
        }
    }
    break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        paint(ps.hdc, x, y, 255);
        //NOWE
        //wywo³anie metody, która dodaje wspó³rzêdne, które ma snake do tablicy
        tabAdd(ps.hdc, x, y, hwnd);
        //koniec NOWEgo
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_CLOSE:
        KillTimer(hwnd, 1);
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_COMMAND:
        if ((HWND)lp == Przycisk1)
            MessageBox(hwnd, "W³aœnie wy³¹czy³eœ gre :)", "?!", MB_OK);
        exit(lp);
        break;
    case WM_TIMER:
        switch (wp) {
        case 1:
        {
            RECT r;
            GetClientRect(hwnd, &r);
            if (x + vx + dx > (r.right - r.left) || (x + vx) < 0) DestroyWindow(hwnd);
            if (y + vy + dy > (r.bottom - r.top) || (y + vy) < 0) DestroyWindow(hwnd);
            // NOWE
            // sprawdzanie czy snake na siebie wje¿d¿a
            for (int i = 1; i < wie-1; i++)
            {
                if (tab[i][0] == x + vx)
                {
                    if (tab[i][1] == y + vy)
                    {
                        DestroyWindow(hwnd);
                    }
                }
            }
            // koniec NOWEgo
            //przesuniecie o wektor
            x += vx;
            y += vy;
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd);
        }
        break;
        }
        break;
    default:
        return DefWindowProc(hwnd, message, wp, lp);
    }
    return 0;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

    WNDCLASSEX wc;

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.cbClsExtra = 0;
    wc.style = 0;
    wc.lpfnWndProc = wnd_proc;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "application_programming_class";

    RegisterClassEx(&wc);

    HWND window = CreateWindowEx(0, "application_programming_class", "Application programming",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE, 0, 0, 1000, 1000, NULL, NULL, hInstance, NULL);

    if (!window) return -1;

    SetTimer(window, 1, 1, NULL);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 1;
}
