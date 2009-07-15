import ObjectClass
from weapons import *
from gamedata import *
from sounds import *

class Unit(ObjectClass.Object):
    def __init__(self):
        ObjectClass.Object.__init__(self)
        self.deathAnim = ObjPic_Hit_ExplosionSmallUnit()
        self.numFrames = 8
        self.speed = 0.0
        self.speedCap = None
        self.turnSpeed = 0.0625

        self.canAttack = True
        self.respondable = True
        self.confirmSound = [MovingOut(), InfantryOut(), Acknowledged(), Affirmative()]
        self.selectSound = [YesSir(), Reporting()]

class AirUnit(Unit):
    def __init__(self):
        Unit.__init__(self)
        self.deathAnim = ObjPic_Hit_ExplosionMedium1()
        self.numDeathFrames = 5;

class GroundUnit(Unit):
    def __init__(self):
        Unit.__init__(self)
        self.deathAnim = ObjPic_Hit_ExplosionSmallUnit()
        self.numDeathFrames = 5

class InfantryUnit(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Trooper()
        self.deathAnim = ObjPic_DeadInfantry()
        self.decayTime = 15
        self.numDeathFrames = 2
        self.armor = 1
        self.maxHealth = self.health = 10
        self.radius = 2
        self.speed = 0.25
        self.viewRange = 4
        self.weaponRange = 6
        self.size = (0.5, 0.5)
        self.weapons = [LargeRocket()]

# vim:ts=4:sw=4:et
