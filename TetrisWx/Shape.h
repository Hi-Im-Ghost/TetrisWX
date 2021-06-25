#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

enum Tetrominoes { NoShape, ZShape, SShape, LineShape,
                  TShape, SquareShape, LShape, MirroredLShape };    //typy mozliwych ksztaltow klockow

class Shape
{
public:
    Shape() { SetShape(NoShape); }          //konstruktor kawalkow
    void SetShape(Tetrominoes shape);       //funkcja uswaiajaca kawalki
    void SetRandomShape();                  //funkcja losujaca

    Tetrominoes GetShape() const { return pieceShape; }     //funkcja pobierajaca kawalek
    int x(int index) const { return coords[index][0]; }     //pobieranie wspolrzednych
    int y(int index) const { return coords[index][1]; }

    int MinX() const;       //zmienne do zapisu wspolrzednych
    int MaxX() const;
    int MinY() const;
    int MaxY() const;

    Shape RotateLeft() const;       //rotacje klockow
    Shape RotateRight() const;

private:
    void SetX(int index, int x) { coords[index][0] = x; }       //ustawianie wspolrzednych
    void SetY(int index, int y) { coords[index][1] = y; }
    Tetrominoes pieceShape;                                     //ustalenie ksztaltu
    int coords[4][2];
};

#endif // SHAPE_H_INCLUDED
