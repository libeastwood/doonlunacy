from ObjectClass import ObjectBase

class Weapon(ObjectBase):
    def __init__(self):
        ObjectBase.__init__(self)
        self.damagePiercing = 2
        self.damageRadius = 2
        self.deathFrame = None
        self.graphics = None
        self.groundBlocked = True
        self.inaccuracy = 2
        self.damage = 5
        self.speed = 9.0
        self.numDeathFrames = 1
        self.numFrames = 1
        self.range = 5
        self.reloadTime = 50

# vim:ts=4:sw=4:et
