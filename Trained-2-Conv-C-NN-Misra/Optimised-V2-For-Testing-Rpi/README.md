## Concept for testing:

**Things consired:**  

* Before each test-scenario, Rpi has been in idle state for 5min, to reach maximum, non-load cpu-temperature.
(Some reference number: 49.4 C)  
* Time-library's sleep-function has been utlized, to contrast spikes in energy-consumption.  

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
0s              |                +3s
|-------------Measure--------------|
|     --S->     |      --R->       |


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


Scen3:

Single large set of images passed once.

                               50 images
                              /
                             or
                    -----------or-- 100 images
                   /
              Pass data
                 /
               2.5s
0s              |                 5s (Actual exec.-time "< 3s", but measuring should last longer)
|-------------Measure--------------|
|     --S->     |      --R->       |


Scen4:

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


Scen5:

Single set of images, with growing image-counts, passed in intervals. In between passes, program is in "idle"-state.

              10       50        100        200
              |        |          |          |
              ----------Pass data-------------
             /         |          |          \
            2s        +4s        +6s        +8s
0s          |          |          |          |        +10s
|------------------------ Measure------------------------|
|   --S->   |  --R+S-> |  --R+S-> |  --R+S-> |  --R+S->  |


Scen6:

Single large small set of images passed once, without sleep function.

      10 images
     /
Pass data
   /
  /
 /        Ns
0s        |                      +3s
|-------------Measure--------------|
|  --R->  |                        |


Scen7:

Single image passed once, without sleep function.

      1 image
     /
Pass data
   /
  /
 /        Ns
0s        |                      +3s
|-------------Measure--------------|
|  --R->  |                        |

```

## Results of testing:

Todo..