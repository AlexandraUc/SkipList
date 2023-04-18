#include <iostream>
#include <fstream>
#include <limits>
#include <math.h>
#include <sstream>
#include <time.h>

const int VAL_MIN = std::numeric_limits<int>::min();
std::ifstream f("abce.in");
std::ofstream g("abce.out");

/*
    Un SkipList va avea un pointer la primul nod din stanga sus ("capul")
    Fiecare nivel va incepe cu un nod "cap" care va tine cea mai mica valore de int
    Capetele vor fi conectate intre ele (jos)
*/

// Intr-un SkipList mergem doar in dreapta si in jos
struct Nod{
    int val;
    Nod* dreapta;
    Nod* jos;

    Nod(int val = VAL_MIN){                  
        this->val = val;
        this->dreapta = NULL;
        this->jos = NULL;
    }
};

class SkipList{
    int nivel;                   // Numarul de nivele        
    int lungime;                 // Numarul de elemente excluzand capetele           
    Nod* cap;                                                      

public:
    SkipList();

    void afisare();
    Nod* cautare(const int&);
    void inserare(const int&);
    void nivelNou(Nod*);
    void sterge(const int&);
    int predecesor(const int&);
    const Nod* succesor(const int&);
    void afisInterval(const int&, const int&, std::ostringstream&);

    ~SkipList();
};

SkipList::SkipList(){                        
    this->nivel = 1;
    this->lungime = 0;                   
    this->cap = new Nod();
}

void SkipList::afisare(){
    Nod* capCurent = this->cap;
    Nod* nodCurent;

    int nivelCurent = nivel;
    while(capCurent != NULL){
        std::cout << "Nivel " << nivelCurent << ": ";
        nodCurent = capCurent->dreapta;

        while(nodCurent != NULL){
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

Nod* SkipList::cautare(const int& val){
    Nod* nodCurent = this->cap;
    
    while(val > nodCurent->val){
         // Mergem ori in dreapta
         if(nodCurent->dreapta != NULL && val >= nodCurent->dreapta->val){
            nodCurent = nodCurent->dreapta;
            // Cat de mult putem
            while(nodCurent->dreapta != NULL && val >= nodCurent->dreapta->val)
                nodCurent = nodCurent->dreapta;
        } 
        else
            // Ori in jos
            if(nodCurent->jos != NULL){
                nodCurent = nodCurent->jos;
            }
            else
                // Ori iesim (pe ultimul nivel si valorea din dreapta e mai mare sau am ajuns la capat)
                break;
    }
    /*
        Gasit => pointer la nod
        Negasit => pointer la nodul care ar fi fost in fata lui (pt inserare si predecesor/ succesor)
    */
    return nodCurent;
}

void SkipList::inserare(const int& val){
    Nod* nodInainte = this->cautare(val);

    ++this->lungime;
    int nivelCurent = 1;

    Nod* nodNou = new Nod(val);
    nodNou->jos = NULL;                // Suntem pe primul nivel
    nodNou->dreapta = nodInainte->dreapta;    
    nodInainte->dreapta = nodNou;

    bool urcare = rand() & 1;
    while(urcare && nivelCurent + 1 <= log(this->lungime)){
        ++nivelCurent;
        // Cream nodul de pe nivelul superior, legand-ul de nodul de sub el
        Nod* nodNivelSuperior = new Nod(val);
        nodNivelSuperior->jos = nodNou;
        nodNou = nodNivelSuperior;    

        // Daca avem un nivel nou
        if(nivel < nivelCurent){
            this->nivelNou(nodNivelSuperior);
        }
        // Altfel adaugam nodul la nivelul deja existent
        else{
            // Trebuie sa cautam pozitia unde va fi inserat nodul
            nodInainte = this->cap;

            // Pornim de pe nivelul maxim si mergem pana la nivelul unde trebuie sa inseram exclusiv
            int nivelDeParcurs = nivel;
            while(nivelDeParcurs > nivelCurent){
                while(nodInainte->dreapta != NULL && val > nodInainte->dreapta->val)
                    nodInainte = nodInainte->dreapta;
                nodInainte = nodInainte->jos;
                --nivelDeParcurs;
            }

            // Parcurgem nivelul unde trebuie sa inseram pentru a gasi pozitia
            while(nodInainte->dreapta != NULL && val > nodInainte->dreapta->val)
                nodInainte = nodInainte->dreapta;
            // Avem nodul dupa care trebuie sa punem noul nod, potrivim legaturile
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

        // Cautarea dar tinem minte vecinul din stanga
        while(val > nodCurent->val){
            if(nodCurent->dreapta != NULL && val >= nodCurent->dreapta->val){
                vecinStanga = nodCurent;
                nodCurent = nodCurent->dreapta;
                while(nodCurent->dreapta != NULL && val >= nodCurent->dreapta->val){
                    vecinStanga = nodCurent;
                    nodCurent = nodCurent->dreapta;
                }
            }
            else
                if(nodCurent->jos != NULL){
                    nodCurent = nodCurent->jos;
                    vecinStanga = vecinStanga->jos;
                    // In cazul in care nu au inaintat deloc in dreapta
                    if(vecinStanga != nodCurent)
                        // Recuperam vecinul dupa ce coboram
                        while(vecinStanga->dreapta != nodCurent)
                            vecinStanga = vecinStanga->dreapta;
                }
                else
                    break;
        }
        
        if(val == nodCurent->val){
            --this->lungime;
            while(nodCurent != NULL){
                bool coboara = 0;

                // Recuperam vecinul dupa ce coboram
                while(vecinStanga->dreapta != nodCurent)
                    vecinStanga = vecinStanga->dreapta;
                
                // Stergem nodul si coboram
                Nod* aux = nodCurent->jos;
                vecinStanga->dreapta = nodCurent->dreapta;
                delete nodCurent;
                nodCurent = aux;

                // Daca nu suntem pe primul nivel si ramane gol il stergem
                if(vecinStanga->val == VAL_MIN && vecinStanga->dreapta == NULL && vecinStanga->jos != NULL){
                    --this->nivel;
                    // Mereu nivelul gol va fi cel mai sus, deci actualizam capul
                    this->cap = vecinStanga->jos;
                    aux = vecinStanga->jos;
                    delete vecinStanga;
                    vecinStanga = aux;
                    coboara = 1;
                }
                // Coboram si vecinul daca nu am facut-o deja
                if(!coboara)
                    vecinStanga = vecinStanga->jos;
            }
        }
    }
}

int SkipList::predecesor(const int& val){
    // Ori numarul insusi ori cea mai mare valore mai mica decat el
    return this->cautare(val)->val;
}

const Nod* SkipList::succesor(const int& val){
    Nod* nodGasit = this->cautare(val);
    if(nodGasit->val == val)
        // Returnam nodul cu numarul insusi
        return nodGasit;

    // Altfel coboram pana la nivelul 1 si returnam vecinul din dreapta
    while(nodGasit->jos != NULL)
        nodGasit = nodGasit->jos;
    return nodGasit->dreapta;
}

void SkipList::afisInterval(const int& mini, const int& maxi, std::ostringstream& buff){
    const Nod* capatStanga = this->succesor(mini);
    while(capatStanga->jos != NULL)
        capatStanga = capatStanga->jos;

    if(capatStanga->val <= maxi)
        buff << capatStanga->val << ' ';

    while(capatStanga->dreapta != NULL && capatStanga->dreapta->val <= maxi){
        capatStanga = capatStanga->dreapta;
        buff << capatStanga->val << ' ';
    }
}

SkipList::~SkipList(){
    while(this->cap != NULL){
        Nod* nodCurent = this->cap->dreapta;
        Nod* aux = nodCurent;

        while(nodCurent != NULL){
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
    srand(time(NULL));

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
                buff << skip.predecesor(x) << "\n";
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