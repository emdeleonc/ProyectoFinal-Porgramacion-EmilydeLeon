#include "interfaz.h"
#include "sistema.h"
#include <iostream>
#include <iomanip>
using namespace std;

void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void mostrarLineaDoble() {
    cout << "========================================================================" << endl;
}

void mostrarLineaSimple() {
    cout << "------------------------------------------------------------------------" << endl;
}

void mostrarEncabezado(string titulo) {
    limpiarPantalla();
    mostrarLineaDoble();
    cout << fixed << setprecision(2);
    cout << "         Sistema de Ventas e Inventarios           " << endl;
    cout << "         Modulo: " << titulo << endl;
    mostrarLineaDoble();
}

void pausarSistema() {
    cout << "\nPresiona Enter para continuar...";
    cin.clear();
    cin.get();
}

void menuPrincipal() {
    int opcion;
    do {
        mostrarEncabezado("Menu Principal");
        cout << "  1. Gestion de Productos\n";
        cout << "  2. Proceso de Venta\n";
        cout << "  3. Reportes y Consultas\n";
        cout << "  4. Utilidades del Sistema\n";
        cout << "  5. Salir\n";
        mostrarLineaSimple();
        
        opcion = leerEntero("Seleccione una opcion: ", 1, 5);
        
        switch(opcion) {
            case 1: menuProductos(); break;
            case 2: try { procesoVenta(); } catch(...) { cout << "\nHay un error en el proceso de venta\n"; pausarSistema(); } break;
            case 3: menuReportes(); break;
            case 4: menuUtilidades(); break;
            case 5: 
                mostrarEncabezado("Salir");
                cout << "\nGracias por usar c:\n\n"; 
                break;
        }
    } while(opcion != 5);
}

void menuProductos() {
    int opcion;
    do {
        mostrarEncabezado("Gestion de productos");
        cout << "  1. Registrar Nuevo Producto\n";
        cout << "  2. Listado de Productos\n";
        cout << "  3. Buscar Producto por Codigo\n";
        cout << "  4. Buscar Producto por Nombre\n";
        cout << "  5. Modificar Precio de un Producto\n";
        cout << "  6. Actualizar Stock\n";
        cout << "  7. Desactivar Producto\n";
        cout << "  8. Regresar\n";
        mostrarLineaSimple();
        opcion = leerEntero("Seleccione una opcion: ", 1, 8);
        
        switch(opcion) {
            case 1: registrarProducto(); break;
            case 2: listarProductos(); break;
            case 3: buscarProductoCodigo(); break;
            case 4: buscarProductoNombre(); break;
            case 5: modificarPrecio(); break;
            case 6: actualizarStock(); break;
            case 7: desactivarProducto(); break;
        }
    } while(opcion != 8);
}

void menuReportes() {
    int opcion;
    do {
        mostrarEncabezado("Reportes de inventario");
        cout << "  1. Productos con Menor Stock\n";
        cout << "  2. Productos Mas Vendidos\n";
        cout << "  3. Ventas Totales por Dia\n";
        cout << "  4. Ventas por Mes\n";
        cout << "  5. Ordenar Inventario\n";
        cout << "  6. Regresar\n";
        mostrarLineaSimple();
        opcion = leerEntero("Seleccione una opcion:", 1, 6);
        
        switch(opcion) {
            case 1: productosMenorStock(); break;
            case 2: productosMasVendidos(); break;
            case 3: ventasTotalesDia(); break;
            case 4: ventasPorMes(); break;
            case 5: menuOrdenamientoProductos(); break;
        }
    } while(opcion != 6);
}

void menuUtilidades() {
    int opcion;
    do {
        mostrarEncabezado("Utilidades del Sistema");
        cout << "  1. Exportar Reporte de Inventario a .txt\n";
        cout << "  2. Reiniciar Inventario\n";
        cout << "  3. Estadisticas Generales\n";
        cout << "  4. Regresar\n";
        mostrarLineaSimple();
        opcion = leerEntero("Seleccione una opcion", 1, 4);
        
        switch(opcion) {
            case 1: exportarReporte(); break;
            case 2: reiniciarInventario(); break;
            case 3: estadisticasGenerales(); break;
        }
    } while(opcion != 4);
}
