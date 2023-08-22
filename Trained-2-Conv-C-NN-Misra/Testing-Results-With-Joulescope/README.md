## Concept for testing:

**Things consired**  

* Before each test-scenario, Rpi has been in idle state for 5min, to reach maximum, non-load cpu-temperature.

* Time-library's sleep-function has been utlized, to contrast spikes in energy-consumption. Some test ran also without
to see, as sleep function is non-essential.

**About value ranges**

Plot unit ranges are based on following knowledge:

Raspberry pi 3 B+ models benchmark values for current draw, according to pidreamble.com, is around: 350mA (idle-state) up to 980mA (heavy load)
(https://www.pidramble.com/wiki/benchmarks/power-consumption)'. And the known working voltage range of mentioned Rpi model is: 4.75 - 5.25V.
Therefore, we can obtain reasonable power consumption range:

For power:

- Min: (4,75 + 5.0V / 2) * 0.35A ≈ 1.7W
 
- Max: (4,75 + 5.0V / 2) * 0.98A ≈ 4.78W

However, it is worth mentioning, that measured idle power consumption of our Rpi configuration, never dropped below 2 Watts.
Similarly, current draw never undercut 400mA and voltages lowerbound was lower than one in benchmark result, being as low as: ≈ 4.6V.
To be more precise, down below are averages, calculated from Rpi's 10s idle data, measured with Joulescope DC Energy Analyser (Model version: JS110)

**Average Current**: 0.43365014285714276 A  
**Average Voltage**: 4.7200709523809525 V  
**Average Power**: 2.0457033333333334 W  


## Visualization of test scenarios and results achieved, with Joulescope DC energy analyzer:

**S**=Sleep    
**R**=Run

```plaintext

Scen1:

Single small set of images passed once, with sleep function.

                               5 images
                              /
                             or
                    -----------or-- 10 images (This option measured)
                   /         or
              Pass data       \
                 /             20 images
                /
               2.5s
0s              |                +3s
|-------------Measure--------------|
|     --S->     |      --R->       |

```

**Scen1-figure:**  
![Scenario 1](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen1.png)


```plaintext

Scen2:

Single large set of images passed once, without sleep function.

      50 images
     /
Pass data
   /
  /
 /        Ns
0s        |                      +3s
|-------------Measure--------------|
|  --R->  |                        |

```

**Scen2-figure:**  
![Scenario 2](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen2.png)

```plaintext

Scen3:

Single large set of images passed once.

                               50 images
                              /
                             or
                    -----------or-- 100 images (This option measured)
                   /
              Pass data
                 /
               2.5s
0s              |                 5s up to 10s
|-------------Measure--------------|
|     --S->     |      --R->       |

```

***Scen3-figure:**  
![Scenario 3](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen3.png)


```plaintext

Scen4:

Single small set of images passed in intervals. In between passes, program is in "idle"-state.

                              5 images
                             /
                            or
                 ------------or-- 10 images (This option measured)
                /           or
           Pass data         \
                \             20 images
                 \
                  \
              -------------------------------
             /         |          |          \
            2s        +4s        +6s        +8s
0s          |          |          |          |        +10s up to 12s
|------------------------ Measure------------------------|
|   --S->   |  --R+S-> |  --R+S-> |  --R+S-> |  --R+S->  |

```

***Scen4-figure:**  
![Scenario 4](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen4.png)


```plaintext

Scen5:

Single set of images, with growing image-counts, passed in intervals. In between passes, program is in "idle"-state.

              10       50        100        200
              |        |          |          |
              ----------Pass data-------------
             /         |          |          \
            2s        +4s        +6s        +8s
0s          |          |          |          |        +10s up to 30s
|------------------------ Measure------------------------|
|   --S->   |  --R+S-> |  --R+S-> |  --R+S-> |  --R+S->  |

```

***Scen5-figure:**  
![Scenario 5](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen5.png)


```plaintext

Scen6:

Single small set of images passed once, without sleep function.

             10 images
            /
       Pass data
           /
         +-2s
0s        |                      +3s
|-------------Measure--------------|
|         |         --R->          |

```

***Scen6-figure:**  
![Scenario 6](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen6.png)


```plaintext

Scen7:

Single image passed once, without sleep function.

             single image
            /
       Pass data
           /
         +-2s
0s        |                      +3s
|-------------Measure--------------|
|         |         --R->          |


```

***Scen7-figure:**  
![Scenario 7](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen7.png)

## Conclusions

## Raw testing-data:

[csv-files](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/tree/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope)
