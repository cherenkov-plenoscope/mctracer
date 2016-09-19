# coding: utf-8
import mctracer as T
import os.path

settings = T.PropagationConfig()

wf = T.WorldFactory()

p = os.path.abspath("../test/test_scenery/tree.xml")
print "trying to load", p
wf.load(p)
Eorld = wf.world()
print Eorld.get_print()

free = T.FreeOrbitCamera(Eorld, settings)
