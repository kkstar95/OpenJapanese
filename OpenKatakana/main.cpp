#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include "resource.h"

// --- Game State ---
int g_score = 0;
int g_round = 1;
int g_correctCount = 0; 
int g_incorrectCount = 0; 
bool g_isAnswering = false;
bool g_isSwapped = false; 
int g_katakanaCount = 0;
int g_currentIndex = 0;
wchar_t g_katakanaList[100][4]; 
wchar_t g_romajiList[100][8]; 
wchar_t g_options[4][8];      
wchar_t g_correctAnswer[8];   

// --- Colors (Dark Mode) ---
COLORREF g_bgColor = RGB(45, 45, 45); 
COLORREF g_textColor = RGB(240, 240, 240); 
HBRUSH g_hBrushWindowBg = NULL; 
HBRUSH g_hBrushBoxBg = NULL;    
HBRUSH g_hBrushGreen = NULL;
HBRUSH g_hBrushRed = NULL;
HBRUSH g_hBrushDarkBg = NULL;   

// --- Fonts ---
HFONT g_hLargeFont = NULL;
HFONT g_hBtnFont = NULL;
HFONT g_hNextFont = NULL;
HFONT g_hTitleFont = NULL;      
HFONT g_hResultFont = NULL;     
HFONT g_hAccuracyFont = NULL;   
HFONT g_hLabelFont = NULL;      

// --- Accelerator Table ---
HACCEL g_hAccel = NULL;

// --- Control IDs ---
#define ID_KATAKANA 1001
#define ID_SCORE    1002
#define ID_ROUND    1003
#define ID_INFO     1004
#define ID_OPT1     1005
#define ID_OPT2     1006
#define ID_OPT3     1007
#define ID_OPT4     1008
#define ID_NEXT     1009
#define ID_TIMER    1
#define ID_PLAY_AGAIN 1010      
#define ID_SWAP     1011        
#define ID_END      1012        

// --- Window Handles (Game) ---
HWND g_hKatakana = NULL;
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

// --- Window Handles (Results Screen) ---
HWND g_hResultsTitle = NULL;
HWND g_hResultsScore = NULL;
HWND g_hResultsAccuracy = NULL;
HWND g_hBtnPlayAgain = NULL;

// --- Katakana & Romaji Data ---
void LoadKatakanaData() {
    g_katakanaCount = 0;
    const wchar_t list[] = {
        0x30A2,0x30A4,0x30A6,0x30A8,0x30AA,0x30AB,0x30AD,0x30AF,0x30B1,0x30B3,
        0x30B5,0x30B7,0x30B9,0x30BB,0x30BD,0x30BF,0x30C1,0x30C4,0x30C6,0x30C8,
        0x30CA,0x30CB,0x30CC,0x30CD,0x30CE,0x30CF,0x30D2,0x30D5,0x30D8,0x30DB,
        0x30DE,0x30DF,0x30E0,0x30E1,0x30E2,0x30E4,0x30E6,0x30E8,0x30E9,0x30EA,
        0x30EB,0x30EC,0x30ED,0x30EF,0x30F2,0x30F3,
        0x30AC,0x30AE,0x30B0,0x30B2,0x30B4,0x30B6,0x30B8,0x30BA,0x30BC,0x30BE,
        0x30C0,0x30C2,0x30C5,0x30C7,0x30C9,0x30D0,0x30D3,0x30D6,0x30D9,0x30DC,
        0x30D1,0x30D4,0x30D7,0x30DA,0x30DD,
        0x30A1,0x30A3,0x30A5,0x30A7,0x30A9
    };
    const wchar_t* romaji[] = {
        L"A",L"I",L"U",L"E",L"O",L"Ka",L"Ki",L"Ku",L"Ke",L"Ko",
        L"Sa",L"Shi",L"Su",L"Se",L"So",L"Ta",L"Chi",L"Tsu",L"Te",L"To",
        L"Na",L"Ni",L"Nu",L"Ne",L"No",L"Ha",L"Hi",L"Fu",L"He",L"Ho",
        L"Ma",L"Mi",L"Mu",L"Me",L"Mo",L"Ya",L"Yu",L"Yo",L"Ra",L"Ri",
        L"Ru",L"Re",L"Ro",L"Wa",L"Wo",L"N",
        L"Ga",L"Gi",L"Gu",L"Ge",L"Go",L"Za",L"Ji",L"Zu",L"Ze",L"Zo",
        L"Da",L"Di",L"Du",L"De",L"Do",L"Ba",L"Bi",L"Bu",L"Be",L"Bo",
        L"Pa",L"Pi",L"Pu",L"Pe",L"Po",
        L"A",L"I",L"U",L"E",L"O"
    };
    int total = sizeof(list) / sizeof(list[0]);
    for(int i=0; i<total; i++) {
        g_katakanaList[g_katakanaCount][0] = list[i];
        g_katakanaList[g_katakanaCount][1] = 0;
        lstrcpyW(g_romajiList[g_katakanaCount], romaji[i]);
        g_katakanaCount++;
    }
}

// --- 1. Check Answer ---
void CheckAnswer(int btnId) {
    if (g_isAnswering) return;

    wchar_t answer[8];
    GetWindowTextW(GetDlgItem(GetParent(g_hKatakana), btnId), answer, 8);

    if (lstrcmpW(answer, g_correctAnswer) == 0) {
        g_score++;
        g_correctCount++; 
        g_bgColor = RGB(0, 100, 0); 
        g_textColor = RGB(255, 255, 255);
        SetWindowTextW(g_hInfo, L"Correct!");
        MessageBeep(MB_OK); 
    } else {
        g_incorrectCount++; 
        if (g_score > 0) g_score--;
        g_bgColor = RGB(150, 0, 0); 
        g_textColor = RGB(255, 255, 255);
        SetWindowTextW(g_hInfo, L"Wrong!");
        MessageBeep(MB_ICONERROR); 
    }

    wchar_t buf[32];
    wsprintfW(buf, L"Score: %d", g_score);
    SetWindowTextW(g_hScore, buf);

    EnableWindow(g_hBtnNext, TRUE);
    g_isAnswering = true;
    InvalidateRect(g_hKatakana, NULL, TRUE);
}

// --- Swap Mode Function ---
void SwapMode() {
    g_isSwapped = !g_isSwapped;
    int idx = g_currentIndex;

    wchar_t topText[8];
    if (g_isSwapped) {
        lstrcpyW(topText, g_romajiList[idx]); 
        lstrcpyW(g_correctAnswer, g_katakanaList[idx]); 
    } else {
        lstrcpyW(topText, g_katakanaList[idx]); 
        lstrcpyW(g_correctAnswer, g_romajiList[idx]); 
    }

    std::vector<int> wrongs;
    int i; 
    for (i = 0; i < g_katakanaCount; i++) {
        if (i != idx) wrongs.push_back(i);
    }

    for (i = (int)wrongs.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = wrongs[i]; wrongs[i] = wrongs[j]; wrongs[j] = temp;
    }

    for (i = 0; i < 3 && i < (int)wrongs.size(); i++) {
        if (g_isSwapped) lstrcpyW(g_options[i], g_katakanaList[wrongs[i]]);
        else lstrcpyW(g_options[i], g_romajiList[wrongs[i]]);
    }
    lstrcpyW(g_options[3], g_correctAnswer);

    for (i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        wchar_t temp[8];
        lstrcpyW(temp, g_options[i]); lstrcpyW(g_options[i], g_options[j]); lstrcpyW(g_options[j], temp);
    }

    SetWindowTextW(g_hKatakana, topText);
    SetWindowTextW(g_hBtn1, g_options[0]);
    SetWindowTextW(g_hBtn2, g_options[1]);
    SetWindowTextW(g_hBtn3, g_options[2]);
    SetWindowTextW(g_hBtn4, g_options[3]);

    g_bgColor = RGB(45, 45, 45); 
    g_textColor = RGB(240, 240, 240); 
    InvalidateRect(g_hKatakana, NULL, TRUE);
    SetWindowTextW(g_hInfo, g_isSwapped ? L"Select the Katakana character" : L"Select the Romaji reading");
    
    g_isAnswering = false;
    EnableWindow(g_hBtnNext, FALSE);
}

// --- 2. Start New Question ---
void StartNewQuestion() {
    if (g_round > 50) {
        extern void ShowResults();
        ShowResults();
        return;
    }

    if (g_isAnswering) {
        g_isAnswering = false;
        EnableWindow(g_hBtnNext, FALSE);
    }

    int idx = rand() % g_katakanaCount;
    g_currentIndex = idx;
    
    wchar_t topText[8];
    if (g_isSwapped) {
        lstrcpyW(topText, g_romajiList[idx]); 
        lstrcpyW(g_correctAnswer, g_katakanaList[idx]); 
    } else {
        lstrcpyW(topText, g_katakanaList[idx]); 
        lstrcpyW(g_correctAnswer, g_romajiList[idx]); 
    }

    std::vector<int> wrongs;
    int i; 
    for (i = 0; i < g_katakanaCount; i++) {
        if (i != idx) wrongs.push_back(i);
    }

    for (i = (int)wrongs.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = wrongs[i]; wrongs[i] = wrongs[j]; wrongs[j] = temp;
    }

    for (i = 0; i < 3 && i < (int)wrongs.size(); i++) {
        if (g_isSwapped) lstrcpyW(g_options[i], g_katakanaList[wrongs[i]]);
        else lstrcpyW(g_options[i], g_romajiList[wrongs[i]]);
    }
    lstrcpyW(g_options[3], g_correctAnswer);

    for (i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        wchar_t temp[8];
        lstrcpyW(temp, g_options[i]); lstrcpyW(g_options[i], g_options[j]); lstrcpyW(g_options[j], temp);
    }

    SetWindowTextW(g_hKatakana, topText);
    SetWindowTextW(g_hBtn1, g_options[0]);
    SetWindowTextW(g_hBtn2, g_options[1]);
    SetWindowTextW(g_hBtn3, g_options[2]);
    SetWindowTextW(g_hBtn4, g_options[3]);

    g_bgColor = RGB(45, 45, 45); 
    g_textColor = RGB(240, 240, 240); 
    InvalidateRect(g_hKatakana, NULL, TRUE);

    wchar_t buf[32];
    wsprintfW(buf, L"Score: %d", g_score); SetWindowTextW(g_hScore, buf);
    wsprintfW(buf, L"Round: %d", g_round); SetWindowTextW(g_hRound, buf);
    g_round++;

    SetWindowTextW(g_hInfo, g_isSwapped ? L"Select the Katakana character" : L"Select the Romaji reading");
    g_isAnswering = false;
    EnableWindow(g_hBtnNext, FALSE);
}

// --- 3. Show Results ---
void ShowResults() {
    ShowWindow(g_hKatakana, SW_HIDE);
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

    ShowWindow(g_hKatakana, SW_SHOW);
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

    StartNewQuestion();
}

// --- 5. Window Procedure ---
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        g_hLargeFont = CreateFontW(180, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 128, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"MS Gothic");
        g_hBtnFont = CreateFontW(28, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma"); 
        g_hNextFont = CreateFontW(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma"); 
        g_hLabelFont = CreateFontW(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
        
        g_hTitleFont = CreateFontW(60, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
        g_hResultFont = CreateFontW(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
        g_hAccuracyFont = CreateFontW(100, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");

        g_hBrushBoxBg = CreateSolidBrush(RGB(45, 45, 45)); 
        g_hBrushGreen = CreateSolidBrush(RGB(0, 100, 0));
        g_hBrushRed = CreateSolidBrush(RGB(150, 0, 0));
        g_hBrushDarkBg = CreateSolidBrush(RGB(20, 20, 20)); 

        g_hKatakana = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 10, 10, 760, 350, hWnd, (HMENU)ID_KATAKANA, NULL, NULL);
        SendMessageW(g_hKatakana, WM_SETFONT, (WPARAM)g_hLargeFont, TRUE);

        g_hScore = CreateWindowW(L"STATIC", L"Score: 0", WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 380, 370, 40, hWnd, (HMENU)ID_SCORE, NULL, NULL);
        SendMessageW(g_hScore, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        
        g_hRound = CreateWindowW(L"STATIC", L"Round: 1", WS_CHILD | WS_VISIBLE | SS_CENTER, 410, 380, 370, 40, hWnd, (HMENU)ID_ROUND, NULL, NULL);
        SendMessageW(g_hRound, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hBtn1 = CreateWindowW(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 10, 440, 370, 70, hWnd, (HMENU)ID_OPT1, NULL, NULL);
        g_hBtn2 = CreateWindowW(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 410, 440, 370, 70, hWnd, (HMENU)ID_OPT2, NULL, NULL);
        g_hBtn3 = CreateWindowW(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 10, 530, 370, 70, hWnd, (HMENU)ID_OPT3, NULL, NULL);
        g_hBtn4 = CreateWindowW(L"BUTTON", L"", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 410, 530, 370, 70, hWnd, (HMENU)ID_OPT4, NULL, NULL);
        
        SendMessageW(g_hBtn1, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        SendMessageW(g_hBtn2, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        SendMessageW(g_hBtn3, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        SendMessageW(g_hBtn4, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hInfo = CreateWindowW(L"STATIC", L"Select the Romaji reading", WS_CHILD | WS_VISIBLE | SS_CENTER, 10, 620, 760, 30, hWnd, (HMENU)ID_INFO, NULL, NULL);
        SendMessageW(g_hInfo, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        g_hBtnSwap = CreateWindowW(L"BUTTON", L"Swap", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 50, 660, 180, 45, hWnd, (HMENU)ID_SWAP, NULL, NULL);
        SendMessageW(g_hBtnSwap, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        g_hLabelSwap = CreateWindowW(L"STATIC", L"(Home)", WS_CHILD | WS_VISIBLE | SS_CENTER, 50, 710, 180, 20, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hLabelSwap, WM_SETFONT, (WPARAM)g_hLabelFont, TRUE);

        g_hBtnNext = CreateWindowW(L"BUTTON", L"Next Question", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 290, 650, 200, 60, hWnd, (HMENU)ID_NEXT, NULL, NULL);
        SendMessageW(g_hBtnNext, WM_SETFONT, (WPARAM)g_hNextFont, TRUE);
        EnableWindow(g_hBtnNext, FALSE);
        g_hLabelNext = CreateWindowW(L"STATIC", L"(Spacebar)", WS_CHILD | WS_VISIBLE | SS_CENTER, 290, 715, 200, 20, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hLabelNext, WM_SETFONT, (WPARAM)g_hLabelFont, TRUE);

        g_hBtnEnd = CreateWindowW(L"BUTTON", L"End", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 550, 660, 180, 45, hWnd, (HMENU)ID_END, NULL, NULL);
        SendMessageW(g_hBtnEnd, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);
        g_hLabelEnd = CreateWindowW(L"STATIC", L"(End)", WS_CHILD | WS_VISIBLE | SS_CENTER, 550, 710, 180, 20, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hLabelEnd, WM_SETFONT, (WPARAM)g_hLabelFont, TRUE);

        g_hResultsTitle = CreateWindowW(L"STATIC", L"Game Over!", WS_CHILD | SS_CENTER, 10, 150, 760, 80, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hResultsTitle, WM_SETFONT, (WPARAM)g_hTitleFont, TRUE);
        g_hResultsScore = CreateWindowW(L"STATIC", L"Final Score: 0", WS_CHILD | SS_CENTER, 10, 300, 760, 60, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hResultsScore, WM_SETFONT, (WPARAM)g_hResultFont, TRUE);
        g_hResultsAccuracy = CreateWindowW(L"STATIC", L"0%", WS_CHILD | SS_CENTER, 10, 400, 760, 120, hWnd, NULL, NULL, NULL);
        SendMessageW(g_hResultsAccuracy, WM_SETFONT, (WPARAM)g_hAccuracyFont, TRUE);
        g_hBtnPlayAgain = CreateWindowW(L"BUTTON", L"Play Again", WS_CHILD | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER, 300, 600, 200, 60, hWnd, (HMENU)ID_PLAY_AGAIN, NULL, NULL);
        SendMessageW(g_hBtnPlayAgain, WM_SETFONT, (WPARAM)g_hBtnFont, TRUE);

        ACCEL accels[] = {
            { FVIRTKEY, VK_HOME, ID_SWAP },
            { FVIRTKEY, VK_END, ID_END },
            { FVIRTKEY, VK_SPACE, ID_NEXT }
        };
        g_hAccel = CreateAcceleratorTableW(accels, 3);

        srand((unsigned int)time(NULL));
        LoadKatakanaData();
        StartNewQuestion();
        SetTimer(hWnd, ID_TIMER, 30000, NULL);
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
        
        if (LOWORD(wParam) == ID_PLAY_AGAIN) PlayAgain(); 
        if (LOWORD(wParam) == ID_SWAP) {
            SwapMode();
        }
        if (LOWORD(wParam) == ID_END) ShowResults();
        return 0;
    case WM_CTLCOLORSTATIC: {
        HDC hdcStatic = (HDC)wParam;
        HWND hCtrl = (HWND)lParam;
        
        SetTextColor(hdcStatic, RGB(240, 240, 240));
        
        if (hCtrl == g_hKatakana) {
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
        if (wParam == ID_TIMER && !g_isAnswering) StartNewQuestion();
        return 0;
    case WM_DESTROY:
        KillTimer(hWnd, ID_TIMER);
        DeleteObject(g_hBrushBoxBg);
        DeleteObject(g_hBrushGreen);
        DeleteObject(g_hBrushRed);
        DeleteObject(g_hBrushDarkBg); 
        DeleteObject(g_hLargeFont);
        DeleteObject(g_hBtnFont);
        DeleteObject(g_hNextFont);
        DeleteObject(g_hLabelFont);
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
    wcex.lpszClassName = L"KatakanaQuizClass";
    wcex.hIconSm = hSmallIcon;
    
    RegisterClassExW(&wcex);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - 800) / 2;
    int y = (screenHeight - 800) / 2;

    HWND hWnd = CreateWindowW(L"KatakanaQuizClass", L"OpenKatakana - Open Source Katakana Test", WS_OVERLAPPEDWINDOW,
        x, y, 800, 800, NULL, NULL, hInstance, NULL);

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
