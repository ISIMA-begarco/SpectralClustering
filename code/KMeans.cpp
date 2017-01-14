#include "KMeans.h"

#include <iostream>
#include <string>
#include <random>
#include <sstream>

using std::cout;
using std::endl;
using std::string;

KMeans::KMeans() : gen(63) {
    //ctor
}

KMeans::~KMeans() {
    //dtor
}

/*******************************************************************************

                   Calcul de la distance au carre

*******************************************************************************/
float KMeans::d2(CImg<float> & g_i,CImg<float> & data, int y) {
    float d = 0.0;

    for(int x = 0 ; x < data.width() ; ++x) {
        d += ((data(x,y)-g_i(x))*(data(x,y)-g_i(x)));
    }

    return d;
}

/*******************************************************************************

                        Fonction de recuperation aleatoire

*******************************************************************************/
CImgList<> KMeans::GetRandomCenter(CImg<float> & attributs, int ncl) {
    CImgList<> centers;

    for(int i = 0 ; i < ncl ; ++i)
    {
        centers.push_back(CImg<float>(attributs.width()));
        int y = (gen()/(float)gen.max())*attributs.height();
        for(int x = 0 ; x < attributs.width() ; ++x)
            centers(i)(x) = attributs(x,y);
    }
    return centers;
}

/*******************************************************************************

                        Fonction de determination de la classe la plus proche

*******************************************************************************/
float KMeans::GetNearestClass(CImg<float> & attributs, int y, CImgList<> & centres) {
    int classe = 0;
    float valeur = FLT_MAX - 1.0;
    float temp;

    for(unsigned i = 0 ; i < centres.size() ; ++i) {
        temp = d2(centres[i],attributs,y);

        if(temp < valeur) {
            valeur = temp;
            classe = i;
        }
    }

    return classe;
}

/*******************************************************************************

                        Fonction de calcul des centres

*******************************************************************************/
CImgList<> KMeans::GetNewCenters(CImg<float> & res, CImg<float> & attributs, int ncl) {
    CImgList<> centers;
    long * nb = new long[ncl];

    for(int i = 0 ; i < ncl ; ++i) {
        centers.push_back(CImg<float>(attributs.width()));
        nb[i] = 0;
    }


    // Sommation
    cimg_forY(attributs, y) {

        long indtmp = (long)res(y);
        for(int x=0;x<attributs.width();++x) {
            centers[indtmp](x) += attributs(x,y);
        }

        nb[indtmp]++;
    }

    // Moyennage
    for(long k=0;k<ncl;++k)
        for(long x=0;x<attributs.width();++x)
            centers[k](x) /= nb[k];

    delete [] nb;
    return centers;
}

/*******************************************************************************

                        Calcul de l'erreur

*******************************************************************************/
float KMeans::GetError(CImg<float> & res, CImg<float> & attributs, CImgList<> & centers) {
    float erreur = 0.0;

    cimg_forX(res, x) {
        erreur += d2(centers((int)res(x)), attributs, x);
    }

    return erreur;
}

/*******************************************************************************

                        Check les changements

*******************************************************************************/
float KMeans::HasChanged(CImg<float> & im1, CImg<float> & im2) {
    bool erreur = false;

    cimg_forXY(im1, x, y) {
        erreur |= (im1(x,y)!=im2(x,y));
    }

    return erreur;
}

/*******************************************************************************

                        Fonction de classification

*******************************************************************************/
CImg<float> KMeans::execute(CImg<float> & attributs, int ncl) {

    CImg<float> res(attributs.height(),1,1,1,0);
    CImg<float> tmp(attributs.height(),1,1,1,0);
    CImgList<float> centre = GetRandomCenter(attributs, ncl);

    bool continuer = true;
    unsigned round = 0;

    while (continuer) {
        // clustering
        cimg_forX(tmp, x)
        {
            tmp(x) = GetNearestClass(attributs,x,centre);
        }

        // recalculer les centres
        centre = GetNewCenters(tmp, attributs, ncl);

        // trop long
        bool changed = HasChanged(res,tmp);
        res = tmp;
        ++round;
        continuer = changed && (round < MAXITER);
    }

    cout << "Terminated in " << round << " iterations." << endl;

    return res;
}

CImg<float> KMeans::operator()(CImg<float> & attributs, int ncl) {
    return this->execute(attributs, ncl);
}

