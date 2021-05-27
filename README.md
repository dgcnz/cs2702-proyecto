# `CS2702` Proyecto Base de Datos II

## Datos Generales
### Integrantes
- Andrea Díaz - 201720031
- Diego Cánez - 201710319
- Maor Roizman - 201810323

### Generales
- Docente: Heider Sanchez
- Ciclo: 2021-01

Requirements
-------------
The basic requirements for this example is a anaconda enviroment:


## Installation on LINUX/UNIX Systems

Download miniconda from

https://docs.conda.io/en/latest/miniconda.html

```
chmod +x Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
source activate base
```

## Installation the following packages

```
conda install -c anaconda cmake
conda install -c conda-forge gtest
conda install -c conda-forge gmock
conda install -c hi2p-perim fmt
conda install -c anaconda ncurses
```

#### Note for osx:
` brew install fmt` and change the following lines in CMakeList.txt
```
$ENV{CONDA_PREFIX}/lib/libgmock_main.so
$ENV{CONDA_PREFIX}/lib/libgmock.so )
```

it should be 
```
$ENV{CONDA_PREFIX}/lib/libgmock_main.a
$ENV{CONDA_PREFIX}/lib/libgmock.a )

```


Build process
-------------
```
./conda/recipes/databases/build.sh
```

run gtest:
```
./hash-gtest
```

or 

```
cd /my_project_path/
mkdir build
cd build
cmake ..
make all
./hash-gtest
```
