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


struct darab{
    string nev;
    deque<string> lkod;
    //ha vmi kell meg ide irjatok batran;
};


struct szoveg { //szerintem a structban inkább egy-egy aritmetikai utasításnak kéne csak lenine, nem az egész kódnak
    string fnev;
    vector<darab> komplett;
    string gkod;
};

struct Threeadress{
    string baloldal;
    vector<string> jobboldal;
    string operand;
    Threeadress(){
        baloldal.clear();
        operand.clear();
    }
};

struct Node{
    string gyoker;
    Node* left;
    Node* right;
    Node(string ujgyok){
        gyoker = ujgyok;
        left = NULL;
        right = NULL;
    }
    Node(string ujgyok, Node* bal, Node* jobb){
        gyoker = ujgyok;
        left = bal;
        right = jobb;
    }
};

struct Tree{
    string nev;
    void ujlevel(string gyoker, Node *level){

    }
    Node *root;
};

ostream& operator << (ostream& kimenet, Threeadress& kiirando){
    kimenet << kiirando.baloldal << "=";
    for(int i = 0; i < kiirando.jobboldal.size(); i++){
        kimenet << kiirando.jobboldal[i];
        if(i == 0) kimenet << kiirando.operand;
    }
}

void syntaxerror(szoveg &pelda){
    ifstream f;
    int i = 0;
    f.open("proba.txt");
}

void lengyel (szoveg &pelda){
    ifstream f,g;
    f.open("proba.txt");
    g.open("proba.txt");
    string aktu;
    string eredet;
    char szam;
    darab elso;
    string vissza;
    stack<char> verem;
    while (f.good()){
        elso.lkod=deque<string>();
        getline(g,eredet,';');
        getline(f,aktu,'=');
        elso.nev=aktu;
        szam='.';
        while (szam!=';' ){
            f >> szam;
            if (szam=='*' || szam=='/' ){
                while (!verem.empty() && verem.top()!='(' && verem.top()!='+' && verem.top()!='-'){
                    vissza=verem.top();
                    elso.lkod.push_back(vissza);
                    verem.pop();
                }
                verem.push(szam);
            }
            else{
                if (szam=='+' || szam=='-' ){
                    while (!verem.empty() && verem.top()!='(' ){
                        vissza=verem.top();
                        elso.lkod.push_back(vissza);
                        verem.pop();
                    }
                    verem.push(szam);
                }
                else{
                    if (szam==')'){
                        while (verem.top()!='('){
                            vissza=verem.top();
                            elso.lkod.push_back(vissza);
                            verem.pop();
                        }
                        verem.pop();
                    }
                    else{
                        if (szam=='('){
                            verem.push('(');
                        }
                        else{
                            if (szam==';'){}
                            else{
                                vissza=szam;
                                elso.lkod.push_back(vissza);
                            }
                        }
                    }
                }
            }

  }
        while (!verem.empty()){
            vissza=verem.top();
            elso.lkod.push_back(vissza);
            verem.pop();
        }
        pelda.komplett.push_back(elso);
        f >> ws;
    }
    deque<string> test=pelda.komplett[0].lkod;
    cout << pelda.komplett[0].nev << "=";
    while(!test.empty()){
        cout << test.front();
        test.pop_front();
    }
}

bool isoperator(string s){
    return(s=="*" || s=="/" || s=="+" || s=="-");
}

stack<Node*> expression_tree (deque<string> postfix_code){
    stack<Node*> uj;
    while(!postfix_code.empty()){
        string current = postfix_code.front();
        if(isoperator(current)){
            Node* bal = uj.top();
            uj.pop();
            Node* jobb = uj.top();
            uj.pop();
            uj.push(new Node(current, bal, jobb));
        }
        else{
            uj.push(new Node(current));
        }
        postfix_code.pop_front();
    }
    return uj;
}

vector<Threeadress> felbont(szoveg pelda){
    vector<Threeadress> visszateres;
    for(darab d : pelda.komplett){
        Threeadress uj;
        if(uj.baloldal.length()==0 && d.nev.length()!=0){
            uj.baloldal = d.nev;
        }
        while(!d.lkod.empty()){
            string current = d.lkod.front();
            if(isalnum(current[0])){
                if(uj.jobboldal.size()<2){
                    uj.jobboldal.push_back(current);
                }
                else{
                    Threeadress* masodik = new Threeadress();
                    masodik->jobboldal.push_back(current);
                }
            }
            else{
                if(!(uj.operand.length()>0)){
                    uj.operand = current;
                }
                else{
                    /*if(uj.operand.compare("*")== 0){    //FMA3
                        if(current=="+"){

                        }
                    }*/
                    //visszateres.push_back(uj);
                    cout << uj << endl;
                    Threeadress* masodik = new Threeadress;
                    masodik->baloldal = uj.jobboldal[0]+uj.operand+uj.jobboldal[1];
                    masodik->operand = current;
                    uj = *masodik;
                    delete masodik;
                    /*uj.baloldal = uj.jobboldal[0]+uj.operand + uj.jobboldal[1];
                    uj.operand = current;
                    uj.jobboldal.clear();*/
                    cout << uj.baloldal;
                }
            }
            d.lkod.pop_front();
        }
        cout << uj;
    }
    return visszateres;
}

void regiszteres(szoveg& pelda, int regnum){
    vector<pair<bool, string> >registers(regnum, make_pair(0, ""));
    vector<pair<string, pair<char, int> > > valtozok;
    deque<string> current = pelda.komplett[0].lkod;
    queue<string> operands;
    string temp;
    while(!current.empty()){
        if(isalnum(current.front()[0])){
            operands.push(current.front());
            for(int i = 0; i < registers.size(); i++){
                if(registers[i].first == 0){
                    registers[i].first = 1;
                    registers[i].second = current.front();
                    temp += "reg["+to_string(i)+"] = "+current.front()+";\n";
                    current.pop_front();
                    break;
                }
            }
        }
        else{
            string op_1 = operands.front();
            operands.pop();
            string op_2 = operands.front();
            operands.pop();
            for(int i = 0; i < registers.size(); i++){
                if(registers[i].first == 0){
                    string ut = "reg["+to_string(i)+ "]= reg["+ to_string(i-2)+"] " +
                        current.front() + "reg["+to_string(i-1)+"];";
                    registers[i-1].first = 0;
                    registers[i-1].second = "";
                    if(i > 1){
                        registers[i-2].first = 0;
                        registers[i-2].second = "";
                    }
                    registers[i].first = 1;
                    registers[i].second = ut;
                    temp += ut+'\n';
                    current.pop_front();
                    operands.push(ut);
                    break;
                }
            }
        }
    }
    for(int i = 0; i < registers.size(); i++){
        if(registers[i].first == 0){
            temp += "\n"+pelda.komplett[0].nev + " = reg["+ to_string(i-1)+"];\n";
            break;
        }
    }
    cout << temp;
}

int main()
{
    szoveg pelda;
    lengyel(pelda);
    stack<Node*> valami = expression_tree(pelda.komplett[0].lkod);
    /*vector<Threeadress> probalkozas = felbont(pelda);
    for(Threeadress t : probalkozas){
        cout << t << endl;
    }*/
    return 0;
}
