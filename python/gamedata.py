from GameDataClass import *

class UI_MouseCursor(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MOUSE.SHP"
        self.tiles = normal_row([0, 1, 2, 3, 4, 5, 6])

class BigPlanet(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "FINALE:BIGPLAN.CPS"

class Fame(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:FAME.CPS"

class MapMach(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MAPMACH.CPS"

class Screen(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:SCREEN.CPS"

class Mentat_Atreides(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MENTATA.CPS"

class Mentat_Ordos(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MENTATO.CPS"

class Mentat_Harkonnen(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MENTATH.CPS"

class Mentat_BeneGesserit(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MENTATM.CPS"
        self.palette = "DUNE:BENE.PAL"

class UI_HouseChoiceBackground(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "CPS"
        self.filename = "ENGLISH:HERALD.ENG"

class UI_HeraldColoredHarkonnen(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = UI_HouseChoiceBackground()
        self.crop = (214, 53, 90, 95)

class UI_HeraldColoredAtreides(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = UI_HouseChoiceBackground()
        self.crop = (20, 53, 90, 95)

class UI_HeraldColoredOrdos(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = UI_HouseChoiceBackground()
        self.crop = (116, 53, 90, 95)

class UI_SelectionBox(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MOUSE.SHP"
        self.index = 6

class UI_Mentat_HeraldHarkonnen(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (9,136, 56, 56)

class UI_Mentat_HeraldAtreides(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (65, 136, 56, 56)

class UI_Mentat_HeraldOrdos(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (121, 136, 56,56)

class UI_MenuBackground(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (0, 1, 63, 67)

class UI_BlankFiller(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = MapMach()
        self.crop = (60, 184, 150, 6)

class UI_TopBorder(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (12, 73, 1, 4)

class UI_BottomBorder(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (12, 72, 1, 4)

class UI_LeftBorder(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (1, 83, 4, 1)

class UI_RightBorder(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (1, 83, 4, 1)

class UI_ButtonUp(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 0

class UI_ButtonUp_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 1

class UI_ButtonDown(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 2

class UI_ButtonDown_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:CHOAM.ENG"
        self.index = 3

class UI_Mentat(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:BTTN.ENG"
        self.index = 0

class UI_Mentat_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:BTTN.ENG"
        self.index = 1

class UI_MentatYes(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 0

class UI_MentatYes_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 1;

class UI_MentatNo(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 2

class UI_MentatNo_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 3

class UI_MentatExit(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 4;

class UI_MentatExit_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 5

class UI_MentatProceed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 6

class UI_MentatProceed_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 7;

class UI_MentatRepeat(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 8

class UI_MentatRepeat_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:MENTAT.ENG"
        self.index = 9

class UI_Options(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP"
        self.filename = "ENGLISH:BTTN.ENG"
        self.index = 2

class UI_Options_Pressed(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.type = "SHP";
        self.filename = "ENGLISH:BTTN.ENG";
        self.index = 3;

class UI_Corner1NW(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (64, 9, 8, 8)
        self.colorkey = 0
        self.putpixel = [(6, 7), (7, 6), (7, 7)]

class UI_Corner1NE(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (248, 9, 8, 8)
        self.colorkey = 0;
        self.fillrect = [(0, 5, 1, 2), (0, 6, 2, 2)]

class UI_Corner1SW(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (64, 23, 8, 8)
        self.colorkey = 0;
        self.putpixel = [(6,0), (7,0), (7,1)]

class UI_Corner1SE(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (248, 23, 8, 8)
        self.colorkey = 0;
        self.putpixel = [(0,0), (0,1), (1,0)]

class UI_Corner2NW(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (10, 137, 12, 12)
        self.colorkey = 0;
        self.drawhline = [(8, 11, 11, 0), (10, 9, 11, 0)] 
        self.drawvline = [(11, 11, 4, 0)]

class UI_Corner2NE(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (51, 137, 12, 12)
        self.colorkey = 0;
        self.drawhline = [(2, 10, 6, 0), (2, 11, 6, 0)]
        self.drawvline = [(0, 4, 11, 0), (1, 9, 11, 0), (2, 10, 11, 0)]

class UI_Corner2SW(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (10, 180, 11, 11)
        self.colorkey = 0;
        self.drawhline = [(2, 0, 9, 0), (6, 1, 9, 0)]

class UI_Corner2SE(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (52, 180, 11, 11)
        self.colorkey = 0;
        self.drawhline = [(0, 0, 7, 0)]
        self.drawvline = [(0, 0, 7, 0), (1, 0, 3, 0), (3, 0, 1, 0)]
 
class UI_Corner3NW(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (1, 73, 12, 12)
        self.colorkey = 0
        self.fillrect = [(6, 6, 5, 5)]

class UI_Corner3NE(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (308, 72, 12, 12)
        self.colorkey = 0;
        self.fillrect = [(0, 7, 5, 5)]

class UI_Corner3SW(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (0, 188, 12, 12)
        self.colorkey = 0;
        self.fillrect = [(7, 0, 5, 5)]

class UI_Corner3SE(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Fame()
        self.crop = (308, 188, 12, 12)
        self.colorkey = 0;
        self.fillrect = [(0, 0, 5, 5)]

class UI_CreditsDigits(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:SHAPES.SHP"
        self.tiles = normal_row([2,3,4,5,6,7,8,9,10,11])

class UI_Credits(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Screen()
        self.crop = (201, 2, 54, 12)

class UI_CreditsCounter(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Screen()
        self.crop = (256, 1, 64, 15)

class UI_RoundBall(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = Screen()
        self.crop = (240, 39, 14, 13)

class ObjPic_Tank_Base(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(0)

class ObjPic_Tank_Gun(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(5)

class ObjPic_Siegetank_Base(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(10)

class ObjPic_Siegetank_Gun(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(15)

class ObjPic_Devastator_Base(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(20)

class ObjPic_Devastator_Gun(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(25)

class ObjPic_Sonictank_Gun(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(30)

class ObjPic_Launcher_Gun(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS2.SHP"
        self.tiles = groundunit_row(35)

class ObjPic_Quad(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(0)

class ObjPic_Trike(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(5)

class ObjPic_Harvester(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(10)

class ObjPic_Harvester_Sand(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        tiles = harvestersand_row(72) + harvestersand_row(73) + harvestersand_row(74)

class ObjPic_MCV(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = groundunit_row(15)

# Something is wrong here, the graphics seems to actually be troopers?
class ObjPic_Carryall(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        tiles = airunit_row(45) + airunit_row(48)

class ObjPic_Frigate(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        tiles = airunit_row(60)

class ObjPic_Ornithopter(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = ornithopter_row(51) + ornithopter_row(52) + ornithopter_row(53)

class ObjPic_Trooper(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = infantry_row(82) + infantry_row(83) + infantry_row(84)

class ObjPic_Infantry(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = infantry_row(73) + infantry_row(74) + infantry_row(75)

class ObjPic_Saboteur(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        tiles = infantry_row(63) + infantry_row(64) + infantry_row(65)

class ObjPic_Sandworm(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([67, 68, 69, 70, 71])

class ObjPic_ConstructionYard(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 17

class ObjPic_Windtrap(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 19

class ObjPic_Refinery(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 21

class ObjPic_Barracks(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 18

class ObjPic_WOR(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 16

class ObjPic_Radar(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 26

class ObjPic_LightFactory(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 12

class ObjPic_Silo(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 25

class ObjPic_HeavyFactory(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 13

class ObjPic_HighTechFactory(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 14

class ObjPic_IX(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 15

class ObjPic_Palace(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 11

# Seems to actually rather be a demolished 4x4 building..?
class ObjPic_RepairYard(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 22

class ObjPic_StarPort(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 20

class ObjPic_GunTurret(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 23

class ObjPic_RocketTurret(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 24

class ObjPic_Wall(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 6

class ObjPic_Bullet_SmallRocket(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = rocket_row(35)

class ObjPic_Bullet_MediumRocket(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = rocket_row(20)

class ObjPic_Bullet_LargeRocket(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS.SHP"
        self.tiles = rocket_row(40)

class ObjPic_Bullet_Small(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 23

class ObjPic_Bullet_Medium(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 24

class ObjPic_Bullet_Sonic(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 9

class ObjPic_Hit_Gas(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([57, 58, 59, 60, 61])

class ObjPic_Hit_Shell(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([2, 3, 4])

class ObjPic_Hit_ExplosionSmall(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([32, 33, 34, 35, 36])

class ObjPic_Hit_ExplosionMedium1(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([47, 48, 49, 50, 51])

class ObjPic_Hit_ExplosionMedium2(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([52, 53, 54, 55, 56])

class ObjPic_Hit_ExplosionLarge1(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([47, 48, 49, 50, 51])

class ObjPic_Hit_ExplosionLarge2(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([42, 43, 44, 45, 46])

class ObjPic_Hit_ExplosionSmallUnit(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([29, 30, 31])

class ObjPic_DeadInfantryOrig(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.mapindex = 4

class ObjPic_DeadInfantry(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = ObjPic_DeadInfantryOrig()
        self.colorkey = 0

class ObjPic_Smoke(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([29,30,31])

class ObjPic_SandWormShimmerMask(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.index = 10

class ObjPic_SpiceBloom(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = 10

class ObjPic_Terrain(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = 9

class ObjPic_ConcreteSlab(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = 8

class ObjPic_AllTerrain(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = [ObjPic_ConcreteSlab(), ObjPic_Terrain(), ObjPic_SpiceBloom()]

class ObjPic_RockDamage(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = 1

class ObjPic_SandDamage(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:UNITS1.SHP"
        self.tiles = normal_row([5, 6, 7])

class HideTile(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:ICON.ICN"
        self.map = "DUNE:ICON.MAP"
        self.row = 7

class ObjPic_Terrain_Hidden(GameDataMod):
    def __init__(self):
        GameDataMod.__init__(self)
        self.gamedata = HideTile()
        self.colorkey = 0

class UI_MouseCursor(GameDataConst):
    def __init__(self):
        GameDataConst.__init__(self)
        self.filename = "DUNE:MOUSE.SHP"
        self.tiles = normal_row([0, 1, 2, 3, 4, 5, 6])

# vim:ts=4:sw=4:et
