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


### Resultados esperados

### Costos de operaciones en términos de acceso a la memoria secundaria

## Descrición de las estructuras implmentadas

### B+Tree
#### Descripción
#### Búsqueda

Todas las funciones de busqueda y acceso son wrappers de `lower_bound`. Esta recorre el arbol hasta llegar a la primera hoja con key mayor o igual `value`.

```cpp
iterator lower_bound(T const &value) const;
```

Por ejemplo, `find` retorna el resultado de `lower_bound` si y solo si este es igual al key.

```cpp
iterator find(T const &value) const;
```

#### Búsqueda por rango

```cpp
pair<iterator, iterator> find(T const &low, T const &high) const;
```
#### Inserción
#### Eliminación

### Sequential File
#### Descripción
#### Búsqueda

#### Búsqueda por rango
#### Inserción
#### Eliminación

## Transaciones y Testing
## Resultados experimentales

## Video de presentación
