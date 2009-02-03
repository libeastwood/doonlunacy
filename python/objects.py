import structures, units, weapons

objects = {}
for structure in structures.structures.iterkeys():
    objects[structure] = structures.structures[structure]

for unit in units.units.iterkeys():
    objects[unit] = units.units[unit]

for weapon in weapons.weapons.iterkeys():
    objects[weapon] = weapons.weapons[weapon]

# vim:ts=4:sw=4:et
