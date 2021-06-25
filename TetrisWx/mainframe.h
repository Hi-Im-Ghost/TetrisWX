#ifndef MAINFRAME_H_INCLUDED
#define MAINFRAME_H_INCLUDED

#include <wx/wx.h>
#include "tetrispanel.h"
#include "scorepanel.h"

//ID ktore zostana pozniej przydzielone do odpowiednich przyciskow.
const int ID_START = 1;
const int ID_WYNIKI = 3;
const int ID_PAUZA = 2;
const int ID_EXIT = 4;
//---------------------------------------------------------------//

class MainFrame : public wxFrame        //Glowne okno programu.
{
public:
    MainFrame(const wxString& title);   //Konstruktor.

    TetrisPanel *tetrispanel;           //Wskaznik do obiektu odpowiadajacego za panel z gra.
    ScorePanel *scorepanel;             //Wskaznik do obiektu odpowiadajacego za panel z punktacja.

    void TStart(wxCommandEvent & event) {tetrispanel->Start();};    //Event uruchamiajacy gre po wcisnieciu przypisanego mu przycisku.
    void TPause(wxCommandEvent & event) {tetrispanel->Pause();};    //Event pauzujacy gre po wcisnieciu przypisanego mu przycisku.
    void TTop(wxCommandEvent & event) {tetrispanel->showTopT();};   //Event otwierajacy okno z najlepszymi wynikami.
    void TExit(wxCommandEvent & event) {Close(true);};              //Event zamykajacy okno z gra konczac gre.

};

#endif // MAINFRAME_H_INCLUDED
