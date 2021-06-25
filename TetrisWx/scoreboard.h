#ifndef SCOREBOARD_H_INCLUDED
#define SCOREBOARD_H_INCLUDED

#include <map>
#include <wx/string.h>
#include <wx/textfile.h>

class scoreboard {
public:
    std::multimap<double, wxString> sorted_Mmap;        //Multimapa przechowujaca posortowana wg. wynikow tabele gotowa do wyswietlenia.
    void saveScore(wxString username, double newscore); //Metoda zapisujaca informacje znajdujace sie w mapie scoreMap do pliku.
    bool readScoreboardFromFile();                      //Metoda czytajaca wyniki z pliku i zapisujaca je do mapy scoreMap.
    bool isBoardEmpty() {return scoreMap.empty();};     //sprawdzanie czy tablica wynikow jest pusta
    bool isTop(double score);                           //sprawdzanie czy jestesmy w top
    void sortBoard();                                   //Metoda sorujaca mape wedlug punktacji.
    void truncate();

private:
    std::map<wxString, double> scoreMap;                //Mapa przechowujaca dane o tabeli wynikow.
};

#endif // SCOREBOARD_H_INCLUDED
