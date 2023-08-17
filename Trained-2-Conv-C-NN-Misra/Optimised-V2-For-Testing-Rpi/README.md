## Concept for testing:

**Things consired:**  

* Before each test-scenario, Rpi has been in idle state for 5min, to 
* reach maximum, non-load cpu-temperature.
* Time-librarys sleep-function has been utlized, to contrast spikes in energy-consumption.  

## Visualization of test scenarios:

**S**=Sleep    
**R**=Run  

```plaintext
Scen1:

Single small set of images passed once.

                               5 images
                              /
                             or
                    -----------or-- 10 images
                   /         or
              Pass data       \
                 /             20 images
                /
               2.5s
0s              |                 5s (Actual exec.-time "< 3s", but measuring should last longer)
|-------------Measure--------------|
|     --S->     |      --R->       |

Scen2:

Single large set of images passed once.

                               50 images
                              /
                             or
                    -----------or-- 100 images
                   /         or
              Pass data       \
                 /             200 images
                /
               2.5s
0s              |                 5s (Actual exec.-time probably "< 3s", but measuring should last longer)
|-------------Measure--------------|
|     --S->     |      --R->       |

Scen3:

Single small set of images passed in intervals. In between passes, program is in "idle"-state.

                              5 images
                             /
                            or
                 ------------or-- 10 images
                /           or
           Pass data         \
                \             20 images
                 \
                  \
              -------------------------------
             /         |          |          \
            2s        +4s        +6s        +8s
0s          |          |          |          |        +10s
|------------------------ Measure------------------------|
|   --S->   |  --R+S-> |  --R+S-> |  --R+S-> |  --R+S->  |

Scen4:

Single set of images, with growing size, passed in intervals. In between passes, program is in "idle"-state.

                              5 images
                             /
                            or
                 ------------or-- 10 images
                /           or
           Pass data         \
                \              20 images
                 \
                  \
              -------------------------------
             /         |          |          \
            2s        +4s        +6s        +8s
0s          |          |          |          |        +10s
|------------------------ Measure------------------------|
|   --S->   |  --R+S-> |  --R+S-> |  --R+S-> |  --R+S->  |


```

## Results of testing:

Todo..
