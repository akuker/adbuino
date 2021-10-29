Data captured on a Wacom table KT-0405-A.

Driver: https://www.macintoshrepository.org/1205-wacom-tablet-2-3



<table>
  <tr>
    <td>0</td>
    <td>1</td>
    <td>2</td>
    <td>3</td>
    <td>4</td>
    <td>5</td>
    <td>6</td>
    <td>7</td>
    <td>8</td>
    <td>9</td>
    <td>10</td>
    <td>11</td>
    <td>12</td>
    <td>13</td>
    <td>14</td>
    <td>15</td>
    <td>16</td>
    <td>17</td>
    <td>18</td>
    <td>19</td>
    <td>20</td>
    <td>21</td>
    <td>22</td>
    <td>23</td>
    <td>24</td>
    <td>25</td>
    <td>26</td>
    <td>27</td>
    <td>28</td>
    <td>29</td>
    <td>30</td>
    <td>31</td>
    <td>32</td>
    <td>33</td>
    <td>34</td>
    <td>35</td>
    <td>36</td>
    <td>37</td>
    <td>38</td>
    <td>39</td>
    
  </tr>
  <tr>
    <td>1</td>
    <td>2</td>
    <td>3</td>
    <td>4</td>
    <td>5</td>
    <td>6</td>
    <td>7</td>
    <td>8</td>
    <td>9</td>
    <td>10</td>
    <td>11</td>
    <td>12</td>
    <td>13</td>
    <td>14</td>
    <td>15</td>
    <td>16</td>
    <td>17</td>
    <td>18</td>
    <td>19</td>
    <td>20</td>
    <td>21</td>
    <td>22</td>
    <td>23</td>
    <td>24</td>
    <td>25</td>
    <td>26</td>
    <td>27</td>
    <td>28</td>
    <td>29</td>
    <td>30</td>
    <td>31</td>
    <td>32</td>
    <td>33</td>
    <td>34</td>
    <td>35</td>
    <td>36</td>
    <td>37</td>
    <td>38</td>
    <td>39</td>
    <td>40</td>
    
  </tr>
  <tr>
    <td colspan=8>[0]</td>
    <td colspan=8>[1]</td>
    <td colspan=8>[2]</td>
    <td colspan=8>[3]</td>
    <td colspan=8>[4]</td>
  </tr>
  <tr>
    <td colspan=2>Unknown always 11</td>
    <td>pen touching</td>
    </td>
    <td colspan=16>Position X</td>
    <td colspan=16>Position Y</td>
    <td colspan=5>Pressure</td>
  </tr>
</table>

<ul>
 
<li>Pressure always goes to 0 when pen is not touching</li>
<li>BC 1A 19 00 12 C0 02 09 is sent when pen is completely removed</li>
</ul>


Reported data:

| Action              | Register | Data               |Raw |
|---------------------|----|--------------------------|---------------------------------------------------------------------|
|Release Top Right    | 4  | BC 1A 19 00 12 C0 02 09  |ADB Command:4C (Talk) addr:4 reg:0 Data[8]:  BC 1A 19 00 12 C0 02 09 | 
|Release Top Left     | 4  | BC 1A 19 00 12 C0 02 09  |ADB Command:4C (Talk) addr:4 reg:0 Data[8]:  BC 1A 19 00 12 C0 02 09 |
|Release Bottom Right | 4  | BC 1A 19 00 12 C0 02 09  |ADB Command:4C (Talk) addr:4 reg:0 Data[8]:  BC 1A 19 00 12 C0 02 09 |
|Release Bottom Left  | 4  | BC 1A 19 00 12 C0 02 09  |ADB Command:4C (Talk) addr:4 reg:0 Data[8]:  BC 1A 19 00 12 C0 02 09 |




| Action              | Register | Data               |Raw |
|---------------------|----|--------------------------|---------------------------------------------------------------------|
|Hover Bottom Left  | 4  | E0 4F 92 37 00  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 4F 92 37 00
|"                  | 4  | E0 57 12 3F 00  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 57 12 3F 00
|"                  | 4  | E0 64 12 3F 00  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 64 12 3F 00
|"                  | 4  | E0 6B 92 36 80  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 6B 92 36 80
|"                  | 4  | E0 70 92 29 00  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 70 92 29 00
|"                  | 4  | E0 70 92 14 80  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 70 92 14 80
|"                  | 4  | E0 68 12 06 00  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 68 12 06 00
|"                  | 4  | E0 5C 92 06 00  |ADB Command:4C (Talk) addr:4 reg:0 Data[5]:  E0 5C 92 06 00

# Initialization Data

```
ADB Command:3F (Talk) addr:3 reg:3 Data[2]:  B5 00
ADB Command:4F (Talk) addr:4 reg:3 Data[2]:  B5 1D
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:4B (Listen) addr:4 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  82 7F
ADB Command:4D (Talk) addr:4 reg:1 Data[8]:  9C 24 19 00 12 C0 02 09
ADB Command:49 (Listen) addr:4 reg:1 Data[2]:  9C 24
ADB Command:4C (Talk) addr:4 reg:0 Data[8]:  BC 1A 19 00 12 C0 02 09
```
# ADB Mouse initialization
```
ADB Command:3F (Talk) addr:3 reg:3 Data[2]:  B3 80
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
ADB Command:3B (Listen) addr:3 reg:3 Data[2]:  87 FF
ADB Command:FB (Listen) addr:F reg:3 Data[2]:  81 FF
```
