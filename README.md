# `CS2702` Proyecto Base de Datos II

## Datos Generales
### Integrantes
- Andrea Díaz - 201720031
- Diego Cánez - 201710319
- Maor Roizman - 201810323

### Generales
- Docente: Heider Sanchez
- Ciclo: 2021-01

## Introducción
### Objetivo del proyecto
En la actualidad, las bases de datos cuplen un rol fundamental. Representan una forma estructurada de almacenar, administrar y gestionar la infromación. Sin estas, ningun sistema de información digital que conocemos habria sido posible. Asi mismo, los algoritmos utilizados para transaccionar con los datos almacenados son tan importantes como las estructuras de datos empleadas. En el presente proyecto se implementarán las estruturas de datos `Sequential File` y `B+ Tree` para almacenar la información y se desarrollará un gestor de base de datos que permitirá insertar, buscar y eliminar registros sobre estas estructuras. 


### Dominio de datos
Se usa un dataset publico de la Kaggle con el que se extrajeron cien, mil, siete mil datos a indexar. Por motivos físicos del computador no se llegaron indexar más datos pues la máquina crasheaba si superaba los treinta mil datos o al tratar de limpiar el archivo, esta se congelaba. Para futuras mejoras a la implementación se recomienda realizar las pruebas en otro dispositivo.

### Resultados esperados
Gracias a la teoría sabemos los tiempos de ejecución a esperar, pero la complejidad de la implementación puede que afecte los resultados en la experimentación. Aun así se comprobara los tiempos de ejecución de cada operación básica.

### Costos de operaciones en términos de acceso a la memoria secundaria

## Descrición de las estructuras implmentadas
La implementación de cada estructura varia pues se profundizo con mayor grado el Extendible Hash para lograr acércanos a los tiempos teóricos. Cabe recalcar que cada función y clase esta explicada dentro de la wiki del repositorio.

### B+Tree
La gran diferencia entre archivos *indexados* y *secuenciales* se encuentra en la visualización de este. En el primero, vemos al archivo 
como una set de *keys* con las cuales podemos acceder a este mientas que en el secuencial, uno solo puede acceder al archivo *secuencialmente*, ir realizando operaciones en el actual o siguiente archivo que se encuentre en memoria. Los dos métodos tienen sus beneficios y desventajas, es por esto que el B+ Tree es una gran alternativa para obtener lo mejor de estos; indexando los archivos como un B Tree y accediendo a estos de manera secuencial a conveniencia.

#### Descripción
Para manejar el B+ Tree se utilizaron tres clases, la principal de creación, una estructura nodo y un page manager que realiza las operaciones de archivos. Dentro de la estructura nodo se almacena el `page_id`, el id de próximo nodo (`next_id`), un counter de numero de keys, un arreglo para almacenar la data y otro que guarda las direcciones de sus hijos correspondientes.

#### Búsqueda
Se lee la header del root y se carga a un nodo temporal para luego poder ser buscado recursivamente dentro del árbol. Esta operación tiene un alto costo pues en cada iteración se tiene que leer el archivo para realizar las comparaciones correspondientes.

#### Búsqueda por rango

#### Inserción
El proceso de inserción es bastantes simple, lee el header del root e inserta el key en el nodo correspondiente dentro del arreglo `children`. Por cada inserción se verifica si el nodo necesita un split o necesita ser guardado pues es hoja. Como en el B+ Tree las keys insertadas necesitan también indexarse dentro del ultimo nivel de nodos hojas, la función `insert` se encarga de colocarla dentro la posición necesaria sin duplicar su valor en el registro.

#### Eliminación

### Sequential File
#### Descripción
#### Búsqueda

#### Búsqueda por rango
#### Inserción
#### Eliminación

## Testing
## Resultados experimentales

## Video de presentación
