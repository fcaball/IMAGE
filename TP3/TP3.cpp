#include "utilitaires.cpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{

    OCTET *ImgIn, *ImgOut;

    int w, h;
    // lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    lire_nb_lignes_colonnes_image_ppm((char *)argv[1], &w, &h);
    int ntaille = w * h;
    allocation_tableau(ImgIn, OCTET, ntaille * 3);
    allocation_tableau(ImgOut, OCTET, ntaille * 3);
    // lire_image_pgm((char *)argv[1], ImgIn, ntaille);
    lire_image_ppm((char *)argv[1], ImgIn, ntaille);

    // vector<int> a=getA0A1fromHisto(ImgIn,ntaille);
    // int a0=a[0];
    // int a1=a[1];
    // int alpha=(-255*a0)/(a1-a0);
    // int beta=255/(a1-a0);
    // cout<<alpha<<" "<<beta<<endl;

    // for (size_t i = 0; i < ntaille; i++)
    // {
    //     ImgOut[i]=(-255/(a1-a0))*(a0-ImgIn[i]);
    // }

    // ecrire_image_pgm((char*) argv[2],ImgOut,w,h);

    OCTET *r;
    allocation_tableau(r,OCTET,ntaille);
    planR(r, ImgIn, ntaille);

    vector<int> a = getA0A1fromHisto(r, ntaille);
    int a0 = a[0];
    int a1 = a[1];
    int alphaR = (-255 * a0) / (a1 - a0);
    int betaR = 255 / (a1 - a0);
    cout << alphaR << " " << betaR << endl;
    OCTET *ROut;
    allocation_tableau(ROut,OCTET,ntaille);

    for (size_t i = 0; i < ntaille; i++)
    {
        ROut[i] = (-255 / (a1 - a0)) * (a0 - ImgIn[i*3]);
    }
    OCTET *g;
    allocation_tableau(g,OCTET,ntaille);
    planR(g, ImgIn, ntaille);
    a = getA0A1fromHisto(g, ntaille);
    a0 = a[0];
    a1 = a[1];
    int alphaG = (-255 * a0) / (a1 - a0);
    int betaG = 255 / (a1 - a0);
    cout << alphaG << " " << betaG << endl;
    OCTET *GOut;
    allocation_tableau(GOut,OCTET,ntaille);

    for (size_t i = 0; i < ntaille; i++)
    {
        GOut[i] = (-255 / (a1 - a0)) * (a0 - ImgIn[i*3+1]);
    }

    OCTET *b;
    allocation_tableau(b,OCTET,ntaille);
    planR(b, ImgIn, ntaille);
    a = getA0A1fromHisto(b, ntaille);
    a0 = a[0];
    a1 = a[1];
    int alphaB = (-255 * a0) / (a1 - a0);
    int betaB = 255 / (a1 - a0);
    cout << alphaB << " " << betaB << endl;
    OCTET *BOut;
    allocation_tableau(BOut,OCTET,ntaille);

    for (size_t i = 0; i < ntaille; i++)
    {
        BOut[i] = (-255 / (a1 - a0)) * (a0 - ImgIn[i*3+2]);
    }

    for (size_t i = 0; i < ntaille; i++)
    {
        ImgOut[i * 3] = ROut[i];
        ImgOut[i * 3 + 1] = GOut[i];
        ImgOut[i * 3 + 2] = BOut[i];
    }

    ecrire_image_ppm((char *)argv[2], ImgOut, w, h);

    return 0;
}
