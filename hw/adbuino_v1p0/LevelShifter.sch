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
P 2850 2450
AR Path="/60F28652" Ref="Q?"  Part="1" 
AR Path="/60F21C45/60F28652" Ref="Q1"  Part="1" 
AR Path="/60F3FFB2/60F28652" Ref="Q2"  Part="1" 
F 0 "Q2" V 3099 2450 50  0000 C CNN
F 1 "BSS138" V 3190 2450 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3050 2375 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 2850 2450 50  0001 L CNN
F 4 "C52895" H 2850 2450 50  0001 C CNN "LCSC"
	1    2850 2450
	1    0    0    -1  
$EndComp
Text Notes 700  750  0    118  ~ 0
http://cdn.sparkfun.com/datasheets/BreakoutBoards/Logic_Level_Bidirectional.pdf
Text HLabel 2550 3150 0    50   Input ~ 0
LV_Data
Text HLabel 2550 1900 0    50   Input ~ 0
HV_Data
Text HLabel 2550 2100 0    50   Input ~ 0
Hi_V
Text HLabel 2550 2850 0    50   Input ~ 0
Low_V
$Comp
L Device:R_Small R?
U 1 1 60F2A54D
P 2700 2850
AR Path="/60F2A54D" Ref="R?"  Part="1" 
AR Path="/60F21C45/60F2A54D" Ref="R8"  Part="1" 
AR Path="/60F3FFB2/60F2A54D" Ref="R10"  Part="1" 
F 0 "R10" V 2504 2850 50  0000 C CNN
F 1 "10k" V 2595 2850 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2700 2850 50  0001 C CNN
F 3 "~" H 2700 2850 50  0001 C CNN
F 4 "C25744" V 2700 2850 50  0001 C CNN "LCSC"
	1    2700 2850
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 2850 2550 2850
$Comp
L Device:R_Small R?
U 1 1 60F2A692
P 2700 2100
AR Path="/60F2A692" Ref="R?"  Part="1" 
AR Path="/60F21C45/60F2A692" Ref="R9"  Part="1" 
AR Path="/60F3FFB2/60F2A692" Ref="R11"  Part="1" 
F 0 "R11" V 2504 2100 50  0000 C CNN
F 1 "10k" V 2595 2100 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2700 2100 50  0001 C CNN
F 3 "~" H 2700 2100 50  0001 C CNN
F 4 "C25744" V 2700 2100 50  0001 C CNN "LCSC"
	1    2700 2100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 2100 2550 2100
Wire Wire Line
	2950 2250 2950 2100
Wire Wire Line
	2800 2100 2950 2100
Wire Wire Line
	2800 2850 2950 2850
Wire Wire Line
	2950 2650 2950 2850
Connection ~ 2950 2850
Wire Wire Line
	2550 2850 2550 2450
Wire Wire Line
	2550 2450 2650 2450
Wire Wire Line
	2950 2100 2950 1900
Wire Wire Line
	2550 1900 2950 1900
Connection ~ 2950 2100
Wire Wire Line
	2950 3150 2950 2850
Wire Wire Line
	2550 3150 2950 3150
Wire Notes Line
	3800 2200 4500 2200
Wire Notes Line
	4500 2200 4500 2500
Wire Notes Line
	4500 2500 3800 2500
Wire Notes Line
	3800 2500 3800 2200
Text Notes 3900 2600 0    50   ~ 0
G
Text Notes 4350 2600 0    50   ~ 0
S
Text Notes 4100 2150 0    50   ~ 0
D
Wire Notes Line
	4050 2200 4050 2000
Wire Notes Line
	4050 2000 4200 2000
Wire Notes Line
	3850 2500 3850 2650
Wire Notes Line
	3850 2650 4000 2650
Wire Notes Line
	4000 2650 4000 2500
Wire Notes Line
	4300 2500 4300 2650
Wire Notes Line
	4300 2650 4450 2650
Wire Notes Line
	4450 2500 4450 2650
Text Notes 4100 1950 0    50   ~ 0
3
Text Notes 3900 2750 0    50   ~ 0
1
Text Notes 4350 2750 0    50   ~ 0
2
Wire Notes Line
	3900 2500 3900 2400
Wire Notes Line
	3900 2400 4050 2400
Wire Notes Line
	4050 2400 4050 2250
Wire Notes Line
	4200 2300 4100 2300
Wire Notes Line
	4200 2000 4200 2300
Wire Notes Line
	4350 2500 4350 2350
Wire Notes Line
	4350 2350 4100 2350
Wire Notes Line
	4300 2350 4300 2250
Wire Notes Line
	4300 2250 4200 2250
Wire Notes Line
	4350 2400 4100 2400
$EndSCHEMATC
