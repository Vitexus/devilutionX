#pragma once

namespace dvl {

void LoadFallbackPalette();
BOOL EndDialog(HWND hDlg, INT_PTR nResult);
BOOL SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
DWORD FormatMessage(DWORD dwFlags, LPCVOID lpSource, DWORD dwMessageId, DWORD dwLanguageId, char *lpBuffer, DWORD nSize, va_list *Arguments);
int DialogBoxParam(HINSTANCE hInstance, LPCSTR msgId, HWND hWndParent, DLGPROC lpDialogFunc, LPARAM dwInitParam);
void UiErrorOkDialog(const char *text, const char *caption, bool error = true);
LPCSTR DVL_MAKEINTRESOURCE(int i);

} // namespace dvl
