class CutsceneClass(object):
    def __init__(self):
        self.filename = None
        self.palette = "INTRO:INTRO.PAL"
        self.fadeIn = False
        self.fadeOut = False
        self.fps = 8.0
        self.hold = 0
        self.sound = None
        self.text = None
        self.textColor = 49
        self.textFadeIn = False
        self.textPosition = (0, 70)
        self.animPosition = (0, -20)
        self.continuation = False
        self.song = None
        self.loop = None

# vim:ts=4:sw=4:et
