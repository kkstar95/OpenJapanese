#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "resource.h"

// --- Include our Kanji data files ---
#include "kanji_data5.h"
#include "kanji_data4.h"
#include "kanji_data3.h"
#include "kanji_data2.h"
#include "kanji_data1.h"

// --- Safe buffer sizes ---
#define MAX_KANJI 5000
#define MAX_READING 128

// --- Game State ---
int g_score = 0;
int g_round = 1;
int g_correctCount = 0;
int g_incorrectCount = 0;
bool g_isAnswering = false;
bool g_isSwapped = false;
int g_kanjiCount = 0;
int g_currentIndex = 0;
int g_maxRound = 100;
int g_buttonIndices[4]; // Remembers which 4 characters are on the buttons

// Storage for kanji and readings
wchar_t g_kanjiList[MAX_KANJI][4];
wchar_t g_readingList[MAX_KANJI][MAX_READING];
wchar_t g_options[4][MAX_READING];
wchar_t g_correctAnswer[MAX_READING];

// --- Colors (Dark Mode) ---
COLORREF g_bgColor = RGB(45, 45, 45);
COLORREF g_textColor = RGB(240, 240, 240);
HBRUSH g_hBrushWindowBg = NULL;
HBRUSH g_hBrushBoxBg = NULL;
HBRUSH g_hBrushGreen = NULL;
HBRUSH g_hBrushRed = NULL;
HBRUSH g_hBrushDarkBg = NULL;

// --- Fonts ---
HFONT g_hLargeFont = NULL;      // 180pt for single Kanji
HFONT g_hReadingFont = NULL;    // 90pt for Hiragana readings
HFONT g_hBtnFont = NULL;
HFONT g_hNextFont = NULL;
HFONT g_hTitleFont = NULL;
HFONT g_hResultFont = NULL;
HFONT g_hAccuracyFont = NULL;
HFONT g_hLabelFont = NULL;
HFONT g_hLevelFont = NULL;

// --- Accelerator Table ---
HACCEL g_hAccel = NULL;

// --- Control IDs ---
#define ID_KANJI      1001
#define ID_SCORE      1002
#define ID_ROUND      1003
#define ID_INFO       1004
#define ID_OPT1       1005
#define ID_OPT2       1006
#define ID_OPT3       1007
#define ID_OPT4       1008
#define ID_NEXT       1009
#define ID_TIMER      1
#define ID_PLAY_AGAIN 1010
#define ID_SWAP       1011
#define ID_END        1012
#define ID_LEVEL_N5   1013
#define ID_LEVEL_N4   1014
#define ID_LEVEL_N3   1015
#define ID_LEVEL_N2   1016
#define ID_LEVEL_N1   1017

// --- Window Handles (Game) ---
HWND g_hMainWindow = NULL;
HWND g_hKanji = NULL;
HWND g_hScore = NULL;
HWND g_hRound = NULL;
HWND g_hInfo = NULL;
HWND g_hBtn1 = NULL;
HWND g_hBtn2 = NULL;
HWND g_hBtn3 = NULL;
HWND g_hBtn4 = NULL;
HWND g_hBtnNext = NULL;
HWND g_hBtnSwap = NULL;
HWND g_hBtnEnd = NULL;
HWND g_hLabelSwap = NULL;
HWND g_hLabelNext = NULL;
HWND g_hLabelEnd = NULL;
HWND g_hBtnN5 = NULL;
HWND g_hBtnN4 = NULL;
HWND g_hBtnN3 = NULL;
HWND g_hBtnN2 = NULL;
HWND g_hBtnN1 = NULL;
HWND g_hResultsTitle = NULL;
HWND g_hResultsScore = NULL;
HWND g_hResultsAccuracy = NULL;
HWND g_hBtnPlayAgain = NULL;

// Forward declaration
void ShowResults();

// --- Helper: Set the correct font for the top box ---
void UpdateTopFont() {
    if (g_isSwapped) {
        SendMessageW(g_hKanji, WM_SETFONT, (WPARAM)g_hReadingFont, TRUE);
    } else {
        SendMessageW(g_hKanji, WM_SETFONT, (WPARAM)g_hLargeFont, TRUE);
    }
}

// --- Data Loading Functions ---
void LoadN5Data() {
    int i, j;
    g_kanjiCount = 0;

    for (i = 0; i < N5_COUNT; i++) {
        if (g_kanjiCount >= MAX_KANJI) break;

        for (j = 0; j < 4; j++) {
            g_kanjiList[g_kanjiCount][j] = 0;
        }

        g_kanjiList[g_kanjiCount][0] = N5_KANJI[i];
        lstrcpynW(g_readingList[g_kanjiCount], N5_READINGS[i], MAX_READING);
        g_kanjiCount++;
    }
}

void LoadN4Data() {
    int i, j;
    LoadN5Data();

    for (i = 0; i < N4_COUNT; i++) {
        if (g_kanjiCount >= MAX_KANJI) break;

        for (j = 0; j < 4; j++) {
            g_kanjiList[g_kanjiCount][j] = 0;
        }

        g_kanjiList[g_kanjiCount][0] = N4_KANJI[i];
        lstrcpynW(g_readingList[g_kanjiCount], N4_READINGS[i], MAX_READING);
        g_kanjiCount++;
    }
}

void LoadN3Data() {
    int i, j;
    LoadN4Data();

    for (i = 0; i < N3_COUNT; i++) {
        if (g_kanjiCount >= MAX_KANJI) break;

        for (j = 0; j < 4; j++) {
            g_kanjiList[g_kanjiCount][j] = 0;
        }

        g_kanjiList[g_kanjiCount][0] = N3_KANJI[i];
        lstrcpynW(g_readingList[g_kanjiCount], N3_READINGS[i], MAX_READING);
        g_kanjiCount++;
    }
}

void LoadN2Data() {
    int i, j;
    LoadN3Data();

    for (i = 0; i < N2_COUNT; i++) {
        if (g_kanjiCount >= MAX_KANJI) break;

        for (j = 0; j < 4; j++) {
            g_kanjiList[g_kanjiCount][j] = 0;
        }

        g_kanjiList[g_kanjiCount][0] = N2_KANJI[i];
        lstrcpynW(g_readingList[g_kanjiCount], N2_READINGS[i], MAX_READING);
        g_kanjiCount++;
    }
}

void LoadN1Data() {
    int i, j;
    LoadN2Data();

    for (i = 0; i < N1_COUNT; i++) {
        if (g_kanjiCount >= MAX_KANJI) break;

        for (j = 0; j < 4; j++) {
            g_kanjiList[g_kanjiCount][j] = 0;
        }

        g_kanjiList[g_kanjiCount][0] = N1_KANJI[i];
        lstrcpynW(g_readingList[g_kanjiCount], N1_READINGS[i], MAX_READING);
        g_kanjiCount++;
    }
}

// --- 1. Check Answer ---
void CheckAnswer(int btnId) {
    if (g_isAnswering) return;

    wchar_t answer[MAX_READING];
    answer[0] = 0;

    GetWindowTextW(GetDlgItem(g_hMainWindow, btnId), answer, MAX_READING);

    if (lstrcmpW(answer, g_correctAnswer) == 0) {
        g_score++;
        g_correctCount++;
        g_bgColor = RGB(0, 100, 0);
        g_textColor = RGB(255, 255, 255);
        SetWindowTextW(g_hInfo, L"Correct!");
        MessageBeep(MB_ICONASTERISK);
    } else {
        g_incorrectCount++;
        if (g_score > 0) g_score--;
        g_bgColor = RGB(150, 0, 0);
        g_textColor = RGB(255, 255, 255);
        SetWindowTextW(g_hInfo, L"Wrong!");
        MessageBeep(MB_ICONHAND);
    }

    wchar_t buf[64];
    wsprintfW(buf, L"Score: %d", g_score);
    SetWindowTextW(g_hScore, buf);

    EnableWindow(g_hBtnNext, TRUE);
    g_isAnswering = true;
    InvalidateRect(g_hKanji, NULL, TRUE);
}

// --- Swap Mode Function ---
void SwapMode() {
    g_isSwapped = !g_isSwapped;
    int idx = g_currentIndex;

    wchar_t topText[MAX_READING];
    topText[0] = 0;

    if (g_isSwapped) {
        lstrcpynW(topText, g_readingList[idx], MAX_READING);
        lstrcpynW(g_correctAnswer, g_kanjiList[idx], MAX_READING);
    } else {
        lstrcpynW(topText, g_kanjiList[idx], MAX_READING);
        lstrcpynW(g_correctAnswer, g_readingList[idx], MAX_READING);
    }

    SetWindowTextW(g_hKanji, topText);
    UpdateTopFont();

    int i;
    for (i = 0; i < 4; i++) {
        if (g_isSwapped) {
            lstrcpynW(g_options[i], g_kanjiList[g_buttonIndices[i]], MAX_READING);
        } else {
            lstrcpynW(g_options[i], g_readingList[g_buttonIndices[i]], MAX_READING);
        }
    }

    SetWindowTextW(g_hBtn1, g_options[0]);
    SetWindowTextW(g_hBtn2, g_options[1]);
    SetWindowTextW(g_hBtn3, g_options[2]);
    SetWindowTextW(g_hBtn4, g_options[3]);

    g_bgColor = RGB(45, 45, 45);
    g_textColor = RGB(240, 240, 240);
    InvalidateRect(g_hKanji, NULL, TRUE);

    SetWindowTextW(g_hInfo, g_isSwapped ? L"Select the Kanji" : L"Select the Reading");

    g_isAnswering = false;
    EnableWindow(g_hBtnNext, FALSE);
}

// --- 2. Start New Question ---
void StartNewQuestion() {
    if (g_round > g_maxRound) {
        ShowResults();
        return;
    }

    if (g_kanjiCount < 4) {
        return;
    }

    if (g_isAnswering) {
        g_isAnswering = false;
        EnableWindow(g_hBtnNext, FALSE);
    }

    int idx = rand() % g_kanjiCount;
    g_currentIndex = idx;

    // Pick 3 unique wrong indices
    std::vector<int> wrongs;
    int i;

    for (i = 0; i < g_kanjiCount; i++) {
        if (i != idx) wrongs.push_back(i);
    }

    for (i = (int)wrongs.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = wrongs[i];
        wrongs[i] = wrongs[j];
        wrongs[j] = temp;
    }

    // Create a pool of 4 indices: [Correct, Wrong1, Wrong2, Wrong3]
    int pool[4] = { idx, wrongs[0], wrongs[1], wrongs[2] };

    // Shuffle the pool so the correct answer isn't always in the same spot
    for (i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = pool[i];
        pool[i] = pool[j];
        pool[j] = temp;
    }

    // Save these indices so SwapMode can use them
    for (i = 0; i < 4; i++) {
        g_buttonIndices[i] = pool[i];
    }

    // Set top text
    wchar_t topText[MAX_READING];
    topText[0] = 0;

    if (g_isSwapped) {
        lstrcpynW(topText, g_readingList[idx], MAX_READING);
        lstrcpynW(g_correctAnswer, g_kanjiList[idx], MAX_READING);
    } else {
        lstrcpynW(topText, g_kanjiList[idx], MAX_READING);
        lstrcpynW(g_correctAnswer, g_readingList[idx], MAX_READING);
    }

    SetWindowTextW(g_hKanji, topText);
    UpdateTopFont();

    // Set button text based on saved indices
    for (i = 0; i < 4; i++) {
        if (g_isSwapped) {
            lstrcpynW(g_options[i], g_kanjiList[pool[i]], MAX_READING);
        } else {
            lstrcpynW(g_options[i], g_readingList[pool[i]], MAX_READING);
        }
    }

    SetWindowTextW(g_hBtn1, g_options[0]);
    SetWindowTextW(g_hBtn2, g_options[1]);
    SetWindowTextW(g_hBtn3, g_options[2]);
    SetWindowTextW(g_hBtn4, g_options[3]);

    g_bgColor = RGB(45, 45, 45);
    g_textColor = RGB(240, 240, 240);
    InvalidateRect(g_hKanji, NULL, TRUE);

    wchar_t buf[64];
    wsprintfW(buf, L"Score: %d", g_score);
    SetWindowTextW(g_hScore, buf);

    wsprintfW(buf, L"Round: %d", g_round);
    SetWindowTextW(g_hRound, buf);

    g_round++;

    SetWindowTextW(g_hInfo, g_isSwapped ? L"Select the Kanji" : L"Select the Reading");

    g_isAnswering = false;
    EnableWindow(g_hBtnNext, FALSE);
}

// --- 3. Show Results ---
void ShowResults() {
    if (g_hMainWindow) {
        KillTimer(g_hMainWindow, ID_TIMER);
    }

    ShowWindow(g_hKanji, SW_HIDE);
    ShowWindow(g_hScore, SW_HIDE);
    ShowWindow(g_hRound, SW_HIDE);
    ShowWindow(g_hBtn1, SW_HIDE);
    ShowWindow(g_hBtn2, SW_HIDE);
    ShowWindow(g_hBtn3, SW_HIDE);
    ShowWindow(g_hBtn4, SW_HIDE);
    ShowWindow(g_hInfo, SW_HIDE);
    ShowWindow(g_hBtnNext, SW_HIDE);
    ShowWindow(g_hBtnSwap, SW_HIDE);
    ShowWindow(g_hBtnEnd, SW_HIDE);
    ShowWindow(g_hLabelSwap, SW_HIDE);
    ShowWindow(g_hLabelNext, SW_HIDE);
    ShowWindow(g_hLabelEnd, SW_HIDE);
    ShowWindow(g_hBtnN5, SW_HIDE);
    ShowWindow(g_hBtnN4, SW_HIDE);
    ShowWindow(g_hBtnN3, SW_HIDE);
    ShowWindow(g_hBtnN2, SW_HIDE);
    ShowWindow(g_hBtnN1, SW_HIDE);

    int totalAnswered = g_correctCount + g_incorrectCount;
    int accuracy = 0;

    if (totalAnswered > 0) {
        accuracy = (g_correctCount * 100) / totalAnswered;
    }

    wchar_t buf[64];

    wsprintfW(buf, L"Final Score: %d", g_score);
    SetWindowTextW(g_hResultsScore, buf);

    wsprintfW(buf, L"%d%%", accuracy);
    SetWindowTextW(g_hResultsAccuracy, buf);

    ShowWindow(g_hResultsTitle, SW_SHOW);
    ShowWindow(g_hResultsScore, SW_SHOW);
    ShowWindow(g_hResultsAccuracy, SW_SHOW);
    ShowWindow(g_hBtnPlayAgain, SW_SHOW);
}

// --- 4. Play Again ---
void PlayAgain() {
    ShowWindow(g_hResultsTitle, SW_HIDE);
    ShowWindow(g_hResultsScore, SW_HIDE);
    ShowWindow(g_hResultsAccuracy, SW_HIDE);
    ShowWindow(g_hBtnPlayAgain, SW_HIDE);

    g_score = 0;
    g_round = 1;
    g_correctCount = 0;
    g_incorrectCount = 0;
    g_isAnswering = false;
    g_isSwapped = false;

    ShowWindow(g_hKanji, SW_SHOW);
    ShowWindow(g_hScore, SW_SHOW);
    ShowWindow(g_hRound, SW_SHOW);
    ShowWindow(g_hBtn1, SW_SHOW);
    ShowWindow(g_hBtn2, SW_SHOW);
    ShowWindow(g_hBtn3, SW_SHOW);
    ShowWindow(g_hBtn4, SW_SHOW);
    ShowWindow(g_hInfo, SW_SHOW);
    ShowWindow(g_hBtnNext, SW_SHOW);
    ShowWindow(g_hBtnSwap, SW_SHOW);
    ShowWindow(g_hBtnEnd, SW_SHOW);
    ShowWindow(g_hLabelSwap, SW_SHOW);
    ShowWindow(g_hLabelNext, SW_SHOW);
    ShowWindow(g_hLabelEnd, SW_SHOW);
    ShowWindow(g_hBtnN5, SW_SHOW);
    ShowWindow(g_hBtnN4, SW_SHOW);
    ShowWindow(g_hBtnN3, SW_SHOW);
    ShowWindow(g_hBtnN2, SW_SHOW);
    ShowWindow(g_hBtnN1, SW_SHOW);

    if (g_hMainWindow) {
        KillTimer(g_hMainWindow, ID_TIMER);
        SetTimer(g_hMainWindow, ID_TIMER, 30000, NULL);
    }

    StartNewQuestion();
}

// --- 5. Window Procedure ---
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        g_hMainWindow = hWnd;

        g_hLargeFont = CreateFontW(180, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 128,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"MS Gothic");

        g_hReadingFont = CreateFontW(90, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 128,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"MS Gothic");

        g_hBtnFont = CreateFontW(28, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hNextFont = CreateFontW(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hLabelFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hLevelFont = CreateFontW(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hTitleFont = CreateFontW(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hResultFont = CreateFontW(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hAccuracyFont = CreateFontW(100, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hBrushBoxBg = CreateSolidBrush(RGB(45, 45, 45));
        g_hBrushGreen = CreateSolidBrush(RGB(0, 100, 0));
        g_hBrushRed = CreateSolidBrush(RGB(150, 0, 0));
        g_hBrushDarkBg = CreateSolidBrush(RGB(20, 20, 20));

        g_hKanji = CreateWindowW(L"STATIC", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE,
            10, 10, 760, 350, hWnd, (HMENU)ID_KANJI, NULL, NULL);
        SendMessageW(g_hKanji, WM_SETFONT, (WPARAM)g_hLargeFont, TRUE);

        int levelY = 380;
        int levelH = 40;
        int levelW = 70;
        int gap = 5;
        int startX = 10;

        g_hBtnN5 = CreateWindowW(L"BUTTON", L"N5",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            startX, levelY, levelW, levelH, hWnd, (HMENU)ID_LEVEL_N5, NULL, NULL);
        SendMessageW(g_hBtnN5, WM_SETFONT, (WPARAM)g_hLevelFont, TRUE);
        EnableWindow(g_hBtnN5, TRUE);

        g_hBtnN4 = CreateWindowW(L"BUTTON", L"N4",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            startX + levelW + gap, levelY, levelW, levelH, hWnd, (HMENU)ID_LEVEL_N4, NULL, NULL);
        SendMessageW(g_hBtnN4, WM_SETFONT, (WPARAM)g_hLevelFont, TRUE);
        EnableWindow(g_hBtnN4, TRUE);

        g_hBtnN3 = CreateWindowW(L"BUTTON", L"N3",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            startX + (levelW + gap) * 2, levelY, levelW, levelH, hWnd, (HMENU)ID_LEVEL_N3, NULL, NULL);
        SendMessageW(g_hBtnN3, WM_SETFONT, (WPARAM)g_hLevelFont, TRUE);
        EnableWindow(g_hBtnN3, TRUE);

        g_hBtnN2 = CreateWindowW(L"BUTTON", L"N2",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            startX + (levelW + gap) * 3, levelY, levelW, levelH, hWnd, (HMENU)ID_LEVEL_N2, NULL, NULL);
        SendMessageW(g_hBtnN2, WM_SETFONT, (WPARAM)g_hLevelFont, TRUE);
        EnableWindow(g_hBtnN2, TRUE);

        g_hBtnN1 = CreateWindowW(L"BUTTON", L"N1",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            startX + (levelW + gap) * 4, levelY, levelW, levelH, hWnd, (HMENU)ID_LEVEL_N1, NULL, NULL);
        SendMessageW(g_hBtnN1, WM_SETFONT, (WPARAM)g_hLevelFont, TRUE);
        EnableWindow(g_hBtnN1, TRUE);

        g_hScore = CreateWindowW(L"STATIC", L"Score: 0",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            410, 380, 175, 40, hWnd, (HMENU)ID_SCORE, NULL, NULL);
        SendMessageW(g_hScore, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hRound = CreateWindowW(L"STATIC", L"Round: 1",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            595, 380, 175, 40, hWnd, (HMENU)ID_ROUND, NULL, NULL);
        SendMessageW(g_hRound, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hBtn1 = CreateWindowW(L"BUTTON", L"",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            10, 440, 370, 70, hWnd, (HMENU)ID_OPT1, NULL, NULL);

        g_hBtn2 = CreateWindowW(L"BUTTON", L"",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            410, 440, 370, 70, hWnd, (HMENU)ID_OPT2, NULL, NULL);

        g_hBtn3 = CreateWindowW(L"BUTTON", L"",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            10, 530, 370, 70, hWnd, (HMENU)ID_OPT3, NULL, NULL);

        g_hBtn4 = CreateWindowW(L"BUTTON", L"",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            410, 530, 370, 70, hWnd, (HMENU)ID_OPT4, NULL, NULL);

        SendMessageW(g_hBtn1, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        SendMessageW(g_hBtn2, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        SendMessageW(g_hBtn3, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        SendMessageW(g_hBtn4, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hInfo = CreateWindowW(L"STATIC", L"Select the Reading",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 620, 760, 30, hWnd, (HMENU)ID_INFO, NULL, NULL);
        SendMessageW(g_hInfo, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hBtnSwap = CreateWindowW(L"BUTTON", L"Swap",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            50, 660, 180, 45, hWnd, (HMENU)ID_SWAP, NULL, NULL);
        SendMessageW(g_hBtnSwap, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hLabelSwap = CreateWindowW(L"STATIC", L"(Home)",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            50, 710, 180, 20, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hLabelSwap, WM_SETFONT, (WPARAM)g_hLabelFont, TRUE);

        g_hBtnNext = CreateWindowW(L"BUTTON", L"Next Question",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            290, 650, 200, 60, hWnd, (HMENU)ID_NEXT, NULL, NULL);
        SendMessageW(g_hBtnNext, WM_SETFONT, (WPARAM)g_hNextFont, TRUE);
        EnableWindow(g_hBtnNext, FALSE);

        g_hLabelNext = CreateWindowW(L"STATIC", L"(Spacebar)",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            290, 715, 200, 20, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hLabelNext, WM_SETFONT, (WPARAM)g_hLabelFont, TRUE);

        g_hBtnEnd = CreateWindowW(L"BUTTON", L"End",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            550, 660, 180, 45, hWnd, (HMENU)ID_END, NULL, NULL);
        SendMessageW(g_hBtnEnd, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hLabelEnd = CreateWindowW(L"STATIC", L"(End)",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            550, 710, 180, 20, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hLabelEnd, WM_SETFONT, (WPARAM)g_hLabelFont, TRUE);

        g_hResultsTitle = CreateWindowW(L"STATIC", L"Game Over!",
            WS_CHILD | SS_CENTER,
            10, 150, 760, 80, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hResultsTitle, WM_SETFONT, (WPARAM)g_hTitleFont, TRUE);

        g_hResultsScore = CreateWindowW(L"STATIC", L"Final Score: 0",
            WS_CHILD | SS_CENTER,
            10, 300, 760, 60, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hResultsScore, WM_SETFONT, (WPARAM)g_hResultFont, TRUE);

        g_hResultsAccuracy = CreateWindowW(L"STATIC", L"0%",
            WS_CHILD | SS_CENTER,
            10, 400, 760, 120, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hResultsAccuracy, WM_SETFONT, (WPARAM)g_hAccuracyFont, TRUE);

        g_hBtnPlayAgain = CreateWindowW(L"BUTTON", L"Play Again",
            WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
            300, 600, 200, 60, hWnd, (HMENU)ID_PLAY_AGAIN, NULL, NULL);
        SendMessageW(g_hBtnPlayAgain, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        ACCEL accels[] = {
            { FVIRTKEY, VK_HOME, ID_SWAP },
            { FVIRTKEY, VK_END, ID_END },
            { FVIRTKEY, VK_SPACE, ID_NEXT }
        };

        g_hAccel = CreateAcceleratorTableW(accels, 3);

        srand((unsigned int)time(NULL));

        LoadN5Data();
        g_maxRound = 100;
        PlayAgain();

        return 0;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == ID_OPT1) CheckAnswer(ID_OPT1);
        if (LOWORD(wParam) == ID_OPT2) CheckAnswer(ID_OPT2);
        if (LOWORD(wParam) == ID_OPT3) CheckAnswer(ID_OPT3);
        if (LOWORD(wParam) == ID_OPT4) CheckAnswer(ID_OPT4);

        if (LOWORD(wParam) == ID_NEXT) {
            if (IsWindowEnabled(g_hBtnNext)) {
                StartNewQuestion();
            }
        }

        if (LOWORD(wParam) == ID_PLAY_AGAIN) {
            PlayAgain();
        }

        if (LOWORD(wParam) == ID_SWAP) {
            SwapMode();
        }

        if (LOWORD(wParam) == ID_END) {
            ShowResults();
        }

        if (LOWORD(wParam) == ID_LEVEL_N5) {
            int result = MessageBoxW(hWnd,
                L"Starting N5 will reset your progress. Are you sure?",
                L"Confirm", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES) {
                LoadN5Data();
                g_maxRound = 100;
                PlayAgain();
            }
        }

        if (LOWORD(wParam) == ID_LEVEL_N4) {
            int result = MessageBoxW(hWnd,
                L"Starting N4 will reset your progress. Are you sure?",
                L"Confirm", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES) {
                LoadN4Data();
                g_maxRound = 250;
                PlayAgain();
            }
        }

        if (LOWORD(wParam) == ID_LEVEL_N3) {
            int result = MessageBoxW(hWnd,
                L"Starting N3 will reset your progress. Are you sure?",
                L"Confirm", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES) {
                LoadN3Data();
                g_maxRound = 500;
                PlayAgain();
            }
        }

        if (LOWORD(wParam) == ID_LEVEL_N2) {
            int result = MessageBoxW(hWnd,
                L"Starting N2 will reset your progress. Are you sure?",
                L"Confirm", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES) {
                LoadN2Data();
                g_maxRound = 700;
                PlayAgain();
            }
        }

        if (LOWORD(wParam) == ID_LEVEL_N1) {
            int result = MessageBoxW(hWnd,
                L"Starting N1 will reset your progress. Are you sure?",
                L"Confirm", MB_YESNO | MB_ICONQUESTION);
            if (result == IDYES) {
                LoadN1Data();
                g_maxRound = 1000;
                PlayAgain();
            }
        }

        return 0;

    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        HWND hCtrl = (HWND)lParam;

        SetTextColor(hdcStatic, RGB(240, 240, 240));

        if (hCtrl == g_hKanji) {
            SetTextColor(hdcStatic, g_textColor);
            SetBkColor(hdcStatic, g_bgColor);

            if (g_bgColor == RGB(0, 100, 0)) return (LRESULT)g_hBrushGreen;
            if (g_bgColor == RGB(150, 0, 0)) return (LRESULT)g_hBrushRed;

            return (LRESULT)g_hBrushBoxBg;
        }

        SetBkColor(hdcStatic, RGB(20, 20, 20));
        return (LRESULT)g_hBrushDarkBg;
    }

    case WM_TIMER:
        if (wParam == ID_TIMER && !g_isAnswering) {
            StartNewQuestion();
        }
        return 0;

    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER);

        DeleteObject(g_hBrushBoxBg);
        DeleteObject(g_hBrushGreen);
        DeleteObject(g_hBrushRed);
        DeleteObject(g_hBrushDarkBg);

        DeleteObject(g_hLargeFont);
        DeleteObject(g_hReadingFont);
        DeleteObject(g_hBtnFont);
        DeleteObject(g_hNextFont);
        DeleteObject(g_hLabelFont);
        DeleteObject(g_hLevelFont);
        DeleteObject(g_hTitleFont);
        DeleteObject(g_hResultFont);
        DeleteObject(g_hAccuracyFont);

        DestroyAcceleratorTable(g_hAccel);

        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hWnd, message, wParam, lParam);
}

// --- 6. Main Entry Point ---
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hBrushWindowBg = CreateSolidBrush(RGB(20, 20, 20));

// Load icons embedded in the EXE
HICON hLargeIcon = LoadIcon(
    hInstance,
    MAKEINTRESOURCE(IDI_APP)
);

HICON hSmallIcon = (HICON)LoadImage(
    hInstance,
    MAKEINTRESOURCE(IDI_APP),
    IMAGE_ICON,
    16,
    16,
    LR_DEFAULTCOLOR
);

    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEXW);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = hLargeIcon;
    wcex.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
    wcex.hbrBackground = g_hBrushWindowBg;
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"KanjiQuizClass";
    wcex.hIconSm = hSmallIcon;

    RegisterClassExW(&wcex);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int x = (screenWidth - 800) / 2;
    int y = (screenHeight - 800) / 2;

    HWND hWnd = CreateWindowW(L"KanjiQuizClass",
        L"OpenKanji - Open Source Kanji Test",
        WS_OVERLAPPEDWINDOW,
        x, y, 800, 800,
        NULL, NULL, hInstance, NULL);

    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!TranslateAcceleratorW(hWnd, g_hAccel, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    DestroyIcon(hLargeIcon);
    DestroyIcon(hSmallIcon);
    DeleteObject(g_hBrushWindowBg);

    return (int)msg.wParam;
}