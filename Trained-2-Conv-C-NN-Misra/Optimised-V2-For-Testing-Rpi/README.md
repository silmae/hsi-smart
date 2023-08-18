## Concept for testing:

**Things consired:**  

* Before each test-scenario, Rpi has been in idle state for 5min, to reach maximum, non-load cpu-temperature.
(Some reference number: 49.4 C)  
* Time-library's sleep-function has been utlized, to contrast spikes in energy-consumption.  

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

**Figure:**:


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

**Figure:**:


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

**Figure:**:


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

**Figure:**:


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

**Figure:**:


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

**Figure:**:


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


**Figure:**:


```

## Results of testing:

[Tested scenarios (.csv-files)](https://gitlab.jyu.fi/hsi/code/hsi-smart/-/tree/main/Trained-2-Conv-C-NN-Misra/Testing-Results-With-Joulescope)
