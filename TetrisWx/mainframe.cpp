#include "mainframe.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
{
    wxBoxSizer *maingrid = new wxBoxSizer(wxHORIZONTAL);    //Tworzenie BoxSizer w ktorym zostana umieszczone panele gry i punktacji
    SetSizer(maingrid);                                     //Ustawianie utworzonego BoxSizer jako sizera.

    scorepanel = new ScorePanel(this);                      //Utworzenie i wskazanie panelu punktacji.
    tetrispanel = new TetrisPanel(this, scorepanel);        //Utworzenie i wskazanie panelu gry, oraz przekazanie utworzonemu obiektowi obiektu scorepanel.

    maingrid->Add(tetrispanel);                             //Dodaje panel z gra do sizera.
    maingrid->Add(scorepanel);                              //Dodaje panel z punktacja do sizera.

    //-O grze-//
    wxMenu *menuMenu = new wxMenu;                          //tworzy opcje w menu
    menuMenu->Append(1, "Autorzy");                         //dodaje opcje do panela menu
    menuMenu->Append(2, "Informacje");                      //dodaje opcje do panela menu

    wxMenuBar *menu = new wxMenuBar;                        //tworzy gorny panel menu
    menu->Append(menuMenu, "&O grze");                      //dodaje do gornego panela

    SetMenuBar(menu);                                       //ustawia menu
    //przylaczanie opcji z menu do wykonywania odpowiednich akcji
    Connect(1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&TetrisPanel::Autorzy); //wyswietlenie autorow
    Connect(2,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&TetrisPanel::Info);   //informacje o grze
    //-------//

    //Tworzy przyciski odpowiadajace za Start, Pauze, Wyniki i wyjcie z gry i umieszcza je w odpowiednich miejscach.--------------//
    wxButton *btn1 = new wxButton(scorepanel, ID_START, wxT("Start"),wxPoint(40,100), wxDefaultSize, 0, wxDefaultValidator);
    wxButton *btn2 = new wxButton(scorepanel, ID_PAUZA, wxT("Pauza"),wxPoint(40,150), wxDefaultSize, 0, wxDefaultValidator);
    wxButton *btn3 = new wxButton(scorepanel, ID_WYNIKI, wxT("Wyniki"),wxPoint(40,200), wxDefaultSize, 0, wxDefaultValidator);
    wxButton *btn4 = new wxButton(scorepanel, ID_EXIT, wxT("Wyjscie"),wxPoint(40,250), wxDefaultSize, 0, wxDefaultValidator);
    //---------------------------------------------------------------------------------------------------------------------------//

    //Przylaczenie przyciskow do wykonywania odpowiednich akcji.-----------------------------------------------------------------//
    Connect(ID_START, wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(TStart));      //Start/Restart gry.
    Connect(ID_PAUZA, wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(TPause));      //Pauza.
    Connect(ID_WYNIKI, wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(TTop));       //Tabela wynikow.
    Connect(ID_EXIT, wxEVT_COMMAND_BUTTON_CLICKED,wxCommandEventHandler(TExit));        //Wyjscie z gry.
    //---------------------------------------------------------------------------------------------------------------------------//

    maingrid->Fit(this);                //Dopasosywanie rozmiaru siatki do pol.
    tetrispanel->SetFocus();            //Ustawienie skupienia na panel z gra.
}

