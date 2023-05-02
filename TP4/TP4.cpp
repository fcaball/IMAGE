#include "utilitaires.cpp"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{

    OCTET *ImgIn, *ImgOut, *ImgSeuil, *ImgFlou;

    int w, h;
    // lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    int ntaille = w * h;
    allocation_tableau(ImgIn, OCTET, ntaille);
    allocation_tableau(ImgOut, OCTET, ntaille);
    allocation_tableau(ImgSeuil, OCTET, ntaille);
    lire_image_pgm((char *)argv[2], ImgSeuil, ntaille);
    lire_image_pgm((char *)argv[1], ImgIn, ntaille);
    // ImgOut = RGBtoY(ImgIn, ntaille);
    // ImgSeuil=erosionNoiretBlanc(ImgSeuil,h,w);
    // ImgSeuil=dilatationNoiretBlanc(ImgSeuil,h,w);
    // ImgOut=seuillageGrey(ImgOut,ntaille,50);
    // ImgFlou = filtrefloucouleur(ImgIn, w, h);

    // for (size_t i = 0; i < ntaille; i++)
    // {
    //     if (ImgSeuil[i] == 255)
    //     {
    //         ImgOut[i * 3] = ImgFlou[i * 3];
    //         ImgOut[i * 3 + 1] = ImgFlou[i * 3 + 1];
    //         ImgOut[i * 3 + 2] = ImgFlou[i * 3 + 2];
    //     }
    //     else
    //     {
    //         ImgOut[i * 3] = ImgIn[i * 3];
    //         ImgOut[i * 3 + 1] = ImgIn[i * 3 + 1];
    //         ImgOut[i * 3 + 2] = ImgIn[i * 3 + 2];
    //     }
    // }

    FILE *histo = fopen("./ROC.dat", "w");
    int SeuilOpti = -1;
    float youden = -1;
    int true_positif_opti = 0.0;
    int faux_positif_opti = 0.0;
    int faux_negatif_opti = 0.0;

    for (size_t k = 0; k < 256; k++)
    {
        float VN = 0.0, FN = 0.0, VP = 0.0, FP = 0.0;

        ImgOut = seuillageGrey(ImgIn, ntaille, k);
        for (size_t i = 0; i < ntaille; i++)
        {
            if (ImgSeuil[i] == 255 && ImgSeuil[i] == ImgOut[i])
                VN++;
            else if (ImgSeuil[i] == 0 && ImgSeuil[i] != ImgOut[i])
                FN++;
            else if (ImgSeuil[i] == 0 && ImgSeuil[i] == ImgOut[i])
                VP++;
            else if (ImgSeuil[i] == 255 && ImgSeuil[i] != ImgOut[i])
                FP++;
        }
        float sensibilite = VP / (VP + FN);
        float specificite = (VN / (VN + FP));
        fprintf(histo, "%f %f\n", 1 - specificite, sensibilite);
        // cout<<1 - (VN / (VN + FP))<<" "<<VP / (VP + FN)<<" "<<k<<endl;
        if (sensibilite + specificite - 1 > youden)
        {
            youden = sensibilite + specificite - 1;
            SeuilOpti = k;
            true_positif_opti = VP;
            faux_positif_opti = FP;
            faux_negatif_opti = FN;
        }
    }

    cout << SeuilOpti << endl;
    float precision=((float)true_positif_opti/((float)true_positif_opti+(float)faux_positif_opti)) ;
    float rappel= ((float)true_positif_opti/((float)true_positif_opti+(float)faux_negatif_opti) );
    cout << "Precision : "<<precision<< endl;
    cout << "Rappel : "<<rappel<< endl;
    cout << "F1 Score : "<<2.0*( precision*rappel)/(precision+rappel)<< endl;

    std::fclose(histo);

    // ecrire_image_ppm((char *)argv[3], ImgOut, w, h);
    // histoGrey(ImgOut, w* h);

    return 0;
}
