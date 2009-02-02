from UnitClass import *

class Carryall(AirUnit):
    def __init__(self):
        AirUnit.__init__(self)
        self.picture = "ObjPic_Carryall"
        self.armor = 2
        self.maxHealth = 100
        self.offset = [8,8]
        self.speed = 1.8
        self.turnSpeed = 0.125
        self.viewRange = 5


class Frigate(AirUnit):
    def __init__(self):
        AirUnit.__init__(self)
        self.picture = "ObjPic_Frigate"
        self.armor = 10
        self.maxHealth = 10000
        self.speed = 1.0
        self.viewRange = 5

class Ornithopter(AirUnit):
    def __init__(self):
        AirUnit.__init__(self)
        self.picture = "ObjPic_Ornithopter"
        self.armor = 3
        self.maxHealth = 100
        self.offset = [8,8]
        self.speed = 1.8
        self.turnSpeed = 0.125
        self.viewRange = 5

class Deviator(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_Deviator"
        self.armor = 5
        self.maxHealth =  200
        self.radius = 6
        self.speed = 0.5
        self.viewRange = 8
        self.weaponRange = 6

class Launcher(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_Launcher"
        self.armor = 7
        self.maxHealth = 300
        self.radius = 6
        self.speed = 0.2

class MCV(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_MCV"
        self.armor = 7
        self.maxHealth = 600
        self.radius = 9
        self.speed = 0.125
        self.viewRange = 7

class Quad(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_Quad"
        self.maxHealth = 450
        self.numWeapons = 2
        self.speed = 0.64
        self.viewRange = 4

class Raider(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_Raider"
        self.armor = 3
        self.maxHealth = 250
        self.speed = 1.3
        self.radius = 4
        self.viewRange = 6

class Sandworm(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_Sandworm"        
        self.armor = 5
        self.maxHealth = 3000
        self.radius = 8
        self.speed = 0.3
        self.turnSpeed = 0.625
        self.viewRange = 8

class SiegeTank(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_SiegeTank"        
        self.armor = 12
        self.maxHealth = 600
        self.radius = 7
        self.speed = 0.3
        self.viewRange = 8

class SonicTank(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_SonicTank"
        self.armor = 7
        self.maxHealth = 350
        self.radius = 6
        self.speed = 0.5
        self.viewRange = 8

class Tank(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.picture = "ObjPic_Tank"
        self.armor = 10
        self.maxHealth = 400
        self.radius = 6
        self.speed = 0.5
        self.viewRange = 6

class Trike(GroundUnit):
    def __init__(self):
        GroundUnit.__init__(self)
        self.armor = 4
        self.maxHealth = 250
        self.picture = "ObjPic_Trike"
        self.radius = 4
        self.speed = 1.0
        self.viewRange = 4

class Fremen(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)

class Infantry(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.picture = "ObjPic_Infantry"
        self.armor = 1
        self.maxHealth = 10
        self.radius = 2
        self.speed = 0.25
        self.viewRange = 4

class Saboteur(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.picture = "ObjPic_Saboteur"
        self.armor = 1
        self.maxHealth = 100
        self.radius = 2
        self.speed = 0.3
        self.viewRange = 4

class Sardaukar(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.armor = 1
        self.maxHealth = 100
        self.speed = 0.18

class Trooper(InfantryUnit):
    def __init__(self):
        InfantryUnit.__init__(self)
        self.armor = 1
        self.maxHealth = 100
        self.radius = 2
        self.speed = 0.2
        self.viewRange = 4

units = {
        'Carryall' : Carryall(),
        'Frigate' : Frigate(),
        'Ornithopter' : Ornithopter(),
        
        'Deviator' : Deviator(),
        'MCV' : MCV(),
        'Quad' : Quad(),
        'Raider' : Raider(),
        'Sandworm' : Sandworm(),
        'Siege Tank' : SiegeTank(),
        'Sonic Tank' : SonicTank(),
        'Tank' : Tank(),
        'Trike' : Trike(),

        'Fremen' : Fremen(),
        'Fremens' : Fremen(),
        'Infantry' : Infantry(),
        'Saboteur' : Saboteur(),
        'Sardaukar' : Sardaukar(),
        'Sardaukars' : Sardaukar(),
        'Soldier' : Infantry(),
        'Trooper' : Trooper(),
        'Troopers' : Trooper(),
        }
        
# vim:ts=4:sw=4:et
