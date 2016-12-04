#include <iostream>
#include <string>

#include "CImg.h"
#include "SpectralClustering.h"

using namespace cimg_library;
using namespace std;

int main(int argc, const char* argv []) {
    string file = argc > 1 ? argv[1] : "fish.bmp";
    CImg<> img = CImg<>(file.c_str()).channel(0);

    int ncl = argc > 2 ? atoi(argv[2]) : 2;    // Nombre de classes

    SpectralClustering algo;

    CImg<unsigned char> etiquette = algo(img,ncl);

    // Affichage finale
    CImgDisplay dispImg(img,"Image originale");
    CImgDisplay dispRes(etiquette.normalize(0,255),"Image segmentee");

    while (!dispImg.is_closed() && !dispRes.is_closed())
    {
        dispImg.wait();
        dispRes.wait();
    }
    return 0;}



