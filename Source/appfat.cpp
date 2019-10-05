#include "diablo.h"
#include "../3rdParty/Storm/Source/storm.h"
#include <string>
#include <config.h>

DEVILUTION_BEGIN_NAMESPACE

char sz_error_buf[256];
BOOL terminating;
int cleanup_thread_id;

char *GetErrorStr(DWORD error_code)
{
	DWORD upper_code;
	int size;
	char *chr;

	upper_code = (error_code >> 16) & 0x1FFF;
	if (upper_code == 0x0878) {
		TraceErrorDS(error_code, sz_error_buf, 256);
	} else if (upper_code == 0x0876) {
		TraceErrorDD(error_code, sz_error_buf, 256);
	} else if (!SErrGetErrorStr(error_code, sz_error_buf, 256)
	    && !FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_code, 0x400, sz_error_buf, 0x100, NULL)) {
		wsprintf(sz_error_buf, "unknown error 0x%08x", error_code);
	}

	size = strlen(sz_error_buf);

	chr = &sz_error_buf[size - 1];
	while (size > 0) {
		size--;
		chr--;

		if (*chr != '\r' && *chr != '\n')
			break;

		*chr = 0x00;
	}

	return sz_error_buf;
}

void TraceErrorDD(HRESULT hError, char *pszBuffer, DWORD dwMaxChars)
{
	const char *szError;

	switch (hError) {
	case DD_OK:
		szError = "DD_OK";
		break;
	case DDERR_INVALIDOBJECT:
		szError = "DDERR_INVALIDOBJECT";
		break;
	case DDERR_OUTOFMEMORY:
		szError = "DDERR_OUTOFMEMORY";
		break;
	default: {
		const char szUnknown[] = "DDERR unknown 0x%x";
		/// ASSERT: assert(dwMaxChars >= sizeof(szUnknown) + 10);
		sprintf(pszBuffer, szUnknown, hError);
		return;
	}
	}

	strncpy(pszBuffer, szError, dwMaxChars);
}

void TraceErrorDS(HRESULT hError, char *pszBuffer, DWORD dwMaxChars)
{
	const char *szError;

	switch (hError) {
	case DS_OK:
		szError = "DS_OK";
		break;
	default: {
		const char szUnknown[] = "DSERR unknown 0x%x";
		/// ASSERT: assert(dwMaxChars >= sizeof(szUnknown) + 10);
		sprintf(pszBuffer, szUnknown, hError);
		return;
	}
	}

	strncpy(pszBuffer, szError, dwMaxChars);
}

char *TraceLastError()
{
	return GetErrorStr(GetLastError());
}

void __cdecl app_fatal(const char *pszFmt, ...)
{
	va_list va;

	va_start(va, pszFmt);
	FreeDlg();

	if (pszFmt)
		MsgBox(pszFmt, va);

	va_end(va);

	dx_cleanup();
	init_cleanup();
	exit(1);
}

void MsgBox(const char *pszFmt, va_list va)
{
	char Text[256];

	wvsprintf(Text, pszFmt, va);

	UiErrorOkDialog("Error", Text);
}

void FreeDlg()
{
	if (terminating && cleanup_thread_id != GetCurrentThreadId())
		Sleep(20000);

	terminating = TRUE;
	cleanup_thread_id = GetCurrentThreadId();

	if (gbMaxPlayers > 1) {
		if (SNetLeaveGame(3))
			Sleep(2000);
	}

	SNetDestroy();
}

void __cdecl DrawDlg(char *pszFmt, ...)
{
	char text[256];
	va_list arglist;

	va_start(arglist, pszFmt);
	wvsprintf(text, pszFmt, arglist);
	va_end(arglist);

	UiErrorOkDialog(PROJECT_NAME, text, false);
}

#ifdef _DEBUG
void assert_fail(int nLineNo, const char *pszFile, const char *pszFail)
{
	app_fatal("assertion failed (%d:%s)\n%s", nLineNo, pszFile, pszFail);
}
#endif

void ErrDlg(char *title, char *text, char *log_file_path, int log_line_nr)
{
	FreeDlg();

	std::string string = text;
	string.append("\n\nThe error occurred at: ");
	string.append(log_file_path);
	string.append(" line " + std::to_string(log_line_nr));

	UiErrorOkDialog(title, string.c_str());

	app_fatal(NULL);
}

void FileErrDlg(const char *error)
{
	FreeDlg();

	if (!error)
		error = "";

	std::string text = "Unable to open a required file.\n"
	                   "\n"
	                   "Verify that the MD5 of diabdat.mpq matches on of the following values\n"
	                   "011bc6518e6166206231080a4440b373\n"
	                   "68f049866b44688a7af65ba766bef75a\n"
	                   "\n"
	                   "The problem occurred when loading:\n";
	text.append(error);

	UiErrorOkDialog("Date File Error", text.c_str());

	app_fatal(NULL);
}

BOOL InsertCDDlg()
{
	std::string text = "Unable to open diabdat.mpq.\n"
	                   "\n"
	                   "Make sure that it is in the game folder and that the file name is in all lowercase.";

	UiErrorOkDialog("Date File Error", text.c_str());

	app_fatal(NULL);
}

void DirErrorDlg(char *error)
{
	std::string text = "Unable to write to location:\n";
	text.append(error);

	UiErrorOkDialog("Read-Only Directory Error", text.c_str());
	app_fatal(NULL);
}

DEVILUTION_END_NAMESPACE
