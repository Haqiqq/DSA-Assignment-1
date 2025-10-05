#include "Polynomial.h"
#include <sstream>
#include <cmath>
#include <map>
using namespace std;

// Node for polynomial 
struct Node {
    int coeff;
    int exp;
    Node* next;
    Node(int c, int e) : coeff(c), exp(e), next(nullptr) {}
};

// Internal storage class (not exposed in header)
class PolyData {
public:
    Node* head;
    PolyData() : head(nullptr) {}
    ~PolyData() {
        Node* curr = head;
        while (curr) {
            Node* tmp = curr->next;
            delete curr;
            curr = tmp;
        }
    }
};

// Keep track of internal data for each Polynomial
static PolyData*& accessData(const Polynomial* p) {
    static map<const Polynomial*, PolyData*> table;
    return table[p];
}


Polynomial::Polynomial() {
    // whenever a Polynomial is created, give it its own PolyData
    accessData(this) = new PolyData();
}

Polynomial::~Polynomial() {
    // free the internal data when a Polynomial goes out of scope
    delete accessData(this);
    accessData(this) = nullptr;
}

void Polynomial::insertTerm(int coeff, int exp) {
    // ignore invalid inputs
    if (coeff == 0 || exp < 0) return;

    PolyData* data = accessData(this);
    Node*& head = data->head;

    Node* prev = nullptr;
    Node* curr = head;

    // find the correct spot (list is sorted by descending exponents)
    while (curr && curr->exp > exp) {
        prev = curr;
        curr = curr->next;
    }

    // if the exponent already exists, just update the coefficient
    if (curr && curr->exp == exp) {
        curr->coeff += coeff;
        // if the result is zero, remove the node completely
        if (curr->coeff == 0) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            delete curr;
        }
        return;
    }

    // or make new term and link it in the right spot
    Node* newNode = new Node(coeff, exp);
    if (!prev) { // insert at head
        newNode->next = head;
        head = newNode;
    } else {     // insert after prev
        newNode->next = curr;
        prev->next = newNode;
    }
}

string Polynomial::toString() const {
    Node* curr = accessData(this)->head;
    if (!curr) return "0"; // empty polynomial is just zero

    ostringstream out;
    bool first = true;

    while (curr) {
        int c = curr->coeff, e = curr->exp;

        // handle signs
        if (c > 0 && !first) out << " + ";
        if (c < 0) out << (first ? "-" : " - ");
        int absC = abs(c);

        // handle coefficient and exponent display rules
        if (e == 0) out << absC;                     // constant term
        else if (e == 1) {                          // linear term
            if (absC != 1) out << absC;
            out << "x";
        } else {                                    // higher powers
            if (absC != 1) out << absC;
            out << "x^" << e;
        }

        first = false;
        curr = curr->next;
    }

    return out.str();
}

Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial result;
    Node* a = accessData(this)->head;
    Node* b = accessData(&other)->head;


    while (a || b) {
        int coeff, exp;
        if (!b || (a && a->exp > b->exp)) {
            coeff = a->coeff; exp = a->exp; a = a->next;
        } else if (!a || (b->exp > a->exp)) {
            coeff = b->coeff; exp = b->exp; b = b->next;
        } else {
            coeff = a->coeff + b->coeff; exp = a->exp;
            a = a->next; b = b->next;
        }
        if (coeff != 0) result.insertTerm(coeff, exp);
    }
    return result;
}

Polynomial Polynomial::multiply(const Polynomial& other) const {
    Polynomial result;
    // multiply each term of first poly with each term of second poly
    for (Node* a = accessData(this)->head; a; a = a->next) {
        for (Node* b = accessData(&other)->head; b; b = b->next) {
            result.insertTerm(a->coeff * b->coeff, a->exp + b->exp);
        }
    }
    return result;
}

Polynomial Polynomial::derivative() const {
    Polynomial result;
    // apply power rule:
    for (Node* a = accessData(this)->head; a; a = a->next) {
        if (a->exp > 0) {
            result.insertTerm(a->coeff * a->exp, a->exp - 1);
        }
    }
    return result;
}
