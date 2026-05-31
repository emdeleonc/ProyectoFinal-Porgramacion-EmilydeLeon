#ifndef SISTEMA_H
#define SISTEMA_H

#include "estructuras.h"

bool existeCodigo(int codigoBuscado);
int leerEntero(const char* mensaje, int min = -999999, int max = 999999);
float leerFloat(const char* mensaje, float min = 0.0);

void registrarProducto();
void listarProductos();
void buscarProductoCodigo();
void buscarProductoNombre();
void modificarPrecio();
void actualizarStock();
void desactivarProducto();
void procesoVenta();
void productosMenorStock();
void productosMasVendidos();
void ventasTotalesDia();
void ventasPorMes();
void menuOrdenamientoProductos();
void ordenarProductos(int criterio, bool ascendente);
void exportarReporte();
void reiniciarInventario();
void estadisticasGenerales();

#endif
