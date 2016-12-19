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
float KMeans::d2(CImg<float> & g_i,CImg<float> & data, int x, int y) {
    float d = 0.0;

    for(int i = 0 ; i < data.depth() ; ++i) {
        d += ((data(x,y,i)-g_i(0,0,i))*(data(x,y,i)-g_i(0,0,i)));
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
        centers.push_back(CImg<float>(1,1,attributs.depth()));
        int x = gen()/(float)gen.max()*attributs.width();
        int y = gen()/(float)gen.max()*attributs.height();
        for(int j = 0 ; j < attributs.depth() ; ++j)
            centers(i)(j) = attributs(x,y,j);
    }
    return centers;
}

/*******************************************************************************

                        Fonction de determination de la classe la plus proche

*******************************************************************************/
float KMeans::GetNearestClass(CImg<float> & attributs, int x, int y, CImgList<> & centres) {
    float classe = 255.0,
          valeur = FLT_MAX;
    float temp;

    for(unsigned i = 0 ; i < centres.size() ; ++i) {
        temp = d2(centres[i],attributs,x,y);

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
    long nb[ncl];

    for(int i = 0 ; i < ncl ; ++i) {
        centers.push_back(CImg<float>(1,1,attributs.depth()));
        nb[i] = 0;
    }

    cimg_forXY(res, x, y) {

        for(int i=0;i<attributs.depth();++i) {
            int indtmp = (int)res(x,y);
            centers[indtmp](i) += attributs(x,y,i);
        }

        nb[(int)res(x,y)]++;
    }

    for(int k=0;k<ncl;++k)
        for(int i=0;i<attributs.depth();++i)
            centers[k](i) /= nb[k];

    return centers;
}

/*******************************************************************************

                        Calcul de l'erreur

*******************************************************************************/
float KMeans::GetError(CImg<float> & res, CImg<float> & attributs, CImgList<> & centers) {
    float erreur = 0.0;

    cimg_forXY(res, x, y) {
        erreur += d2(centers((int)res(x,y)), attributs, x, y);
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

    CImg<float> res(attributs.width(),attributs.height(),1,1,0);
    CImg<float> tmp(attributs.width(),attributs.height(),1,1,0);
    CImgList<float> centre = GetRandomCenter(attributs, ncl);

    bool continuer = true;
    unsigned round = 0;
    float erreur = FLT_MAX;

    while (continuer) {
        // clustering
        cimg_forXY(res, x, y)
        {
            tmp(x,y) = GetNearestClass(attributs,x,y,centre);
        }

        // recalculer les centres
        centre = GetNewCenters(tmp, attributs, ncl);

        // trop long

        float new_error = GetError(tmp, attributs, centre);
        bool changed = HasChanged(res,tmp);
        res = tmp;
        ++round;
        continuer = /*(new_error < erreur); &&*/ changed && (round < MAXITER);

        cout << "Round " << round << " \tGain: " << 100*(erreur-new_error)/(erreur) << "%" << endl;
        erreur = new_error;
        for(int c = 0 ; c < ncl ; ++c)
            cout<< "\t" << centre[c](0) << "\t" << centre[c](1) << endl;
    }

    return res;
}

CImg<float> KMeans::operator()(CImg<float> & attributs, int ncl) {
    return this->execute(attributs, ncl);
}

