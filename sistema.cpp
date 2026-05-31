#include "sistema.h"
#include "interfaz.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <stdexcept>  
using namespace std;

int leerEntero(const char* mensaje, int min, int max) {
    int valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            if (valor >= min && valor <= max) {
                cin.ignore(1000, '\n');
                return valor;
            }
        }
        cout << "Entrada fuera del rango\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

float leerFloat(const char* mensaje, float min) {
    float valor;
    while (true) {
        cout << mensaje;
        if (cin >> valor) {
            if (valor >= min) {
                cin.ignore(1000, '\n');
                return valor;
            }
        }
        cout << "Entrada invalida.\n";
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

bool existeCodigo(int codigoBuscado) {
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) return false;

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.codigo == codigoBuscado && producto.activo) {
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

void registrarProducto() {
    mostrarEncabezado("Registro de Producto");
    try {
        Producto producto;
        ofstream archivo("productos.dat", ios::binary | ios::app);
        if (!archivo) throw runtime_error("Error con el archivo de productos");

        do {
            producto.codigo = leerEntero("Ingrese Codigo del Producto: ", 1, 99999);
            if (existeCodigo(producto.codigo)) {
                cout << "El codigo ya esta registrado\n";
                producto.codigo = -1;
            }
        } while (producto.codigo <= 0);

        cout << "Ingrese Nombre del Producto: ";
        cin.getline(producto.nombre, 50);

        producto.precio = leerFloat("Ingrese Precio Unitario Q: ", 0.01);
        producto.stock = leerEntero("Ingrese Stock inicial: ", 0, 10000);
        producto.vendidos = 0;
        producto.activo = true;

        archivo.write((char*)&producto, sizeof(producto));
        archivo.close();
        cout << "\nProducto guardado correctamente\n";
    }
    catch (const exception& e) {
        cout << "\nHay una excepcion: " << e.what() << endl;
    }
    pausarSistema();
}

void listarProductos() {
    mostrarEncabezado("Productos");
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);

    if (!archivo) {
        cout << "\nNo se muestran productos\n";
        pausarSistema();
        return;
    }

    cout << left << setw(10) << "Codigo" 
         << setw(30) << "Nombre del producto" 
         << setw(12) << "Precio" 
         << setw(10) << "Stock" 
         << setw(10) << "Vendidos" << endl;
    mostrarLineaSimple();

    int totalActivos = 0;
    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.activo) {
            cout << left << setw(10) << producto.codigo 
                 << setw(30) << producto.nombre 
                 << "Q" << setw(11) << producto.precio 
                 << setw(10) << producto.stock 
                 << setw(10) << producto.vendidos << endl;
            totalActivos++;
        }
    }
    archivo.close();
    
    if(totalActivos == 0) cout << "\nNo hay productos activos\n";
    mostrarLineaSimple();
    pausarSistema();
}

void buscarProductoCodigo() {
    mostrarEncabezado("Buscar por codigo");
    Producto producto;
    int codigoBuscado = leerEntero("Ingrese el codigo: ", 1, 99999);
    bool encontrado = false;

    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) { cout << "\nError al abrir el archivo\n"; pausarSistema(); return; }

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.codigo == codigoBuscado && producto.activo) {
            mostrarLineaSimple();
            cout << " -> Codigo:   " << producto.codigo << endl;
            cout << " -> Nombre:   " << producto.nombre << endl;
            cout << " -> Precio:   Q" << producto.precio << endl;
            cout << " -> Stock:    " << producto.stock << " unidades" << endl;
            cout << " -> Ventas:   " << producto.vendidos << " unidades" << endl;
            mostrarLineaSimple();
            encontrado = true;
            break;
        }
    }
    if (!encontrado) cout << "\nNo hay producto con este codigo\n";
    archivo.close();
    pausarSistema();
}

void buscarProductoNombre() {
    mostrarEncabezado("Buscar por nombre");
    char nombreBuscado[50];
    cout << "Ingrese el nombre del producto a buscar: ";
    cin.getline(nombreBuscado, 50);
    
    Producto producto;
    bool encontrado = false;
    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) { cout << "\nArchivo no disponible\n"; pausarSistema(); return; }

    cout << "\nCoincidencias encontradas:\n";
    mostrarLineaSimple();

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (strstr(producto.nombre, nombreBuscado) != NULL && producto.activo) {
            cout << "[" << producto.codigo << "] " << producto.nombre << " | Q" << producto.precio << " | Stock: " << producto.stock << endl;
            encontrado = true;
        }
    }
    mostrarLineaSimple();
    if (!encontrado) cout << "No se encontro producto\n";
    archivo.close();
    pausarSistema();
}

void modificarPrecio() {
    mostrarEncabezado("Modificar Precio");
    Producto producto;
    int codigoBuscado = leerEntero("Codigo del producto a modificar: ", 1, 99999);
    bool encontrado = false;

    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
    if (!archivo) { cout << "\nError al abir el archivo\n"; pausarSistema(); return; }

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.codigo == codigoBuscado && producto.activo) {
            cout << "\nProducto Seleccionado: " << producto.nombre << " (Precio actual: Q" << producto.precio << ")\n";
            producto.precio = leerFloat("Ingrese el nuevo precio (Q): ", 0.01);
            
            archivo.seekp(-static_cast<streamoff>(sizeof(Producto)), ios::cur);
            archivo.write((char*)&producto, sizeof(producto));
            encontrado = true;
            cout << "\nPrecio actualizado correctamente\n";
            break;
        }
    }
    if (!encontrado) cout << "\nNo se encontro codigo\n";
    archivo.close();
    pausarSistema();
}

void actualizarStock() {
    mostrarEncabezado("Actualizar Stock");
    Producto producto;
    int codigoBuscado = leerEntero("Codigo del producto: ", 1, 99999);
    bool encontrado = false;

    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
    if (!archivo) { cout << "\nError al abir el archivo\n"; pausarSistema(); return; }

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.codigo == codigoBuscado && producto.activo) {
            cout << "\nProducto Seleccionado: " << producto.nombre << " (Stock actual: " << producto.stock << ")\n";
            producto.stock = leerEntero("Ingrese la nueva cantidad: ", 0, 50000);
            
            archivo.seekp(-static_cast<streamoff>(sizeof(Producto)), ios::cur);
            archivo.write((char*)&producto, sizeof(producto));
            encontrado = true;
            cout << "\nSe actualizo stock correctamente\n";
            break;
        }
    }
    if (!encontrado) cout << "\nProducto no encontrado\n";
    archivo.close();
    pausarSistema();
}

void desactivarProducto() {
    mostrarEncabezado("Desactivar Producto");
    Producto producto;
    int codigoBuscado = leerEntero("Codigo del producto a dar de baja: ", 1, 99999);
    bool encontrado = false;

    fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
    if (!archivo) { cout << "\nError en el archivo\n"; pausarSistema(); return; }

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.codigo == codigoBuscado && producto.activo) {
            cout << "\n¿Deseas desactivar el procto: " << producto.nombre << "?\n";
            int confirmo = leerEntero("1 = Si, 2 = No: ", 1, 2);
            if (confirmo == 1) {
                producto.activo = false;
                archivo.seekp(-static_cast<streamoff>(sizeof(Producto)), ios::cur);
                archivo.write((char*)&producto, sizeof(producto));
                cout << "\nProducto desactivado correctamente\n";
            } else {
                cout << "\nSe cancelo desactivacion\n";
            }
            encontrado = true;
            break;
        }
    }
    if (!encontrado) cout<<"\nNo se encontro codigo\n";
    archivo.close();
    pausarSistema();
}

void procesoVenta() {
    mostrarEncabezado("Proceso de Nueva Venta");
    Venta venta;
    Producto producto;
    int codigoBuscado, cantidad, opcion, contador = 0, totalArticulos = 0;

    venta.subtotal = 0; venta.iva = 0; venta.descuento = 0; venta.total = 0;
    venta.numeroVenta = leerEntero("Ingrese Numero de Factura: ", 1, 999999);
    venta.dia = leerEntero("Dia (1-31): ", 1, 31);
    venta.mes = leerEntero("Mes (1-12): ", 1, 12);

    do {
        if (contador >= 20) {
            cout << "\nSe alcanzó numero de ventas posibles\n";
            break;
        }

        bool encontrado = false;
        fstream archivo("productos.dat", ios::binary | ios::in | ios::out);
        if(!archivo) throw runtime_error("Fallo en registros");

        codigoBuscado = leerEntero("\nIngrese Codigo de Producto: ", 1, 99999);

        while (archivo.read((char*)&producto, sizeof(producto))) {
            if (producto.codigo == codigoBuscado && producto.activo) {
                encontrado = true;
                cout << " -> Venta: " << producto.nombre << " | Precio: Q" << producto.precio << " | Disponibles: " << producto.stock << endl;

                if (producto.stock == 0) {
                    cout << "No hay stock suficiente\n";
                    break;
                }

                do {
                    cantidad = leerEntero("Cantidad Solicitada: ", 1, 1000);
                    if (cantidad > producto.stock) {
                        cout << "Stock Insuficiente";
                    }
                } while (cantidad <= 0 || cantidad > producto.stock);

                venta.detalles[contador].codigoProducto = producto.codigo;
                strcpy(venta.detalles[contador].nombre, producto.nombre);
                venta.detalles[contador].cantidad = cantidad;
                venta.detalles[contador].precio = producto.precio;
                venta.detalles[contador].subtotal = producto.precio * cantidad;
                
                venta.subtotal += venta.detalles[contador].subtotal;
                totalArticulos += cantidad;
                producto.stock -= cantidad;
                producto.vendidos += cantidad;
                
                archivo.seekp(-static_cast<streamoff>(sizeof(Producto)), ios::cur);
                archivo.write((char*)&producto, sizeof(producto));
                contador++;
                cout << "Se agrego producto a la venta\n";
                break;
            }
        }
        archivo.close();
        if (!encontrado) cout << "No se encontro el codigo\n";
        
        cout << "\n1. Agregar otro item a la factura\n2. Finalizar Compra\n";
        opcion = leerEntero("Seleccione: ", 1, 2);

    } while (opcion == 1);

    if (contador == 0) {
        cout << "\nNo se selecciono producto\n";
        pausarSistema();
        return;
    }
    venta.cantidadProductos = contador;
    venta.iva = venta.subtotal * 0.12;

    if (venta.subtotal > 500.0) {
        venta.descuento = venta.subtotal * 0.05;
    }
    if (totalArticulos % 5 == 0) {
        venta.descuento += 25.0;
    }
    venta.total = venta.subtotal + venta.iva - venta.descuento;
    venta.total = round(venta.total * 100.0) / 100.0;

    mostrarEncabezado("Factura de Venta");
    cout << "Factura No: " << venta.numeroVenta << "   Fecha: " << venta.dia << "/" << venta.mes << "/2026" << endl;
    mostrarLineaSimple();
    cout << left << setw(25) << "Descripcion" << setw(10) << "Cant." << setw(12) << "Precio U." << setw(12) << "Subtotal" << endl;
    mostrarLineaSimple();
    for (int i = 0; i < contador; i++) {
        cout << left << setw(25) << venta.detalles[i].nombre 
             << setw(10) << venta.detalles[i].cantidad 
             << "Q" << setw(11) << venta.detalles[i].precio 
             << "Q" << setw(11) << venta.detalles[i].subtotal << endl;
    }
    mostrarLineaDoble();
    cout << right << setw(50) << "Subtotal: " << "Q" << venta.subtotal << endl;
    cout << right << setw(50) << "IVA 12%: " << "Q" << venta.iva << endl;
    cout << right << setw(50) << "Descuentps: " << "Q" << venta.descuento << endl;
    cout << right << setw(50) << "Total a Pagar: " << "Q" << venta.total << endl;
    mostrarLineaDoble();

    ofstream archivoVentas("ventas.dat", ios::binary | ios::app);
    archivoVentas.write((char*)&venta, sizeof(venta));
    archivoVentas.close();
    
    cout << "\nVenta guardada correctamente\n";
    pausarSistema();
}

void productosMenorStock() {
    mostrarEncabezado("Productos con Menor Stock)");
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) { cout << "\nNo hay registros\n"; pausarSistema(); return; }

    cout << left << setw(15) << "Codigo" << setw(40) << "Nombre del Producto" << setw(15) << "Stock Actual" << endl;
    mostrarLineaSimple();
    int alertas = 0;
    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.stock <= 5 && producto.activo) {
            cout << left << setw(15) << producto.codigo << setw(40) << producto.nombre << setw(15) << producto.stock<< endl;
            alertas++;
        }
    }
    archivo.close();
    if (alertas == 0) cout << "\nProductos con buen stock\n";
    mostrarLineaSimple();
    pausarSistema();
}

void productosMasVendidos() {
    mostrarEncabezado("Productos Mas Vendidos");
    Producto productos[100];
    Producto aux;
    int contador = 0;
    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) { cout << "\nNo se encontraron datos\n"; pausarSistema(); return; }

    while (archivo.read((char*)&productos[contador], sizeof(Producto)) && contador < 100) {
        contador++;
    }
    archivo.close();

    for (int i = 0; i < contador - 1; i++) {
        for (int j = 0; j < contador - i - 1; j++) {
            if (productos[j].vendidos < productos[j + 1].vendidos) {
                aux = productos[j];
                productos[j] = productos[j + 1];
                productos[j + 1] = aux;
            }
        }
    }

    cout << left << setw(10) << "Lugar" << setw(40) << "Nombre" << setw(15) << "Unidades Vendidas" << endl;
    mostrarLineaSimple();
    for (int i = 0; i < contador; i++) {
        if (productos[i].activo) {
            cout << left << "No. " << setw(6) << i + 1 << setw(40) << productos[i].nombre << setw(15) << productos[i].vendidos << endl;
        }
    }
    pausarSistema();
}

void menuOrdenamientoProductos() {
    mostrarEncabezado("Ordenar Inventario");
    cout << "  1. Ordenar por precio de forma ascendente\n";
    cout << "  2. Ordenar por precio de forma descendente\n";
    cout << "  3. Ordenar por stock de forma ascendente\n";
    cout << "  4. Ordenar por stock de forma descendente\n";
    mostrarLineaSimple();
    int op = leerEntero("Selecciona: ", 1, 4);
    
    switch (op) {
        case 1: ordenarProductos(1, true); break;
        case 2: ordenarProductos(1, false); break;
        case 3: ordenarProductos(2, true); break;
        case 4: ordenarProductos(2, false); break;
    }
}

void ordenarProductos(int criterio, bool ascendente) {
    Producto productos[200];
    int contador = 0;
    ifstream archivo("productos.dat", ios::binary);
    while (archivo.read((char*)&productos[contador], sizeof(Producto)) && contador < 200) {
        contador++;
    }
    archivo.close();

    for (int i = 0; i < contador - 1; i++) {
        int indiceObjetivo = i;
        for (int j = i + 1; j < contador; j++) {
            bool condicion = false;
            if (criterio == 1) { // Por Precio
                condicion = ascendente ? (productos[j].precio < productos[indiceObjetivo].precio) : (productos[j].precio > productos[indiceObjetivo].precio);
            } else { // Por Stock
                condicion = ascendente ? (productos[j].stock < productos[indiceObjetivo].stock) : (productos[j].stock > productos[indiceObjetivo].stock);
            }
            if (condicion) indiceObjetivo = j;
        }
        Producto temp = productos[i];
        productos[i] = productos[indiceObjetivo];
        productos[indiceObjetivo] = temp;
    }

    mostrarEncabezado("Productos Ordenados");
    cout << left << setw(10) << "Codigo" << setw(35) << "Nombre" << setw(15) << "Precio" << setw(10) << "Stock" << endl;
    mostrarLineaSimple();
    for(int i=0; i<contador; i++) {
        if(productos[i].activo) {
            cout << left << setw(10) << productos[i].codigo << setw(35) << productos[i].nombre << "Q" << setw(14) << productos[i].precio << setw(10) << productos[i].stock << endl;
        }
    }
    pausarSistema();
}

void ventasTotalesDia() {
    mostrarEncabezado("Totales por Dia");
    Venta venta;
    int diaBuscado = leerEntero("Ingrese el dia de consulta: ", 1, 31);
    int mesBuscado = leerEntero("Ingrese el mes de consulta: ", 1, 12);
    float totalDia = 0;
    int facturasEmitidas = 0;

    ifstream archivo("ventas.dat", ios::binary);
    if (!archivo) { cout << "\nNo hay ventas registradas\n"; pausarSistema(); return; }

    while (archivo.read((char*)&venta, sizeof(venta))) {
        if (venta.dia == diaBuscado && venta.mes == mesBuscado) {
            totalDia += venta.total;
            facturasEmitidas++;
        }
    }
    archivo.close();
    
    cout << "\n========================================\n";
    cout << "  Facturas procesadas: " << facturasEmitidas << endl;
    cout << "  Ingresos totales diarios: Q" << totalDia << endl;
    cout << "========================================\n";
    pausarSistema();
}

void ventasPorMes() {
    mostrarEncabezado("Ventar Anuales");
    Venta venta;
    float matrizVentas[12][31] = {0};
    ifstream archivo("ventas.dat", ios::binary);

    if (!archivo) { cout << "\nNo hay registros\n"; pausarSistema(); return; }

    while (archivo.read((char*)&venta, sizeof(venta))) {
        if (venta.mes >= 1 && venta.mes <= 12 && venta.dia >= 1 && venta.dia <= 31) {
            matrizVentas[venta.mes - 1][venta.dia - 1] += venta.total;
        }
    }
    archivo.close();

    const char* meses[] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
    
    cout << left << setw(18) << "Mes" << "Venta mensual acumulada" << endl;
    mostrarLineaSimple();
    
    float granTotalAnual = 0;
    for (int i = 0; i < 12; i++) {
        float sumaMes = 0;
        for (int j = 0; j < 31; j++) {
            sumaMes += matrizVentas[i][j];
        }
        cout << left << setw(18) << meses[i] << "Q" << sumaMes << endl;
        granTotalAnual += sumaMes;
    }
    mostrarLineaDoble();
    cout << "Venta Anual en tienda: Q" << granTotalAnual << endl;
    mostrarLineaDoble();
    pausarSistema();
}

void exportarReporte() {
    mostrarEncabezado("Exportar Reporte a txt");
    Producto producto;
    ifstream archivo("productos.dat", ios::binary);
    ofstream reporte("reporte.txt");

    if (!archivo || !reporte) { cout << "\n[?] Error grave al crear el reporte en disco duro.\n"; pausarSistema(); return; }

    reporte << "========================================================================\n";
    reporte << "                           Reporte de Inventario                         \n";
    reporte << "========================================================================\n\n";
    reporte << left << setw(10) << "Codigo" << setw(30) << "Producto" << setw(12) << "Precio" << setw(10) << "Stock" << setw(10) << "Vendidos" << "\n";
    reporte << "------------------------------------------------------------------------\n";

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.activo) {
            reporte << left << setw(10) << producto.codigo 
                    << setw(30) << producto.nombre 
                    << "Q" << setw(11) << producto.precio 
                    << setw(10) << producto.stock 
                    << setw(10) << producto.vendidos << "\n";
        }
    }
    archivo.close();
    reporte.close();

    cout << "\nSe genero reporte txt\n";
    pausarSistema();
}

void reiniciarInventario() {
    mostrarEncabezado("Reiniciar Inventario");
    cout << "¿Deseas formatear el inventario? (s/n): ";
    char opcion;
    cin >> opcion;
    cin.ignore(1000, '\n');

    if (opcion == 's' || opcion == 'S') {
        ofstream archivo("productos.dat", ios::binary | ios::trunc);
        archivo.close();
        cout << "\nSe reinicio el inventario\n";
    } else {
        cout << "\nSe cancelo la accion\n";
    }
    pausarSistema();
}

void estadisticasGenerales() {
    mostrarEncabezado("Estadisticas Generales");
    Producto producto;
    int totalProductos = 0;
    int stockTotal = 0;
    int vendidosTotal = 0;
    float valorInventario = 0;

    ifstream archivo("productos.dat", ios::binary);
    if (!archivo) { cout << "\nNo se encontraron datos\n"; pausarSistema(); return; }

    while (archivo.read((char*)&producto, sizeof(producto))) {
        if (producto.activo) {
            totalProductos++;
            stockTotal += producto.stock;
            vendidosTotal += producto.vendidos;
            valorInventario += (producto.stock * producto.precio);
        }
    }
    archivo.close();

    cout << "  +--------------------------------------------------------+\n";
    cout << "    -Cantidad de productos diferentes activos: " << totalProductos << " tipos\n";
    cout << "    -Total de stock en tienda:        " << stockTotal << " unidades\n";
    cout << "    -Cantidad de ventas totales:     " << vendidosTotal << " unidades\n";
    cout << "    -Total monetario en inventario: Q" << valorInventario << "\n";
    cout << "  +--------------------------------------------------------+\n";
    pausarSistema();
}
