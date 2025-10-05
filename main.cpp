// includes main for all three codes, in commented forms.
// uncomment respective task main to test run.

#include <iostream>
#include "polynomial.h"
using namespace std;

int main() {
    Polynomial p1;
    p1.insertTerm(3, 4);
    p1.insertTerm(2, 2);
    p1.insertTerm(-1, 1);
    p1.insertTerm(5, 0);

    Polynomial p2;
    p2.insertTerm(1, 4);
    p2.insertTerm(1, 0);

    Polynomial sum = p1.add(p2);

    Polynomial p3;
    p3.insertTerm(2, 1);

    Polynomial prod = sum.multiply(p3);

    Polynomial deriv = p1.derivative();

    cout << "p1 = " << p1.toString() << endl;
    cout << "p2 = " << p2.toString() << endl;
    cout << "sum = " << sum.toString() << endl;
    cout << "prod = " << prod.toString() << endl;
    cout << "derivative(p1) = " << deriv.toString() << endl;

    return 0;
}


/**
//Task 2 text editor one, main for that
#include "TextEditor.h"

int main() {
    TextEditor ed;

    ed.insertChar('a');
    ed.insertChar('b');
    ed.moveLeft();
    ed.insertChar('c');
    ed.deleteChar();      // now deletes 'b' at cursor
    ed.moveLeft();
    ed.moveLeft();
    ed.moveRight();
    ed.moveRight();

    return 0;
}



//Task 3 main uno.

#include "UNO.h"
#include <iostream>

int main() {
    UNOGame g(2);         // 2 players
    g.initialize();

    std::cout << g.getState() << "\n";
    g.playTurn();
    std::cout << g.getState() << "\n";
    g.playTurn();
    std::cout << g.getState() << "\n";
    g.playTurn();
    std::cout << g.getState() << "\n";

    return 0;
}
**/
