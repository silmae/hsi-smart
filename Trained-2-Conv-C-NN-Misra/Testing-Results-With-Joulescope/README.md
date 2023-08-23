## Concept for testing:

**Things consired**  

* Before each test-scenario, Rpi has been in idle state for 5min, to reach maximum, non-load cpu-temperature.

* Time-library's sleep-function has been utlized, to contrast spikes in energy-consumption. Some test ran also without
to see, as sleep function is non-essential.

* Usage of display (for Rpi) and excess resistance in connections, with measuring-tool, does not affect end-results, as configuration's idle-values are used as contrast point.

**About figures and their value ranges**

Plot unit ranges are based on following knowledge:

Raspberry pi 3 B+ models benchmark values for current draw, according to pidreamble.com, is around: 350mA (idle-state) up to 980mA (heavy load)
(https://www.pidramble.com/wiki/benchmarks/power-consumption)'. And the known working voltage range of mentioned Rpi model is: 4.75 - 5.25V.
Therefore, we can obtain reasonable power range:

- Min: (4,75 + 5.0V / 2) * 0.35A ≈ 1.7W
 
- Max: (4,75 + 5.0V / 2) * 0.98A ≈ 4.78W

However, it is worth mentioning, that measured idle power of our Rpi configuration, never dropped below 2 Watts.
Similarly, current draw never undercut 400mA and voltages lowerbound was lower than one in benchmark result, being as low as: ≈ 4.6V.
To be more precise, down below are averages, calculated from Rpi's 10s idle data, measured with Joulescope DC Energy Analyser (Model version: JS110)

**Average Current**: 0.43365 A  
**Average Voltage**: 4.72000 V  
**Average Power**: 2.04570 W  


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

**Scen3-figure:**  
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

**Scen4-figure:**  
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

**Scen5-figure:**  
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

**Scen6-figure:**  
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

**Scen7-figure:**  
![Scenario 7](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/raw/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope/Plotting/Images/scen7.png)

## Conclusions

NN (Neural Network) was tested with variuous ways, to raise probability of produced testing-data to match the behavior of the actual application. Tests included passing data in different intervals, with high and small image counts. Single passes were also carried.

For single image passed through, rise in current draw was an average of 20,086 mA, up to 20.1 mA. With 10 images passed through, current draw rise 58,775 mA. Noteworthy finding is that, when larger set of images were passed with and without intervals, current draw never exceeded 62.7mA. With voltage, similar findings were obtained. Single image caused on average voltage drop of 0.008 V and other forms of tests
had voltage drops in range of 0.037 to 0.040 V.

This indicates, that running the neural network on a batch of images is more energy-efficient than running it on individual images. Therefore batch processing allows for better utilization of resources and potentially reduces the overhead associated with starting and stopping the processing for each image.

Also, the fact that the current draw for processing larger sets of images never exceeded 62.7mA suggests that there might be some level of diminishing returns in terms of energy consumption as the batch size increases. After a certain point, increasing the batch size further might not lead to significant increases in current draw, implying that the energy efficiency plateaus. This can also been seen as program-wise, as program simply repeats certain set of tasks for input data, and those tasks contribute for peaks in energy consumption.

Findings suggests that there might be an optimal processing rate or interval that balances processing efficiency with energy consumption. Running the program with intervals could allow the system to "rest" between processing bursts, leading to more stable and efficient energy consumption.

When thinking about optimization potential, the difference between the single image and batch processing current draws is quite significant (20,086mA againts 58,775mA). This suggests that there might be room for optimization in the neural network's code, to further improve energy efficiency. What those optimizations could be? Well, memory usage has already been quite well optimised, so possibility of finding and reducing unnecessary computations and/or improving algorithmic efficiency should be noted.

In summary, program seems already to be energy-efficient, in current state. The measurements indicates that batch processing is more energy-efficient than single-image processing, and there might be an optimal processing interval that helps balance processing efficiency and energy consumption. The observed current draws also suggest that there could be potential for further optimization to enhance the energy efficiency.