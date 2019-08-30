EESchema Schematic File Version 4
LIBS:hotplate_ctl-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 4
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Mechanical:MountingHole_Pad H1
U 1 1 5C6B6961
P 9250 5950
F 0 "H1" H 9350 6001 50  0000 L CNN
F 1 "M3.2" H 9350 5910 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 9250 5950 50  0001 C CNN
F 3 "~" H 9250 5950 50  0001 C CNN
	1    9250 5950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H2
U 1 1 5C6B740C
P 9650 5950
F 0 "H2" H 9750 6001 50  0000 L CNN
F 1 "M3.2" H 9750 5910 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 9650 5950 50  0001 C CNN
F 3 "~" H 9650 5950 50  0001 C CNN
	1    9650 5950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H3
U 1 1 5C6B74A5
P 10050 5950
F 0 "H3" H 10150 6001 50  0000 L CNN
F 1 "M3.2" H 10150 5910 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 10050 5950 50  0001 C CNN
F 3 "~" H 10050 5950 50  0001 C CNN
	1    10050 5950
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole_Pad H4
U 1 1 5C6B7501
P 10450 5950
F 0 "H4" H 10550 6001 50  0000 L CNN
F 1 "M3.2" H 10550 5910 50  0000 L CNN
F 2 "MountingHole:MountingHole_3.2mm_M3_DIN965_Pad" H 10450 5950 50  0001 C CNN
F 3 "~" H 10450 5950 50  0001 C CNN
	1    10450 5950
	1    0    0    -1  
$EndComp
$Sheet
S 3750 3900 2200 1050
U 5D622339
F0 "power_switch" 50
F1 "power_switch_sheet.sch" 50
F2 "IRON_CTRL" I R 5950 4050 50 
$EndSheet
$Sheet
S 3750 2550 2200 1000
U 5D62255D
F0 "stm32" 50
F1 "stm32_sheet.sch" 50
F2 "SPI_MOSI" O R 5950 2650 50 
F3 "~OLED_CS" O R 5950 2850 50 
F4 "SPI_SCK" O R 5950 2750 50 
F5 "~OLED_RESET" O R 5950 2950 50 
F6 "OLED_D~C" O R 5950 3050 50 
F7 "IRON_CTRL" O R 5950 3400 50 
$EndSheet
$Sheet
S 6650 2550 950  1000
U 5D641A38
F0 "oled" 50
F1 "oled.sch" 50
F2 "SPI_SDIN" I L 6650 2650 50 
F3 "SPI_SCLK" I L 6650 2750 50 
F4 "~SPI_CS" I L 6650 2850 50 
F5 "~RESET" I L 6650 2950 50 
F6 "D~C" I L 6650 3050 50 
$EndSheet
$Comp
L power:Earth #PWR0116
U 1 1 5D7532C2
P 9250 6150
F 0 "#PWR0116" H 9250 5900 50  0001 C CNN
F 1 "Earth" H 9250 6000 50  0001 C CNN
F 2 "" H 9250 6150 50  0001 C CNN
F 3 "~" H 9250 6150 50  0001 C CNN
	1    9250 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9250 6150 9250 6050
$Comp
L power:Earth #PWR0117
U 1 1 5D7539D9
P 9650 6150
F 0 "#PWR0117" H 9650 5900 50  0001 C CNN
F 1 "Earth" H 9650 6000 50  0001 C CNN
F 2 "" H 9650 6150 50  0001 C CNN
F 3 "~" H 9650 6150 50  0001 C CNN
	1    9650 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 6050 9650 6150
$Comp
L power:Earth #PWR0118
U 1 1 5D753E1A
P 10050 6150
F 0 "#PWR0118" H 10050 5900 50  0001 C CNN
F 1 "Earth" H 10050 6000 50  0001 C CNN
F 2 "" H 10050 6150 50  0001 C CNN
F 3 "~" H 10050 6150 50  0001 C CNN
	1    10050 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10050 6150 10050 6050
$Comp
L power:Earth #PWR0119
U 1 1 5D7542EE
P 10450 6150
F 0 "#PWR0119" H 10450 5900 50  0001 C CNN
F 1 "Earth" H 10450 6000 50  0001 C CNN
F 2 "" H 10450 6150 50  0001 C CNN
F 3 "~" H 10450 6150 50  0001 C CNN
	1    10450 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	10450 6150 10450 6050
Wire Wire Line
	5950 2650 6650 2650
Wire Wire Line
	6650 2750 5950 2750
Wire Wire Line
	5950 2850 6650 2850
Wire Wire Line
	6650 2950 5950 2950
Wire Wire Line
	5950 3050 6650 3050
Wire Wire Line
	5950 3400 6150 3400
Wire Wire Line
	6150 3400 6150 4050
Wire Wire Line
	6150 4050 5950 4050
Text Notes 7050 6700 0    50   ~ 0
Hot Plate Reflow Soldering Controller Rev. 1
Text Notes 7050 6800 0    50   ~ 0
August 2019
Text Notes 7050 7100 0    50   ~ 0
T. Music
Wire Notes Line
	750  6600 750  7550
Wire Notes Line
	750  7550 5650 7550
Wire Notes Line
	5650 7550 5650 6600
Wire Notes Line
	5650 6600 750  6600
Text Notes 850  7200 0    50   ~ 0
-> All capacitors and resistors are in 0402 (inch) SMD Package if not noted otherwise\n-> The SMD resistors that could not provide the voltage rating necessary have been replaced by two resistors in series\n-> The wiring on the 230 V side provides > 5 mm isolation between tracks and components\n-> Please do not use for currents > 8 A, the triac cooling may not be sufficient for that\n-> If driving inductive loads please adapt the snubber circuit accordingly\n
Text Notes 850  6750 0    59   ~ 0
Notes:
Text Notes 850  7400 0    50   ~ 0
Licensed under CC BY-SA 4.0
Wire Notes Line
	8950 5500 8950 6400
Wire Notes Line
	8950 6400 11000 6400
Wire Notes Line
	11000 6400 11000 5500
Wire Notes Line
	11000 5500 8950 5500
Text Notes 9050 5650 0    50   ~ 0
Mechanical
$EndSCHEMATC
