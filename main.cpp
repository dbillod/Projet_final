#include<iostream>
#include<strstream>
#include<vector>
#include<cmath>
#include<fstream>
#include<sstream>
#include<string>
#include<stdio.h>
#include<stdlib.h>

#include"Fonc.h"


using namespace std;




int main()
{



    cout<<"***********************************************************"<<endl;
    cout<<"Bienvenue dans le projet de prevision de series temporelles"<<endl;
    cout<<"***********************************************************"<<endl;
    cout<<" "<<endl;


     //////////////////////////////////////////////
     //////////////// NOTA BENE ///////////////////
     //////////////////////////////////////////////

    //Ce fichier doit avoir un pr�sentation pr�cise//
    //Il doit avoir � la premi�re ligne une lettre//
    //Puis la s�rie correspondante, de la plus ancienne valeur � la plus r�cente//
    //Puis une autre letre , puis une autre s�rie..//

    /////ATTENTION//////
    //le fichier texte contenant les donnees doit obligatoirement se terminer par le mot: fin//

    cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    cout<<"Ce code necessite une presentation precise du fichier txt a utiliser"<<endl;
    cout<<"Consulter le fichier Formes_donnees.txt pour plus d'explications"<<endl;
    cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    cout<<" "<<endl;

	cout<<"Veuillez indiquer le chemin d'acces du fichier txt contenant les series a utiliser"<<endl;
	cout<<"--------------------------------------------"<<endl;
	cout<<"!!Le chemin ne doit pas contenir d'accents!!"<<endl;
    cout<<"--------------------------------------------"<<endl;


	string chemin;
    getline(cin,chemin);

    cout<<"Combien de valeurs vous voulez prevoir (nombre entier) ?"<<endl;
    cout<<" "<<endl;

    int nbcoup;
    cin>> nbcoup;

	string sflux = chemin;


    ifstream monFlux(sflux);



    if(monFlux)
   {
      //L'ouverture s'est bien pass�e, on peut donc lire

    string ligne; //cette variable stocke les lignes lues
    vector <double> TAB;
    int nbdouble = 0; //cette variable stocke le nombre de valeurs totales du fichier
    int nbseries = 0; //cette variable stocke le nombre de s�ries pr�sentes dans le fichier
      while(getline(monFlux, ligne))
      {
         if (ligne=="fin")
         {
             break;
         }
             if (ligne =="")
             {
                    continue;
             }
         istringstream istr(ligne);
         double d;
         if (istr>>d){
         nbdouble=nbdouble +1;//on compte le nombre total de valeurs
         TAB.push_back(d);
         }
         else
         {
            nbseries++;
         }
      }


        int tailletab = nbdouble/nbseries; //tailletab est la taille commune de toutes les s�ries

        vector <vector <double> > tabdata = construire_matrice_data(nbseries,tailletab,TAB);

    /////////////////////////////////////////////////////////////
    //On veut trouver les meilleurs k et h par cross validation//
    /////////////////////////////////////////////////////////////

        vector <double> erreurkh; //tableau qui stocke les erreurs de pr�diction pour un couple (k,h) donn�
        vector <double> erreurmoykh; // tableau qui stocke les erreurs et le couple correspondant

        vector <vector <double > > Indice_utileglobal; //tableau de tableaux (matrice) dont le p-�me �l�ment est le tableau des utlit�s des s�ries explicativesd pour la p-�me distance
        vector <double>Ensemble_previsionsp0; //tableau qui stocke les pr�visions pour la distance0, i.e distanceinf
        vector <double>Ensemble_previsionsp1;
        vector <double>Ensemble_previsionsp2;

        //////////////////////////////////////////////////////////////////////
        // On va stocker le nom des series explicatives sous forme x1, x2...//
        //////////////////////////////////////////////////////////////////////

        vector <string> Nom_series;
          for(int i =0; i<nbseries;i++){
            string Res ;
            stringstream convert ;
            convert << i+1;
            Res = convert.str(); //on passe i en string

            string sxi = "x" + Res;//on cr�e xi
            Nom_series.push_back(sxi);
            }

		  //On fixe le nombre maximum de plus proches voisins et la taille maximale des s�quences � comparer
		  int kmax=10;
		  int hmax=10;


/////////////////////////////////////////////////////////////////////////////////////
//Pour prevoir plusieurs valeurs on va le faire distance par distance////////////////
//Donc on cr�e 3 tableaux de tableaux (matrice) qui stockeront les donn�es updat�es//
/////////////////////////////////////////////////////////////////////////////////////
    vector <vector <double> >tabdatap0=tabdata; //va stockerl les donn�es updat�es pour la distance0
    vector <vector <double> >tabdatap1=tabdata;
    vector <vector <double> >tabdatap2=tabdata;

    int nbprevision = 0;

    while(nbcoup>0){
            cout<<"****************************"<<endl;
            cout<<"Prevision de y en T+"<<nbprevision+1<<endl;
            cout<<"****************************"<<endl;
            nbprevision+=1;


            nbcoup = nbcoup-1;

      for (int p =0;p<3;p++){
            ////////////////////////////////////////////
            //On va faire varier le choix de distances//
            ////////////////////////////////////////////

		  ////////////////////////////////////////////////////////////////////
		  //Selon le choix de la distance, on ne prend pas les m�mes donn�es//
		  ////////////////////////////////////////////////////////////////////

            if(p==0){
                tabdata = tabdatap0;
            }
            if (p==1){
                tabdata = tabdatap1;
            }
            if(p==2){
                tabdata = tabdatap2;
            }


      for(int k=1; k<kmax; k++){

	  for(int h=2; h<hmax;h++){



	  for(int t = hmax;  t< tailletab ;t++){

            vector < vector <double> > tabdata_coupe = coupertab(t, tabdata); //matrice ou tableau de tableux qui va stocker les donn�es tronqu�es

		  //on va renormer les tableaux
		vector <vector<double> >tabdata_r;
		for(int i =0; i<nbseries;i++){
			tabdata_r.push_back(renorm(tabdata_coupe[i])); //on renorme les donn�es pour pouvoir les comparer
		}



//////////////////////////////////////////////////////////
	  //On a construit un tableau r�sumant les donn�es//
//////////////////////////////////////////////////////////



				vector <vector <vector <double> > > Mat_totale; // on va stocker dans Mat_totale toutes les sous �quences de taille h de toutes les s�ries

				for(int i =0; i<nbseries;i++){
				Mat_totale.push_back(matrice_sous_seq(tabdata_r[i],h));
			   }

				vector<double> Seq_a_predire = extract(tabdata_r[0],t-h,h);// Seq_a_predire est la derni�re s�quence de y de taille h


                //Tabdist est un tableau de tableaux (matrice) dont le premier �l�ment est un tableau de taille h qui comporte tailletab-h �l�mets qui sont les distances des s�quences extraites par rapport � ce qu'il faut pr�dire
                vector <vector <double>  >  Tabdist=construire_matrice_distance(nbseries,h,t,Mat_totale,Seq_a_predire,p);



				vector <double> Tabdist_simple= simplifietab(nbseries, h,  t, Tabdist );


				vector <double> Positions_min = argminmultiple(Tabdist_simple,k);


				vector < vector <double> > Seq_min; //seq-min contient les tableaux les plus proches de seq_a_predire
				vector <double> Val_suiv; //Ce tableau va stocker les valeurs suivantes des seq_min, sur lesquelles on va moyenner pour obtenir notre pr�diction

				for(int i =0; i<k;i++){
					int quotient = (Positions_min[i])/(t-h+1);
					int reste = Positions_min[i]-quotient*(t-h+1);
					Seq_min.push_back(Mat_totale[quotient+1][reste]);
					if(reste == t-h){
					 Val_suiv.push_back(Mat_totale[quotient+1][reste][h-1]);//Si on tombe sur le dernier terme d'une s�rie, on ne prend pas la valeur suivante mais ce terme-l�
					}
					else {
					Val_suiv.push_back( Mat_totale[quotient+1][reste+1][h-1] );
					}
				}

				double prevision = moyenne(Val_suiv); //Valeur pr�vue

				erreurkh.push_back(abs(prevision-tabdata[0][t]));


	  }//fin du for t
        double m = moyenne(erreurkh);
        erreurmoykh.push_back(k);
        erreurmoykh.push_back(h);
        erreurmoykh.push_back(m);
	  }//fin du for h
	  }//fin du for k


	  int longueur = erreurmoykh.size()/3;
	  vector <double> erreurmoy;
	  for (int i =1; i<longueur; i++ ){
            erreurmoy.push_back(erreurmoykh[3*i-1]); // on stocke les m, i.e erreurs moyennes
	  };

    int arg = argmin(erreurmoy);
    int bestk = erreurmoykh[3*arg]; //best k est notre k optimal par cross-validation
    int besth = erreurmoykh[3*arg +1];


    cout<<"//////////////////////////////////////////////////////////////////////"<<endl;
    cout<<"     p =    "<<p<<" (Relatif a la distance choisie)"<<endl;
    cout<<"//////////////////////////////////////////////////////////////////////"<<endl;
    cout<<" "<<endl;

    cout<<"--------------------------------"<<endl;
    cout<<"Resultats de la cross-validation"<<endl;
    cout<<"--------------------------------"<<endl;
    cout<<" "<<endl;
    cout<<"best k = " << bestk<<endl;
    cout<<"best h = " << besth<<endl;
    cout<<" "<<endl;

////////////////////////////////////////////
  //On a donc trouv� nos k et h optimaux//
////////////////////////////////////////////


///////////////////////////////////////////////////////////////
//Il faut maintenant estimer avec ces k et h notre prochain y//
///////////////////////////////////////////////////////////////

          ///////////////////////////////
		  //on va renormer les tableaux//
		  ///////////////////////////////
		vector <vector<double> >tabdata_r;
		for(int i =0; i<nbseries;i++){
			tabdata_r.push_back(renorm(tabdata[i]));
		}


//////////////////////////////////////////////////////////
	  //On a construit un tableau r�sumant les donn�es//
//////////////////////////////////////////////////////////


vector <vector <vector <double> > > Mat_totale; // on va stocker dans Mat_totale toutes les sous �quences de taille h

				for(int i =0; i<nbseries;i++){
				Mat_totale.push_back(matrice_sous_seq(tabdata_r[i],besth));
			   }

				vector<double> Seq_a_predire = extract(tabdata_r[0],tailletab-besth,besth);// Seq_a_predire est la derni�re s�q de y d etaille h

                //Tabdist est un tableau de tableaux (matrice) dont le premier �l�ment est un tableau de taille h qui comprte tailletab-h �l�mets qui sont les distances des s�quences extraites // � ce qu'il faut pr�dire
                vector <vector <double>  >  Tabdist=construire_matrice_distance(nbseries,besth,tailletab,Mat_totale,Seq_a_predire,p);

				vector <double> Tabdist_simple= simplifietab(nbseries, besth,  tailletab, Tabdist );

				vector <double> Positions_min = argminmultiple(Tabdist_simple,bestk);

                vector < vector <double> > Seq_min; //seq-min contient les tableaux les plus proches de seq_a_predire
				vector <double> Val_suiv; //Ce tableau va stocker les valeurs suivantes des seq_min
				vector <double> Indice_utile;
				//double Indice_util[nbseries-1]; //On cr�e ce tableau pour savoir quelles s�ries ont �t� les plus utiles

				for (int i=0; i< nbseries -1; i++){
					Indice_utile.push_back(0); //On remplit le tableau de 0, valeur par d�faut de l'utilit� d'une s�rie
				}


				for(int i =0; i<bestk;i++){
					int quotient = (Positions_min[i])/(tailletab-besth+1);
					int reste = Positions_min[i]-quotient*(tailletab-besth+1);
					Indice_utile[quotient] =Indice_utile[quotient]+ 100/(double)bestk;// On stocke en pourcentage l'inter�t de la s�rie comme sa part dans les plus proches voisins

					Seq_min.push_back(Mat_totale[quotient+1][reste]); //On se place ne quotient +1 pour ne pas tomber sur la s�rie � pr�voir
					if(reste == tailletab-besth){
					 Val_suiv.push_back(Mat_totale[quotient+1][reste][besth-1]);//Si on tombe sur le dernier terme d'une s�rie, on ne prend pas la valeur suivante mais ce terme
					}
					else {
					Val_suiv.push_back( Mat_totale[quotient+1][reste+1][besth-1] );
					}
				}

				/*vector <double> Indice_utile;
                for (int i=0; i< nbseries-1;i++){
                    Indice_utile.push_back(Indice_util[i]);
                }*/




				double prevision = moyenne(Val_suiv);

                cout<<"------------------------------------------------------"<<endl;
                cout<<"Prevision avec parametres optimaux"<<endl;
				cout<<"------------------------------------------------------"<<endl;
				cout<< "La prevision pour le prochain terme de y renorme est " << prevision<<endl;
				cout<< "La prevision pour le prochain terme de y est "<< prevision * ecart_type(tabdata[0])<<endl;


				//Suivant la distance choisie, on va uipdater les donn�es des tabdatap en leur ajoutant la derni�re valeur pr�ite
				if(p==0){
                    tabdatap0[0].push_back(prevision*ecart_type(tabdata[0]));
                    Ensemble_previsionsp0.push_back(prevision * ecart_type(tabdata[0]));
				}
				if (p==1){
                    tabdatap1[0].push_back(prevision*ecart_type(tabdata[0]));
                    Ensemble_previsionsp1.push_back(prevision * ecart_type(tabdata[0]));
				}
				if (p==2){
                    tabdatap2[0].push_back(prevision*ecart_type(tabdata[0]));
                    Ensemble_previsionsp2.push_back(prevision * ecart_type(tabdata[0]));
				}

				double prog = 100* (prevision * ecart_type(tabdata[0])- tabdata[0][tailletab-1] )/tabdata[0][tailletab-1];
				cout<<"Soit une variation de "<<prog<<"%"<<endl;
				cout<<" "<<endl;


                cout<< "----------------------------------------"<<endl;
                cout<< "Niveau d'utilite des series explicatives"<<endl;
                cout<< "----------------------------------------"<<endl;

                for (int i =0; i< nbseries-1;i++){
                    cout<<"La serie "<<Nom_series[i]<<" a ete utile a "<<Indice_utile[i]<<"%"<<endl;
                }

                Indice_utileglobal.push_back(Indice_utile);
                cout<<" "<<endl;

      }//Fin du for p

	  //On supprime des donn�es updat�es la premi�re valeur pour garder des s�ries de m�me taille
      (tabdatap0[0]).erase(tabdatap0[0].begin()+0);
      (tabdatap1[0]).erase(tabdatap1[0].begin()+0);
      (tabdatap2[0]).erase(tabdatap2[0].begin()+0);


    }//fin du while nbcoup

    vector <vector <double> > Ensemble_previsions;
    Ensemble_previsions.push_back(Ensemble_previsionsp0);
    Ensemble_previsions.push_back(Ensemble_previsionsp1);
    Ensemble_previsions.push_back(Ensemble_previsionsp2);

      cout<<"-------------------------------------------------------------"<<endl;
      cout<<"Resume des resultats avec les differentes distances (p=0,1,2)"<<endl;
      cout<<"-------------------------------------------------------------"<<endl;
      cout<<" "<<endl;


      cout<<" p "<<" Series utiles "<<" Prediction "<<endl;

	  //On va ici g�rer l'affichage des r�sultats suivant nbprevisiobn et nbseries
      for (int p =0; p<3;p++){
            if(nbseries-1<= nbprevision){
                for (int n=0;n<nbprevision;n++){

                        if(n==0){
                            cout<<"p="<<p<<"   "<<Nom_series[n]<<" "<<"("<<Indice_utileglobal[p][n]<<"%)"<<"       "<<Ensemble_previsions[p][n]<<"      "<<endl;
                                }
                        if(n<nbseries-1&&n>0){
                            cout<<"   "<<"   "<<Nom_series[n]<<" "<<"("<<Indice_utileglobal[p][n]<<"%)"<<"       "<<Ensemble_previsions[p][n]<<"      "<<endl;
                        }
                        if(n>=nbseries-1){
                            cout<<"   "<<"   "<<"   "<<"    "<<"       "<<Ensemble_previsions[p][n]<<"      "<<endl;
                        }
                }
            }

            if (nbseries-1>nbprevision){
                    for (int n=0;n<nbseries-1;n++){

                        if(n==0){
                            cout<<"p="<<p<<"   "<<Nom_series[n]<<" "<<"("<<Indice_utileglobal[p][n]<<"%)"<<"       "<<Ensemble_previsions[p][n]<<"      "<<endl;
                                }
                        if(n<nbprevision&&n>0){
                            cout<<"   "<<"   "<<Nom_series[n]<<" "<<"("<<Indice_utileglobal[p][n]<<"%)"<<"       "<<Ensemble_previsions[p][n]<<"      "<<endl;
                        }
                        if(n>=nbprevision){
                            cout<<"   "<<"   "<<Nom_series[n]<<" "<<"("<<Indice_utileglobal[p][n]<<"%)"<<endl;
                        }
                    }
            }
      }


   }
   else
   {
      cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
   }








    return 0;
}
