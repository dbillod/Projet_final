#ifndef FONC_H_INCLUDED
#define FONC_H_INCLUDED

#include<vector>
#include<string>


///////////////////////////
// Fonctions statistiques//
///////////////////////////

double min_tab (std::vector <double> tab);


double max_tab(std::vector <double> tab);


double distance2( std::vector <double> tab1,std::vector <double> tab2);


double distance1(std::vector <double> tab1,std::vector <double> tab2);


double distanceinf(std::vector <double> tab1,std::vector <double> tab2);


double choix_dist(std::vector <double> tab1, std::vector <double> tab2, int p);


int argmin (std::vector <double> tab);

/////////////////////////////////////////////
//Fonctions propres au projet en lui-même//
/////////////////////////////////////////////

std::vector <double> extract(std::vector <double> tab,int indice_debut,int h);


std::vector <std::vector<double> > matrice_sous_seq(std::vector <double> tab,int h);


std::vector <double> argminmultiple(std::vector <double> tab, int k);


double moyenne(std::vector <double> tab);


double ecart_type (std::vector <double> tab);


std::vector <double> renorm(std::vector <double> tab);


void afficher (std::vector <double> tab);


std::vector < std::vector <double> > tabdata2(int nbseries, int tailletab, std::vector <double> TAB );


std::vector <std::vector <double> > coupertab(int nbseries, int t, std::vector <std::vector<double> > tabdata);


std::vector <std::vector <double> > tabdist(int nbseries, int h,int t ,std::vector <std::vector <std::vector <double> > >  Mat_totale, std::vector<double> Seq_a_predire,int p );


std::vector <double> simplifietab(int nbseries, int h, int t, std::vector <std::vector <double> > Tabdist );


#endif // FONC_H_INCLUDED
