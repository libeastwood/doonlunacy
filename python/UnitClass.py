import ObjectClass
from weapons import *

class Unit(ObjectClass.Object):
    def __init__(self):
        ObjectClass.Object.__init__(self)
        self.deathFrame = None
        self.numDeathFrames = 1

        self.speed = 0.0
        self.speedCap = None
        self.turnSpeed = 0.0625

        self.canAttack = True
        self.respondable = True

        self.weapons = []

class AirUnit(Unit):
    def __init__(self):
        Unit.__init__(self)
	self.deathFrame = "Object_AirUnitExplosion"
	self.numDeathFrames = 5;

class GroundUnit(Unit):
    def __init__(self):
        Unit.__init__(self)

class InfantryUnit(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_Trooper"

	self.deathFrame = "ObjPic_DeadInfantry"
	self.numDeathFrames = 2

        self.armor = 1
	self.maxHealth = 10
	self.radius = 2
	self.speed = 0.25
	self.viewRange = 4
	self.weaponRange = 6

	self.weapons = [SmallRocket()]

kos = 1        
# vim:ts=4:sw=4:et
