#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <vector>

// --- Game State ---
int g_score = 0;
int g_round = 1;
int g_correctCount = 0; 
int g_incorrectCount = 0; 
bool g_isAnswering = false;
bool g_isSwapped = false; 
int g_hiraganaCount = 0;
int g_currentIndex = 0;
wchar_t g_hiraganaList[100][4]; 
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
#define ID_HIRAGANA 1001
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
HWND g_hHiragana = NULL;
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

// --- Hiragana & Romaji Data ---
void LoadHiraganaData() {
    g_hiraganaCount = 0;
    const wchar_t list[] = {
        0x3042,0x3044,0x3046,0x3048,0x304A,0x304B,0x304D,0x304F,0x3051,0x3053,
        0x3055,0x3057,0x3059,0x305B,0x305D,0x305F,0x3061,0x3064,0x3066,0x3068,
        0x306A,0x306B,0x306C,0x306D,0x306E,0x306F,0x3072,0x3075,0x3078,0x307B,
        0x307E,0x307F,0x3080,0x3081,0x3082,0x3084,0x3086,0x3088,0x3089,0x308A,
        0x308B,0x308C,0x308D,0x308F,0x3092,0x3093,
        0x304C,0x304E,0x3050,0x3052,0x3054,0x3056,0x3058,0x305A,0x305C,0x305E,
        0x3060,0x3062,0x3065,0x3067,0x3069,0x3070,0x3073,0x3076,0x3079,0x307C,
        0x3071,0x3074,0x3077,0x307A,0x307D,
        0x3041,0x3043,0x3045,0x3047,0x3049
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
        g_hiraganaList[g_hiraganaCount][0] = list[i];
        g_hiraganaList[g_hiraganaCount][1] = 0;
        lstrcpyW(g_romajiList[g_hiraganaCount], romaji[i]);
        g_hiraganaCount++;
    }
}

// --- 1. Check Answer ---
void CheckAnswer(int btnId) {
    if (g_isAnswering) return;

    wchar_t answer[8];
    GetWindowTextW(GetDlgItem(GetParent(g_hHiragana), btnId), answer, 8);

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
    InvalidateRect(g_hHiragana, NULL, TRUE);
}

// --- Swap Mode Function ---
void SwapMode() {
    g_isSwapped = !g_isSwapped;
    int idx = g_currentIndex;

    wchar_t topText[8];
    if (g_isSwapped) {
        lstrcpyW(topText, g_romajiList[idx]); 
        lstrcpyW(g_correctAnswer, g_hiraganaList[idx]); 
    } else {
        lstrcpyW(topText, g_hiraganaList[idx]); 
        lstrcpyW(g_correctAnswer, g_romajiList[idx]); 
    }

    std::vector<int> wrongs;
    int i; 
    for (i = 0; i < g_hiraganaCount; i++) {
        if (i != idx) wrongs.push_back(i);
    }

    for (i = (int)wrongs.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = wrongs[i]; wrongs[i] = wrongs[j]; wrongs[j] = temp;
    }

    for (i = 0; i < 3 && i < (int)wrongs.size(); i++) {
        if (g_isSwapped) lstrcpyW(g_options[i], g_hiraganaList[wrongs[i]]);
        else lstrcpyW(g_options[i], g_romajiList[wrongs[i]]);
    }
    lstrcpyW(g_options[3], g_correctAnswer);

    for (i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        wchar_t temp[8];
        lstrcpyW(temp, g_options[i]); lstrcpyW(g_options[i], g_options[j]); lstrcpyW(g_options[j], temp);
    }

    SetWindowTextW(g_hHiragana, topText);
    SetWindowTextW(g_hBtn1, g_options[0]);
    SetWindowTextW(g_hBtn2, g_options[1]);
    SetWindowTextW(g_hBtn3, g_options[2]);
    SetWindowTextW(g_hBtn4, g_options[3]);

    g_bgColor = RGB(45, 45, 45); 
    g_textColor = RGB(240, 240, 240); 
    InvalidateRect(g_hHiragana, NULL, TRUE);
    SetWindowTextW(g_hInfo, g_isSwapped ? L"Select the Hiragana character" : L"Select the Romaji reading");
    
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

    int idx = rand() % g_hiraganaCount;
    g_currentIndex = idx;
    
    wchar_t topText[8];
    if (g_isSwapped) {
        lstrcpyW(topText, g_romajiList[idx]); 
        lstrcpyW(g_correctAnswer, g_hiraganaList[idx]); 
    } else {
        lstrcpyW(topText, g_hiraganaList[idx]); 
        lstrcpyW(g_correctAnswer, g_romajiList[idx]); 
    }

    std::vector<int> wrongs;
    int i; 
    for (i = 0; i < g_hiraganaCount; i++) {
        if (i != idx) wrongs.push_back(i);
    }

    for (i = (int)wrongs.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = wrongs[i]; wrongs[i] = wrongs[j]; wrongs[j] = temp;
    }

    for (i = 0; i < 3 && i < (int)wrongs.size(); i++) {
        if (g_isSwapped) lstrcpyW(g_options[i], g_hiraganaList[wrongs[i]]);
        else lstrcpyW(g_options[i], g_romajiList[wrongs[i]]);
    }
    lstrcpyW(g_options[3], g_correctAnswer);

    for (i = 3; i > 0; i--) {
        int j = rand() % (i + 1);
        wchar_t temp[8];
        lstrcpyW(temp, g_options[i]); lstrcpyW(g_options[i], g_options[j]); lstrcpyW(g_options[j], temp);
    }

    SetWindowTextW(g_hHiragana, topText);
    SetWindowTextW(g_hBtn1, g_options[0]);
    SetWindowTextW(g_hBtn2, g_options[1]);
    SetWindowTextW(g_hBtn3, g_options[2]);
    SetWindowTextW(g_hBtn4, g_options[3]);

    g_bgColor = RGB(45, 45, 45); 
    g_textColor = RGB(240, 240, 240); 
    InvalidateRect(g_hHiragana, NULL, TRUE);

    wchar_t buf[32];
    wsprintfW(buf, L"Score: %d", g_score); SetWindowTextW(g_hScore, buf);
    wsprintfW(buf, L"Round: %d", g_round); SetWindowTextW(g_hRound, buf);
    g_round++;

    SetWindowTextW(g_hInfo, g_isSwapped ? L"Select the Hiragana character" : L"Select the Romaji reading");
    g_isAnswering = false;
    EnableWindow(g_hBtnNext, FALSE);
}

// --- 3. Show Results ---
void ShowResults() {
    ShowWindow(g_hHiragana, SW_HIDE);
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

    ShowWindow(g_hHiragana, SW_SHOW);
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

        g_hHiragana = CreateWindowW(L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, 10, 10, 760, 350, hWnd, (HMENU)ID_HIRAGANA, NULL, NULL);
        SendMessageW(g_hHiragana, WM_SETFONT, (WPARAM)g_hLargeFont, TRUE);

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
        LoadHiraganaData();
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
        
        if (hCtrl == g_hHiragana) {
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

    // --- Load Custom Icon from File ---
    HICON hLargeIcon = (HICON)LoadImageW(NULL, L"Hiragana.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
    HICON hSmallIcon = (HICON)LoadImageW(NULL, L"Hiragana.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
    
    if (!hLargeIcon) hLargeIcon = LoadIcon(NULL, IDI_APPLICATION);
    if (!hSmallIcon) hSmallIcon = LoadIcon(NULL, IDI_APPLICATION);

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
    wcex.lpszClassName = L"HiraganaQuizClass";
    wcex.hIconSm = hSmallIcon;
    
    RegisterClassExW(&wcex);

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int x = (screenWidth - 800) / 2;
    int y = (screenHeight - 800) / 2;

    // --- UPDATED TITLE ---
    HWND hWnd = CreateWindowW(L"HiraganaQuizClass", L"OpenHiragana - Open Source Hiragana Test", WS_OVERLAPPEDWINDOW,
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