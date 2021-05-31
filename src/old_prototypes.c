#include <stdio.h>
#include <stdlib.h>

//#include "src/sqlite3.h"
//
//int callback(void *NotUsed, int argc, char **argv, char **azColName) {
//    NotUsed = 0;
//    for (int i = 0; i < argc; i++) {
//        argv[i][0] = '$';
//        printf("%s ", argv[i] ? argv[i] : "NULL");
//    }
//    printf("\n");
//    return 0;
//}
//
//int main() {
//    printf("SQLITE VERSION %s\n", sqlite3_libversion());
//    printf("This is %s SQLITE DB\n\n", (sqlite3_threadsafe() ? "multithreaded" : "monothreaded"));
//
//    sqlite3 *db;
//    char *err = 0;
//
//    int rc = sqlite3_open("test.db", &db);
//    if (rc != SQLITE_OK) {
//        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
//        sqlite3_close(db);
//
//        return EXIT_FAILURE;
//    }
//
//    const char *sql1 = "DROP TABLE IF NOT EXISTS Cars;"
//                       "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
//                       "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
//                       "INSERT INTO Cars VALUES(2, 'Hummer', 41400);"
//                       "INSERT INTO Cars VALUES(3, 'Volvo', 29000);";
//
//    rc = sqlite3_exec(db, sql1, 0, 0, &err);
//    if (rc != SQLITE_OK) {
//        fprintf(stderr, "SQL error: %s\n", err);
//
//        sqlite3_free(err);
//        sqlite3_close(db);
//
//        return EXIT_FAILURE;
//    }
//
//    const char *sql2 = "SELECT * FROM Cars WHERE Price < 30000";
//    sqlite3_stmt *stmt = NULL;
//
//    int retval, idx;
//    char sql[2048];
//
//    sprintf(sql, "select Something from Somewhere;");
//
//    retval = sqlite3_prepare_v2(db, sql2, -1, &stmt, 0);
//
//    if (retval) {
//        printf("Selecting data from DB Failed (err_code=%d)\n", retval);
//        return 1;
//    }
//
//    idx = 0;
//    while (1) {
//        retval = sqlite3_step(stmt);
//
//        if (retval == SQLITE_ROW) {
//            int val = (int) sqlite3_column_int(stmt, 0);
//            printf("%d\n", val);
//        } else if (retval == SQLITE_DONE) {
//            break;
//        } else {
//            sqlite3_finalize(stmt);
//            printf("Some error encountered\n");
//            break;
//        }
//    }
//
//    sqlite3_finalize(stmt);
//
//    printf("%s\n", sqlite3_str_value(sqlite3_str_new(db)));
//
//    sqlite3_close(db);
//
//    return EXIT_SUCCESS;, -1
//}

#include <windows.h>

#define Red  RGB(255,0,0)
#define Lime RGB(206,255,0)
#define Blue RGB(0,0,255)

static HWND hConWnd;

int BCX_Line(HWND, int, int, int, int, int, HDC);

int BCX_Circle(HWND, int, int, int, int, int, HDC);

HWND GetConsoleWndHandle(void);

int main()
{
    hConWnd = GetConsoleWndHandle();
    if (hConWnd)
    {
        // be creative here, draw your own circles or lines

        //   hwin, centerX, centerY, radius, pencolor
        BCX_Circle(hConWnd, 150, 130, 105, Blue, 0, 0);
        //   hwin, ulcX, ulcY, lrcX, lrcY, pencolor
        BCX_Line(hConWnd, 5, 5, 300, 250, Red, 0);
        BCX_Line(hConWnd, 295, 5, 5, 250, Lime, 0);

        getchar();  // wait
    }
    return 0;
}


int BCX_Line (HWND Wnd,int x1,int y1,int x2,int y2,int Pen,HDC DrawHDC)
{
    int a,b=0;
    HPEN hOPen;
    // penstyle, width, color
    HPEN hNPen = CreatePen(PS_SOLID, 2, Pen);
    if (!DrawHDC) DrawHDC = GetDC(Wnd), b = 1;
    hOPen = (HPEN)SelectObject(DrawHDC, hNPen);
    // starting point of line
    MoveToEx(DrawHDC, x1, y1, NULL);
    // ending point of line
    a = LineTo(DrawHDC, x2, y2);
    DeleteObject(SelectObject(DrawHDC, hOPen));
    if (b) ReleaseDC(Wnd, DrawHDC);
    return a;
}


// converts circle(centerX,centerY,radius,pen) to WinApi function
// ellipse inside box with upper left and lower right coordinates
int BCX_Circle(HWND Wnd,int X,int Y,int R,int Pen,int Fill,HDC DrawHDC)
{
    int a, b = 0;
    if (!DrawHDC) DrawHDC = GetDC(Wnd), b = 1;
    // penstyle, width, color
    HPEN   hNPen = CreatePen(PS_SOLID, 2, Pen);
    HPEN   hOPen = (HPEN)SelectObject(DrawHDC, hNPen);
    HBRUSH hOldBrush;
    HBRUSH hNewBrush;
    // if true will fill circle with pencolor
    if (Fill)
    {
        hNewBrush = CreateSolidBrush(Pen);
        hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
    }
    else
    {
        hNewBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
        hOldBrush = (HBRUSH)SelectObject(DrawHDC, hNewBrush);
    }
    a = Ellipse(DrawHDC, X-R, Y+R, X+R, Y-R);
    DeleteObject(SelectObject(DrawHDC, hOPen));
    DeleteObject(SelectObject(DrawHDC, hOldBrush));
    if (b) ReleaseDC(Wnd, DrawHDC);
    return a;
}


// the hoop ...
HWND GetConsoleWndHandle(void)
{
    HWND hConWnd;
    OSVERSIONINFO os;
    char szTempTitle[64], szClassName[128], szOriginalTitle[1024];

    os.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
    GetVersionEx( &os );
    // may not work on WIN9x
    if ( os.dwPlatformId == VER_PLATFORM_WIN32s ) return 0;

    GetConsoleTitle( szOriginalTitle, sizeof( szOriginalTitle ) );
    sprintf( szTempTitle,"%u - %u", GetTickCount(), GetCurrentProcessId() );
    SetConsoleTitle( szTempTitle );
    Sleep( 40 );
    // handle for NT
    hConWnd = FindWindow( NULL, szTempTitle );
    SetConsoleTitle( szOriginalTitle );

    // may not work on WIN9x
    if ( os.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
    {
        hConWnd = GetWindow( hConWnd, GW_CHILD );
        if ( hConWnd == NULL )  return 0;
        GetClassName( hConWnd, szClassName, sizeof ( szClassName ) );
        while ( strcmp( szClassName, "ttyGrab" ) != 0 )
        {
            hConWnd = GetNextWindow( hConWnd, GW_HWNDNEXT );
            if ( hConWnd == NULL )  return 0;
            GetClassName( hConWnd, szClassName, sizeof( szClassName ) );
        }
    }
    return hConWnd;
}