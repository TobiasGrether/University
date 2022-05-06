#include <stdio.h>

int main()
{
    double k = (double)9 / 5; /* Gleitkommakonstante zum Umrechnen */

    printf("Temperatur in Grad Celsius: \n");

    for (int i = 1; i <= 20; i++)
    {
        double fahrenheit = k * i + 32;

        if (i < 10)
        {
            printf(" %i | %6.2lf\n", i, fahrenheit);
        }else{
            printf("%i | %6.2lf\n", i, fahrenheit);
        }
    }
}
