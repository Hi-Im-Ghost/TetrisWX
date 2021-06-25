#include "scoreboard.h"
#include <wx/file.h>
#include <wx/textfile.h>

void scoreboard::saveScore(wxString username, double newscore)
{
    wxFile file;                        //Tworzy zmienna file odpowiadajaca za plik z wynikami.
    /*
    Uzylem wxFile zamiast wxTextFile poniewaz ten pierwszy udostepnia
    funkcje tworzenia pliku z flaga Overwrite, co pozwala mi na
    tworzenie calego pliku od poczatku za kazdym razem, dzieki czemu
    nie musze go czyscic, lub nadpisywac specyficznej linijki, a moge
    od razu zapisac cala tabele umieszczona w zmiennej scoreMap w pliku scoreboard.h.
    */

    scoreMap[username] = newscore;      //Przypisywanie punktow do nazwy urzytkownika za pomoca mapy zdefniowanej w scoreboard.h.
    file.Create("wyniki", true);        //Tworzenie pliku i jednoczesne go otwarcie.
    sortBoard();                        //Sortowanie tablicy wedlug punktacji.

    wxString scorestr;                  //String ten sluzy do konwersji liczby typu double na ciag wxString.
    for (auto& it : sorted_Mmap)                   //Petla zapisujaca wartosci mapy do pliku.
    {
        scorestr = "";                          //"Czyszczenie" ciagu scorestr, by nastepna przypisana do niej wartosc nie dopisala sie do poprzedniej.

        scorestr.sprintf("%.0f", it.first);    //Konwersja typu double na wxString za pomoca metody sprintf().
                                                //Matoda ta jest uzyta by poprzez %.0f bozbyc sie zapisywania wartosci double
                                                //w notacji naukowej, co robily inne metody.

        file.Write(it.second);                   //Wpisanie do pliku nazwy uzytkownika.
        file.Write("\n");                       //Wstawienie nowej linijki.
        file.Write(scorestr);                   //Wpisanie do pliku punktow uzytkownika.
        file.Write("\n");
    }


    file.Close();                               //Zamykanie pliku

    return;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

void scoreboard::sortBoard()
{

    sorted_Mmap.erase(sorted_Mmap.begin(), sorted_Mmap.end());   //Czyszczenie dotychczasowej zawartosci mmultimapy.

    for(auto& it : scoreMap)                                     //Przepisywanie wartosci z mapy do multimapy, gdzie wartosc pierwsza w mapie wchodzi do multimapy jako druga,
    {                                                            //a wartosc druga jako pierwsza.
        sorted_Mmap.insert({it.second, it.first});
    }

    truncate();

    return;
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

bool scoreboard::readScoreboardFromFile()
{

    wxTextFile file("wyniki");              //Tworzenie zmiennej file o typie wxTextFile pozwalajacym na czytanie pliku linijka po linijce.

    if (!file.Exists())                     //Sprawdza czy plik istnieje.
        return false;

    file.Open();                            //Otwarcie przypisanego wyzej pliku.

    if(!file.IsOpened())                    //Sprawdzanie, czy plik zostal poprawnie otworzony.
        return false;


    wxString filestr, name;                 //wxString filestr sluzy do zapisywania w niej kazdej kolejnej linijki pliku.
                                            //wxString name sluzy do zapisania w niej nazwy uzytkownika ze zmiennej filestr.
                                            //"name" jest potrzebne, poniewaz zmienna filestr przechodzi na nastepna linie w pliku zanim dane
                                            //zaczna zapisywac sie do mapy.

    double dscore;                          //double dscore sluzy do konwersji punktow uzytkownika z typu wxString.

    for(filestr = file.GetFirstLine(); !file.Eof(); filestr = file.GetNextLine())       //Petla iterujaca przez caly plik.
    {

        name = filestr;                     //Pierwszy, oraz co drugi wiersz w pliku, to nazwa uzytkownika.
        filestr = file.GetNextLine();       //Odczytanie nastepnej linii, ktora musi byc wynikiem punktowym danego uzytkownika.
        filestr.ToDouble(&dscore);          //konwersja typu wxString na double.
        scoreMap[name] = dscore;            //Przypisanie wartosci punktowej do uzytkownika w mapie.

        //Poniewaz w srodku petli zostala wykonana metoda GetNextLine(), wskaznik linijki pliku zostal przesuniety, czyli
        //nastepne wywolanie petli ponownie natrafi na nazwe uzytkownika.
    }
    file.Close();                           //Zamykanie pliku.
    return true;                            //Funkcja bool zwraca true po poprawnie wykonanej operacji.
}

bool scoreboard::isTop(double score)        //sprawdzanie czy znajdujemy sie w top
{
    if ( sorted_Mmap.size() < 10)
        return true;

    auto it = sorted_Mmap.begin();
    if (it->first >= score)
        return false;
    return true;
}

void scoreboard::truncate()         //ustawianie rozmiaru
{
    auto rozm = sorted_Mmap.size();
    if (rozm <= 10)
        return;

    auto it = sorted_Mmap.begin();
    advance(it, rozm - 10);
    sorted_Mmap.erase(sorted_Mmap.begin(), it);
}
