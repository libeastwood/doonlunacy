import ObjectClass
from WeaponClass import Weapon

class Structure(ObjectClass.Object):
    def __init__(self):
        ObjectClass.Object.__init__(self)
        self.animate = True
        self.capacity = 0
        self.deathAnim = ""
        self.firstAnimFrame = 2
        self.lastAnimFrame = 3
        self.health = self.maxHealth = 1000
        self.powerRequirement = 0
        self.size = (2,2)
        self.upgradeLevels = 0
        self.upgradeCost = 0
        self.weapons = []

class Builder(Structure):
    def __init__(self):
        Structure.__init__(self)
        self.upgradeLevels = 1
        self.upgradeCost = 500

# vim:ts=4:sw=4:et
