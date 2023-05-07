#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <sstream>
#include <time.h>

const int VAL_MIN = std::numeric_limits<int>::min();
std::ifstream f("abce.in");
std::ofstream g("abce.out");

struct Nod{
    int val;
    Nod* dreapta;
    Nod* jos;

    Nod(int val = VAL_MIN): val(val), dreapta(nullptr), jos(nullptr) {}
};

class SkipList{
    int nivel;                               
    int lungime;                            
    Nod* cap;                                                      

public:
    SkipList(): nivel(1), lungime(0), cap(new Nod()) {}

    void afisare();
    void inserare(const int&);
    void nivelNou(Nod*);
    void sterge(const int&);
    void afisInterval(const int&, const int&, std::ostringstream&);
    const Nod* cautare(const int&);
    const Nod* succesor(const int&);

    ~SkipList();
};

void SkipList::afisare(){
    Nod* capCurent = this->cap;
    Nod* nodCurent;

    int nivelCurent = nivel;
    while(capCurent != nullptr){
        std::cout << "Nivel " << nivelCurent << ": ";
        nodCurent = capCurent->dreapta;

        while(nodCurent != nullptr){
            std::cout << nodCurent->val << " ";
            nodCurent = nodCurent->dreapta;
        }
        std::cout << "\n";

        capCurent = capCurent->jos;
        --nivelCurent;
    }
}

void SkipList::nivelNou(Nod* nodLegatura){
    ++this->nivel;
    // Cream cap nou in stanga sus care va fi legat de capul anterior
    Nod* capNou = new Nod();
    capNou->jos = this->cap;
    this->cap = capNou;
    // Legam noul cap de singurul nod de pe nivel
    this->cap->dreapta = nodLegatura;
}

const Nod* SkipList::cautare(const int& val){
    Nod* nodCurent = this->cap;
    
    while(val > nodCurent->val){
        // Mergem in dreapta cat se poate
        while(nodCurent->dreapta != nullptr && val >= nodCurent->dreapta->val)
            nodCurent = nodCurent->dreapta;
        
        // Apoi in jos
        if(nodCurent->jos != nullptr){
            nodCurent = nodCurent->jos;
        }
        // Sau iesim (ori am gasit val ori nu mai avem unde sa mergem)
        else
            break;
    }
    return nodCurent;
}

void SkipList::inserare(const int& val){
    Nod* nodInainte = const_cast<Nod*>(this->cautare(val));

    ++this->lungime;
    int nivelCurent = 1;

    Nod* nodNou = new Nod(val);
    nodNou->jos = nullptr;                       
    nodNou->dreapta = nodInainte->dreapta;    
    nodInainte->dreapta = nodNou;

    bool urcare = rand() & 1;
    while(urcare && nivelCurent + 1 <= log(this->lungime)){
        ++nivelCurent;

        // Tre sa facem nivel nou
        Nod* nodNivelSuperior = new Nod(val);
        nodNivelSuperior->jos = nodNou;
        nodNou = nodNivelSuperior;    

        if(nivel < nivelCurent){
            this->nivelNou(nodNivelSuperior);
        }
       
        else{
            // Altfel tre sa cautam pozitia unde va fi inserat
            nodInainte = this->cap;

            int nivelDeParcurs = nivel;
            while(nivelDeParcurs > nivelCurent){
                while(nodInainte->dreapta != nullptr && val > nodInainte->dreapta->val)
                    nodInainte = nodInainte->dreapta;
                nodInainte = nodInainte->jos;
                --nivelDeParcurs;
            }

            while(nodInainte->dreapta != nullptr && val > nodInainte->dreapta->val)
                nodInainte = nodInainte->dreapta;
        
            nodNivelSuperior->dreapta = nodInainte->dreapta;
            nodInainte->dreapta = nodNivelSuperior;

            urcare = rand() & 1;
        }
    }
}

void SkipList::sterge(const int& val){
    if(this->lungime > 0){
        Nod* nodCurent = this->cap;
        Nod* vecinStanga = this->cap;

        // Cautarea da pastram vecinul din stanga
        while(val > nodCurent->val){
            if(nodCurent->dreapta != nullptr && val >= nodCurent->dreapta->val){
                vecinStanga = nodCurent;
                nodCurent = nodCurent->dreapta;
                while(nodCurent->dreapta != nullptr && val >= nodCurent->dreapta->val){
                    vecinStanga = nodCurent;
                    nodCurent = nodCurent->dreapta;
                }
            }
            else
                if(nodCurent->jos != nullptr){
                    nodCurent = nodCurent->jos;
                    vecinStanga = vecinStanga->jos;
                    // Doar daca au inaintat in dreapta
                    if(vecinStanga != nodCurent)
                        // Recuperam vecinul din stanga dupa ce coboram
                        while(vecinStanga->dreapta != nodCurent)
                            vecinStanga = vecinStanga->dreapta;
                }
                else
                    break;
        }
        
        if(val == nodCurent->val){
            --this->lungime;
            while(nodCurent != nullptr){
                bool coboara = 0;

                while(vecinStanga->dreapta != nodCurent)
                    vecinStanga = vecinStanga->dreapta;
                
                // Stergem nodul si coboram
                Nod* aux = nodCurent->jos;
                vecinStanga->dreapta = nodCurent->dreapta;
                delete nodCurent;
                nodCurent = aux;

                // Daca nu suntem pe primul nivel si a ramas gol il stergem
                if(vecinStanga->val == VAL_MIN && vecinStanga->dreapta == nullptr && vecinStanga->jos != nullptr){
                    --this->nivel;
                    this->cap = vecinStanga->jos;
                    aux = vecinStanga->jos;
                    delete vecinStanga;
                    vecinStanga = aux;
                    coboara = 1;
                }
                // Coboram vecinul daca nu am facut-o deja
                if(!coboara)
                    vecinStanga = vecinStanga->jos;
            }
        }
    }
}

const Nod* SkipList::succesor(const int& val){
    const Nod* nodGasit = this->cautare(val);
    // Am gasit val
    if(nodGasit->val == val)
        return nodGasit;

    // ALtfel coboram pana jos si afisam vecinul din dreapta
    while(nodGasit->jos != nullptr)
        nodGasit = nodGasit->jos;
    return nodGasit->dreapta;
}

void SkipList::afisInterval(const int& mini, const int& maxi, std::ostringstream& buff){
    const Nod* capatStanga = this->succesor(mini);
    while(capatStanga->jos != nullptr)
        capatStanga = capatStanga->jos;

    if(capatStanga->val <= maxi)
        buff << capatStanga->val << ' ';

    while(capatStanga->dreapta != nullptr && capatStanga->dreapta->val <= maxi){
        capatStanga = capatStanga->dreapta;
        buff << capatStanga->val << ' ';
    }
}

SkipList::~SkipList(){
    while(this->cap != nullptr){
        Nod* nodCurent = this->cap->dreapta;
        Nod* aux = nodCurent;

        while(nodCurent != nullptr){
            nodCurent = nodCurent->dreapta;
            delete aux;
            aux = nodCurent;
        }

        aux = this->cap->jos;
        delete this->cap;
        this->cap = aux;
    }
}

int main(){
    srand(time(nullptr));

    SkipList skip;
    int q, operatie, x;
    std::ostringstream buff;
 
    f >> q;
    while(q){
        f >> operatie;
        switch(operatie){
            case 1:{
                f >> x;
                skip.inserare(x);
                break;
            }

            case 2:{
                f >> x;
                skip.sterge(x);
                break;
            }

            case 3:{
                f >> x;
                buff << (skip.cautare(x)->val == x) << "\n";
                break;
            }

            case 4:{
                f >> x;
                buff << skip.cautare(x)->val << "\n";
                break;
            }

            case 5:{
                f >> x;
                buff << skip.succesor(x)->val << "\n";
                break;
            }

            case 6:{
                int y;
                f >> x >> y; 
                skip.afisInterval(x, y, buff);
                buff << "\n";
                break;
            }
        }
        --q;
    }

    g << buff.str();

    f.close();
    g.close();
    return 0;
}
