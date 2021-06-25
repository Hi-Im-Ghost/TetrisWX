#ifndef SCOREPANEL_H_INCLUDED
#define SCOREPANEL_H_INCLUDED

#include <wx/wx.h>

class ScorePanel : public wxPanel           //Klasa dzedziczy z wxPanel.
{
public:
    ScorePanel(wxFrame *parent);
    void displayScore(double score);        //Metoda wyswietlajaca przekazana jej liczbe punktow w polu scoreText.

private:
    wxStaticText *scoreText;                //Sluzy do wyspietlania aktualnej liczby punktow w rozgrywce.
};

#endif // SCOREPANEL_H_INCLUDED
