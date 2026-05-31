#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

struct Producto {
    int codigo;
    char nombre[50];
    float precio;
    int stock;
    int vendidos;
    bool activo;
};

struct DetalleVenta {
    int codigoProducto;
    char nombre[50];
    int cantidad;
    float precio;
    float subtotal;
};

struct Venta {
    int numeroVenta;
    int cantidadProductos;
    DetalleVenta detalles[20];
    float subtotal;
    float iva;
    float descuento;
    float total;
    int dia;
    int mes;
};

#endif
