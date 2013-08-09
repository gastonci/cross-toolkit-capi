//#include <winH_win32.c>
#include <map_writter.c>

//int tonchy()
int main()
{
    /*mkWin(800,600,"pokeCard game");
    go();*/
    mapWritter x = MAPWRITTER;
    int y = '0';
    char z[17];
    char URL1[]="imgTesta\0",URL2[]=".bmp\0";
    openMW(&x,"test.map\0");
    writteMap(&x);
    cleanMap(&x);
    addImage(&x,"hola\0");
    while(y <= '9')
    {
        strcpy(z,URL1);
        z[8] = y;
        z[9] = '\0';
        strcat(z,URL2);
        addImage(&x,z);
        y++;
    }
    closeMap(&x);
    return 0;
}
