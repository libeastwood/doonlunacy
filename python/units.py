from UnitClass import *
from common import *
from gamedata import *
from weapons import *

class Carryall(AirUnit):
    def __init__(self):
        AirUnit.__init__(self)
        self.graphic = ObjPic_Carryall()
        self.armor = 2
        self.health = self.maxHealth = 100
        self.offset = Point(8,8)
        self.speed = 1.8
        self.turnSpeed = 0.125
        self.viewRange = 5


class Frigate(AirUnit):
    def __init__(self):
        AirUnit.__init__(self)
        self.graphic = ObjPic_Frigate()
        self.armor = 10
        self.health = self.maxHealth = 10000
        self.speed = 1.0
        self.viewRange = 5

class Ornithopter(AirUnit):
    def __init__(self):
        AirUnit.__init__(self)
        self.graphic = ObjPic_Ornithopter()
        self.armor = 3
        self.health = self.maxHealth = 100
        self.offset = Point(8,8)
        self.speed = 1.8
        self.turnSpeed = 0.125
        self.viewRange = 5

class Devastator(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Devastator_Base()
        self.topGraphic = ObjPic_Devastator_Gun()
        self.armor = 15
        self.health = self.maxHealth = 1000
        self.radius = 7
        self.speed = 0.2
        self.viewRange = 8

class Deviator(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Tank_Base()
        self.topGraphic = ObjPic_Launcher_Gun()
        self.armor = 5
        self.health = self.maxHealth =  200
        self.radius = 6
        self.speed = 0.5
        self.viewRange = 8
        self.weaponRange = 6

class Launcher(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Tank_Base()
        self.topGraphic = ObjPic_Launcher_Gun()
        self.armor = 7
        self.health = self.maxHealth = 300
        self.radius = 6
        self.speed = 0.2

class MCV(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_MCV()
        self.armor = 7
        self.health = self.maxHealth = 600
        self.radius = 9
        self.speed = 0.125
        self.viewRange = 7

class Quad(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Quad()
        self.health = self.maxHealth = 450
        self.speed = 0.64
        self.viewRange = 4

class Raider(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Trike()
        self.armor = 3
        self.health = self.maxHealth = 250
        self.speed = 1.3
        self.radius = 4
        self.viewRange = 6
        self.weapons = [RegularGun(), RegularGun()]

class Sandworm(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Sandworm()
        self.armor = 5
        self.health = self.maxHealth = 3000
        self.radius = 8
        self.speed = 0.3
        self.turnSpeed = 0.625
        self.viewRange = 8

class SiegeTank(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Siegetank_Base()
        self.topGraphic = ObjPic_Siegetank_Gun()
        self.armor = 12
        self.health = self.maxHealth = 600
        self.radius = 7
        self.speed = 0.3
        self.viewRange = 8

class SonicTank(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Tank_Base()
        self.topGraphic = ObjPic_Sonictank_Gun()
        self.armor = 7
        self.health = self.maxHealth = 350
        self.radius = 6
        self.speed = 0.5
        self.viewRange = 8

class Tank(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.graphic = ObjPic_Tank_Base()
        self.topGraphic = ObjPic_Tank_Gun()
        self.armor = 10
        self.health = self.maxHealth = 400
        self.radius = 6
        self.speed = 0.5
        self.viewRange = 6

class Trike(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.armor = 4
        self.health = self.maxHealth = 250
        self.graphic = ObjPic_Trike()
        self.radius = 4
        self.speed = 1.0
        self.viewRange = 4
        self.weapons = [MediumRocket(), MediumRocket(), MediumRocket(), MediumRocket()]

class Fremen(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)

class Infantry(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.graphic = ObjPic_Infantry()
        self.armor = 1
        self.health = self.maxHealth = 10
        self.radius = 2
        self.speed = 0.25
        self.viewRange = 4
        self.weapons = [InfantryGun()]

class Saboteur(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.graphic = ObjPic_Saboteur()
        self.armor = 1
        self.health = self.maxHealth = 100
        self.radius = 2
        self.speed = 0.3
        self.viewRange = 4

class Sardaukar(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.armor = 1
        self.health = self.maxHealth = 100
        self.speed = 0.18

class Trooper(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.armor = 1
        self.health = self.maxHealth = 100
        self.radius = 2
        self.speed = 0.2
        self.viewRange = 4

units = {
        'Carryall' : Carryall,
        'Frigate' : Frigate,
        'Ornithopter' : Ornithopter,
        
        'Deviator' : Deviator,
        'Devastator' : Devastator,
        'MCV' : MCV,
        'Quad' : Quad,
        'Raider' : Raider,
        'Raider Trike' : Raider,
        'Sandworm' : Sandworm,
        'Siege Tank' : SiegeTank,
        'Sonic Tank' : SonicTank,
        'Tank' : Tank,
        'Trike' : Trike,

        'Fremen' : Fremen,
        'Fremens' : Fremen,
        'Infantry' : Infantry,
        'Saboteur' : Saboteur,
        'Sardaukar' : Sardaukar,
        'Sardaukars' : Sardaukar,
        'Soldier' : Infantry,
        'Trooper' : Trooper,
        'Troopers' : Trooper,
        }
        
# vim:ts=4:sw=4:et
