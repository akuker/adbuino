EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 3
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
L Transistor_FET:ZXM61N03F Q?
U 1 1 60F28652
P 2450 2350
AR Path="/60F28652" Ref="Q?"  Part="1" 
AR Path="/60F21C45/60F28652" Ref="Q1"  Part="1" 
AR Path="/60F3FFB2/60F28652" Ref="Q2"  Part="1" 
F 0 "Q2" V 2699 2350 50  0000 C CNN
F 1 "BSS138" V 2790 2350 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 2650 2275 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 2450 2350 50  0001 L CNN
F 4 "C52895" H 2450 2350 50  0001 C CNN "LCSC"
	1    2450 2350
	0    1    1    0   
$EndComp
Text Notes 700  750  0    118  ~ 0
http://cdn.sparkfun.com/datasheets/BreakoutBoards/Logic_Level_Bidirectional.pdf
Text HLabel 1750 2050 1    50   Input ~ 0
LV_Data
Text HLabel 3000 2050 1    50   Input ~ 0
HV_Data
Text HLabel 2800 2050 1    50   Input ~ 0
Hi_V
Text HLabel 2050 2050 1    50   Input ~ 0
Low_V
$Comp
L Device:R_Small R?
U 1 1 60F2A54D
P 2050 2200
AR Path="/60F2A54D" Ref="R?"  Part="1" 
AR Path="/60F21C45/60F2A54D" Ref="R8"  Part="1" 
AR Path="/60F3FFB2/60F2A54D" Ref="R10"  Part="1" 
F 0 "R10" V 1854 2200 50  0000 C CNN
F 1 "10k" V 1945 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2050 2200 50  0001 C CNN
F 3 "~" H 2050 2200 50  0001 C CNN
F 4 "C17414" V 2050 2200 50  0001 C CNN "LCSC"
	1    2050 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2050 2100 2050 2050
$Comp
L Device:R_Small R?
U 1 1 60F2A692
P 2800 2200
AR Path="/60F2A692" Ref="R?"  Part="1" 
AR Path="/60F21C45/60F2A692" Ref="R9"  Part="1" 
AR Path="/60F3FFB2/60F2A692" Ref="R11"  Part="1" 
F 0 "R11" V 2604 2200 50  0000 C CNN
F 1 "10k" V 2695 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2800 2200 50  0001 C CNN
F 3 "~" H 2800 2200 50  0001 C CNN
F 4 "C17414" V 2800 2200 50  0001 C CNN "LCSC"
	1    2800 2200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2800 2100 2800 2050
Wire Wire Line
	2650 2450 2800 2450
Wire Wire Line
	2800 2300 2800 2450
Wire Wire Line
	2050 2300 2050 2450
Wire Wire Line
	2250 2450 2050 2450
Connection ~ 2050 2450
Text Notes 2100 2950 0    50   ~ 0
Optional Bypass\n
Wire Wire Line
	2050 2050 2450 2050
Wire Wire Line
	2450 2050 2450 2150
Wire Wire Line
	2800 2450 3000 2450
Wire Wire Line
	3000 2050 3000 2450
Connection ~ 2800 2450
Wire Wire Line
	1750 2450 2050 2450
Wire Wire Line
	1750 2050 1750 2450
$EndSCHEMATC
