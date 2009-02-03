import structures, units

objects = {}
for structure in structures.structures.iterkeys():
    objects[structure] = structures.structures[structure]

for unit in units.units.iterkeys():
    objects[unit] = units.units[unit]

# vim:ts=4:sw=4:et
