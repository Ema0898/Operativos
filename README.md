# Operativos
Este repositorio corresponde al curso de Principios de Sistemas Operativos de la Carrera de Ingeniería en Computadores del Instituto Tecnológico de Costa Rica

## Tarea2
En esta tarea se implemento un juego de tanques booteable desarrollado en x86. Para la realización de este código, se tomo como base el siguiente repositorio: https://github.com/Pusty/realmode-assembly.git. El tutorial se puede encontrar en la siguiente página: https://0x00sec.org/t/realmode-assembly-writing-bootable-stuff-part-1/2901

### Instalación
#### NASM
Para poder compilar el programa es necesario instalar el compilador NASM esto se puede realizar con el siguiente comando, en este caso se instala para el sistema operativo Ubuntu 18.04 LTS.
```
sudo apt-get install nasm
```

#### QEMU
Para la emulación se utiliza el programa Qemu. En Linux se puede instalar con el siguiente comando.
```
sudo apt-get install qemu
```

#### Quemado en USB
Para quemar el binario compilado en un disposito de almacenamiento USB se utiliza el siguiente comando.
```
sudo dd if=bootable_tanks.bin of=/dev/sdb bs=512 count=3
```

Si se desa utilizar Windows se puede utilizar la herramienta HDD Raw Copy

#### Historial
* 1.0.0
  * Se agrega la versión funcional del proyecto.
  
## Proyecto 1
En este proyecto se implementó un sistema productor-consumidor utilizando memoria compartida y semáforos. Para la realización de este proyecto, se utilizó como base los códigos encontrados en la siguiente página web http://www.chuidiang.org/clinux/ipcs/mem_comp.php. En este proyecto se utilizan distribuciones exponenciales formales. El código para la distribución exponencial fue tomado del siguiente link https://stackoverflow.com/questions/34558230/generating-random-numbers-of-exponential-distribution y el código de la distribución de Poisson fue tomado del siguiente link https://topic.alibabacloud.com/a/use-c-language-to-generate-poisson-distribution-random-number-instance-source-code_1_31_20012031.html

### Instalación
Para ejecutar el código es necesario instalar los siguientes programas
#### GCC y Make
Para la compilación es necesario instalar gcc. Para la construcción se utiliza la herramienta make. Esto se puede realizar mediante el siguiente comando.
```
sudo apt install build-essential
```

#### Ejecución 
Para ejecutar los programas es necesario utilizar los siguientes comandos.

##### Inicializador
Este puede ejecutarse con cualquiera de los siguientes comandos
```
./bin/initializer <buffer_size> <buffer_name>
./initializer <buffer_size> <buffer_name>
```

##### Productor
Este puede ejecutarse con cualquiera de los siguientes comandos
```
./bin/producer <buffer_name> <time_medium>
./producer <buffer_name> <time_medium>
```

##### Consumidor
Este puede ejecutarse con cualquiera de los siguientes comandos
```
./bin/consumer <buffer_name> <time_medium> <operation_mode>
./consumer <buffer_name> <time_medium> <operation_mode>
```

##### Finalizador
Este puede ejecutarse con cualquiera de los siguientes comandos
```
./bin/finalizer <buffer_name>
./finalizer <buffer_name>
```

#### Historial
* 1.0.0
  * Se agrega la versión funcional del proyecto.
