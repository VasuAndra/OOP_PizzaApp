#include <iostream>
#include <vector>
#include <fstream>
using namespace std;
ifstream f("/Users/camiub/Desktop/Andra_drive/C++_programe/An1_Sem2/POO/POO_pr2_var4/fis.txt");
float val_manopera=5;
class c_abstract
{
protected:
    char denumire[100];
    float pret_unit;
public:
    virtual void calcul_pret()=0;
    virtual void afisare(ostream& os)
    {
        os<<denumire;
    }
};
class c_ingredient:public c_abstract
{
    int cantitate_gr;
    float valoare;
public:
    c_ingredient (char* p_den,int p_cant,float p_pret) // constructor
    {
        init(p_den,p_cant,p_pret);
    }
    c_ingredient()
    {
        char nimic='\0';
        init(&nimic,0,0);
    }
    ~ c_ingredient ()  //destructor
    {
        int i;
        for(i=0;i<strlen(denumire);i++)
            denumire[i]='\0';
        cantitate_gr=0;
        valoare=0;
        pret_unit=0;
        
    }
    
    void init(char* p_den,int p_cant,float p_pret);
    float get_val();
    void calcul_pret();
    void copiereIngred(const c_ingredient &y);
    c_ingredient operator= (c_ingredient y);
    friend ostream& operator<<(ostream& os, c_ingredient& i);
    friend istream& operator>>(istream& is, c_ingredient& i);
    
};
void c_ingredient:: init(char* p_den,int p_cant,float p_pret)
{
    strcpy(denumire,p_den);
    if(p_cant<0)
        throw invalid_argument( "Nu se poate cantitate <0" );
    cantitate_gr=p_cant;
    pret_unit=p_pret;
    calcul_pret();
}
float c_ingredient:: get_val()
{
    return valoare;
}
void c_ingredient:: calcul_pret()
{
    valoare=cantitate_gr*pret_unit;
}
void c_ingredient :: copiereIngred(const c_ingredient &y)
{
    strcpy(denumire,y.denumire);
    cantitate_gr=y.cantitate_gr;
    pret_unit=y.pret_unit;
    valoare=y.valoare;
}
c_ingredient c_ingredient :: operator= (c_ingredient y)
{
    (*this).copiereIngred(y);
    return *this;
}
ostream& operator<<(ostream& os,c_ingredient& i)
{
    static_cast<c_abstract&> (i).afisare(os);
    if(i.cantitate_gr>0)
      os<<" ("<<i.cantitate_gr<<" gr)";
    os<<endl;
    
    return os;
}
istream& operator>>(istream& is, c_ingredient& i)
{
    is>>i.denumire>>i.cantitate_gr>>i.pret_unit;
    i.calcul_pret();
    return is;
}
class c_pizza : public c_ingredient
{
    int nr_ingred;
    int eVeget; //0-NU 1-DA
    int cant_vanduta;
    vector<c_ingredient*> v_ingrediente;
public:
    c_pizza (char* p_den,int p_nr_ing,int p_veget) // constructor
    {
        init(p_den,p_nr_ing,p_veget);
    }
    c_pizza ()
    {
        char nimic='\0';
        init(&nimic,0,0);
    }
    ~ c_pizza ()  //destructor
    {
        int i;
        for(i=0;i<strlen(denumire);i++)
            denumire[i]='\0';
        nr_ingred=0;
        pret_unit=0;
        eVeget=0;
    }
    int get_eVeget()
    {
        return eVeget;
    }
    void init(char* p_den,int p_nr_ing,int p_veget);
    float get_pret();
    int get_cant_vanduta()
    {
        return cant_vanduta;
    }
    void increment_pizza_vand()
    {
        cant_vanduta++;
    }
    void calcul_pret();
    void copierePizza(const c_pizza &y);
    c_pizza operator= (c_pizza y);
    friend ostream& operator<<(ostream& os, c_pizza& p);
    friend istream& operator>>(istream& is, c_pizza& p);
    
};
void c_pizza:: init(char* p_den,int p_nr_ing,int p_veget)
{
    strcpy(denumire,p_den);
    nr_ingred=p_nr_ing;
    eVeget=p_veget;
    cant_vanduta=0;
    pret_unit=0;
    if(nr_ingred<0)
        throw invalid_argument( "Nr ingrediente nu poate fi <0" );
}
float c_pizza:: get_pret()
{
    calcul_pret();
    return pret_unit;
}
void c_pizza:: calcul_pret()
{
    int i;
    pret_unit=0;
    for(i=0;i<nr_ingred;i++)
        pret_unit=pret_unit+ v_ingrediente[i]->get_val();
    pret_unit=pret_unit+val_manopera;
}
void c_pizza :: copierePizza(const c_pizza &y)
{
    char *c=new char[100];
    strcpy(c,y.denumire);
    v_ingrediente.clear();
    init(c,y.nr_ingred,y.eVeget);
    int i;
    for(i=0;i<y.nr_ingred;i++)
        v_ingrediente.push_back(y.v_ingrediente[i]);
    
    
}
c_pizza c_pizza :: operator= (c_pizza y)
{
    (*this).copierePizza(y);
    return *this;
}
ostream& operator<<(ostream& os, c_pizza& p)
{
    int i;
    os<<static_cast<c_ingredient&> (p);
    os<<" pret:"<<p.pret_unit<<" lei"<<endl<<"(";
    for(i=0;i<p.nr_ingred;i++)
    {
        p.v_ingrediente[i]->calcul_pret();
        os<<*(p.v_ingrediente[i])<<"   ";
    }
    os<<")"<<endl;
    
    return os;
}
istream& operator>>(istream& is, c_pizza& p)
{
    char *c=new char[100];
    int nr_ingr,veg,i;
    c_ingredient* ingr;
    //nume nr_ingr 0/1 veg
    is>>c>>nr_ingr>>veg;
    p.init(c,nr_ingr,veg);
    for(i=0;i<p.nr_ingred;i++)
    {
        ingr=new c_ingredient();
        is>>*ingr;
        p.v_ingrediente.push_back(ingr);
    }
    p.calcul_pret();
    return is;
}
template <class T> class c_meniu
{
    T nr_pizza;
    vector<c_pizza*> v_pizza;
    
public:
    c_meniu (int p_nr_pizza,char* p_den,int p_nr_ing,int p_veget) // constructor
    {
        c_pizza(p_den,p_nr_ing,p_veget);
        nr_pizza=p_nr_pizza;
    }
    c_meniu ()
    {
        c_pizza();
        nr_pizza=0;
    }
    
    T get_nr_pizza()
    {
        return nr_pizza;
    }
    
    void operator += (c_pizza* p)
    {
        v_pizza.push_back(p);
        nr_pizza++;
    }
    void afisare_meniu();
    c_pizza* get_pizza(int p_nr_pizza)
    {
        if(p_nr_pizza>nr_pizza)
            throw invalid_argument( "Nr dat este mai mare decat numarul total de pizza" );
            return v_pizza[p_nr_pizza];
    }
    int val_pizza_veg_vandute();
};
template<class T> int c_meniu<T>::val_pizza_veg_vandute()
{
    int i,val_totala=0;
    for(i=0;i<nr_pizza;i++)
        if(v_pizza[i].get_eVeget()==1)
            val_totala=val_totala (v_pizza[i].get_pret()*v_pizza[i].get_cant_vanduta());
            return val_totala;
}
template<class T> void c_meniu<T>::afisare_meniu()
{
    int nr=get_nr_pizza(),i;
    if(nr==0)
        cout<<"Meniul nu contine pizza";
    else
       for (i=0;i<nr;i++)
       {
           v_pizza[i]->calcul_pret();
            cout <<i+1<<". "<< *(v_pizza[i]);
       }
}
class c_comanda
{
    c_meniu<int> *meniu;
    vector <c_pizza*> pozitii_comand;
    int nr_poz_comand,nr_km;
    float val_tot;
public:
    c_comanda (c_meniu<int> *p_meniu) // constructor
    {
        meniu=p_meniu;
        nr_poz_comand=0;
        val_tot=0;
        nr_km=0;
    }
    c_comanda ()
    {
        meniu=NULL;
        nr_poz_comand=0;
        val_tot=0;
        nr_km=0;
        
    }
    void adauga_pozitie(int p_poz);
    void set_km(int p_nr_km)
    {
        nr_km=p_nr_km;
        val_comanda_veget();
        val_comanda();
    }
    int get_nr_km()
    {
        return nr_km;
    }
    float val_comanda()
    {
        int i;
        int f_nr_km;
        
        val_tot=0;
        for(i=0;i<nr_poz_comand;i++)
            val_tot=val_tot+(pozitii_comand[i]->get_pret() * pozitii_comand[i]->get_cant_vanduta());
        if(nr_km>0)
        {
            f_nr_km=nr_km/10;
            val_tot=val_tot+(0.05*val_tot*(f_nr_km));
        }
        return val_tot;
    }
    float val_comanda_veget()
    {
        int i,f_nr_km;
        int val_tot_veget=0;
        for(i=0;i<nr_poz_comand;i++)
            if(pozitii_comand[i]->get_eVeget()==1)
                val_tot_veget=val_tot_veget+( pozitii_comand[i]->get_pret() * pozitii_comand[i]->get_cant_vanduta() );
        if(nr_km>0)
        {
            f_nr_km=nr_km/10;
         val_tot_veget=val_tot_veget+ (0.05*val_tot_veget*(f_nr_km));
        }
            return val_tot_veget;
    }
    friend ostream& operator<<(ostream& os, const c_comanda& p);
};
ostream& operator<<(ostream& os, const c_comanda& p)
{
    int i;
    for(i=0;i<p.nr_poz_comand;i++)
    os<<i+1<<"."<<*(p.pozitii_comand[i])<<endl;
    return os;
}
void c_comanda::adauga_pozitie(int p_poz)
{
    c_pizza* p;
    if(p_poz>meniu->get_nr_pizza())
        throw invalid_argument( "Pozitia nu exista");
    if(p_poz<0)
        throw invalid_argument( "Pozitia este <0");
    p=meniu->get_pizza(p_poz);
    pozitii_comand.push_back(p);
    p->increment_pizza_vand();
    
    nr_poz_comand++;
}
int main()
{
    c_meniu<int> *meniu;
    c_comanda *comanda;
    int nr_pizza,opt,nr,nr_km;
    f >> nr_pizza;
    meniu=new c_meniu<int>();
    comanda=new c_comanda(meniu);
    for (int i = 0; i < nr_pizza; i++)
        {
                c_pizza *p = new c_pizza();
                f>> *p;
                *meniu += p;
        }
    
    do
    {
        cout << "\nIntroduceti optiunea:\n";
        cout << "1-Afisare meniu\n";
        cout << "2-Adauga comanda\n";
        cout << "3-Dati nr de km \n";
        cout << "4-Tipareste comanda\n";
        cout << "5-Calcul valoare comanda\n";
        cout << "6-Calcul valoare comanda vegetariana \n";
        cout<<  "7-Comanda noua \n";
        cout<<  "0-Iesire\n";
        cin >> opt;
        
        switch (opt)
        {
            case 1:
            {
                try{
                    meniu->afisare_meniu();
                }
                catch (...)
                {
                    cout << "Tiparirea nu a reusit";
                }
                break;
                
            }
            case 2:
            {
                try {
                cout<<"Dati nr pizza pe care doriti sa o comandati";
                cin>>nr;
                comanda->adauga_pozitie(nr-1);
                }
                catch(...)
                {
                    cout << "Pozitie nu exista in meniu";
                }
                break;
            }
            case 3:
            {
                cout<<"Dati nr de km ";
                cin>>nr_km;
                comanda->set_km(nr_km);
                break;
            }
            case 4:
            {
                cout<<*comanda;
                break;
            }
            case 5:
            {
                cout<<comanda->val_comanda()<<"lei"<<"("<<comanda->get_nr_km()<<" km)";
                break;
            }
            case 6:
            {
                cout<<comanda->val_comanda_veget()<<"lei"<<"("<<comanda->get_nr_km()<<" km)";
                break;
            }
            case 7:
            {
                delete comanda;
                comanda=new c_comanda(meniu);
            }
                
        }
    }
    while (opt >= 1 && opt <= 7);
    return 0;
}

/*
 
 
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 1
 1. funghi
 pret:760 lei
 (ciuperci (10 gr)
 ketchup (5 gr)
 porumb (20 gr)
 )
 2. carnivora
 pret:305 lei
 (salam (5 gr)
 sunca (4 gr)
 )
 3. formaggi
 pret:624 lei
 (cascaval (4 gr)
 mozzarela (6 gr)
 brie (7 gr)
 )
 4. pollo
 pret:945 lei
 (porumb (5 gr)
 ketchup (6 gr)
 pui (10 gr)
 cascaval (7 gr)
 )
 5. margherita
 pret:155 lei
 (cascaval (5 gr)
 ketchup (5 gr)
 )
 
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 2
 Dati nr pizza pe care doriti sa o comandati1
 
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 2
 Dati nr pizza pe care doriti sa o comandati2
 
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 3
 Dati nr de km 75
 
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 4
 1.funghi
 pret:760 lei
 (ciuperci (10 gr)
 ketchup (5 gr)
 porumb (20 gr)
 )
 
 2.carnivora
 pret:305 lei
 (salam (5 gr)
 sunca (4 gr)
 )
 
 
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 5
 1437.75lei(75 km)
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 6
 1026lei(75 km)
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 7
 
 Introduceti optiunea:
 1-Afisare meniu
 2-Adauga comanda
 3-Dati nr de km
 4-Tipareste comanda
 5-Calcul valoare comanda
 6-Calcul valoare comanda vegetariana
 7-Comanda noua
 0-Iesire
 0
 Program ended with exit code: 0 */
