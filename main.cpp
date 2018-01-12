#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <queue>
#include <algorithm>
#include <deque>
#include <utility>
#include <cctype>
#include <new>

using namespace std;

struct Node {
    string gyoker;
    Node* left;
    Node* right;
    Node * parent;
    Node(string ujgyok) {
        gyoker = ujgyok;
        left = NULL;
        right = NULL;
    }
    Node(string ujgyok, Node* szulo) {
        gyoker = ujgyok;
        parent = szulo;
    }
};


struct darab {
    string nev;
    deque<string> lkod;
    //ha vmi kell meg ide irjatok batran;
};


struct linkedNode {
    Node *tartalom;
    linkedNode* kovi;
};

struct szoveg { //szerintem a structban inkább egy-egy aritmetikai utasításnak kéne csak lenine, nem az egész kódnak
    string fnev;
    vector<darab> komplett;
    vector<pair<string, Node*> > fak;
    string gkod;
};


struct Threeadress {
    string baloldal;
    vector<string> jobboldal;
    string operand;
    Threeadress() {
        baloldal.clear();
        operand.clear();
    }
};


bool syntaxerror(szoveg &pelda){
    ifstream f;
    int i = 0;
    f.open("proba.txt");
    string nev;
    string kod;
    char aktu='.';
    bool baj;
    baj=0;
    while (f.good())
        {
            while (aktu!='=')
            {
                f >> aktu;
                if (aktu=='=') break;
                if (!isupper(aktu))
                {
                    cerr << "Error a kifejezesben, rossz karakter:" << aktu << endl;

                    baj=1;
                }

            }
            while (aktu!=';')
            {
                    f >> aktu;
                    if (aktu==';') break;
                    if (isupper(aktu))
                    {
                        cerr << "Error az aritmetikai kodban, rossz karakter:" << aktu << endl;
                    baj=1;
                    }

            }
            f >> ws;
        }
return(baj);
}
//Prajczer Péter látta (15:36)



void inorder_bejar_kiir(Node* node) {
    if(node == NULL) {
        return;
    }
    inorder_bejar_kiir(node->left);
    cout << node->gyoker;
    inorder_bejar_kiir(node->right);

}

void stackberak(linkedNode **ref_to_top, Node *uj) {
    linkedNode* newNode;
    newNode->tartalom = uj;
    newNode->kovi = *ref_to_top;
}

bool isempty(linkedNode *top) {
    if(top==NULL) return 1;
    else return 0;
}

bool isoperator(string s) {
    return(s=="*" || s=="/" || s=="+" || s=="-");
}

Node* stackbolkivesz(linkedNode** ref_to_top){
    Node* eredmeny;
    linkedNode *top;
    top = *ref_to_top;
    eredmeny = top->tartalom;
    *ref_to_top = top->kovi;
    delete top;
    return eredmeny;
}

vector<Threeadress> inorder_threea_nonrec(Node* node) {
    vector<Threeadress> vissza;
    Node *current = node;
    linkedNode *sztekk = NULL;
    bool vege = 0;
    while(!vege){
        if(current != NULL){
            stackberak(&sztekk, current);
            current = current->left;
        }
        else{
            if(!isempty(sztekk)){
                current = stackbolkivesz(&sztekk);
            }
        }
    }

}

ostream& operator << (ostream& kimenet, Threeadress& kiirando) {
    kimenet << kiirando.baloldal << "=";
    for(int i = 0; i < kiirando.jobboldal.size(); i++) {
        kimenet << kiirando.jobboldal[i];
        if(i == 0) kimenet << kiirando.operand;
    }
}


void lengyel (szoveg &pelda) {
    fstream f,g,h;
    f.open("proba.txt");
    g.open("proba.txt");
    h.open("proba.txt");
    string aktu;
    char aktu2;
    char kuka;
    int korr;
    korr=0;
    string eredet;
    string szam;
    darab elso;
    string vissza;
    vissza="";
    stack<string> verem;
    while (f.good()) {
        elso.lkod=deque<string>();
        getline(g,eredet,';');
        cout << eredet << endl;
        getline(f,aktu,'=');
        elso.nev=aktu;
        cout << aktu<< endl;
        getline(h,aktu,'=');
        h >> kuka;
        vissza=kuka;
        szam=".";
        while (szam!=";" ) {
            while (korr >1&& f.good()) {
                f >> kuka;
                korr--;
            }
            f >> aktu2;
            szam=aktu2;
            if (szam=="*" || szam=="/" ) {
                while (!verem.empty() && verem.top()!="(" && verem.top()!="+" && verem.top()!="-") {
                    elso.lkod.push_back(verem.top());
                    verem.pop();
                }
                verem.push(szam);
            } else {
                if (szam=="+" || szam=="-" ) {
                    while (!verem.empty() && verem.top()!="(" ) {
                        elso.lkod.push_back(verem.top());
                        verem.pop();
                    }
                    verem.push(szam);
                } else {

                    if (szam==")") {
                        while (verem.top()!="(") {
                            elso.lkod.push_back(verem.top());
                            verem.pop();
                        }
                        h >> kuka;
                        verem.pop();

                    } else {
                        if (szam=="(") {
                            verem.push("(");
                            h >> kuka;
                        } else {
                            if (szam==";") {
                                ;
                            } else {
                                if (korr==1) {
                                    h >> kuka;
                                    korr--;
                                }
                                vissza=szam;
                                if (h.good()) {
                                    h >> aktu2;
                                    korr++;
                                }
                                while (isalnum(aktu2) && h.good()) {
                                    vissza+=aktu2;
                                    h >> aktu2;
                                    korr++;
                                }
                                elso.lkod.push_back(vissza);
                                vissza="";
                            }
                        }
                    }
                }
            }
        }
        while (!verem.empty()) {
            elso.lkod.push_back(verem.top());
            verem.pop();
        }
        cout << endl;
        pelda.komplett.push_back(elso);
        f >> ws;
    }
    /*deque<string> test=pelda.komplett[0].lkod;
    while(!test.empty()) {
        cout << test.front();
        cout << endl;
        test.pop_front();
        }*/
}



void make_tree(szoveg& pelda) {
    Node *t, *t1, *t2;
    for(darab d : pelda.komplett) {
        stack<Node*> sztekk;
        deque<string> current = d.lkod;
        while(!current.empty()) {
            if(!isoperator(current.front())) {
                t = new Node(current.front());
                sztekk.push(t);
                current.pop_front();
            } else {
                t = new Node(current.front());
                t1 = sztekk.top();
                sztekk.pop();
                t2 = sztekk.top();
                sztekk.pop();
                t->left = t1;
                t->right = t2;
                sztekk.push(t);
                current.pop_front();
            }
        }
        t = sztekk.top();
        sztekk.pop();
        pair<string, Node*> temp = make_pair(d.nev, t);
        pelda.fak.push_back(temp);
    }
}


int main() {
    szoveg pelda;
    lengyel(pelda);
    make_tree(pelda);
    inorder_bejar_kiir(pelda.fak[0].second);
    return 0;
}
