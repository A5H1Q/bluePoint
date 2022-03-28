#include <windows.h>

HHOOK my_hook;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT ch1, ch2, ch3; // Store combobox values
HINSTANCE g_hinst;
void play_pos();
void prev_traq();
void nxt_traq();

LRESULT __stdcall k_Callback1(int Code, WPARAM wParam, LPARAM lParam) {
  PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;

  if (wParam == WM_KEYDOWN && Code == HC_ACTION) {
    switch (key->vkCode) {
    case VK_MEDIA_PLAY_PAUSE:
      play_pos();
      break;
    case VK_MEDIA_PREV_TRACK:
      prev_traq();
      break;
    case VK_MEDIA_NEXT_TRACK:
      nxt_traq();
      break;
    case VK_F10:
      MessageBox(NULL, TEXT(" Application terminated succefully"),
                 TEXT(" bluePoint"),
                 MB_SETFOREGROUND | MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
      exit(0);
      break;
    }
  }
  return CallNextHookEx(NULL, Code, wParam, lParam);
}

int main() {
  HINSTANCE hInstance;
  HWND hwnd;
  MSG msg;
  WNDCLASSW wc = {0};
  wc.lpszClassName = L"bluePoint";
  wc.hInstance = hInstance;
  wc.lpfnWndProc = WndProc;
  g_hinst = hInstance;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.hCursor = LoadCursor(0, IDC_ARROW);
  RegisterClassW(&wc);
  hwnd = CreateWindowW(wc.lpszClassName, L"bluePoint",
                       WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU, 100, 100,
                       350, 370, 0, 0, hInstance, 0);
  LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
  lStyle &= ~(WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
  SetWindowLong(hwnd, GWL_STYLE, lStyle);
  SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 350, 370,
               SWP_NOMOVE | SWP_FRAMECHANGED);
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HFONT hFont;
  static HWND borderBox, comboBox1, comboBox2, comboBox3, label1, label2,
      label3, label4, inputBox, okButton, addButton;
  const wchar_t *items[] = {
      L"None", L"Space bar", L"Left Key", L"Right Key", L" - Add New key",
  };

  switch (msg) {
  case WM_CREATE:
    // Create window controls
    hFont =
        CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
                   OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                   DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
    borderBox = CreateWindowW(L"Button", L" Remappings ",
                              WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 25, 20, 300,
                              250, hwnd, 0, GetModuleHandle(0), 0);
    // SET-1
    label1 = CreateWindowW(L"Static", L"Play / Pause  :", WS_CHILD | WS_VISIBLE,
                           50, 70, 130, 60, hwnd, NULL, g_hinst, NULL);
    comboBox1 =
        CreateWindowW(L"Combobox", NULL,
                      WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_DROPDOWNLIST,
                      160, 70, 140, 300, hwnd, NULL, g_hinst, NULL);

    // SET-2
    label2 = CreateWindowW(L"Static", L"Next Track  :", WS_CHILD | WS_VISIBLE,
                           50, 130, 130, 60, hwnd, NULL, g_hinst, NULL);
    comboBox2 =
        CreateWindowW(L"Combobox", NULL,
                      WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_DROPDOWNLIST,
                      160, 130, 140, 300, hwnd, NULL, g_hinst, NULL);

    // SET-3
    label3 = CreateWindowW(L"Static", L"Prev Track  :", WS_CHILD | WS_VISIBLE,
                           50, 190, 130, 60, hwnd, NULL, g_hinst, NULL);
    comboBox3 =
        CreateWindowW(L"Combobox", NULL,
                      WS_CHILD | WS_VISIBLE | CBS_DROPDOWN | CBS_DROPDOWNLIST,
                      160, 190, 140, 300, hwnd, NULL, g_hinst, NULL);

    // Bottom controls
    okButton = CreateWindowW(L"Button", L"Apply",
                             WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 240, 290,
                             80, 30, hwnd, (HMENU)200, g_hinst, NULL);
    addButton =
        CreateWindowW(L"Button", L"Add Key", WS_CHILD | BS_DEFPUSHBUTTON, 240,
                      290, 80, 30, hwnd, (HMENU)101, g_hinst, NULL);
    label4 =
        CreateWindowW(L"Static", L"©️ ashik saleem", WS_CHILD | WS_VISIBLE,
                      40, 295, 150, 30, hwnd, NULL, g_hinst, NULL);

    inputBox =
        CreateWindowW(L"Edit", L"  Enter V-Key code..", WS_CHILD | WS_BORDER,
                      25, 290, 190, 30, hwnd, NULL, NULL, NULL);

    // Populate comboboxes
    for (int i = 0; i < 5; i++) {
      SendMessageW(comboBox1, CB_ADDSTRING, 0, (LPARAM)items[i]);
      SendMessageW(comboBox2, CB_ADDSTRING, 0, (LPARAM)items[i]);
      SendMessageW(comboBox3, CB_ADDSTRING, 0, (LPARAM)items[i]);
    }

    // Set Default combobox values
    SendMessage(comboBox1, CB_SETCURSEL, (WPARAM)1, (LPARAM)0);
    SendMessage(comboBox2, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);
    SendMessage(comboBox3, CB_SETCURSEL, (WPARAM)2, (LPARAM)0);

    SendMessage(label4, WM_SETFONT, WPARAM(hFont), TRUE);
    // EnableWindow(inputBox, true);
    // SetFocus(inputBox);
    break;

  case WM_COMMAND:

    if (HIWORD(wParam) == BN_CLICKED) {
      if (LOWORD(wParam) == 200) {
        ch1 = SendMessage(comboBox1, CB_GETCURSEL, 0, 0);
        ch2 = SendMessage(comboBox2, CB_GETCURSEL, 0, 0);
        ch3 = SendMessage(comboBox3, CB_GETCURSEL, 0, 0);
        my_hook = SetWindowsHookEx(WH_KEYBOARD_LL, k_Callback1, NULL, 0);
        ShowWindow(hwnd, 0); // Hide window
        MessageBox(NULL,
                   TEXT(" Now listening to keyboard events, Click F10 to Exit"),
                   TEXT(" bluePoint"),
                   MB_SETFOREGROUND | MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
        MSG msg1;
        while (GetMessage(&msg1, NULL, 0, 0) != 0) {
          TranslateMessage(&msg1);
          DispatchMessageW(&msg1);
        }
        if (my_hook)
          UnhookWindowsHookEx(my_hook);
        return TRUE;
      } else {
        TCHAR buff[1024];
        GetWindowText(inputBox, buff, 1024);
        if (ch1 == 4) {
          int n = SendMessage(comboBox1, CB_GETCOUNT, 0, 0);
          SendMessage(comboBox1, CB_ADDSTRING, 0, (LPARAM)TEXT(buff));
          SendMessage(comboBox1, CB_SETCURSEL, (WPARAM)n, (LPARAM)0);
        }
        if (ch2 == 4) {
          int n = SendMessage(comboBox2, CB_GETCOUNT, 0, 0);
          SendMessage(comboBox2, CB_ADDSTRING, 0, (LPARAM)TEXT(buff));
          SendMessage(comboBox2, CB_SETCURSEL, (WPARAM)n, (LPARAM)0);
        }
        if (ch3 == 4) {
          int n = SendMessage(comboBox3, CB_GETCOUNT, 0, 0);
          SendMessage(comboBox3, CB_ADDSTRING, 0, (LPARAM)TEXT(buff));
          SendMessage(comboBox3, CB_SETCURSEL, (WPARAM)n, (LPARAM)0);
        }
        ShowWindow(label4, SW_SHOW);
        ShowWindow(okButton, SW_SHOW);
        ShowWindow(addButton, SW_HIDE);
        ShowWindow(inputBox, SW_HIDE);
      }
    }
    if (HIWORD(wParam) == CBN_SELENDOK) {
      ch1 = SendMessage(comboBox1, CB_GETCURSEL, 0, 0);
      ch2 = SendMessage(comboBox2, CB_GETCURSEL, 0, 0);
      ch3 = SendMessage(comboBox3, CB_GETCURSEL, 0, 0);
      if (ch1 == 4 || ch2 == 4 || ch3 == 4) {
        ShowWindow(label4, SW_HIDE);
        ShowWindow(okButton, SW_HIDE);
        ShowWindow(addButton, SW_SHOW);
        ShowWindow(inputBox, SW_SHOW);
      } else {
        ShowWindow(label4, SW_SHOW);
        ShowWindow(okButton, SW_SHOW);
        ShowWindow(addButton, SW_HIDE);
        ShowWindow(inputBox, SW_HIDE);
      }
    }
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void prev_traq() {
  // Sends Prev event to Active foreground window

  INPUT ps;
  ps.type = INPUT_KEYBOARD;
  ps.ki.wScan = 0;
  ps.ki.time = 0;
  ps.ki.dwExtraInfo = 0;
  ps.ki.wVk = 0x25; // virtual-key code
  ps.ki.dwFlags = 0;
  SendInput(1, &ps, sizeof(INPUT));
  // Release key
  ps.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &ps, sizeof(INPUT));
}

void nxt_traq() {
  // Sends Next event to Active foreground window

  INPUT ns;
  ns.type = INPUT_KEYBOARD;
  ns.ki.wScan = 0;
  ns.ki.time = 0;
  ns.ki.dwExtraInfo = 0;
  ns.ki.wVk = 0x20; // virtual-key code
  ns.ki.dwFlags = 0;
  SendInput(1, &ns, sizeof(INPUT));
  // Release key
  ns.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &ns, sizeof(INPUT));
}

void play_pos() {
  // Sends Play pause event to Active foreground window

  INPUT pp;
  pp.type = INPUT_KEYBOARD;
  pp.ki.wScan = 0;
  pp.ki.time = 0;
  pp.ki.dwExtraInfo = 0;
  pp.ki.wVk = 0x20; // virtual-key code
  pp.ki.dwFlags = 0;
  SendInput(1, &pp, sizeof(INPUT));
  // Release key
  pp.ki.dwFlags = KEYEVENTF_KEYUP;
  SendInput(1, &pp, sizeof(INPUT));
}