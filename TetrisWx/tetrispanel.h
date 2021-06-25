#ifndef TETRISPANEL_H_INCLUDED
#define TETRISPANEL_H_INCLUDED

#include <wx/wx.h>
#include "Shape.h"
#include "scorepanel.h"
#include "scoreboard.h"

class TetrisPanel : public wxPanel                      //Klasa dziedziczy z wxPanel.
{
public:
    TetrisPanel(wxFrame *parent, ScorePanel *span);     //Konstruktor.
    void Start();                                       //Zacznij gre
    void Pause();                                       //Pauza
    void AddToPoints(double value) {punkty += value;};  //Dodawanie wartosci double do zmiennej "punkty".
    double GetPoints() {return punkty;};                //Pobieranie wartosci zmiennej "punkty".
    void sendScore() {sp->displayScore(punkty);};       //Aktualizacja wyswietlanej na panelu obok punktacji.
    void endGame();                                     //Kroki podejmowane podczas gdy gracz przegra.
    void showTopT();                                    //tabela wynikow

    void Autorzy(wxCommandEvent& event);                //event otwierajacy okno z autorami
    void Info(wxCommandEvent& event);                   //event otwierajaco okno z informacjami

    ScorePanel *sp;                                     //Wskaznik na panel punktacji wykorzystywany w celu wysy³ania tam punktow i informacji o nastepnym klocku.
    wxStaticText *pausedText;                           //tekst pokazujacy sie gdy gra jest spauzowana lub zakonczona.
    scoreboard *sb;                                     //Utworzenie wskaznika do obiektu tabeli wynikow.

protected:
    void OnPaint(wxPaintEvent& event);                  //Event wykonujacy sie za kazdym razem, gdy plansza jest rysowana.
    void OnKeyDown(wxKeyEvent& event);                  //Event wywolujacy sie za kazdym razem, gdy wciskany jest przycisk.
    void OnTimer(wxCommandEvent& event);                //Event wywolujacy sie z czestotliwoscia timera.


private:
    enum { BoardWidth = 10, BoardHeight = 22 };         //Szerokosc i wysokosc planszy uzywane potem m.in. do czyszczenia jej.

    Tetrominoes & ShapeAt(int x, int y) { return board[(y * BoardWidth) + x]; }     //

    int SquareWidth() { return GetClientSize().GetWidth() / BoardWidth; }           //szerokosc ksztaltow
    int SquareHeight() { return GetClientSize().GetHeight() / BoardHeight; }        //wysokosc ksztaltow
    void ClearBoard();                                                              //Czyszczenie planszy z jakichkolwiek klockow.
    void DropDown();                                                                //Natychmiastowe upuszczenie klocka na sam dol.
    void OneLineDown();                                                             //przesuwanie linii
    void PieceDropped();                                                            //ustalenie ostatniej pozycji klocka
    void RemoveFullLines();                                                         //usuwanie wierszy
    void NewPiece();                                                                //Wylosowanie i ustawienie nowego klocka.
    bool TryMove(const Shape& newPiece, int newX, int newY);                        //poruszanie klockami
    void DrawSquare(wxPaintDC &dc, int x, int y, Tetrominoes shape);                //rysowanie ksztaltow

    wxTimer *timer;             //Timer dzialajacy na okreslonej czestotlwosci i poruszajacy klockami.
    bool isStarted;             //sprawdzanie czy gra sie zaczela
    bool isPaused;              //sprawdzanie czy gra jest zapauzowana
    bool isFallingFinished;     //sprawdzanie czy klocek spadl
    Shape curPiece;             //pozycja klocka
    int curX;                   //wspolrzedna x
    int curY;                   //wspolrzedna y
    int numLinesRemoved;        //liczba wpelni ulozonych wierszy.
    int iloscTetris;            //liczba pokazujaca ile razy udalo sie graczowi wykonac zagranie "tetris".
    double punkty;              //Punkty zdobyte w rozgrywce.
    Tetrominoes board[BoardWidth * BoardHeight];    //tablica ksztaltow klockow

};

#endif // TETRISPANEL_H_INCLUDED
