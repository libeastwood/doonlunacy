import ObjectClass

class Weapon(ObjectClass.Object):
    def __init__(self):
        ObjectClass.Object.__init__(self)
        self.damage = 5
        self.damagePiercing = 2
        self.damageRadius = 2
        self.deathAnim = None
        self.graphic = None
        self.groundBlocked = True
        self.inaccuracy = 2
        self.numDeathFrames = 1
        self.numFrames = 1
        # this is where it actually draws the graphic
        # cause it will draw from top left, not the middle
        self.offset = (8,8)
        self.range = 5
        self.reloadTime = 50
        self.speed = 9.0


# vim:ts=4:sw=4:et
