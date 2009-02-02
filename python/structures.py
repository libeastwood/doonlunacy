from StructureClass import *

class Barracks(Builder):
    def __init__(self):
        Builder.__init__(self)
        self.maxHealth = 800
        self.powerRequirement = 20
        self.viewRange = 4

class ConstructionYard(Builder):
    def __init__(self):
        Builder.__init__(self)
        self.maxHealth = 1500
        self.powerRequirement = 0
        self.viewRange = 6

class GunTurret(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.maxHealth = 700
        self.powerRequirement = 10
        self.size = (1,1)
        self.turnSpeed = 0.01

class HeavyFactory(Builder):
    def __init__(self):
        Builder.__init__(self)
        self.powerRequirement = 35
        self.size = (3,2)
        self.upgradeLevels = 2

class HighTechFactory(Builder):
    def __init__(self):
        Builder.__init__(self)
        self.powerRequirement = 35
        self.size = (3,2)


class LightFactory(Builder):
    def __init__(self):
        Builder.__init__(self)
        self.powerRequirement = 20

class Starport(Builder):
    def __init__(self):
        Builder.__init__(self)
        self.maxHealth = 3000
        self.powerRequirement = 80
        self.size = (3,3)
        self.viewRange = 6

class Wor(Builder):
    def __init__(self):
        Builder.__init__(self)
        self.maxHealth = 1500
        self.powerRequirement = 20


class Concrete(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.maxHealth = 5
        self.size = (1,1)

class IX(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.powerRequirement = 40
        self.viewRange = 3

class Palace(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.maxHealth = 2000
        self.powerRequirement = 200
        self.viewRange = 8

class Radar(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.powerRequirement = 20
        self.viewRange = 8

class Refinery(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.capacity = 2000
        self.maxHealth = 1500
        self.powerRequirement = 30
        self.size = (3,2)
        self.viewAngle = 6

class RepairYard(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.size = (3,2)
        self.powerRequirement = 30

class RocketTurret(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.maxHealth = 700
        self.powerRequirement = 15
        self.size = (1,1)
        self.viewRange = 7

class Silo(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.capacity = 1000
        self.maxHealth = 750
        self.viewRange = 3
    
class Wall(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.maxHealth = 500
        self.size = (1,1)
        self.viewRange = 2

class Windtrap(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.viewRange = 3

structures = {
        'Barracks' : Barracks(),
        'Const Yard' : ConstructionYard(),
        'Light Fctry' : LightFactory(),
        'Heavy Fctry' : HeavyFactory(),
        'Hi-Tech' : HighTechFactory(),
        'Star Port' : Starports(),
        'WOR' : WOR(),

        'Concrete' : Concrete(),
        'IX' : IX(),
        'Radar' : Radar(),
        'Refinery' : Refinery(),
        'Repair Yard' : RepairYard(),
        'R-Turret' : RocketTurret(),
        'Turret' : GunTurret(),
        'Spice Silo' : Silo(),
        'Wall' : Wall(),
        'Windtrap' : Windtrap(),
        }

# vim:ts=4:sw=4:et
