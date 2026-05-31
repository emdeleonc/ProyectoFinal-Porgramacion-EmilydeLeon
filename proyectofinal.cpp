#include "interfaz.h"
#include <iostream>
using namespace std;

int main() {
    cout.sync_with_stdio(false);
    mostrarEncabezado("Menu principal");
    cout<<"\n\n";
    cout<<"Desarrollado por: Emily de Leon\n";
    cout<<"Carnet: 9941-25-13659\n\n";
    pausarSistema();
    menuPrincipal();

    return 0;
}
