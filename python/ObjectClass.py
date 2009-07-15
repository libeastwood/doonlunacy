class Object(object):
    def __init__(self):
        self.animFrames = 1
        self.armor = 0
        self.deathAnim = None
        self.decayTime = 0
        self.drawnAngle = 2
        self.angle = 256 / 8 * self.drawnAngle
        self.drawnPos = (0,0)
        self.explosionSize = 1
        self.graphic = None
        self.topGraphic = None
        self.guardRange = 0
        self.numDeathFrames = 1
        self.numFrames = 1
        self.health = self.maxHealth = 100
        self.radius = 0
        self.realPos = (0.0, 0.0)
        self.size = (1, 1)
        self.offset = (self.size[0]/2.0,self.size[1]/2.0)
        self.topOffset = None
        self.speed = 0
        self.turnSpeed = 0
        self.viewRange = 5
        self.weapons = []

# vim:ts=4:sw=4:et
