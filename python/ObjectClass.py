class Object(ObjectBase):
    def __init__(self):
        ObjectBase.__init__(self)
        self.picture = None

        self.animFrames = 1

        self.drawnAngle = 2
        self.angle = 256 / 8 * self.drawnAngle

        self.drawn_pos = (0,0)
        self.offset = (0,0)
        self.real_pos = (0.0, 0.0)
        self.size = (1, 1)

        self.armor = 0
        self.maxHealth = 100
        self.health = self.maxHealth

        self.radius = 0
        self.guardRange = 0
        self.viewRange = 5

	
# vim:ts=4:sw=4:et
