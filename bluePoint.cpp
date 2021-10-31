#include <windows.h>

HHOOK my_hook;
void prev_slide();
void nxt_slide();

LRESULT __stdcall k_Callback1(int Code, WPARAM wParam, LPARAM lParam) {
  PKBDLLHOOKSTRUCT key = (PKBDLLHOOKSTRUCT)lParam;

  if (wParam == WM_KEYDOWN && Code == HC_ACTION) {
    switch (key->vkCode) {
    case VK_MEDIA_PLAY_PAUSE:
      nxt_slide();
      break;
    case VK_MEDIA_PREV_TRACK:
      prev_slide();
      break;
    case VK_MEDIA_NEXT_TRACK:
      prev_slide();
      break;
    }
  }

  return CallNextHookEx(NULL, Code, wParam, lParam);
}
void prev_slide() {
  // Sends "LEFT ARROW" key to Active foreground window

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

void nxt_slide() {
  // Sends "SPACE" key to Active foreground window

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

int main() {
  HWND Stealth;
  AllocConsole();
  Stealth = FindWindowA("ConsoleWindowClass", NULL);
  ShowWindow(Stealth, 0); // Hide Console window
  my_hook = SetWindowsHookEx(WH_KEYBOARD_LL, k_Callback1, NULL, 0);
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0) != 0) {
    TranslateMessage(&msg);
    DispatchMessageW(&msg);
  }
  if (my_hook)
    UnhookWindowsHookEx(my_hook);
  return TRUE;
}