#include <wx/wx.h>
#include "mainframe.h"

class TetrisApp : public wxApp
{
public:
    virtual bool OnInit();
};

IMPLEMENT_APP(TetrisApp)

bool TetrisApp::OnInit()
{
    srand(time(NULL));

    MainFrame *mainframe = new MainFrame("TetrisWx");   //Utworzenie glownego okna aplikacji i ustawienie tytulu

    mainframe->Centre();                                //Sprawia ze aplikacja pojawia sie na srodku ekranu
    mainframe->Show(true);                              //Sprawia ze aplikazja wogole sie pojawia na ekranie
    return true;
}
