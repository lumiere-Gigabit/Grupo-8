#include <iostream>
#include <fstream>
#include <string>

using namespace std;
struct NodoProceso {
    int id;
    string nombre;
    int prioridad;
    string estado;
    NodoProceso* sig;

    NodoProceso(int _id, string _nombre, int _prio, string _estado = "LISTO") {
        id = _id;
        nombre = _nombre;
        prioridad = _prio;
        estado = _estado;
        sig = NULL;
    }
};
struct NodoCola {
    NodoProceso* proceso;
    NodoCola* sig;

    NodoCola(NodoProceso* _proceso) {
        proceso = _proceso;
        sig = NULL;
    }
};
struct NodoPila {
    int idBloque;
    int idProceso;
    NodoPila* sig;

    NodoPila(int _idB, int _idP, NodoPila* _sig = NULL) {
        idBloque = _idB;
        idProceso = _idP;
        sig = _sig;
    }
};
NodoProceso* lista = NULL;
NodoCola* cola = NULL;
NodoPila* pila = NULL;
bool existeIDProceso(int id) {
    NodoProceso* aux = lista;
    while (aux != NULL) 
	{ 
        if (aux->id == id) return true;
        aux = aux->sig;
    }
    return false;
}
bool existeIDBloque(int idBloque) {
    NodoPila* aux = pila;
    while (aux != NULL) { 
        if (aux->idBloque == idBloque) return true;
        aux = aux->sig;
    }
    return false;
}
void desencolarPorID(int id) {
    if (cola == NULL) return;
    if (cola->proceso->id == id) {
        NodoCola* temp = cola;
        cola = cola->sig;
        delete temp;
        return;
    }
    NodoCola* aux = cola;
    while (aux->sig != NULL && aux->sig->proceso->id != id) {
        aux = aux->sig;
    }
    if (aux->sig != NULL) {
        NodoCola* temp = aux->sig;
        aux->sig = temp->sig;
        delete temp;
    }
}
void liberarMemoriaPorProceso(int idProceso) {
    NodoPila* act = pila;
    NodoPila* prev = NULL;
    while (act != NULL) {
        if (act->idProceso == idProceso) {
            NodoPila* temp = act;
            if (prev == NULL) {
                pila = act->sig;
                act = pila;
            } else {
                prev->sig = act->sig;
                act = act->sig;
            }
            delete temp;
        } else {
            prev = act;
            act = act->sig;
        }
    }
}
void registrarProceso() {
    int id, prio;
    string nombre;

    cout << "Ingrese ID del proceso (>0): "; cin >> id;
    cout << "Ingrese Nombre del proceso: "; cin >> nombre;
    cout << "Ingrese Prioridad (1 = Mayor urgencia): "; cin >> prio;
    if (id <= 0 || nombre.empty() || prio < 1) {
        cout << " [ERROR] Entrada invalida. ID > 0, Nombre no vacio y Prioridad >= 1.\n";
        return;
    }
    if (existeIDProceso(id)) {
        cout << " [ERROR] El ID " << id << " ya existe en la lista.\n";
        return;
    }
    NodoProceso* nuevoP = new NodoProceso(id, nombre, prio);
    nuevoP->sig = lista;
    lista = nuevoP;
    NodoCola* nuevoC = new NodoCola(nuevoP);
    if (cola == NULL || prio < cola->proceso->prioridad) {
        nuevoC->sig = cola;
        cola = nuevoC;
    } else {
        NodoCola* aux = cola;
        while (aux->sig != NULL && aux->sig->proceso->prioridad <= prio) {
            aux = aux->sig;
        }
        nuevoC->sig = aux->sig;
        aux->sig = nuevoC;
    }
    cout << " [EXITO] Proceso registrado y encolado correctamente.\n";
}
void buscarOModificar() {
    int subOp, id;
    cout << " 1. Buscar Proceso por ID\n";
    cout << " 2. Modificar Prioridad de Proceso (LISTO)\n";
    cout << " Seleccione: "; cin >> subOp;

    cout << "Ingrese ID del proceso: "; cin >> id;
    NodoProceso* aux = lista;
    while (aux != NULL) {
        if (aux->id == id) {
            if (subOp == 1) {
                cout << " -> ENCONTRADO: ID " << aux->id << " | Nombre: " << aux->nombre 
                     << " | Prioridad: " << aux->prioridad << " | Estado: " << aux->estado << "\n";
            } else if (subOp == 2) {
                if (aux->estado != "LISTO") {
                    cout << " [ERROR] Solo se modifica la prioridad si esta LISTO.\n";
                    return;
                }
                int nuevaP;
                cout << "Nueva Prioridad (>=1): "; cin >> nuevaP;
                if (nuevaP < 1) { cout << " [ERROR] Prioridad invalida.\n"; return; }
                aux->prioridad = nuevaP;
                desencolarPorID(id);
                NodoCola* nuevoC = new NodoCola(aux);
                if (cola == NULL || nuevaP < cola->proceso->prioridad) {
                    nuevoC->sig = cola;
                    cola = nuevoC;
                } else {
                    NodoCola* cAux = cola;
                    while (cAux->sig != NULL && cAux->sig->proceso->prioridad <= nuevaP) {
                        cAux = cAux->sig;
                    }
                    nuevoC->sig = cAux->sig;
                    cAux->sig = nuevoC;
                }
                cout << " [EXITO] Prioridad actualizada y Cola reordenada.\n";
            }
            return;
        }
        aux = aux->sig;
    }
    cout << " [ERROR] Proceso no encontrado.\n";
}
void eliminarProceso() {
    int id;
    cout << "Ingrese ID del proceso a eliminar: "; cin >> id;
    if (lista == NULL) { cout << " [ERROR] Lista vacia.\n"; return; }

    NodoProceso* act = lista;
    NodoProceso* prev = NULL;

    while (act != NULL && act->id != id) {
        prev = act;
        act = act->sig;
    }

    if (act == NULL) {
        cout << " [ERROR] Proceso no encontrado.\n";
        return;
    }

    if (prev == NULL) lista = act->sig;
    else prev->sig = act->sig;

    desencolarPorID(id);
    liberarMemoriaPorProceso(id);

    delete act;
    cout << " [EXITO] Proceso, sus referencias de CPU y bloques de memoria fueron eliminados.\n";
}
void ejecutarProceso() {
    if (cola == NULL) {
        cout << " [INFO] No hay procesos listos para ejecutar.\n";
        return;
    }

    NodoCola* temp = cola;
    cola = cola->sig; 

    temp->proceso->estado = "EJECUTADO";
    cout << " [EJECUCION] Proceso " << temp->proceso->nombre 
         << " (ID: " << temp->proceso->id << ") cambiado a EJECUTADO.\n";
    delete temp;
}
void asignarMemoria() {
    int idB, idP;
    cout << "Ingrese ID del Bloque de Memoria: "; cin >> idB;
    cout << "Ingrese ID del Proceso Asociado: "; cin >> idP;

    if (idB <= 0) { cout << " [ERROR] ID de bloque debe ser positivo.\n"; return; }
    if (!existeIDProceso(idP)) { cout << " [ERROR] El proceso ID " << idP << " no existe.\n"; return; }
    if (existeIDBloque(idB)) { cout << " [ERROR] El Bloque ID " << idB << " ya esta en uso.\n"; return; }

    pila = new NodoPila(idB, idP, pila); 
    cout << " [EXITO] Bloque " << idB << " asignado en la Cima de la Pila (LIFO).\n";
}
void liberarMemoria() {
    if (pila == NULL) {
        cout << " [INFO] La Pila de Memoria esta vacia.\n";
        return;
    }

    NodoPila* temp = pila;
    pila = pila->sig; // Pop

    cout << " [LIBERACION] Liberado Bloque ID: " << temp->idBloque 
         << " de la cima de la pila (Proceso ID: " << temp->idProceso << ").\n";
    delete temp;
}
void mostrarEstructuras() {
    cout << "\n================ 1. LISTA GENERAL DE PROCESOS ================\n";
    NodoProceso* p = lista;
    if (p == NULL) cout << " (Lista vacia)\n";
    while (p != NULL) {
        cout << " ID: " << p->id << " | Nombre: " << p->nombre 
             << " | Prioridad: " << p->prioridad << " | Estado: " << p->estado << "\n";
        p = p->sig;
    }

    cout << "\n================ 2. COLA DE CPU (ORDEN DE PRIORIDAD) ================\n";
    NodoCola* c = cola;
    if (c == NULL) cout << " (Cola vacia)\n";
    else cout << "FRENTE -> ";
    while (c != NULL) {
        cout << "[ID: " << c->proceso->id << " | Prio: " << c->proceso->prioridad << "] -> ";
        c = c->sig;
    }
    if (cola != NULL) cout << "FIN\n";

    cout << "\n================ 3. PILA DE MEMORIA (REGIMEN LIFO) ================\n";
    NodoPila* m = pila;
    if (m == NULL) cout << " (Pila vacia)\n";
    else cout << "CIMA\n  |\n  v\n";
    while (m != NULL) {
        cout << "  [Bloque ID: " << m->idBloque << " | Proceso ID: " << m->idProceso << "]\n";
        m = m->sig;
    }
}
void persistencia() {
    int subOp;
    cout << " 1. Guardar estado en procesos.txt\n";
    cout << " 2. Cargar estado desde procesos.txt\n";
    cout << " Seleccione: "; cin >> subOp;

    if (subOp == 1) {
        ofstream arch("procesos.txt");
        if (!arch.is_open()) { cout << " [ERROR] No se pudo escribir el archivo.\n"; return; }

        NodoProceso* p = lista;
        arch << "#PROCESOS\n";
        while (p != NULL) {
            arch << p->id << " " << p->nombre << " " << p->prioridad << " " << p->estado << "\n";
            p = p->sig;
        }

        NodoPila* m = pila;
        arch << "#MEMORIA\n";
        while (m != NULL) {
            arch << m->idBloque << " " << m->idProceso << "\n";
            m = m->sig;
        }
        arch.close();
        cout << " [EXITO] Datos guardados en procesos.txt.\n";
    } 
    else if (subOp == 2) {
        ifstream arch("procesos.txt");
        if (!arch.is_open()) { cout << " [INFO] No existe archivo previo.\n"; return; }

        string etiqueta;
        int id, prio, idB, idP;
        string nombre, estado;

        arch >> etiqueta; 
        while (arch >> id >> nombre >> prio >> estado) {
            if (id == 0 && nombre == "#MEMORIA") break;
            NodoProceso* nuevoP = new NodoProceso(id, nombre, prio, estado);
            nuevoP->sig = lista;
            lista = nuevoP;

            if (estado == "LISTO") {
                NodoCola* nuevoC = new NodoCola(nuevoP);
                if (cola == NULL || prio < cola->proceso->prioridad) {
                    nuevoC->sig = cola;
                    cola = nuevoC;
                } else {
                    NodoCola* cAux = cola;
                    while (cAux->sig != NULL && cAux->sig->proceso->prioridad <= prio) {
                        cAux = cAux->sig;
                    }
                    nuevoC->sig = cAux->sig;
                    cAux->sig = nuevoC;
                }
            }
        }

        while (arch >> idB >> idP) {
            pila = new NodoPila(idB, idP, pila);
        }

        arch.close();
        cout << " [EXITO] Estado restaurado desde procesos.txt.\n";
    }
}
int main() {
    int opcion;
    do {
        cout << "\n=============================================\n";
        cout << "   SISTEMA DE GESTION DE PROCESOS \n";
        cout << "=============================================\n";
        cout << " 1. Registrar Proceso (Lista y Cola)\n";
        cout << " 2. Buscar / Modificar Prioridad de Proceso\n";
        cout << " 3. Eliminar Proceso (Limpieza en Cascada)\n";
        cout << " 4. Ejecutar Siguiente Proceso (CPU)\n";
        cout << " 5. Asignar Bloque de Memoria (Pila Push)\n";
        cout << " 6. Liberar Memoria de la Cima (Pila Pop)\n";
        cout << " 7. Mostrar Estado (Lista, Cola y Pila)\n";
        cout << " 8. Persistencia de Datos (Guardar / Cargar)\n";
        cout << " 0. Salir\n";
        cout << "=============================================\n";
        cout << " Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: registrarProceso(); break;
            case 2: buscarOModificar(); break;
            case 3: eliminarProceso(); break;
            case 4: ejecutarProceso(); break;
            case 5: asignarMemoria(); break;
            case 6: liberarMemoria(); break;
            case 7: mostrarEstructuras(); break;
            case 8: persistencia(); break;
            case 0: cout << " Saliendo del sistema...\n"; break;
            default: cout << " [ERROR] Opcion invalida.\n";
        }
    } while (opcion != 0);

    return 0;
}
