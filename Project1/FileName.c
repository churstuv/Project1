#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <Windows.h>

#define MAP_WIDTH 23
#define MAP_HEIGHT 15
#define TOTAL_TILES 200
#define COLORS_NUM 10 // ?? ?”ì²­?¬í•­ ë°˜ì˜: NUM_COLORS -> COLORS_NUM

// 1. ?€??êµ¬ì¡°ì²??•ì˜ (ì²?ë²ˆì§¸ ë°©ì‹: struct ?¤ì›Œ?œë? ê·¸ë?ë¡??¬ìš©!)
struct Tile {
    int color;       // 0 ~ 9 (10ê°€ì§€ ?‰ìƒ)
    int x, y;        // ê·¸ë¦¬??ì¢Œí‘œ
    bool isExists;   // ?„ì¬ ì¹¸ì— ?€?¼ì´ ì¡´ì¬?˜ëŠ”ê°€?
}; // <-- êµ¬ì¡°ì²??•ì˜ ?ì—???¸ë?ì½œë¡ (;) ?„ìˆ˜!

// ?„ì—­ ë³€?˜ë¡œ ê²Œì„??? ì–¸ (?ë£Œ???´ë¦„ ?ì— structë¥?ê¼?ë¶™ì—¬ì¤?)
struct Tile board[MAP_HEIGHT][MAP_WIDTH];

int score = 0;
int gameTime = 120; // ?œí•œ?œê°„ 120ì´?

// ?¨ìˆ˜ ? ì–¸ (?„ë¡œ? í???
void initBoard();
void printBoard();
void searchAndMatch(int startX, int startY);
void shuffleBoard();
bool checkDeadlock();

int main(void) {
    system("cls");
    // 1. ?œìˆ˜ ?ì„±ê¸?ì´ˆê¸°??(ê°€??ë¨¼ì? ?¤í–‰?˜ì–´????)
    srand((unsigned int)time(NULL));

    // 2. ê²Œì„??ì´ˆê¸°??(ë¬´ì‘??200ê°?ë°°ì¹˜)
    initBoard();

    // 3. ?ŒìŠ¤?¸ìš© ë©”ì¸ ê²Œì„ ë£¨í”„ (?°ë???ì¢Œí‘œ ?…ë ¥ ë°©ì‹)
    while (gameTime > 0) {
        system("cls"); // ?”ë©´ ì§€?°ê¸° (Windows ?„ìš©)

        printf("=== Color Tiles ë°±ì—”???ŒìŠ¤??===\n");
        printf("?„ì¬ ?ìˆ˜: %d ??| ?¨ì? ?œê°„: %d ì´?n\n", score, gameTime);

        // ë§?ì¶œë ¥
        printBoard();

        // êµì°© ?íƒœ ê²€???ˆì‹œ
        if (checkDeadlock()==true) {
            printf("\n[êµì°© ?íƒœ ê°ì?!] ?€?¼ì„ ?ë™?¼ë¡œ ?¬ë°°ì¹˜í•©?ˆë‹¤.\n");
            Sleep(2000); // 2ì´??€ê¸?
            shuffleBoard();
            continue;
        }

        // ?°ë????…ë ¥ ?ŒìŠ¤?¸ìš© (?˜ì¤‘???„ë¡ ?¸ì—”?œê? ë§ˆìš°???´ë¦­ ì¢Œí‘œë¡??€ì²´í•  ë¶€ë¶?
        int inputX, inputY;
        printf("\n?´ë¦­??ë¹?ì¹¸ì˜ ì¢Œí‘œë¥??…ë ¥?˜ì„¸??(X Y ?…ë ¥, ì¢…ë£Œ??-1 -1): ");
        scanf_s("%d %d", &inputX, &inputY);

        if (inputX == -1 && inputY == -1) {
            printf("ê²Œì„??ì¢…ë£Œ?©ë‹ˆ??\n");
            break;
        }

        // ?…ë ¥ ì¢Œí‘œ ?ˆì™¸ ì²˜ë¦¬
        if (inputX < 0 || inputX >= MAP_WIDTH || inputY < 0 || inputY >= MAP_HEIGHT) {
            printf("?˜ëª»??ë²”ìœ„??ì¢Œí‘œ?…ë‹ˆ??\n");
            Sleep(1000);
            continue;
        }

        // ê·œì¹™ 1: ë¸”ë¡??ì¡´ì¬?˜ëŠ” ì¹¸ì„ ?´ë¦­?ˆì„ ?ŒëŠ” ?„ë¬´???¼ë„ ë°œìƒ?˜ì? ?ŠìŒ
        if (board[inputY][inputX].isExists) {
            printf("?€?¼ì´ ?´ë? ì¡´ì¬?˜ëŠ” ì¹¸ì? ?´ë¦­?????†ìŠµ?ˆë‹¤!\n");
            Sleep(1000);
            continue;
        }

        // ?ìƒ‰ ë°?ë§¤ì¹­ ë¡œì§ ?¤í–‰
        searchAndMatch(inputX, inputY);

        // ?œê°„ ê°ì†Œ ?œë??ˆì´??(ë£¨í”„ ???Œë§ˆ??1ì´ˆì”© ê°ì†Œ?œë‹¤ê³?ê°€??
        gameTime--;
    }

    printf("\n=== GAME OVER ===\n");
    printf("ìµœì¢… ?ìˆ˜: %d ??n", score);

    return 0;
}

// ?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€
// ë¹„ì–´?ˆëŠ” ë°±ì—”???µì‹¬ ?¨ìˆ˜ êµ¬ì—­
// ?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€?€

// [ë¯¸ì…˜ 1] 23x15 ê³µê°„ ì¤?'ì¤‘ë³µ ?†ì´ ë¬´ì‘??200ì¹??ë§Œ 10ê°€ì§€ ?‰ìƒ ?€??ë°°ì¹˜?˜ê¸°
void initBoard() {
    // 1?¨ê³„: ?°ì„  ëª¨ë“  345ì¹¸ì„ ê¹¨ë—?˜ê²Œ ë¹ˆì¹¸ ?íƒœ(-1, false)ë¡?ë°€?´ë²„ë¦¬ê¸°
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            board[y][x].color = -1;
            board[y][x].x = x;
            board[y][x].y = y;
            board[y][x].isExists = false;
        }
    }

    // 2?¨ê³„: ì¤‘ë³µ ?†ì´ ë¬´ì‘??ì¢Œí‘œë¥??¬ë°©?¼ë¡œ ì°”ëŸ¬ê°€ë©???200ê°?ì±„ìš°ê¸?
    int count = 0;
    while (count < TOTAL_TILES) {
        int randx = rand() % MAP_WIDTH;
        int randy = rand() % MAP_HEIGHT;

        if (board[randy][randx].isExists == false) {
            board[randy][randx].isExists = true;
            board[randy][randx].color = rand() % COLORS_NUM; // ?˜ì •???ìˆ˜ ?ìš©
            count++;
        }
        else {
            continue; // ?? ?”ì²­?¬í•­ ë°˜ì˜: ëª…ì‹œ??continue ?¬ìš©
        }
    }
}

// ?°ë??ì— ë§??°ì´?°ë? ?ˆì˜ê²?ì°ì–´ì£¼ëŠ” ?¨ìˆ˜
void printBoard() {
    // ?ë‹¨ Xì¶??¸ë±??ê°€?´ë“œ
    printf("   ");
    for (int x = 0; x < MAP_WIDTH; x++) printf("%2d", x);
    printf("\n   ");
    for (int x = 0; x < MAP_WIDTH; x++) printf("--");
    printf("\n");

    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("%2d|", y); // ì¢Œì¸¡ Yì¶??¸ë±??ê°€?´ë“œ
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (board[y][x].isExists) {
                printf("%2d", board[y][x].color); // ?€?¼ì´ ?ˆìœ¼ë©??‰ìƒ ?«ì ì¶œë ¥
            }
            else {
                printf(" ."); // ë¹?ì¹¸ì? ??.)?¼ë¡œ ?œì‹œ
            }
        }
        printf("\n");
    }
}

// [ë¯¸ì…˜ 2] ?´ë¦­ ?„ì¹˜ ê¸°ì? ?í•˜ì¢Œìš° ìµœë‹¨ê±°ë¦¬ ?€??ë§¤ì¹­ ë°??ìˆ˜/?˜ë„???°ì‚°
void searchAndMatch(int startX, int startY) {
    // ?¬ê¸°???¬ë°© ?ìƒ‰ ë¡œì§ ?‘ì„± ?ˆì •
    struct Tile* leftTile = NULL;
    struct Tile* rightTile = NULL;
    struct Tile* upTile = NULL;
    struct Tile* downTile = NULL;
    printf("\n(%d, %d) ì¢Œí‘œë¥?ê¸°ì??¼ë¡œ 4ë°©í–¥ ?ìƒ‰???˜í–‰?´ì•¼ ?©ë‹ˆ??\n", startX, startY);
    for (int x = startX - 1; x >= 0; x--) {
        if (board[startY][x].isExists == true) {
            leftTile = &board[startY][x];
            break;
        }
    }
    for (int x = startX + 1; x < MAP_WIDTH; x++) {
        if (board[startY][x].isExists == true) {
            rightTile = &board[startY][x];
            break;
        }
    }
    for (int y = startY - 1; y >= 0; y--) {
        if (board[y][startX].isExists == true) {
            upTile = &board[y][startX];
            break;
        }
    }
    for (int y = startY + 1; y < MAP_HEIGHT; y++) {
        if (board[y][startX].isExists == true) {
            downTile = &board[y][startX];
            break;
        }
    }
    int colorCount[COLORS_NUM] = { 0 }; // ê°??‰ìƒë³„ë¡œ ë°œê²¬???€???˜ë? ?¸ëŠ” ë°°ì—´
    if (leftTile != NULL) {
        colorCount[(*leftTile).color] += 1;
    }
    if (rightTile != NULL) {
        colorCount[(*rightTile).color] += 1;
    }
    if (upTile != NULL) {
        colorCount[(*upTile).color] += 1;
    }
    if (downTile != NULL) {
        colorCount[(*downTile).color] += 1;
    }

    int colorMatched = 0;
    for (int i = 0; i < COLORS_NUM; i++) {
        if (colorCount[i] > 1) {
            score += colorCount[i];
            colorMatched = 1;
            if (upTile != NULL && (*upTile).color == i) {
                (*upTile).isExists = false;
            }
            if (downTile != NULL && (*downTile).color == i) {
                (*downTile).isExists = false;
            }
            if (leftTile != NULL && (*leftTile).color == i) {
                (*leftTile).isExists = false;
            }
            if (rightTile != NULL && (*rightTile).color == i) {
                (*rightTile).isExists = false;
            }
        }
    }
    if (colorMatched == 0) {
        gameTime -= 10;
    }

    Sleep(1500);
}

// [ë¯¸ì…˜ 3] êµì°© ?íƒœ ???„ì¬ ?”ë©´???¨ì? ?€?¼ë“¤???„ì¹˜ë¥?ë¬´ì‘?„ë¡œ ?¤ì‹œ ?ê¸°
void shuffleBoard() {
    int remainTile[TOTAL_TILES] = { 0 };
    int count = 0;
    
    for (int startY = 0; startY < MAP_HEIGHT; startY++) {
		for (int startX = 0; startX < MAP_WIDTH; startX++) {
			if (board[startY][startX].isExists == true) {
				remainTile[count] = board[startY][startX].color;
				count++;
			}
		}
    }

    for (int i = 0; i < count; i++) {
        int newIndex = rand() % count;
        int tmp = remainTile[i];
        remainTile[i] = remainTile[newIndex];
        remainTile[newIndex] = tmp;
    }

    int idx = 0;
    for (int startY = 0; startY < MAP_HEIGHT; startY++) {
        for (int startX = 0; startX < MAP_WIDTH; startX++) {
            if (board[startY][startX].isExists == true) {
                board[startY][startX].color = remainTile[idx];
                idx++;
            }
        }
    }
}

// [ë¯¸ì…˜ 4] ???´ìƒ ë§ì¶œ ???ˆëŠ” ?€???ì´ ?†ëŠ” êµì°© ?íƒœ(Deadlock) ê²€??
bool checkDeadlock() {
    return false; // ?„ì§ ê²€??ê¸°ëŠ¥???†ìœ¼ë¯€ë¡???ƒ ê±°ì§“(false)?¼ë¡œ ?„ì‹œ ë°˜í™˜
}