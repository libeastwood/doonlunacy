from AnimationClass import *

class Anim_AtreidesPlanet(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:FARTR.WSA"
        self.framerate = 12.0

class Anim_HarkonnenPlanet(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:FHARK.WSA"
        self.framerate = 12.0

class Anim_OrdosPlanet(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:FORDOS.WSA"
        self.framerate = 12.0

class Anim_AtreidesEyes(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPA.SHP"
        self.framerate = 0.5
        self.index = (0,4)

class Anim_AtreidesMouth(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPA.SHP"
        self.framerate = 5.0
        self.index = (5,9)

class Anim_AtreidesShoulder(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPA.SHP"
        self.framerate = 1.0
        self.index = (10,10)

class Anim_AtreidesBook(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPA.SHP"
        self.framerate = 0.1
        self.index = (11,12)

class Anim_HarkonnenEyes(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPH.SHP"
        self.framerate = 0.3
        self.index = (0,4)

class Anim_HarkonnenMouth(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPH.SHP"
        self.framerate = 0.5
        self.index = (5,9)

class Anim_HarkonnenShoulder(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPH.SHP"
        self.framerate = 1
        self.index = (10,10)

class Anim_OrdosEyes(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPO.SHP"
        self.framerate = 0.5
        self.index = (0,4)

class Anim_OrdosMouth(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPO.SHP"
        self.framerate = 5
        self.index = (5,9)

class Anim_OrdosShoulder(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)	
        self.filename = "DUNE:MENSHPO.SHP"
        self.framerate = 1.0
        self.index = (10,10)

class Anim_OrdosRing(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPO.SHP"
        self.framerate = 6.0
        self.index = (11,14)

class Anim_BeneGesseritEyes(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPM.SHP"
        self.palette = "DUNE:BENE.PAL"
        self.framerate = 0.5
        self.index = (0,4)

class Anim_BeneGesseritMouth(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:MENSHPM.SHP"
        self.palette = "DUNE:BENE.PAL"
        self.framerate = 5.0
        self.index = (5,9)

class Anim_Win1(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:WIN1.WSA"

class Anim_Win2(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:WIN2.WSA"

class Anim_Lose1(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:LOSTBILD.WSA"

class Anim_Lose2(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:LOSTVEHC.WSA"

class Anim_Barracks(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:BARRAC.WSA"

class Anim_Carryall(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:CARRYALL.WSA"

class Anim_ConstructionYard(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:CONSTRUC.WSA"

class Anim_Fremen(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:FREMEN.WSA"

class Anim_DeathHand(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:GOLD-BB.WSA"

class Anim_Devastator(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:HARKTANK.WSA"

class Anim_Harvester(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:HARVEST.WSA"

class Anim_Radar(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:HEADQRTS.WSA"

class Anim_HighTechFactory(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:HITCFTRY.WSA"

class Anim_SiegeTank(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:HTANK.WSA"

class Anim_HeavyFactory(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:HVYFTRY.WSA"

class Anim_Trooper(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:HYINFY.WSA"

class Anim_Infantry(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:INFANTRY.WSA"

class Anim_IX(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:IX.WSA"

class Anim_LightFactory(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:LITEFTRY.WSA"

class Anim_Tank(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:LTANK.WSA"

class Anim_MCV(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:MCV.WSA"

class Anim_Deviator(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:ORDRTANK.WSA"

class Anim_Ornithopter(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:ORNI.WSA"

class Anim_Raider(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:OTRIKE.WSA"

class Anim_Palace(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:PALACE.WSA"

class Anim_Quad(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:QUAD.WSA"

class Anim_Refinery(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:REFINERY.WSA"

class Anim_RepairYard(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:REPAIR.WSA"

class Anim_Launcher(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:RTANK.WSA"

class Anim_RocketTurret(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:RTURRET.WSA"

class Anim_Saboteur(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:SABOTURE.WSA"

class Anim_Slab1(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:SLAB.WSA"

class Anim_SonicTank(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:STANK.WSA"

class Anim_StarPort(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:STARPORT.WSA"

class Anim_Silo(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:STORAGE.WSA"

class Anim_Trike(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:TRIKE.WSA"

class Anim_GunTurret(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:TURRET.WSA"

class Anim_Wall(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:WALL.WSA"

class Anim_WindTrap(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:WINDTRAP.WSA"

class Anim_WOR(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:WOR.WSA"

class Anim_Sandworm(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:WORM.WSA"

class Anim_Sardaukar(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:SARDUKAR.WSA"

class Anim_Frigate(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "MENTAT:FRIGATE.WSA"

class Anim_Static(AnimationClass):
    def __init__(self):
        AnimationClass.__init__(self)
        self.filename = "DUNE:STATIC.WSA"

# vim:ts=4:sw=4:et
