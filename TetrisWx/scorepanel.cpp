#include "scorepanel.h"
#include "mainframe.h"

ScorePanel::ScorePanel(wxFrame *parent) : wxPanel(parent, 0, 0, 180, 340, wxBORDER_NONE)                //Tworzony panel ma wymiary 180x340px.
{
    wxStaticText *text = new wxStaticText(this, wxID_ANY, "Punkty:", wxPoint(20, 20), wxDefaultSize);    //Tworzy tekst mowiacy "Punkty:" i umieszcza go w odpowiednim miejscu w panelu.
    scoreText = new wxStaticText(this, wxID_ANY, "00000000", wxPoint(20, 40), wxDefaultSize);            //Tworzy tekst z punktacja aktualizowana podczas gry i umieszcza go na panelu.

}

void ScorePanel::displayScore(double score)
{
    wxString wynik;                                     //Tworzy string "wynik" ktory bedzie uzyty do wyswietlenia punktacji.
    size_t l = wxString::FromDouble(score).length();    //Zapisuje dlugosc ciagu znakow zajmowanych przez ilosc punktow.

    if (l < 8)                                          //Sprawdza czy "dlugosc" punktacji nie przekracza 8.
    {
        for(int i=0; i < 8 - l; i++)                    //Najpierw wkleja do wyniku tyle zer, ile brakuje punktom znakow, by osiagnac dlugosc 8
        {
            wynik.append('0');
        }
    }
    wynik.append(wxString::FromDouble(score));          //Dolacza do dolaczanych wczesniej zer punkty gracza.
    scoreText->SetLabel(wynik);                         //Aktualizuje punktacje w scoreText.
}
