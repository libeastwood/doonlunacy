from structures import *
from units import *
from weapons import *
from gamedata import *

objects = {}
for structure in structures.iterkeys():
    objects[structure] = structures[structure]

for unit in units.iterkeys():
    objects[unit] = units[unit]

# vim:ts=4:sw=4:et
