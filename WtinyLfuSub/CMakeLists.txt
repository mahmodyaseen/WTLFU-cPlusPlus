cmake_minimum_required(VERSION 3.15)
project(WTinyLfu)

set(CMAKE_CXX_STANDARD 11)

add_executable(WTinyLfu Simulator.cpp
        Admission/countmin4/countMin4.cpp
        Admission/countmin4/countMin4.h
        Admission/Frequency.h
        Admission/Admittor.h
        BasicSettings.h
        Admission/countmin4/PeriodicResetCountMin4.h
        membership/bloom/bloomFilter.h
        Admission/TinyLfu.h
        policy/PolicyStats.h
        Stopwatch.h
        policy/sketch/WindowTinyLfuPolicy.h
        policy/Policy.h
        policy/AccessEvent.h
        policy/sketch/climbing/HillClimber.h
        policy/sketch/climbing/hill/SimpleClimber.h
        policy/sketch/climbing/AbstractClimber.h
        policy/sketch/climbing/HillClimberType.h
        policy/sketch/climbing/gradient/Adam.h
        policy/sketch/climbing/HillClimberWindowTinyLfuPolicy.h
        policy/sketch/climbing/gradient/Nadam.h
        policy/sketch/climbing/gradient/Stochastic.h
        policy/sketch/climbing/gradient/AmsGrad.h
        Admission/countmin4/ClimberResetCountMin4.cpp
        Admission/countmin4/ClimberResetCountMin4.h
        Admission/countmin4/IncrementalResetCountMin4.h
        Admission/perfect/PerfectFrequency.h
        Admission/table/RandomRemovalFrequencyTable.h
        Admission/countmin64/countMin64.h
        Admission/countmin64/countMin64TLfu.h Admission/tinycache/HashedItem.h Admission/tinycache/TinySetIndexing.h Admission/tinycache/HashFunctionPraser.h Admission/tinycache/tinyCache.h Admission/tinycache/TinyCacheAdapter.h Admission/tinycache/tinyCacheSketch.h)