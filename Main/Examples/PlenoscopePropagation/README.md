Check with Valgrind
-------------------

```
valgrind --tool=memcheck --track-origins=yes --leak-check=full /home/sebastian/raytracing/build/mctPlenoscopePropagation -l lightfield_calibration/ -c propagation_config.xml -i run161.evtio -o run161.52.pl -r 0 > vg46.txt 2>&1
```