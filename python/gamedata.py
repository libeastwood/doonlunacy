from GameDataClass import *

class UI_MouseCursor(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MOUSE.SHP"
        self.tiles = normal_row([0, 1, 2, 3, 4, 5, 6])

class BigPlanet(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "FINALE:BIGPLAN.CPS"

class Fame(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:FAME.CPS"

class MapMach(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MAPMACH.CPS"

class Screen(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:SCREEN.CPS"

class Mentat_Atreides(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MENTATA.CPS"

class Mentat_Ordos(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MENTATO.CPS"

class Mentat_Harkonnen(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MENTATH.CPS"

class Mentat_BeneGesserit(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MENTATM.CPS"
        self.palette = "DUNE:BENE.PAL"

class UI_HouseChoiceBackground(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "CPS"
        self.filename = "ENGLISH:HERALD.ENG"

class UI_SelectionBox(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MOUSE.SHP"
        self.index = 6

class UI_Mentat_HeraldHarkonnen(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (9,136, 56, 56)

class UI_Mentat_HeraldAtreides(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (65, 136, 56, 56)

class UI_Mentat_HeraldOrdos(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (121, 136, 56,56)

class UI_MenuBackground(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (0, 0, 50, 50)

class UI_BlankFiller(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = MapMach()
        self.crop = (60, 184, 150, 6)

class UI_TopBorder(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (12, 73, 1, 4)

class UI_BottomBorder(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (12, 72, 1, 4)

class UI_LeftBorder(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (1, 83, 4, 1)

class UI_RightBorder(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (1, 83, 4, 1)

class UI_ButtonUp(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 0

class UI_ButtonUp_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 1

class UI_ButtonDown(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 2

class UI_ButtonDown_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 3

class UI_Mentat(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:BTTN.ENG"
        self.index = 0

class UI_Mentat_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:BTTN.ENG"
        self.index = 1

class UI_MentatYes(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 0

class UI_MentatYes_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 1;

class UI_MentatNo(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        filename = "ENGLISH:MENTAT.ENG"
        self.index = 2

class UI_MentatNo_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 3

class UI_MentatExit(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 4;

class UI_MentatExit_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 5

class UI_MentatProceed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 6

class UI_MentatProceed_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 7;

class UI_MentatRepeat(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 8

class UI_MentatRepeat_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 9

class UI_Options(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:BTTN.ENG"
        self.index = 2

class UI_Options_Pressed(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.type = "SHP";
        self.filename = "ENGLISH:BTTN.ENG";
        self.index = 3;

class UI_Corner1NW(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (64, 9, 8, 8)
        self.colorkey = 0
        self.putpixel = [(6, 7), (7, 6), (7, 7)]

class UI_Corner1NE(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (248, 9, 8, 8)
        self.colorkey = 0;
        self.fillrect = [(0, 5, 1, 2), (0, 6, 2, 2)]

class UI_Corner1SW(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (64, 23, 8, 8)
        self.colorkey = 0;
        self.putpixel = [(6,0), (7,0), (7,1)]

class UI_Corner1SE(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (248, 23, 8, 8)
        self.colorkey = 0;
        self.putpixel = [(0,0), (0,1), (1,0)]

class UI_Corner2NW(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (10, 137, 12, 12)
        self.colorkey = 0;
        self.drawhline = [(8, 11, 11, 0), (10, 9, 11, 0)] 
        self.drawvline = [(11, 11, 4, 0)]

class UI_Corner2NE(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (51, 137, 12, 12)
        self.colorkey = 0;
        self.drawhline = [(2, 10, 6, 0), (2, 11, 6, 0)]
        self.drawvline = [(0, 4, 11, 0), (1, 9, 11, 0), (2, 10, 11, 0)]

class UI_Corner2SW(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (10, 180, 11, 11)
        self.colorkey = 0;
        self.drawhline = [(2, 0, 9, 0), (6, 1, 9, 0)]

class UI_Corner2SE(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (52, 180, 11, 11)
        self.colorkey = 0;
        self.drawhline = [(0, 0, 7, 0)]
        self.drawvline = [(0, 0, 7, 0), (1, 0, 3, 0), (3, 0, 1, 0)]
 
class UI_Corner3NW(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (1, 73, 12, 12)
        self.colorkey = 0
        self.fillrect = [(6, 6, 5, 5)]

class UI_Corner3NE(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (308, 72, 12, 12)
        self.colorkey = 0;
        self.fillrect = [(0, 7, 5, 5)]

class UI_Corner3SW(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (0, 188, 12, 12)
        self.colorkey = 0;
        self.fillrect = [(7, 0, 5, 5)]

class UI_Corner3SE(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = Fame()
        self.crop = (308, 188, 12, 12)
        self.colorkey = 0;
        self.fillrect = [(0, 0, 5, 5)]

class ObjPic_Tank_Base(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(0)

class ObjPic_Tank_Gun(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(5)

class ObjPic_Siegetank_Base(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(10)

class ObjPic_Siegetank_Gun(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(15)

class ObjPic_Devastator_Base(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(20)

class ObjPic_Devastator_Gun(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(25)

class ObjPic_Sonictank_Gun(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(30)

class ObjPic_Launcher_Gun(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(35)

class ObjPic_Quad(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(0)

class ObjPic_Trike(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(5)

class ObjPic_Harvester(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(10)

class ObjPic_Harvester_Sand(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        tiles = harvestersand_row(72) + harvestersand_row(73) + harvestersand_row(74)

class ObjPic_MCV(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(15)

# Something is wrong here, the graphics seems to actually be troopers?
class ObjPic_Carryall(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        tiles = airunit_row(45) + airunit_row(48)

class ObjPic_Frigate(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        tiles = airunit_row(60)

class ObjPic_Ornithopter(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = ornithopter_row(51) + ornithopter_row(52) + ornithopter_row(53)

class ObjPic_Trooper(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = infantry_row(82) + infantry_row(83) + infantry_row(84)

class ObjPic_Infantry(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = infantry_row(73) + infantry_row(74) + infantry_row(75)

class ObjPic_Saboteur(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        tiles = infantry_row(63) + infantry_row(64) + infantry_row(65)

class ObjPic_Sandworm(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([67, 68, 69, 70, 71])

class ObjPic_ConstructionYard(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 17

class ObjPic_Windtrap(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 19

class ObjPic_Refinery(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 21

class ObjPic_Barracks(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 18

class ObjPic_WOR(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 16

class ObjPic_Radar(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 26

class ObjPic_LightFactory(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 12

class ObjPic_Silo(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 25

class ObjPic_HeavyFactory(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 13

class ObjPic_HighTechFactory(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 14

class ObjPic_IX(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 15

class ObjPic_Palace(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 11

# Seems to actually rather be a demolished 4x4 building..?
class ObjPic_RepairYard(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 22

class ObjPic_StarPort(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 20

class ObjPic_GunTurret(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 23

class ObjPic_RocketTurret(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 24

class ObjPic_Wall(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 6
        self.tilepos = (1,1)
        num = 75

class ObjPic_Bullet_SmallRocket(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = rocket_row(35)

class ObjPic_Bullet_MediumRocket(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = rocket_row(20)

class ObjPic_Bullet_LargeRocket(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = rocket_row(40)

class ObjPic_Bullet_Small(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 23

class ObjPic_Bullet_Medium(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 24

class ObjPic_Bullet_Sonic(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 9

class ObjPic_Hit_Gas(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([57, 58, 59, 60, 61])

class ObjPic_Hit_Shell(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([2, 3, 4])

class ObjPic_Hit_ExplosionSmall(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([32, 33, 34, 35, 36])

class ObjPic_Hit_ExplosionMedium1(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([47, 48, 49, 50, 51])

class ObjPic_Hit_ExplosionMedium2(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([52, 53, 54, 55, 56])

class ObjPic_Hit_ExplosionLarge1(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([47, 48, 49, 50, 51])

class ObjPic_Hit_ExplosionLarge2(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([42, 43, 44, 45, 46])

class ObjPic_Hit_ExplosionSmallUnit(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([29, 30, 31])

class ObjPic_DeadInfantryOrig(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 4
        self.tilepos = (1,1)
        self.num = 6

class ObjPic_DeadInfantry(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = ObjPic_DeadInfantryOrig()
        self.colorkey = 0

class ObjPic_Smoke(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([29,30,31])

class ObjPic_SandWormShimmerMask(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 10

class ObjPic_Terrain(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = (124, 209)

class ObjPic_RockDamage(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = (1, 6)

class ObjPic_SandDamage(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([5, 6, 7])

class HideTile(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = (108, 123)


class ObjPic_Terrain_Hidden(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.gcobject = HideTile()
        self.colorkey = 0

class UI_MouseCursor(GameData):
    def __init__(self):
        GameData.__init__(self)
        self.filename = "DUNE:MOUSE.SHP"
        self.tiles = normal_row([0, 1, 2, 3, 4, 5, 6])

gamedata = {
        'BigPlanet' : BigPlanet(),
        'Fame' : Fame(),
        'MapMach' : MapMach(),
        'Screen' : Screen(),
        'Mentat_Atreides' : Mentat_Atreides(),
        'Mentat_Ordos' : Mentat_Ordos(),
        'Mentat_Harkonnen' : Mentat_Harkonnen(),
        'Mentat_BeneGesserit' : Mentat_BeneGesserit(),
        'UI_Mentat_HeraldHarkonnen' : UI_Mentat_HeraldHarkonnen(),
        'UI_Mentat_HeraldAtreides' : UI_Mentat_HeraldAtreides(),
        'UI_Mentat_HeraldOrdos' : UI_Mentat_HeraldOrdos(),
        'UI_MenuBackground' : UI_MenuBackground(),
        'UI_BlankFiller' : UI_BlankFiller(),
        'UI_TopBorder' : UI_TopBorder(),
        'UI_BottomBorder' : UI_BottomBorder(),
        'UI_LeftBorder' : UI_LeftBorder(),
        'UI_RightBorder' : UI_RightBorder(),
        'UI_HouseChoiceBackground' : UI_HouseChoiceBackground(),
        'UI_SelectionBox' : UI_SelectionBox(),
        'UI_ButtonUp' : UI_ButtonUp(),
        'UI_ButtonUp_Pressed' : UI_ButtonUp_Pressed(),
        'UI_ButtonDown' : UI_ButtonDown(),
        'UI_ButtonDown_Pressed' : UI_ButtonDown_Pressed(),
        'UI_Mentat' : UI_Mentat(),
        'UI_Mentat_Pressed' : UI_Mentat_Pressed(),
        'UI_MentatYes' : UI_MentatYes(),
        'UI_MentatYes_Pressed' : UI_MentatYes_Pressed(),
        'UI_MentatNo' : UI_MentatNo(),
        'UI_MentatNo_Pressed' : UI_MentatNo_Pressed(),
        'UI_MentatExit' : UI_MentatExit(),
        'UI_MentatExit_Pressed' : UI_MentatExit_Pressed(),
        'UI_MentatProceed' : UI_MentatProceed(),
        'UI_MentatProceed_Pressed' : UI_MentatProceed_Pressed(),
        'UI_MentatRepeat' : UI_MentatRepeat(),
        'UI_MentatRepeat_Pressed' : UI_MentatRepeat_Pressed(),
        'UI_Options' : UI_Options(),
        'UI_Options_Pressed' : UI_Options_Pressed(),
        'UI_Corner1NW' : UI_Corner1NW(),
        'UI_Corner1NE' : UI_Corner1NE(),
        'UI_Corner1SW' : UI_Corner1SW(),
        'UI_Corner1SE' : UI_Corner1SE(),
        'UI_Corner2NW' : UI_Corner2NW(),
        'UI_Corner2NE' : UI_Corner2NE(),
        'UI_Corner2SW' : UI_Corner2SW(),
        'UI_Corner2SE' : UI_Corner2SE(),
        'UI_Corner3NW' : UI_Corner3NW(),
        'UI_Corner3NE' : UI_Corner3NE(),
        'UI_Corner3SW' : UI_Corner3SW(),
        'UI_Corner3SE' : UI_Corner3SE(),
        'ObjPic_Tank_Base' : ObjPic_Tank_Base(),
        'ObjPic_Tank_Gun' : ObjPic_Tank_Gun(),
        'ObjPic_Siegetank_Base' : ObjPic_Siegetank_Base(),
        'ObjPic_Siegetank_Gun' : ObjPic_Siegetank_Gun(),
        'ObjPic_Devastator_Base' : ObjPic_Devastator_Base(),
        'ObjPic_Devastator_Gun' : ObjPic_Devastator_Gun(),
        'ObjPic_Sonictank_Gun' : ObjPic_Sonictank_Gun(),
        'ObjPic_Launcher_Gun' : ObjPic_Launcher_Gun(),
        'ObjPic_Quad' : ObjPic_Quad(),
        'ObjPic_Trike' : ObjPic_Trike(),
        'ObjPic_Harvester' : ObjPic_Harvester(),
        'ObjPic_Harvester_Sand' : ObjPic_Harvester_Sand(),
        'ObjPic_MCV' : ObjPic_MCV(),
        'ObjPic_Carryall' : ObjPic_Carryall(),
        'ObjPic_Frigate' : ObjPic_Frigate(),
        'ObjPic_Ornithopter' : ObjPic_Ornithopter(),
        'ObjPic_Trooper' : ObjPic_Trooper(),
        'ObjPic_Infantry' : ObjPic_Infantry(),
        'ObjPic_Saboteur' : ObjPic_Saboteur(),
        'ObjPic_Sandworm' : ObjPic_Sandworm(),
        'ObjPic_ConstructionYard' : ObjPic_ConstructionYard(),
        'ObjPic_Windtrap' : ObjPic_Windtrap(),
        'ObjPic_Refinery' : ObjPic_Refinery(),
        'ObjPic_Barracks' : ObjPic_Barracks(),
        'ObjPic_WOR' : ObjPic_WOR(),
        'ObjPic_Radar' : ObjPic_Radar(),
        'ObjPic_LightFactory' : ObjPic_LightFactory(),
        'ObjPic_Silo' : ObjPic_Silo(),
        'ObjPic_HeavyFactory' : ObjPic_HeavyFactory(),
        'ObjPic_HighTechFactory' : ObjPic_HighTechFactory(),
        'ObjPic_IX' : ObjPic_IX(),
        'ObjPic_Palace' : ObjPic_Palace(),
        'ObjPic_RepairYard' : ObjPic_RepairYard(),
        'ObjPic_StarPort' : ObjPic_StarPort(),
        'ObjPic_GunTurret' : ObjPic_GunTurret(),
        'ObjPic_RocketTurret' : ObjPic_RocketTurret(),
        'ObjPic_Wall' : ObjPic_Wall(),
        'ObjPic_Bullet_SmallRocket' : ObjPic_Bullet_SmallRocket(),
        'ObjPic_Bullet_MediumRocket' : ObjPic_Bullet_MediumRocket(),
        'ObjPic_Bullet_LargeRocket' : ObjPic_Bullet_LargeRocket(),
        'ObjPic_Bullet_Small' : ObjPic_Bullet_Small(),
        'ObjPic_Bullet_Medium' : ObjPic_Bullet_Medium(),
        'ObjPic_Bullet_Sonic' : ObjPic_Bullet_Sonic(),
        'ObjPic_Hit_Gas' : ObjPic_Hit_Gas(),
        'ObjPic_Hit_Shell' : ObjPic_Hit_Shell(),
        'ObjPic_Hit_ExplosionSmall' : ObjPic_Hit_ExplosionSmall(),
        'ObjPic_Hit_ExplosionMedium1' : ObjPic_Hit_ExplosionMedium1(),
        'ObjPic_Hit_ExplosionMedium2' : ObjPic_Hit_ExplosionMedium2(),
        'ObjPic_Hit_ExplosionLarge1' : ObjPic_Hit_ExplosionLarge1(),
        'ObjPic_Hit_ExplosionLarge2' : ObjPic_Hit_ExplosionLarge2(),
        'ObjPic_Hit_ExplosionSmallUnit' : ObjPic_Hit_ExplosionSmallUnit(),
        'ObjPic_DeadInfantryOrig' : ObjPic_DeadInfantryOrig(),
        'ObjPic_Smoke' : ObjPic_Smoke(),
        'ObjPic_SandWormShimmerMask' : ObjPic_SandWormShimmerMask(),
        'ObjPic_Terrain' : ObjPic_Terrain(),
        'ObjPic_RockDamage' : ObjPic_RockDamage(),
        'ObjPic_SandDamage' : ObjPic_SandDamage(),
        'HideTile' : HideTile(),
        'UI_MouseCursor' : UI_MouseCursor(),
        }

# vim:ts=4:sw=4:et
