#ifndef COMMUNICATE_H_INCLUDED
#define COMMUNICATE_H_INCLUDED

#include "mainframe.h"
#include <wx/wxprec.h>


class Communicate : public wxFrame
{
public:
    Communicate(const wxString& title);


    TetrisPanel *m_lp;
    ScorePanel *m_rp;
    wxPanel *m_parent;

};

#endif // COMMUNICATE_H_INCLUDED
