from PlayerSideClass import *

class Atreides(PlayerSide):
    name = "Atreides"
    color = 160

    def __init__(self):
        PlayerSide.__init__(self)
        self.canBuild.extend([Trike(), Soldier(), Infantry(), Ornithopter(),
            Barracks()])
        self.ixProvides.append(SonicTank())
        self.palaceProvides.extend([Fremen(), Fremens()])

class Ordos(PlayerSide):
    name = "Ordos"
    color = 176

    def __init__(self):
        PlayerSide.__init__(self)
        self.canBuild.extend([Raider(), Soldier(), Infantry(), Trooper(),
            Troopers(), WOR()])
        self.ixProvides.append(Deviator())
        self.palaceProvides.append(Saboteur())

class Harkonnen(PlayerSide):
    name = "Harkonnen"
    color = 144

    def __init__(self):
        PlayerSide.__init__(self)
        self.canBuild.extend([Trike(), Trooper(), Troopers(), WOR()])
        self.ixProvides.append(Devastator())
        self.palaceProvides.append(LargeRocket())

class Fremen(PlayerSide):
    name = "Fremen"
    color = 192

    def __init__(self):
        PlayerSide.__init__(self)
        self.canBuild.extend([Trike(), Soldier(), Soldiers(), Ornithopter(),
            Fremen(), Fremens(), Barracks()])
        self.ixProvides.append(SonicTank())

class Sardaukar(PlayerSide):
    name = "Sardaukar"
    color = 208

    def __init__(self):
        PlayerSide.__init__(self)
        self.canBuild.extend([Trike(), Sardaukar(), Sardaukars(), WOR()])
        self.ixProvides.append(Devastator())
        self.palaceProvides.append(LargeRocket())

class Mercenary(PlayerSide):
    name = "Mercenary"
    color = 128

    def __init__(self):
        PlayerSide.__init__(self)
        self.canBuild.extend([Trike(), Trooper(), Troopers(), WOR()])
        self.ixProvides.append(Deviator())
        self.palaceProvides.append(Saboteur())

# vim:ts=4:sw=4:et
