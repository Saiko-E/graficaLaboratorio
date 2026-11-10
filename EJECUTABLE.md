# Ejecutable para Windows (64 bits)

Abre `dist\graficaLaboratorio\graficaLaboratorio.exe` con doble clic.
En la carpeta distribuida, el ejecutable esta junto a este archivo.

Para compartir el programa, copia **toda la carpeta `graficaLaboratorio`**:
las DLL y las carpetas `Models` y `Shader` son necesarias. Puedes comprimir
esa carpeta en un ZIP y extraerla en la otra computadora antes de abrir el EXE.
El equipo necesita soporte para OpenGL 3.3 y los controladores de su tarjeta grafica.

## Volver a generar el ejecutable

Requisitos: Windows de 64 bits, [MSYS2](https://www.msys2.org/) y su compilador
MinGW64 (`mingw-w64-x86_64-gcc`). La primera compilacion necesita Internet.

Desde PowerShell, en la raiz del proyecto:

```powershell
powershell -ExecutionPolicy Bypass -File .\build-windows.ps1
```

Si MSYS2 esta en otra ubicacion:

```powershell
.\build-windows.ps1 -MsysRoot D:\msys64
```

El script descarga las bibliotecas en `build\windows`, compila una version
optimizada y copia sus dependencias y recursos a `dist\graficaLaboratorio`.
Las bibliotecas se reutilizan en las siguientes compilaciones.

## Controles basicos

- `1` a `8`: camaras del estadio; `9`: vista exterior.
- `0`: camara libre; `W/A/S/D` y raton para moverte.
- `Espacio`: iniciar o pausar la secuencia; `L`: reiniciar la secuencia automatica.
- `N`: ir a penales; `P`: tirar un penal; `R`: reiniciar el partido.
- `Escape`: cerrar.
