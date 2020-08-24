# WTLFU-cPlusPlus
## Welcome to WTinyLfu++
**WTinyLfu++** is a frequency based cache admission policy in order to boost the effectiveness of caches subject to skewed access distributions. Given a newly accessed item and an
eviction candidate from the cache, our scheme decides, based on the recent access history, whether it is worth admitting the new item into the cache at the expense of the eviction candidate.

##### WTinyLfu++ is based on the research papers evaluating its novel eviction policy.
 - [TinyLFU: A Highly Efficient Cache Admission Policy]("https://dl.acm.org/authorize?N41277")  by Gil Einziger, Roy Friedman, Ben Manes
- [Adaptive Software Cache Management]("https://dl.acm.org/authorize?N675830") by Gil Einziger, Ohad Eytan, Roy Friedman, Ben Manes\

You can also see [Caffine caching library](https://github.com/ben-manes/caffeine), which implements WtinyLfu in java.

### Usage
Run the [Simulator](https://github.com/mahmodyaseen/WTLFU-cPlusPlus/blob/master/WtinyLfuSub/Simulator.cpp) in an IDE after specifying the desired [BasicSettings](https://github.com/mahmodyaseen/WTLFU-cPlusPlus/blob/master/WtinyLfuSub/BasicSettings.h). 
argumets are:-
```sh
$ [Test File] [adaptive / sketch]
```

## Sample Report
### you can only run each policy independently.
| policy name | HitRate | Hits | Misses | Requests | Evictions | Time |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|Sketch.WindowTinyLFU.Periodec 1% | 55.75% | 8842 | 7016 | 15858 | 6504 | 123ms |
|Sketch.WindowTinyLFU.Incremental 1% | 53.21% | 8438 | 7420 | 15858 | 6908 | 113ms |
|Sketch.WindowTinyLFU.climber 1% | 53.27% | 8447 | 7411 | 15858 | 6899 | 102ms |
|Sketch.WindowTinyLFU.count-min-64 1% | 56.28% | 8925 | 6933 | 15858 | 6421 | 127ms |
|Sketch.WindowTinyLFU.random-table 1% | 55.59% | 8814 | 7044 | 15868 | 6532 | 363ms |
|Sketch.WindowTinyLFU.perfect-table 1% | 55.90% | 8864 | 6994 | 15868 | 6482 | 123ms |


### Adaptive Window eviction policies:-
| policy name | HitRate | Hits | Misses | Requests | Evictions | Time |
| ------ | ------ | ------ | ------ | ------ | ------ | ------ |
|sketch.HillClimberWindowTinyLfu.Adam [1-5%] | 56.60% | 8884 | 6974 | 15858 | 6462 | 116 ms |
|sketch.HillClimberWindowTinyLfu.Nadam| 55.95% | 8872 | 6986 | 15858 | 6474 | 297 ms |
|sketch.HillClimberWindowTinyLfu.simple-climber | 55.83% | 8853 | 7005 | 15858 | 6493 | 329 ms |
|sketch.HillClimberWindowTinyLfu.stochastic | 56.21% | 8914 | 6944 | 15858 | 6432 | 351 ms |
|sketch.HillClimberWindowTinyLfu.ams-grad | 55.78% | 8846 | 7012 | 15858 | 6500 | 323 ms |

# Todo's
- change the way running each policy, insted of changing in [BasicSettings](https://github.com/mahmodyaseen/WTLFU-cPlusPlus/blob/master/WtinyLfuSub/BasicSettings.h) make it more reachable.
- make sure Cmake is the minimum requierd version, and reachable on linux
- more explanation on each API.

