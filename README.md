# MADE_24_Windtunnel_Code
## Repository for the MADE course at Reykjavik University (RU)
This repository provides resources for the Precision Machine Design course taught in the fall semester of 2024 at Reykjavik University.    
It supports project organization, code structure, and documentation for wind tunnel simulations and data analysis.

## Overview
The MADE_24_Windtunnel_Code repository offers:

### DOCS:
Datasheets, images and project raw data

### Windtunnel_Code:
This folder holds the main code for controlling and operating the wind tunnel system in the `src` and `include` folder.  
Platform IO was used for the development and the library versions are specified in the `platform.ini` file. If this project is compiled for the first time, platform IO will automatically download the necessary libraries and install them on the system.  

The `src_old` folder contains different stages of the project's development:
- `main_calib.cpp`: Initial calibration file from the HX711_ADC library.
- `main_calib_new.cpp`: Modified calibration file from the HX711_ADC library.
- `main_2loadcells.cpp`: First working code for the two-axis load cell
- `main.cpp`: First tests with the LCD.


### Windtunnel_General_Calculations:
Includes general calculation tools, along with pictures and schematics, for analyzing wind tunnel data.  

There are two subfolders in this folder, each requires a virtual pyhton environment to run the jupyter notebook file. Once the environment is created the necessary packages need to be installed with this comand: `pip install -r requirements.txt`
Each folder contains it's own `requirements.txt` file.

#### Evaluating Test Results
Multiple Jupyther notebooks to evaluate the raw data in the `Docs` folder. 

#### Platform Angular Adjustment
Jupyter notebook to calculate the platform angle for different configuration of the linkage.  


### Windtunnel_Schematic:

KICAD 8.0 was used to draw the schematic for the measurement system. This repository also includes a 3D model of the PCB and the libraries for the XFW-HX711 module. 
