# `CS2702` Proyecto Base de Datos II
<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/database_home.png?raw=true" width="60%">
</p>

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

## Interfaz Gráfica
La interfaz gráfica fue desarrollada utilizando **NodeJS** escrito en Javascript, para poder comunicar el frontend con el programa en C++ que ejecuta las instrucciones sobre las estructuras de datos seleccionadas.

<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/graphic_interface.png?raw=true" width="60%">
</p>

La interfaz permite ejecutar consultas sobre los datasets utilizando cualquiera de los dos índices. Se diseño un parser para ejecutar las distintas funciones utilizando una versión adaptada de SQL. Las instrucciones consideradas fueron las siguientes:

Seleccionar todos los registros de una tabla de la base de datos.
```
SELECT * FROM dataset
```

Búsqueda por rango entre los registros con un begin <= registro.id <= end.
```
SELECT * FROM users WHERE id RANGE 2 TO 10
```

Seleccionar un registro específico según su id.
```
SELECT * FROM users WHERE id = 2
```

Insertar todos los datos desde un archivo `.csv`.
```
INSERT * INTO users FROM dataset.csv
```

Eliminar un registro utilizando su `id`.
```
DELETE FROM users WHERE id = 12
```

Seleccionar uno de los dos índices disponibles. `B`: B+Tree o `SF`: Sequential File.
```
USING INDEX index_type
```

## Descripción de las estructuras implementadas
La implementación de cada estructura varia pues se profundizo con mayor grado el Extendible Hash para lograr acércanos a los tiempos teóricos. Cabe recalcar que cada función y clase esta explicada dentro de la wiki del repositorio.

### B+Tree
La gran diferencia entre archivos *indexados* y *secuenciales* se encuentra en la visualización de este. En el primero, vemos al archivo 
como una set de *keys* con las cuales podemos acceder a este mientas que en el secuencial, uno solo puede acceder al archivo *secuencialmente*, ir realizando operaciones en el actual o siguiente archivo que se encuentre en memoria. Los dos métodos tienen sus beneficios y desventajas, es por esto que el B+ Tree es una gran alternativa para obtener lo mejor de estos; indexando los archivos como un B Tree y accediendo a estos de manera secuencial a conveniencia.


#### Descripción
Para manejar el B+ Tree se utilizaron tres clases, la principal de creación, una estructura nodo y un page manager que realiza las operaciones de archivos. Dentro de la estructura nodo se almacena el `page_id`, el id de próximo nodo (`next_id`), un counter de numero de keys, un arreglo para almacenar la data y otro que guarda las direcciones de sus hijos correspondientes.

```cpp
class btree
{
    shared_ptr<pagemanager> pm;
    ...
};
```

```cpp
struct node
{
    long page_id{-1};
    long next_id{-1};
    long count{0};

    array<T, BTREE_ORDER + 1>    data;
    array<long, BTREE_ORDER + 2> children;
    array<bool, BTREE_ORDER + 1> valid;
    ...
}
```

#### Búsqueda

Se desciende desde el root hasta la hoja comparando la llave de busqueda con las llaves los nodos en nuestro camino. Tanto el costo en tiempo como en accesos a memoria es de `O(log n)`.

```cpp
optional<T> point_search(T const &key);
```


#### Búsqueda por rango

La busqueda por rango no es mas que dos busquedas puntuales por los limites del rango y recuperar secuencialmente los valores entre ambas hojas encontradas. Tanto el costo en tiempo como en accesos a memoria es de `O(log n + m)`, donde `m` es la cantidad de registros en el rango (peor caso `m = O(n)`).


```cpp
vector<T> range_search(T const &low_key, T const &high_key);
```

#### Inserción

El proceso de inserción es bastantes simple, lee el header del root e inserta el key en el nodo correspondiente dentro del arreglo `children`. Por cada inserción se verifica si el nodo necesita un split o necesita ser guardado pues es hoja. Como en el B+ Tree las keys insertadas necesitan también indexarse dentro del ultimo nivel de nodos hojas, la función `insert` se encarga de colocarla dentro la posición necesaria sin duplicar su valor en el registro.

```cpp
void insert(const T &value);
```

#### Eliminación

La eliminacion en esta implementacion consiste en encontrar la hoja correspondiente a la llave de busqueda y marcarla como eliminada. 


```cpp
bool remove(T const &key);
```

### Sequential File

El Sequential File cuenta con una simple inserción y busqueda, aunque se requiere tiempo extra de pre ordenamiento para mantener el archivo auxiliar ordenado fisicamente, este tiempo extra nos sirve para rapidas búsquedas.

#### Descripción
Dentro de nuestra implementación usaremos la clase `BufferFile` para manejar todas las lecturas y escrituras a disco. De esta manera podemos utilizar la estructura principal como controller según la cantidad de registros que nuestro archivo auxiliar pueda soportar. La estructura de `SequentialFile` contiene un template de bloques que puede soportar en el archivo auxiliar. 

Se creo una estructura auxiliar a guardar dentro del los archivos auxiliares, dentro de esta guardamos las posiciones de siguiente registro, siguiente a eliminar y la posicion de lectura a disco.

```
  struct Register {
    int next_register; 
    int next_delete;
    int pos_disk; // pos dentro del index.dat
    int key; 

    Register () {
      next_register = -1;
      next_delete = -1;
    }

    Register (int key, int pos) : key(key), pos_disk(pos) {
      next_register = -1;
      next_delete = -1;
    }
  };
```

De esta manera evitamos guardar tanta informacion en archivos auxiliares y podemos procesar datos de largo variable.

#### Inserción
La estructura recibe un archivo como input y lo empieza a indexar linea por linea, leyendo solo la primera columna que por default se ha seleccionado como la columna de id (`key`), luego la estructura principal le entrega el `key` y la posicion de esta en el archivo principal. Dentro del `BufferFile` me encargo de insertar los `Registers` en el archivo auxliar, dentro de cada inserción lo inserto al final del archivo auxiliar pero actualizo punteros de `next_register`. Como sabemos que una de las ventajas del Sequential File es que los registros se encuentren ordenados fisicamente, la estructura tiene un template de tamaño de bloque y cuando la cantidad de inserciones supera a ese número, se escribe en un archivo auxiliar en orden y se elimina el anterior. Al final de la inserción de todo el archivo de nuevo se hace merge del registro para asegurarnos que nuestro archivo se encuentra correctamente ordenado fisicamente.

#### Búsqueda
Al pricipio se implementó una busqueda secuencial de keys dentro del archivo auxiliar pues no se tenia un buen manejo de archivos, pero una vez que tuvimos la función de `merge_file` se pudo implementar la busqueda binaria que se esperaba. Una vez encontrada la key la funcion `find(key)` retorna la posición del key en el archivo auxiliar y llama a `read_disk(addr)` que imprime el registro completo de archivo principal.

#### Búsqueda por rango
Usando la misma lógica que la búqueda, encontramos la primera key y luego leemos el archivo secuencialmente, imprimiendo cada registro de disco, hasta llegar a la segunda key.

#### Eliminación
Siguiendo con la forma teórica de la estructura, hacemos un `find()` de la `key` que nos retorna la posición la cual leemos usando `seekg(pos)` dentro del auxiliar y rescribimos el header del archivo con el next delete nuevo y modificamos el `next_delete` de la key a eliminar. De esta manera usamos un algoritmo LIFO, donde la funcion `find()` y `merge_file()` ignoran los archivos que no tengan `-1` en `next_delete`. Para evitar problemas el último registro de la lista de registros a eliminar es `-2`.

## Testing
Las pruebas fueron realizadas con cinco iteraciones sobre los distintos datasets. El promedio fue colocado en las siguientes tablas para poder comparar tanto el tiempo de acceso a memoria por parte de cada estructura como su tiempo de ejecución para cada operación implementada.

### Insertar
<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/sequential_file_insertion.png?raw=true" width="40%">
</p>

<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/btreeinsertion.png?raw=true" width="40%">
</p>

### Busqueda Puntual
<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/sequential_file_busqueda_puntual.png?raw=true" width="40%">
</p>

<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/btreepointsearch.png?raw=true" width="40%">
</p>

### Busqueda por Rango
<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/sequential_file_busqueda_rango.png?raw=true" width="40%">
</p>

<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/btreerangesearch.png?raw=true" width="40%">
</p>

### Eliminación
<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/sequential_file_delete.png?raw=true" width="40%">
</p>

<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/btreedelete.png?raw=true" width="40%">
</p>

## Resultados experimentales

En el test podemos notar la diferencia entre la complejidad asintótica de ambas estructuras. Por ejemplo, con 10k, se nota claramente el comportamiento lineal del Sequential File y el comportamiento logarítmico del B+Tree.

<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/graph-1.png?raw=true" width="65%">
</p>

Respecto a la interfaz gráfica, se logró realizar con exito su implementación. Es importante notar que la experiencia de usuario utilizando el índice del `B+Tree` fue bastante más agil que con el `Sequential File`. Esto responde a lo mencionado anteriormente, en donde con un dataset de 10 mil registros el comportamiento del B+tree es logaritmico mientras que el del Sequential File es lineal.

<p align="center">
<img src="https://github.com/dgcnz/cs2702-proyecto/blob/master/imgs/graph-2.png?raw=true" width="65%">
</p>

Finalmente, se pudo observar que si bien el `B+Tree` (BT) tiene considerablemente un tiempo de ejecución menor al `Sequential File` (SF), en términos de acceso a disco, analizado en la función de búsqueda por rango, el BT realiza 127 accesos mientras el SF solo 13 aproximadamente.

## Video de presentación

El video se subió a una carpeta de Google Drive y puede ser accedido con la cuenta de la UTEC desde [el siguiente enlace](https://drive.google.com/file/d/1SkVbmhARCn9FNkvNXsJyOllfyDazrDIa/view?usp=sharing).


