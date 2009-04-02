import ObjectClass
from weapons import *

class Unit(ObjectClass.Object):
    def __init__(self):
        ObjectClass.Object.__init__(self)
        self.deathAnim = "ObjPic_Hit_ExplosionSmallUnit"
        self.speed = 0.0
        self.speedCap = None
        self.turnSpeed = 0.0625

        self.canAttack = True
        self.respondable = True
        self.confirmSound = ["MovingOut", "InfantryOut", "Acknowledged", "Affirmative"]
        self.selectSound = ["YesSir", "Reporting"]

        self.weapons = []

class AirUnit(Unit):
    def __init__(self):
        Unit.__init__(self)
        self.deathAnim = "Object_AirUnitExplosion"
        self.numDeathFrames = 5;

class GroundUnit(Unit):
    def __init__(self):
        Unit.__init__(self)
        self.deathAnim = "ObjPic_Hit_ExplosionSmallUnit"
        self.numDeathFrames = 5

class InfantryUnit(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = "ObjPic_Trooper"
        self.deathAnim = "ObjPic_DeadInfantry"
        self.numDeathFrames = 2
        self.armor = 1
        self.maxHealth = self.health = 10
        self.radius = 2
        self.speed = 0.25
        self.viewRange = 4
        self.weaponRange = 6
        self.weapons = [SmallRocket()]

# vim:ts=4:sw=4:et
