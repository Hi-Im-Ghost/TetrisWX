#include "tetrispanel.h"
#include "scorepanel.h"
//----------------------------------------------------------------------------------------------------------------//
//######################################//  Konstruktor TetrisPanel  //###########################################//
//----------------------------------------------------------------------------------------------------------------//
TetrisPanel::TetrisPanel(wxFrame *parent, ScorePanel *span) : wxPanel(parent, 0, 0, 180, 340, wxBORDER_THEME)
{
    timer = new wxTimer(this, 1);   //Utworzenie timera.

    //Inicjalizacja podstawowych zmiennych-//
    isFallingFinished = false;
    isStarted = false;
    isPaused = false;
    numLinesRemoved = 0;
    curX = 0;
    curY = 0;
    sp = span;
    sb = new scoreboard();
    //------------------------------------//

    pausedText = new wxStaticText(this, wxID_ANY, "PAUZA");     //Utworzenie napisu PAUZA.
    pausedText->Center();                                       //Wysrodkowanie go w stosunku do panelu z gra.
    pausedText->Hide();                                         //Ukrycie tekstu.

    ClearBoard();                                               //Wyczyszczenie planszy.

    //Przypisanie zdarzen do odpowiednich funkcji.-------------------------//
    Connect(wxEVT_PAINT, wxPaintEventHandler(TetrisPanel::OnPaint));
    Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(TetrisPanel::OnKeyDown));
    Connect(wxEVT_TIMER, wxCommandEventHandler(TetrisPanel::OnTimer));
    //---------------------------------------------------------------------//

    if (sb->readScoreboardFromFile())
        sb->sortBoard();
}
//----------------------------------------------------------------------------------------------------------------//
//################################################################################################################//
//----------------------------------------------------------------------------------------------------------------//

using namespace std;

void TetrisPanel::Start()
{
    if (isPaused)       //Start nie dziala gdy gra jest spauzowana.
        return;

    isStarted = true;               //Zadeklarowanie zaczenia gry.
    isFallingFinished = false;      //Spadanie klocka nie moze byc zakonczone gdy gra dopiero sie zaczyna.
    numLinesRemoved = 0;            //Resetowanie licznika usunietych linii.
    punkty = 0;                     //wyzerowanie punktow
    sendScore();
    ClearBoard();                   //wyczyszczenie panelu z gra

    pausedText->Hide();             //ukrycie napisu
    pausedText->SetLabel("PAUZA");

    NewPiece();                     //tworzenie nowego obiektu
    this->SetFocus();               //wybranie panela z gra
    timer->Start(300);              //uruchomienie timera
}

void TetrisPanel::Pause()
{
    this->SetFocus();
    if (!isStarted)                 //sprawdzenie czy gra sie uruchomila
        return;

    isPaused = !isPaused;                   //zapauzowanie gry i sprawdzanie czy jest juz zapauzowana
    if (isPaused) {
            pausedText->Show();             //wyswietlenie tekstu pauza
        timer->Stop();                      //zatrzymanie timera
    } else {
        pausedText->Hide();                 //ukrycie tekstu
        timer->Start(300);                  //wznowienie gry
    }
    Refresh();
}

void TetrisPanel::showTopT()
{
    if(isStarted)
        if(!isPaused)                   //zapauzowanie gry
            Pause();

    if ( sb->isBoardEmpty() )                   //jesli tablica wynikow jest puta wysiwetl komunikat
    {
        wxMessageBox("Brak wynikow");
        return;
    }

    sb->sortBoard();                    //wyswietl tablice wynikow

    wxString strtop;
    int i = 1;
    for(auto it = sb->sorted_Mmap.rbegin(); it != sb->sorted_Mmap.rend(); it++)     //Wyswietlanie mapy od konca
    {
        wxString buf;
        buf.sprintf("%d. %s - %.0f\n",i, it->second, it->first);        //wswietlanie
        strtop << buf;
        i++;
    }

    wxMessageDialog msgD(this, strtop, "TOP-10", wxICON_NONE);  //utworzenie okna

    msgD.ShowModal();   //wyswietlenie utworzonego okna
}

void TetrisPanel::OnPaint(wxPaintEvent& event) //rysowanie ksztaltow
{
    wxPaintDC dc(this);

    wxSize size = GetClientSize(); //pobieranie rozmiaru
    int boardTop = size.GetHeight() - BoardHeight * SquareHeight();


    for (int i = 0; i < BoardHeight; ++i) { //rysowanie wszystkich ksztaltow
        for (int j = 0; j < BoardWidth; ++j) {
            Tetrominoes shape = ShapeAt(j, BoardHeight - i - 1); //pobieranie kwadratow
            if (shape != NoShape)
                DrawSquare(dc, 0 + j * SquareWidth(),
                           boardTop + i * SquareHeight(), shape); //umieszczanie pod spodem planszy
        }
    }

    if (curPiece.GetShape() != NoShape) { //rysowanie rzeczywistego elementu ktory spada
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);   //wpisywanie kordow w zmienne
            int y = curY - curPiece.y(i);
            DrawSquare(dc, 0 + x * SquareWidth(),   //rysowanie
                       boardTop + (BoardHeight - y - 1) * SquareHeight(),
                       curPiece.GetShape());    //ustawianie
        }
    }
}

void TetrisPanel::OnKeyDown(wxKeyEvent& event)
{
    if (!isStarted || curPiece.GetShape() == NoShape) {     //Pominiecie zdarzenia, gdy gra sie nie rozpoczela, lub gracz nie ma klocka.
        event.Skip();
        return;
    }

    int keycode = event.GetKeyCode();                       //Pobranie wartosci przycisku z klawiatury.
    if (keycode == WXK_ESCAPE) {                            //Po nacisnieciu klawisza escape nastepuje wylaczenie gry.
        exit(true);
    }

    if (keycode == 80) {                                //Po nacisnieciu klawisza P nastepuje pauza.
        Pause();
        return;
    }
    if (isPaused)                                           //Nic nie rob, gdy gra jest spauzowana.
        return;

    switch (keycode) {
    case WXK_LEFT:
        TryMove(curPiece, curX - 1, curY);                  //Strzalka w lewo probuje przesunac klocek w lewo.
        break;
    case WXK_RIGHT:
        TryMove(curPiece, curX + 1, curY);                  //Strzalka w prawo probuje przesunac klocek w prawo.
        break;
    case WXK_DOWN:
        TryMove(curPiece.RotateRight(), curX, curY);        //Strzalka w dol probuje obrocic klocek w prawo.
        break;
    case WXK_UP:
        TryMove(curPiece.RotateLeft(), curX, curY);         //Strzalka w gore probuje obrocic klocek w lewo.
        break;
    case WXK_SPACE:                                         //Spacja upuszcza klocek na sam dol.
        DropDown();
        break;
    case WXK_CONTROL:                                       //Przycisk control przyspiesza opadanie klocka.
        OneLineDown();
        break;
    default:
        event.Skip();                                       //Ignoruj zdarzenie, gdy wcisniety zostal inny przycisk.
    }

}

void TetrisPanel::OnTimer(wxCommandEvent& event)
{
    if (isFallingFinished) {        //Jezeli klocek spadnie wez nowy klocek.
        isFallingFinished = false;
        NewPiece();                 //tworzymy nowy klocek
    } else {
        OneLineDown();              //Klocek jeszcze nie spadl, wiec przesunecie linie w dol.
    }
}

void TetrisPanel::ClearBoard()
{
    for (int i = 0; i < BoardHeight * BoardWidth; ++i)  //Iterowanie przez cala tablice i ustawianie pustych klockow.
        board[i] = NoShape;
}

void TetrisPanel::DropDown()
{
    int newY = curY;
    while (newY > 0) {
        if (!TryMove(curPiece, curX, newY - 1))         //przesuwanie klocka dopoki jest mozliwosc
            break;
        --newY;
    }
    PieceDropped();                                     //wywolanie metody zapisujacej ostatnia pozycje klocka
    AddToPoints(1);                                     //Gracz dostaje 1 pkt. za kazde natychmiastowe upuszczenie klocka.
    sendScore();                                        //wysylanie punktu
}

void TetrisPanel::OneLineDown()
{
    if (!TryMove(curPiece, curX, curY - 1))             //spadanie o 1 linie
        PieceDropped();                                 //wywolanie metody zapisujacej ostatnia pozycje klocka
}

void TetrisPanel::PieceDropped()
{
    for (int i = 0; i < 4; ++i) {
        int x = curX + curPiece.x(i);
        int y = curY - curPiece.y(i);
        ShapeAt(x, y) = curPiece.GetShape();        //pobieranie pozycji klocka
    }

    RemoveFullLines();                              //usuwamy zapelniona linie

    if (!isFallingFinished)                         //sprawdzamy czy spadl i tworzymy nowy
        NewPiece();
}

void TetrisPanel::RemoveFullLines()
{
     int numFullLines = 0;

     for (int i = BoardHeight - 1; i >= 0; --i) {
         bool lineIsFull = true;

         for (int j = 0; j < BoardWidth; ++j) {
             if (ShapeAt(j, i) == NoShape) {
                 lineIsFull = false;
                 break;
             }
         }

         if (lineIsFull) {                                      //jesli linia jest pelna
             ++numFullLines;                                    //zwieksz licznik
             for (int k = i; k < BoardHeight - 1; ++k) {         //przeniesc wszystkie linie powyzej pelnego o 1 w dol
                 for (int j = 0; j < BoardWidth; ++j)
                     ShapeAt(j, k) = ShapeAt(j, k + 1);
             }
             AddToPoints(100);          //daj punkty za usuniecie
             sendScore();
         }
     }

     if (numFullLines > 0) {
         numLinesRemoved += numFullLines;
         if (numFullLines == 4)
         {
             AddToPoints(400);
             iloscTetris += 1;
             sendScore();
         }
         isFallingFinished = true;
         curPiece.SetShape(NoShape);
         Refresh();
     }
 }

void TetrisPanel::NewPiece()                        //metoda tworzenia nowego kawalka
{
    curPiece.SetRandomShape();                  //losowanie ksztaltu
    curX = BoardWidth / 2 + 1;
    curY = BoardHeight - 1 + curPiece.MinY();   //pobieranie pozycji

    if (!TryMove(curPiece, curX, curY))
    {
        endGame();      //Gdy klocek nie jest w stanie sie ruszyc zaraz po pojawieniu, zakoncz gre.
    }
}

bool TetrisPanel::TryMove(const Shape& newPiece, int newX, int newY)            //przesuwanie ksztaltow
{
    for (int i = 0; i < 4; ++i) {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);                                   //pobieranie pozycji i sprawdzanie
        if (x < 0 || x >= BoardWidth || y < 0 || y >= BoardHeight)
            return false;                                           //jesli nasz obiekt znajduje sie na krawedzi planszy
        if (ShapeAt(x, y) != NoShape)                               //lub przylega do innego zwracamy false
            return false;                                           //w przeciwnym razie umieszczamy opadajacy ksztalt w nowej pozycji i zwracamy true
    }

    curPiece = newPiece;
    curX = newX;
    curY = newY;
    Refresh();
    return true;
}

void TetrisPanel::endGame()
{
    curPiece.SetShape(NoShape);             //Ustawienie uzywanego klocka na brak klocka.
    timer->Stop();                          //Zatrzymanie timera.
    isStarted = false;                      //Deklaracja zakonczenia gry.
    pausedText->SetLabel("KONIEC GRY");     //Zmiana napisu pauzy na KONIEC GRY.
    pausedText->Show();                     //Wyswietlenie napisu Koniec Gry.

    if (!sb->isTop(punkty))
        return;

    wxTextEntryDialog Dnazwa(this, "Podaj swoja nazwe (Do 10 znakow)", "Znajdujesz sie w TOP10");
    Dnazwa.SetMaxLength(10);
    Dnazwa.ShowModal();

    sb->saveScore(Dnazwa.GetValue(), punkty);
}

void TetrisPanel::DrawSquare(wxPaintDC& dc, int x, int y, Tetrominoes shape)
{
    static wxColour colors[] = { wxColour(0, 0, 0), wxColour(255, 0, 0),  //kolor klocków
             wxColour(255, 20, 147), wxColour(0, 0, 255),
             wxColour(255, 255, 0), wxColour(255, 0, 255),
             wxColour(0,255, 127), wxColour(0, 255, 255) };

    static wxColour light[] = { wxColour(0, 0, 0), wxColour(255, 255, 255), //obramowanie zewnatrz
             wxColour(255, 255, 255), wxColour(255, 255, 255),
             wxColour(255, 255, 255), wxColour(255, 255, 255),
             wxColour(255, 255, 255), wxColour(255, 255, 255) };

    static wxColour dark[] = { wxColour(0, 0, 0), wxColour(0, 0, 0),  //obramowanie wewnatrz
             wxColour(0, 0, 0), wxColour(0, 0, 0),
             wxColour(0, 0, 0), wxColour(0, 0, 0),
             wxColour(0, 0, 0), wxColour(0, 0, 0) };


    wxPen pen(light[int(shape)]);       //rysowanie zewnetrzych linii
    pen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(pen);

    dc.DrawLine(x, y + SquareHeight() - 1, x, y);
    dc.DrawLine(x, y, x + SquareWidth() - 1, y);

    wxPen darkpen(dark[int(shape)]);        //rysowanie wewnetrznych linii
    darkpen.SetCap(wxCAP_PROJECTING);
    dc.SetPen(darkpen);

    dc.DrawLine(x + 1, y + SquareHeight() - 1,
        x + SquareWidth() - 1, y + SquareHeight() - 1);
    dc.DrawLine(x + SquareWidth() - 1,
        y + SquareHeight() - 1, x + SquareWidth() - 1, y + 1);

    dc.SetPen(*wxTRANSPARENT_PEN);                  //zapelnanie kolorem powstalego ksztaltu
    dc.SetBrush(wxBrush(colors[int(shape)]));
    dc.DrawRectangle(x + 1, y + 1, SquareWidth() - 2,
        SquareHeight() - 2);
}

void TetrisPanel::Autorzy(wxCommandEvent& event)
{
    char autorzy[100] = "Adrian Chmielowiec \nMateusz Kubas";
    wxString msg = autorzy;
    wxMessageBox(msg, _("Autorzy"));
}

void TetrisPanel::Info(wxCommandEvent& event)
{
    char informacje[300] = "Dostepne skroty klawiszowe:\n 1. SPACE - upuszcza klocek\n 2. CTRL - przyspiesza spadanie klocka\n 3. P - pauza\n 4. ESC - wyjscie\nGra oparta na orginalnych zalozeniach gry TETRIS autorstwa Aleksieja Pazytnowa i jego wspolpracownikow z 6 czerwca 1984r";
    wxString msg = informacje;
    wxMessageBox(msg, _("Informacje"));
}
