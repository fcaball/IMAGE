#include "utilitaires.cpp"
#include <iostream>

using namespace std;

OCTET *Q1(OCTET *ImgIn, int ntaille)
{
    int seuil;

    // moyenne
    int *OCCGRIS;

    allocation_tableau(OCCGRIS, int, 256);

    for (int j = 0; j < ntaille; j++)
    {
        OCCGRIS[ImgIn[j]] += 1;
    }

    for (size_t i = 0; i < 256; i++)
    {
        seuil += OCCGRIS[i] * i;
    }

    seuil /= ntaille;
    return seuillageGrey(ImgIn, ntaille, seuil);
}

OCTET *Q2(OCTET *ImgIn, int h, int w)
{
    return erosionNoiretBlanc(ImgIn, h, w);
}

OCTET *Q3(OCTET *ImgIn, int h, int w)
{
    return dilatationNoiretBlanc(ImgIn, h, w);
}

OCTET *Q4(OCTET *ImgIn, int h, int w)
{
    return fermetureNoiretBlanc(ImgIn, h, w);
}

OCTET *Q5(OCTET *ImgIn, int h, int w)
{
    return ouvertureNoiretBlanc(ImgIn, h, w);
}

OCTET *Q6(OCTET *ImgIn, int h, int w)
{
    OCTET *Und = Q3(ImgIn, h, w);
    OCTET *Deuxd = Q3(Und, h, w);
    OCTET *Troisd = Q3(Deuxd, h, w);

    OCTET *Une = Q2(Troisd, h, w);
    OCTET *Deuxe = Q2(Une, h, w);
    OCTET *Troise = Q2(Deuxe, h, w);
    OCTET *Quatree = Q2(Troise, h, w);
    OCTET *Cinqe = Q2(Quatree, h, w);
    OCTET *Sixe = Q2(Cinqe, h, w);

    return Sixe;
}

OCTET *Q7(OCTET *ImgIn, int h, int w)
{
    OCTET *dilat = Q3(ImgIn, h, w);
    OCTET *ero = Q2(ImgIn, h, w);
    return differenceNoiretBlanc(dilat, ero, w, h);
}

OCTET *QBonusGrey(OCTET *ImgIn, int h, int w)
{
    OCTET *greyErosion = erosionGrey(ImgIn, h, w);
    char *s = "ErosionGrey.pgm";
    ecrire_image_pgm((char *)s, greyErosion, w, h);

    OCTET *greyDilatation = dilatationGrey(ImgIn, h, w);
    ecrire_image_pgm((char *)"DilatationGrey.pgm", greyDilatation, w, h);

    OCTET *greyFermeture = fermetureGrey(ImgIn, h, w);
    ecrire_image_pgm((char *)"FermetureGrey.pgm", greyFermeture, w, h);

    OCTET *greyOuverture = ouvertureGrey(ImgIn, h, w);
    ecrire_image_pgm((char *)"OuvertureGrey.pgm", greyOuverture, w, h);
}

OCTET *QBonusColor(OCTET *ImgIn, int h, int w)
{
    OCTET *r;
    allocation_tableau(r, OCTET, w * h);
    planR(r, ImgIn, w * h);
    OCTET *g;
    allocation_tableau(g, OCTET, w * h);
    planV(g, ImgIn, w * h);
    OCTET *b;
    allocation_tableau(b, OCTET, w * h);
    planB(b, ImgIn, w * h);
    OCTET *red = erosionGrey(r, h, w);
    OCTET *green = erosionGrey(g, h, w);
    OCTET *blue = erosionGrey(b, h, w);
    OCTET *rgb;
    allocation_tableau(rgb, OCTET, w * h * 3);
    for (size_t i = 0; i < w * h; i++)
    {
        rgb[i * 3] = red[i];
        rgb[i * 3 + 1] = green[i];
        rgb[i * 3 + 2] = blue[i];
    }

    ecrire_image_ppm((char *)"ErosionColortest.ppm", rgb, w, h);

    red = dilatationGrey(r, h, w);
    green = dilatationGrey(g, h, w);
    blue = dilatationGrey(b, h, w);
    rgb;
    for (size_t i = 0; i < w * h; i++)
    {
        rgb[i * 3] = red[i];
        rgb[i * 3 + 1] = green[i];
        rgb[i * 3 + 2] = blue[i];
    }
    ecrire_image_ppm((char *)"DilatationColortest.ppm", rgb, w, h);

    red = fermetureGrey(r, h, w);
    green = fermetureGrey(g, h, w);
    blue = fermetureGrey(b, h, w);
    rgb;
    for (size_t i = 0; i < w * h; i++)
    {
        rgb[i * 3] = red[i];
        rgb[i * 3 + 1] = green[i];
        rgb[i * 3 + 2] = blue[i];
    }
    ecrire_image_ppm((char *)"FermetureColortest.ppm", rgb, w, h);

    red = ouvertureGrey(r, h, w);
    green = ouvertureGrey(g, h, w);
    blue = ouvertureGrey(b, h, w);
    rgb;
    for (size_t i = 0; i < w * h; i++)
    {
        rgb[i * 3] = red[i];
        rgb[i * 3 + 1] = green[i];
        rgb[i * 3 + 2] = blue[i];
    }
    ecrire_image_ppm((char *)"OuvertureColortest.ppm", rgb, w, h);
}

int main(int argc, char const *argv[])
{
    std::cout << "Saisissez la question a effectuer" << std::endl;
    char val;
    std::cin >> val;
    OCTET *ImgIn, *ImgOut;

    int w, h;
    // lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    lire_nb_lignes_colonnes_image_pgm((char *)argv[1], &w, &h);
    int ntaille = w * h;
    allocation_tableau(ImgIn, OCTET, ntaille * 3);
    // lire_image_pgm((char *)argv[1], ImgIn, ntaille);
    lire_image_pgm((char *)argv[1], ImgIn, ntaille);
    switch (val)
    {
    case '1':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q1(ImgIn, ntaille);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);
        break;
    case '2':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q2(ImgIn, h, w);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);

        break;
    case '3':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q3(ImgIn, h, w);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);

        break;
    case '4':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q4(ImgIn, h, w);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);

        break;
    case '5':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q5(ImgIn, h, w);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);

        break;
    case '6':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q6(ImgIn, h, w);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);
        break;

    case '7':
        if (argc != 3)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre et le nom de l'image destination\n";
            return 1;
        }
        ImgOut = Q7(ImgIn, h, w);
        ecrire_image_pgm((char *)argv[2], ImgOut, w, h);
        break;

    case '8':
        if (argc != 2)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre\n";
            return 1;
        }
        ImgOut = QBonusGrey(ImgIn, h, w);
        break;

    case '9':
        if (argc != 2)
        {
            std::cout << "vous devez donner le nom de l'image source en paramètre\n";
            return 1;
        }

        ImgOut = QBonusColor(ImgIn, h, w);
        break;

    default:
        break;
    }

    return 0;
}
