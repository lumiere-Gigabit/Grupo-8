# Sistema de Gestión de Procesos (Simulador de SO)

Este proyecto es una aplicación de consola desarrollada en **C++** que simula cómo un Sistema Operativo administra los procesos y la memoria interna del computador. El software utiliza estructuras de datos dinámicas (punteros) para simular la planificación de la CPU y la asignación LIFO de la memoria.

## 🛠️ Estructuras de Datos Implementadas

El sistema coordina de manera interna tres estructuras fundamentales:
*   **Lista Enlazada Simple (`NodoProceso`):** Almacena el registro histórico y general de todos los procesos creados con sus respectivos estados (`LISTO`, `EJECUTADO`, etc.).
*   **Cola de Prioridad (`NodoCola`):** Gestiona los procesos listos para entrar a la CPU. Los procesos se ordenan automáticamente según su nivel de urgencia (donde la prioridad `1` representa la mayor urgencia).
*   **Pila (`NodoPila`):** Controla la asignación y liberación de bloques de memoria bajo el régimen **LIFO** (Last In, First Out). La memoria se asigna haciendo un *Push* en la cima y se libera mediante un *Pop*.

## 🖥️ Características Principales

*   **Registro de Procesos:** Validación automática de IDs únicos y encolamiento por prioridad.
*   **Modificación Dinámica:** Permite alterar la prioridad de un proceso `LISTO` reordenando la cola en tiempo real.
*   **Eliminación en Cascada:** Al remover un proceso, el sistema limpia de forma automática sus referencias en la cola de CPU y libera todos sus bloques asignados en la pila de memoria para evitar fugas de memoria (*memory leaks*).
*   **Persistencia de Datos:** Guarda y carga de forma completa el estado del simulador (procesos y memoria) mediante el archivo físico `procesos.txt`.

## 🚀 Compilación y Ejecución

Para ejecutar este simulador, requiere un compilador de C++ compatible con el estándar C++11 o superior (como `g++`).

1. Abra una terminal en la ruta del proyecto.
2. Compile el código fuente:
   g++ -std=c++11 "SISTEMA DE GESTIÓN DE PROCESOS.cpp" -o SistemaGestion

3. Inicie la aplicación:
   ./SistemaGestion

## 📂 Archivos del Proyecto

*   `SISTEMA DE GESTIÓN DE PROCESOS.cpp`: Código fuente completo con la lógica de nodos, punteros y el menú interactivo.
*   `Diagrama de flujo 2.vsdx` y `Diagrma de flujo.vsdx`: Diagramas visuales diseñados en Microsoft Visio que detallan la lógica del sistema.
