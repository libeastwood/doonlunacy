class Object(object):
    def __init__(self):
        self.animFrames = 1
        self.armor = 0
        self.deathAnim = None
        self.drawnAngle = 2
        self.angle = 256 / 8 * self.drawnAngle
        self.drawnPos = (0,0)
        self.explosionSize = 1
        self.graphic = None
        self.guardRange = 0
        self.maxHealth = 100
        self.numDeathFrames = 1
        self.numFrames = 1
        self.health = self.maxHealth
        self.offset = (0,0)
        self.radius = 0
        self.realPos = (0.0, 0.0)
        self.size = (1, 1)
        self.speed = 0
        self.turnSpeed = 0
        self.viewRange = 5

# vim:ts=4:sw=4:et
