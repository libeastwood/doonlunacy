from units import *
from structures import *

class PlayerSide(object):
    name = None
    color = None
    canBuild = [Carryall(), Launcher(), MCV(), Quad(), SiegeTank(), Tank(),
            GunTurret(), HeavyFactory(), HighTechFactory(), LightFactory(),
            Starport(), Concrete(), IX(), Palace(), Radar(), Refinery(),
            Silo(), RepairYard(), RocketTurret(), Wall(), Windtrap()]
    ixProvides = []
    palaceProvides = []

# vim:ts=4:sw=4:et
