EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
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
L MCU_Module:Arduino_Nano_v3.x A1
U 1 1 60CFF5EF
P 5850 2250
F 0 "A1" H 5850 1161 50  0000 C CNN
F 1 "Arduino_Nano_v3.x" H 5850 1070 50  0000 C CNN
F 2 "Module:Arduino_Nano" H 5850 2250 50  0001 C CIN
F 3 "http://www.mouser.com/pdfdocs/Gravitech_Arduino_Nano3_0.pdf" H 5850 2250 50  0001 C CNN
	1    5850 2250
	1    0    0    -1  
$EndComp
$Comp
L Connector:Mini-DIN-6 J1
U 1 1 60D00A7A
P 2700 1350
F 0 "J1" H 2700 1717 50  0000 C CNN
F 1 "Mini-DIN-6" H 2700 1626 50  0000 C CNN
F 2 "adbuino_v0p1:TE_5749180-1" H 2700 1350 50  0001 C CNN
F 3 "http://service.powerdynamics.com/ec/Catalog17/Section%2011.pdf" H 2700 1350 50  0001 C CNN
	1    2700 1350
	1    0    0    -1  
$EndComp
$Comp
L Connector:Mini-DIN-6 J2
U 1 1 60D02AD2
P 2700 2050
F 0 "J2" H 2700 2417 50  0000 C CNN
F 1 "Mini-DIN-6" H 2700 2326 50  0000 C CNN
F 2 "adbuino_v0p1:TE_5749180-1" H 2700 2050 50  0001 C CNN
F 3 "http://service.powerdynamics.com/ec/Catalog17/Section%2011.pdf" H 2700 2050 50  0001 C CNN
	1    2700 2050
	1    0    0    -1  
$EndComp
$Comp
L Connector:Mini-DIN-4 J3
U 1 1 60D02E49
P 7900 1100
F 0 "J3" H 7900 1467 50  0000 C CNN
F 1 "Mini-DIN-4" H 7900 1376 50  0000 C CNN
F 2 "adbuino_v0p1:CUI_MD2-40ST" H 7900 1100 50  0001 C CNN
F 3 "http://service.powerdynamics.com/ec/Catalog17/Section%2011.pdf" H 7900 1100 50  0001 C CNN
	1    7900 1100
	1    0    0    -1  
$EndComp
$Comp
L Connector:Mini-DIN-4 J4
U 1 1 60D03A34
P 7900 1800
F 0 "J4" H 7900 2167 50  0000 C CNN
F 1 "Mini-DIN-4" H 7900 2076 50  0000 C CNN
F 2 "adbuino_v0p1:CUI_MD2-40ST" H 7900 1800 50  0001 C CNN
F 3 "http://service.powerdynamics.com/ec/Catalog17/Section%2011.pdf" H 7900 1800 50  0001 C CNN
	1    7900 1800
	1    0    0    -1  
$EndComp
Text GLabel 8200 1100 2    50   Input ~ 0
ADB_1_DATA
Text GLabel 8200 1000 2    50   Input ~ 0
ADB_1_5v
Text GLabel 7600 1000 0    50   Input ~ 0
ADB_1_GND
Text GLabel 7600 1100 0    50   Input ~ 0
ADB_1_PWR_ON
Text GLabel 7600 1700 0    50   Input ~ 0
ADB_2_GND
Text GLabel 7600 1800 0    50   Input ~ 0
ADB_2_PWR_ON
Text GLabel 8200 1700 2    50   Input ~ 0
ADB_2_5v
Text GLabel 8200 1800 2    50   Input ~ 0
ADB_2_DATA
Text GLabel 3000 1450 2    50   Input ~ 0
PS2_1_Data
Text GLabel 3000 1350 2    50   Input ~ 0
PS2_1_GND
Text GLabel 3000 1250 2    50   Input ~ 0
PS2_1_CLK
Text GLabel 3000 2150 2    50   Input ~ 0
PS2_2_DATA
Text GLabel 3000 2050 2    50   Input ~ 0
PS2_2_GND
Text GLabel 3000 1950 2    50   Input ~ 0
PS2_2_CLK
Text GLabel 2400 2050 0    50   Input ~ 0
PS2_2_5v
Text GLabel 2400 1350 0    50   Input ~ 0
PS2_1_5v
$Comp
L Connector:TestPoint_Small TP4
U 1 1 60D061C4
P 2200 2150
F 0 "TP4" H 2152 2150 50  0000 R CNN
F 1 "TestPoint_Small" H 2248 2105 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 2400 2150 50  0001 C CNN
F 3 "~" H 2400 2150 50  0001 C CNN
	1    2200 2150
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Small TP2
U 1 1 60D07A5E
P 2200 1450
F 0 "TP2" H 2152 1450 50  0000 R CNN
F 1 "TestPoint_Small" H 2248 1405 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 2400 1450 50  0001 C CNN
F 3 "~" H 2400 1450 50  0001 C CNN
	1    2200 1450
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Small TP3
U 1 1 60D0824E
P 2200 1950
F 0 "TP3" H 2152 1950 50  0000 R CNN
F 1 "TestPoint_Small" H 2248 1905 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 2400 1950 50  0001 C CNN
F 3 "~" H 2400 1950 50  0001 C CNN
	1    2200 1950
	1    0    0    -1  
$EndComp
$Comp
L Connector:TestPoint_Small TP1
U 1 1 60D085C0
P 2200 1250
F 0 "TP1" H 2152 1250 50  0000 R CNN
F 1 "TestPoint_Small" H 2248 1205 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 2400 1250 50  0001 C CNN
F 3 "~" H 2400 1250 50  0001 C CNN
	1    2200 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 1250 2400 1250
Wire Wire Line
	2400 1450 2200 1450
Wire Wire Line
	2400 1950 2200 1950
Wire Wire Line
	2400 2150 2200 2150
Text GLabel 10200 1800 0    50   Input ~ 0
ADB_1_DATA
Text GLabel 10200 2400 0    50   Input ~ 0
ADB_1_5v
Text GLabel 10200 2500 0    50   Input ~ 0
ADB_2_5v
Text GLabel 10200 1900 0    50   Input ~ 0
ADB_2_DATA
Text GLabel 10200 2700 0    50   Input ~ 0
ADB_1_GND
Text GLabel 10200 2100 0    50   Input ~ 0
ADB_1_PWR_ON
Text GLabel 10200 2800 0    50   Input ~ 0
ADB_2_GND
Text GLabel 10200 2200 0    50   Input ~ 0
ADB_2_PWR_ON
$Comp
L Connector:TestPoint_Small TP9
U 1 1 60D09D61
P 10700 1900
F 0 "TP9" H 10652 1900 50  0000 R CNN
F 1 "TestPoint_Small" H 10748 1855 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 10900 1900 50  0001 C CNN
F 3 "~" H 10900 1900 50  0001 C CNN
	1    10700 1900
	-1   0    0    1   
$EndComp
Text GLabel 10500 1800 2    50   Input ~ 0
ADB_DATA
Wire Wire Line
	10450 1800 10450 1900
Wire Wire Line
	10200 1900 10250 1900
Connection ~ 10450 1900
Wire Wire Line
	10450 1900 10700 1900
Wire Wire Line
	10500 1800 10450 1800
Wire Wire Line
	10200 1800 10250 1800
Wire Wire Line
	10250 1800 10250 1900
Connection ~ 10250 1900
Wire Wire Line
	10250 1900 10450 1900
$Comp
L Connector:TestPoint_Small TP10
U 1 1 60D0C7F4
P 10700 2200
F 0 "TP10" H 10652 2200 50  0000 R CNN
F 1 "TestPoint_Small" H 10748 2155 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 10900 2200 50  0001 C CNN
F 3 "~" H 10900 2200 50  0001 C CNN
	1    10700 2200
	-1   0    0    1   
$EndComp
Text GLabel 10500 2100 2    50   Input ~ 0
ADB_PWR_ON
Wire Wire Line
	10450 2100 10450 2200
Connection ~ 10450 2200
Wire Wire Line
	10450 2200 10700 2200
Wire Wire Line
	10500 2100 10450 2100
Wire Wire Line
	10250 2200 10450 2200
Wire Wire Line
	10200 2100 10250 2100
Wire Wire Line
	10250 2100 10250 2200
Wire Wire Line
	10200 2200 10250 2200
Connection ~ 10250 2200
Text GLabel 2850 2700 0    50   Input ~ 0
PS2_1_Data
Text GLabel 2850 3600 0    50   Input ~ 0
PS2_1_GND
Text GLabel 2850 2800 0    50   Input ~ 0
PS2_2_DATA
Text GLabel 2850 3700 0    50   Input ~ 0
PS2_2_GND
Text GLabel 2850 3100 0    50   Input ~ 0
PS2_2_CLK
Text GLabel 5350 1650 0    50   Input ~ 0
ADB_DATA
Text GLabel 5350 1750 0    50   Input ~ 0
ADB_PWR_ON
$Comp
L Connector:TestPoint_Small TP11
U 1 1 60D0EAE8
P 10700 2500
F 0 "TP11" H 10652 2500 50  0000 R CNN
F 1 "TestPoint_Small" H 10748 2455 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 10900 2500 50  0001 C CNN
F 3 "~" H 10900 2500 50  0001 C CNN
	1    10700 2500
	-1   0    0    1   
$EndComp
Wire Wire Line
	10200 2400 10250 2400
Wire Wire Line
	10250 2400 10250 2500
Wire Wire Line
	10200 2500 10250 2500
Connection ~ 10250 2500
$Comp
L Connector:TestPoint_Small TP12
U 1 1 60D0F9F3
P 10700 2800
F 0 "TP12" H 10652 2800 50  0000 R CNN
F 1 "TestPoint_Small" H 10748 2755 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 10900 2800 50  0001 C CNN
F 3 "~" H 10900 2800 50  0001 C CNN
	1    10700 2800
	-1   0    0    1   
$EndComp
Connection ~ 10450 2800
Wire Wire Line
	10450 2800 10700 2800
Wire Wire Line
	10250 2800 10450 2800
Wire Wire Line
	10200 2700 10250 2700
Wire Wire Line
	10250 2700 10250 2800
Wire Wire Line
	10200 2800 10250 2800
Connection ~ 10250 2800
Text GLabel 5350 2550 0    50   Input ~ 0
ADB_5v
$Comp
L Connector:TestPoint_Small TP5
U 1 1 60D15DB5
P 3350 2800
F 0 "TP5" H 3302 2800 50  0000 R CNN
F 1 "TestPoint_Small" H 3398 2755 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 3550 2800 50  0001 C CNN
F 3 "~" H 3550 2800 50  0001 C CNN
	1    3350 2800
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 2700 3100 2800
Wire Wire Line
	2850 2800 2900 2800
Connection ~ 3100 2800
Wire Wire Line
	3100 2800 3350 2800
Wire Wire Line
	3150 2700 3100 2700
Wire Wire Line
	2850 2700 2900 2700
Wire Wire Line
	2900 2700 2900 2800
Connection ~ 2900 2800
Wire Wire Line
	2900 2800 3100 2800
$Comp
L Connector:TestPoint_Small TP6
U 1 1 60D15DC5
P 3350 3100
F 0 "TP6" H 3302 3100 50  0000 R CNN
F 1 "TestPoint_Small" H 3398 3055 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 3550 3100 50  0001 C CNN
F 3 "~" H 3550 3100 50  0001 C CNN
	1    3350 3100
	-1   0    0    1   
$EndComp
Wire Wire Line
	3100 3000 3100 3100
Connection ~ 3100 3100
Wire Wire Line
	3100 3100 3350 3100
Wire Wire Line
	3150 3000 3100 3000
Wire Wire Line
	2900 3100 3100 3100
Wire Wire Line
	2850 3000 2900 3000
Wire Wire Line
	2900 3000 2900 3100
Wire Wire Line
	2850 3100 2900 3100
Connection ~ 2900 3100
$Comp
L Connector:TestPoint_Small TP7
U 1 1 60D15DD5
P 3350 3400
F 0 "TP7" H 3302 3400 50  0000 R CNN
F 1 "TestPoint_Small" H 3398 3355 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 3550 3400 50  0001 C CNN
F 3 "~" H 3550 3400 50  0001 C CNN
	1    3350 3400
	-1   0    0    1   
$EndComp
Wire Wire Line
	2850 3300 2900 3300
Wire Wire Line
	2900 3300 2900 3400
Wire Wire Line
	2850 3400 2900 3400
Connection ~ 2900 3400
$Comp
L Connector:TestPoint_Small TP8
U 1 1 60D15DE5
P 3350 3700
F 0 "TP8" H 3302 3700 50  0000 R CNN
F 1 "TestPoint_Small" H 3398 3655 50  0001 L CNN
F 2 "TestPoint:TestPoint_Pad_D1.0mm" H 3550 3700 50  0001 C CNN
F 3 "~" H 3550 3700 50  0001 C CNN
	1    3350 3700
	-1   0    0    1   
$EndComp
Connection ~ 3100 3700
Wire Wire Line
	3100 3700 3350 3700
Wire Wire Line
	2900 3700 3100 3700
Wire Wire Line
	2850 3600 2900 3600
Wire Wire Line
	2900 3600 2900 3700
Wire Wire Line
	2850 3700 2900 3700
Connection ~ 2900 3700
Text GLabel 2850 3400 0    50   Input ~ 0
PS2_2_5v
Text GLabel 2850 3300 0    50   Input ~ 0
PS2_1_5v
Text GLabel 3150 2700 2    50   Input ~ 0
PS2_Data
Text GLabel 2850 3000 0    50   Input ~ 0
PS2_1_CLK
Text GLabel 3150 3000 2    50   Input ~ 0
PS2_CLK
Text GLabel 6350 2650 2    50   Input ~ 0
PS2_Data
Text GLabel 6350 2750 2    50   Input ~ 0
PS2_CLK
Text GLabel 5350 2250 0    50   Input ~ 0
PS2_5v
Text GLabel 3150 3800 2    50   Input ~ 0
EXT_GND
Wire Wire Line
	3100 3700 3100 3800
Wire Wire Line
	3100 3800 3150 3800
Text GLabel 5350 2450 0    50   Input ~ 0
GND
Text GLabel 10500 2900 2    50   Input ~ 0
EXT_GND
Wire Wire Line
	10450 2800 10450 2900
Wire Wire Line
	10450 2900 10500 2900
Text GLabel 3150 3300 2    50   Input ~ 0
PS2_5v
Wire Wire Line
	3150 3300 3100 3300
Wire Wire Line
	2900 3400 3100 3400
Wire Wire Line
	3100 3400 3350 3400
Connection ~ 3100 3400
Wire Wire Line
	3100 3300 3100 3400
Wire Wire Line
	10500 2400 10450 2400
Wire Wire Line
	10250 2500 10450 2500
Wire Wire Line
	10450 2500 10700 2500
Connection ~ 10450 2500
Wire Wire Line
	10450 2400 10450 2500
Text GLabel 10500 2400 2    50   Input ~ 0
ADB_5v
NoConn ~ 6350 1650
NoConn ~ 6350 1750
NoConn ~ 6350 2050
NoConn ~ 6350 2250
NoConn ~ 6350 2350
NoConn ~ 6350 2450
NoConn ~ 6350 2550
NoConn ~ 6350 2850
NoConn ~ 6350 2950
$Comp
L Jumper:SolderJumper_2_Bridged JP1
U 1 1 60D5E183
P 6550 3350
F 0 "JP1" H 6550 3555 50  0000 C CNN
F 1 "SolderJumper_2_Bridged" H 6550 3464 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 6550 3350 50  0001 C CNN
F 3 "~" H 6550 3350 50  0001 C CNN
	1    6550 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6400 3350 6300 3350
Wire Wire Line
	5950 3350 5950 3250
Wire Wire Line
	5850 3250 5950 3250
Connection ~ 5950 3250
Text GLabel 6700 3350 2    50   Input ~ 0
EXT_GND
NoConn ~ 5350 2950
NoConn ~ 5350 2850
NoConn ~ 5350 2750
NoConn ~ 5350 2650
NoConn ~ 5350 2350
NoConn ~ 5350 2150
NoConn ~ 5350 2050
NoConn ~ 5350 1950
NoConn ~ 5350 1850
NoConn ~ 5750 1250
NoConn ~ 5950 1250
NoConn ~ 6050 1250
$Comp
L power:GND #PWR?
U 1 1 60D76EE2
P 6300 3350
F 0 "#PWR?" H 6300 3100 50  0001 C CNN
F 1 "GND" H 6305 3177 50  0000 C CNN
F 2 "" H 6300 3350 50  0001 C CNN
F 3 "" H 6300 3350 50  0001 C CNN
	1    6300 3350
	1    0    0    -1  
$EndComp
Connection ~ 6300 3350
Wire Wire Line
	6300 3350 5950 3350
$EndSCHEMATC
