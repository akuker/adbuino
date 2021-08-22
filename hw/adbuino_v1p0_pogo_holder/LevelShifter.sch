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
P 3050 2450
AR Path="/60F28652" Ref="Q?"  Part="1" 
AR Path="/60F21C45/60F28652" Ref="Q1"  Part="1" 
AR Path="/60F3FFB2/60F28652" Ref="Q2"  Part="1" 
F 0 "Q2" V 3299 2450 50  0000 C CNN
F 1 "BSS138" V 3390 2450 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 3250 2375 50  0001 L CIN
F 3 "https://www.onsemi.com/pub/Collateral/BSS138-D.PDF" H 3050 2450 50  0001 L CNN
F 4 "C52895" H 3050 2450 50  0001 C CNN "LCSC"
	1    3050 2450
	1    0    0    1   
$EndComp
Text Notes 700  750  0    118  ~ 0
http://cdn.sparkfun.com/datasheets/BreakoutBoards/Logic_Level_Bidirectional.pdf
Text HLabel 2550 3550 0    50   Input ~ 0
LV_Data
Text HLabel 2550 1400 0    50   Input ~ 0
HV_Data
Text HLabel 2550 1600 0    50   Input ~ 0
Hi_V
Text HLabel 2550 3250 0    50   Input ~ 0
Low_V
$Comp
L Device:R_Small R?
U 1 1 60F2A54D
P 2700 3250
AR Path="/60F2A54D" Ref="R?"  Part="1" 
AR Path="/60F21C45/60F2A54D" Ref="R8"  Part="1" 
AR Path="/60F3FFB2/60F2A54D" Ref="R10"  Part="1" 
F 0 "R10" V 2504 3250 50  0000 C CNN
F 1 "10k" V 2595 3250 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2700 3250 50  0001 C CNN
F 3 "~" H 2700 3250 50  0001 C CNN
F 4 "C25744" V 2700 3250 50  0001 C CNN "LCSC"
	1    2700 3250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 3250 2550 3250
$Comp
L Device:R_Small R?
U 1 1 60F2A692
P 2700 1600
AR Path="/60F2A692" Ref="R?"  Part="1" 
AR Path="/60F21C45/60F2A692" Ref="R9"  Part="1" 
AR Path="/60F3FFB2/60F2A692" Ref="R11"  Part="1" 
F 0 "R11" V 2504 1600 50  0000 C CNN
F 1 "10k" V 2595 1600 50  0000 C CNN
F 2 "Resistor_SMD:R_0402_1005Metric" H 2700 1600 50  0001 C CNN
F 3 "~" H 2700 1600 50  0001 C CNN
F 4 "C25744" V 2700 1600 50  0001 C CNN "LCSC"
	1    2700 1600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2600 1600 2550 1600
Wire Wire Line
	2800 1600 2950 1600
Wire Wire Line
	2800 3250 2950 3250
Connection ~ 2950 3250
Wire Wire Line
	2950 1600 2950 1400
Wire Wire Line
	2550 1400 2950 1400
Connection ~ 2950 1600
Wire Wire Line
	2950 3550 2950 3250
Wire Wire Line
	2550 3550 2950 3550
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
Wire Wire Line
	2550 2450 2550 3250
$Comp
L Jumper:SolderJumper_3_Bridged12 JP3
U 1 1 61220677
P 3150 1600
AR Path="/60F3FFB2/61220677" Ref="JP3"  Part="1" 
AR Path="/60F21C45/61220677" Ref="JP1"  Part="1" 
F 0 "JP3" H 3150 1805 50  0000 C CNN
F 1 "SolderJumper_3_Bridged12" H 3150 1714 50  0000 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_RoundedPad1.0x1.5mm" H 3150 1600 50  0001 C CNN
F 3 "~" H 3150 1600 50  0001 C CNN
	1    3150 1600
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_3_Bridged12 JP4
U 1 1 612272EF
P 3150 3250
AR Path="/60F3FFB2/612272EF" Ref="JP4"  Part="1" 
AR Path="/60F21C45/612272EF" Ref="JP2"  Part="1" 
F 0 "JP4" H 3150 3363 50  0000 C CNN
F 1 "SolderJumper_3_Bridged12" H 3150 3454 50  0000 C CNN
F 2 "Jumper:SolderJumper-3_P1.3mm_Bridged12_RoundedPad1.0x1.5mm" H 3150 3250 50  0001 C CNN
F 3 "~" H 3150 3250 50  0001 C CNN
	1    3150 3250
	1    0    0    1   
$EndComp
Wire Wire Line
	2850 2450 2550 2450
Wire Wire Line
	3150 2250 3150 1750
Wire Wire Line
	3150 3100 3150 2650
Wire Wire Line
	2950 3550 3700 3550
Wire Wire Line
	3700 3550 3700 1600
Wire Wire Line
	3700 1600 3350 1600
Connection ~ 2950 3550
Wire Wire Line
	2950 1400 3750 1400
Wire Wire Line
	3750 1400 3750 3250
Wire Wire Line
	3750 3250 3350 3250
Connection ~ 2950 1400
$EndSCHEMATC
