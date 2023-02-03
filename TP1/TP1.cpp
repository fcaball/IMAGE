#include "utilitaires.cpp"
#include <iostream>

OCTET *Q1(OCTET *ImgIn, size_t nTaille, int S)
{
    return seuillageGrey(ImgIn, nTaille, S);
}

OCTET *Q2(OCTET *ImgIn, size_t nTaille, int S1, int S2, int S3)
{
    return seuillageGrey(ImgIn, nTaille, S1, S2, S3);
}

void Q3(OCTET *ImgIn, int nW, int nH, bool col, int i)
{
    profilGrey(ImgIn, nW, nH, col, i);
}

void Q4(OCTET *ImgIn, int ntaille)
{
    histoGrey(ImgIn, ntaille);
}

void Q5(OCTET *ImgIn, int ntaille)
{
    histoCouleur(ImgIn, ntaille);
}

OCTET *Q6(OCTET *ImgIn, int ntaille, int SR, int SG, int SB)
{
    seuillageCouleur(ImgIn, ntaille, SR, SG, SB);
}

OCTET *Q7(OCTET *ImgIn, int ntaille)
{
    int seuil;

     //moyenne
     int *OCCGRIS;

     allocation_tableau(OCCGRIS, int, 256);

     for (int j = 0; j < ntaille; j++)
     {
         OCCGRIS[ImgIn[j]] += 1;
     }

     for (size_t i = 0; i < 256; i++)
     {
         seuil+=OCCGRIS[i]*i;
     }


    seuil /= ntaille;
    return Q1(ImgIn, ntaille, seuil);
}

int main(int argc, char const *argv[])
{
    std::cout << "Saisissez la question a effectuer" << std::endl;
    char val;
    std::cin >> val;
    OCTET *ImgIn, *ImgOut;

    int w, h;
    // lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    lire_nb_lignes_colonnes_image_ppm((char *)argv[1], &w, &h);
    int ntaille = w * h;
    allocation_tableau(ImgIn, OCTET, ntaille * 3);
    // lire_image_pgm((char *)argv[1], ImgIn, ntaille);
    lire_image_ppm((char *)argv[1], ImgIn, ntaille);
    switch (val)
    {
    case '1':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q1(ImgIn, ntaille, 128);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);
        break;
    case '2':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q2(ImgIn, ntaille, 60, 128, 190);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);

        break;
    case '3':
        if (argc != 2)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre\n";
            return 1;
        }
        Q3(ImgIn, w, h, false, 250);
        break;
    case '4':
        if (argc != 2)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre\n";
            return 1;
        }
        Q4(ImgIn, ntaille);
        break;
    case '5':
        if (argc != 2)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre\n";
            return 1;
        }
        Q5(ImgIn, ntaille);
        break;
    case '6':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }

        ImgOut = Q6(ImgIn, ntaille, 100, 100, 100);
        ecrire_image_ppm((char *)argv[2], ImgOut, w, h);

        break;
    case '7':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q7(ImgIn, ntaille);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);

        break;

    default:
        break;
    }

    return 0;
}
