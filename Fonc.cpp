#include "Fonc.h"

#include <climits>
#include<iostream>
#include<strstream>
#include<vector>
#include<cmath>
#include<typeinfo>
#include<fstream>
#include<sstream>
#include<string>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

double min_tab (vector <double> tab)
{
	int n = tab.size();
	double m = tab[0];
	for (int i = 0; i<n; i++)
	{
		if (tab[i]<m)
		{
			m = tab[i];
		}
	}
	return m;
}

double max_tab(vector <double> tab)
{
	int n = tab.size();
	double m;
	for (int i =0; i<n;i++)
	{
	tab[i]=-tab[i];
	}
	m=min_tab(tab);
	return -m;
}

double distance2( vector <double> tab1,vector <double> tab2) {
    // C'est la distance euclidienne entre deux vecteurs
	int n1 = tab1.size();
	int n2 = tab2.size();
	double d =0;
	if (n1 != n2)
	{
		cout<< "Problème de taille - fonction distance"<<endl;
	}
	else
	{
		for(int i =0; i< n1; i++)
		{
			d= d + (tab1[i]-tab2[i])*(tab1[i]-tab2[i]);
		}
	}
	return sqrt(d);
}


double distance1( vector <double> tab1, vector <double> tab2){
    //C'est la norme 1 de la différence entre deux vecteurs
    int n1 = tab1.size();
    int n2 = tab2.size();
    double d= 0;
    if (n1!= n2)
    {
		cout<< "Problème de taille - fonction distance"<<endl;
	}
    else
	{
		for(int i =0; i< n1; i++)
		{
			d= d + abs((tab1[i]-tab2[i]));
		}
	}
	return d;


}

double distanceinf(vector <double> tab1, vector <double> tab2){
    //C'est la norme infinie de la différence entre deux vecteurs
    int n1 = tab1.size();
    int n2 = tab2.size();
    vector <double> T;
    double d= 0;
    if (n1!= n2)
    {
		cout<< "Problème de taille - fonction distance"<<endl;
	}
    else
	{
		for(int i =0; i< n1; i++)
		{
			T.push_back(abs((tab1[i]-tab2[i])));
		}
	}
	return max_tab(T);


}

double choix_dist(vector <double> tab1, vector <double> tab2, int p){

    //En fonction de p on va calculer distance , distance 1 ou distance inf
    if (p == 1){
        return distance1(tab1, tab2);
    }
    if (p==2){
        return distance2(tab1,tab2);
    }
    if(p==0){
        return distanceinf(tab1,tab2);
    }

}

int argmin (vector <double> tab){
	double m = min_tab(tab);
	int i =0;
	double a = 0;
	while (a ==0){
		if( tab[i]==m)
		{
			return i;
			a = 1;
		}
		else
		{
			i++;
		}
	}
}

vector <double> extract(vector <double> tab,int indice_debut,int h)  /* extraction d’une sous sequence de taille h et de premier terme tab indice_debut */
{
    vector <double> sub_tab;
	int n=tab.size();
	if(indice_debut<0|indice_debut>=n)
	{
    	cout << "erreur d'indice"<<endl;
	}

	if ( ( (h>n)  |  (h<=0) ) )
	{
    	cout << "taille h de la sous-sequence erronee"<<endl;

	}
	for(int i=indice_debut;i<h+indice_debut;i++)
	{
    	sub_tab.push_back(tab[i]);
	}
	return sub_tab;
}

vector <vector<double> > matrice_sous_seq(vector <double> tab,int h){ /*matrice contenant les n-h+1 sous-sequences de tab de taille h*/

	vector <vector<double> > mat;
	int n=tab.size();
	for (int i=0;i<n-h+1;i++)
	{
	mat.push_back(extract(tab,i,h));
	}
	return mat;
}



vector <double> argminmultiple(vector <double> tab, int k){
	vector <double> tab_argmin;
    for (int i=0;i<k;i++)
    {
    int j=argmin(tab);
    tab_argmin.push_back(j);
    tab[j]=ULLONG_MAX;         // le tableau tab ne sera pas modifie a la sortie de la fonction (passage d'argument par valeur)
    }
    return tab_argmin;
}



double moyenne(vector <double> tab){
	int n = tab.size();
	double m =0;
	for(int i =0 ;i<n; i++){
		m = m + (tab[i]/n);
	}
	return (m);
}

double ecart_type (vector <double> tab){
    int n = tab.size();
    double m = moyenne(tab);
    double stddev =0;
    for (int i =0; i<n;i++){
        stddev = stddev + (tab[i]-m)*(tab[i]-m)/n;

    }
    return (sqrt(stddev));

}

vector <double> renorm(vector <double> tab){
	int n = tab.size();
	double m = moyenne (tab);
	vector <double> tab2(n,1);
	double stddev= 0;
	for(int i =0 ; i<n;i++){
		stddev = stddev + ((tab[i]-m)*(tab[i]-m))/n;
	}
	if (stddev ==0)
	{
		cout <<"le tableau ne varie pas" <<endl;
		return tab2;
	}
	for(int i = 0; i<n; i++){
		tab2[i]= (tab[i])/sqrt(stddev);
	}
	return tab2;
}


void afficher (vector <double> tab){
	int n = tab.size();
	for (int i =0 ; i<n; i++){
		cout<< tab[i]<<endl;
	}
}

//On va coder des petites fonctions pour alléger le main

vector < vector <double> > construire_matrice_data(int nbseries, int tailletab, vector <double> TAB ){
    vector <vector <double> > tabdata3;
    for (int i = 0 ; i<nbseries; i++){
              vector <double> data;
              for(int j =0;j<tailletab;j++){
                  data.push_back(TAB[i*tailletab+j]);
              }
              tabdata3.push_back(data);
          }
    return tabdata3;
}

vector <vector <double> > coupertab(int t, vector <vector<double> > tabdata){
		  int n=tabdata.size();
		  int m=tabdata[0].size();
          vector < vector <double> > tabdata_coupe;
          for(int  i= 0; i<n;i++)
            {
                vector<double> data;
              for(int j =0; j< t; j++)
              {
                  data.push_back(tabdata[i][j]);
              }
              tabdata_coupe.push_back(data);
            }
    return tabdata_coupe;
}


vector <vector <double> > construire_matrice_distance(int nbseries, int h,int t ,vector <vector <vector <double> > >  Mat_totale, vector<double> Seq_a_predire,int p ){
                vector <vector <double> > Tabdist;
				for(int i =1; i<nbseries;i++){
						vector <double> temp_disttab;
						for (int indice_debut = 0; indice_debut<t-h+1;indice_debut++){
							temp_disttab.push_back(  choix_dist( Mat_totale[i][indice_debut] , Seq_a_predire,p )  );

						}
						Tabdist.push_back( temp_disttab );
						//cout<<"i="<<i<<endl;
				}
				return Tabdist;
}

vector <double> simplifietab(int nbseries, int h, int t, vector <vector <double> > Tabdist ){
				vector <double> Tabdist_simple ;
				for (int i =0; i<nbseries-1;i++){
					for (int j = 0;j<t-h+1;j++){
					Tabdist_simple.push_back(Tabdist[i][j]);
					}
				}
				return Tabdist_simple;
}
