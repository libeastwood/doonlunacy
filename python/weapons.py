from WeaponClass import *

class InfantryGun(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 2
        self.damageRadius = 2
        self.groundBlocked = True
        self.inaccuracy = 2
        self.damage = 5
        self.speed = 9.0
        self.numDeathFrames = 1
        self.deathFrame = "ObjPic_Bullet_Small"
        self.numFrames = 1
        self.graphics = "ObjPic_Bullet_Small"

class RegularGun(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 2
        self.damageRadius = 2
        self.groundBlocked = True
        inaccuracy = 2
        self.damage = 18
        self.speed = 9.0
        self.numDeathFrames = 1
        self.deathFrame = "ObjPic_Bullet_Small"
        self.numFrames = 1
        self.graphic = "ObjPic_Bullet_Small"

class DeviatorRocket(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 0
        self.damageRadius = 16
        inaccuracy = 16
        self.damage = 0
        self.speed = 4.0
        self.numDeathFrames = 5
        self.deathFrame = "ObjPic_Hit_Gas"
        self.deathSound = "Sound_Hit_ExplosionGas"
        self.numFrames = 16
        self.graphic = "ObjPic_Bullet_MediumRocket"

class SmallRocket(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 8
        self.damageRadius = 5
        inaccuracy = 6
        self.damage = 15 //30//50
        self.speed = 4.0
        self.numDeathFrames = 5
        self.deathFrame = "ObjPic_Hit_ExplosionSmall"
        self.numFrames = 16
        self.graphic = "ObjPic_Bullet_SmallRocket"

class SmallRocketHeavy(Weapon):
    def __init__(self):
        Weapon.__init__(self)

        self.damagePiercing = 12
        self.damageRadius = 5
        inaccuracy = 6
        self.damage = 15
        self.speed = 4.0
        self.numDeathFrames = 5
        self.deathFrame = "ObjPic_Hit_ExplosionSmall"
        self.numFrames = 16
        self.graphic = "ObjPic_Bullet_SmallRocket"

class MediumRocket(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 8
        self.damageRadius = 4

        #if (newShooter->getItemID() == Unit_Launcher)
        #    inaccuracy = 20
        #else
        #    inaccuracy = 8

        self.damage = 60
        self.speed = 5.0
        self.numDeathFrames = 5
        self.deathFrame = "ObjPic_Hit_ExplosionSmall"
        self.numFrames = 16
        self.graphic = "ObjPic_Bullet_MediumRocket"

class LargeRocket(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 8
        self.damageRadius = 16
        inaccuracy = 0
        self.damage = 100
        self.speed = 4.0
        self.numDeathFrames = 5
        self.deathFrame = "ObjPic_Hit_ExplosionSmall"
        self.deathSound = "Sound_ExplosionLarge"
        self.numFrames = 16
        self.graphic = "ObjPic_Bullet_LargeRocket"

class Shell(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 12
        self.damageRadius = 3
        self.groundBlocked = True
        inaccuracy = 6
        self.damage = 30//50
        self.speed = 9.0
        self.numDeathFrames = 3
        self.deathFrame = "ObjPic_Hit_Shell"
        self.numFrames = 1
        self.graphic = "ObjPic_Bullet_Medium"

class Sonic(Weapon):
    def __init__(self):
        Weapon.__init__(self)
        self.damagePiercing = 4
        self.damageRadius = 8
        inaccuracy = 4
        self.damage = 40//50
        self.speed = 4.0
        self.numDeathFrames = 1
        self.deathFrame = "ObjPic_Bullet_Sonic"
        self.numFrames = 1
        self.graphic = "ObjPic_Bullet_Sonic"

weapons = {
        'Infantry Gun' : InfantryGun(),
        'Regular Gun' : RegularGun(),
        'Deviator Rocket' : DeviatorRocket(),
        'Small Rocket' : SmallRocket(),
        'Small Rocket Heavy' : SmallRocketHeavy(),
        'Medium Rocket' : MediumRocket(),
        'Large Rocket' : LargeRocket(),
        'Shell' : Shell(),
        'Sonic' : Sonic(),
        }

# vim:ts=4:sw=4:et
