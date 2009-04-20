from CutsceneClass import *
from sounds import *

intro = []

scene = CutsceneClass()
scene.filename = "INTRO:WESTWOOD.WSA"
scene.palette = "INTRO:WESTWOOD.PAL"
scene.fadeOut = True
scene.song = 0
scene.fps = 5.7
scene.hold = 20
intro.append(scene)

scene = CutsceneClass()
scene.fadeOut = True
scene.hold = 15
scene.fps = 7.3
scene.text = [ (0, "and") ]
scene.textPosition = (0,0)
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:VIRGIN.CPS"
scene.fadeOut = True
scene.fps = 7.7
scene.hold = 15
intro.append(scene)

scene = CutsceneClass()
scene.fadeOut = True
scene.hold = 10
scene.fps = 7.2
scene.song = 1
scene.text = [(0, 1)]
scene.textPosition = (0,0)
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO1.WSA"
scene.fps = 9.3
scene.fadeOut = True
scene.hold = 35
scene.loop = (1,1,0,20)
scene.sound = {
        70 : (Intro_Dune()),
        85 : (Intro_TheBuilding(), Intro_OfADynasty())
        }
scene.text = [(70, "a.k.a. Doon Lunacy")]
scene.textPosition = (0,30)
scene.animPosition = (0, -100)
scene.textFadeIn = False
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO2.WSA"
scene.fps = 5.4
scene.fadeIn = True
scene.fadeOut = True
scene.hold = 20
scene.sound = {
        10 : (Intro_ThePlanetArrakis(), Intro_KnownAsDune())
        }
scene.text = [(10, 3)]
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO3.WSA"  
scene.fps = 5.4
fade_in = True
scene.fadeOut = True
scene.hold = 35
scene.text = [(0, 4), (33, 5)]
scene.sound = {
        0 : (Intro_LandOfSand()),
        33 : (Intro_Home(), Intro_OfTheSpice(), Intro_Melange())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO9.WSA"
scene.fps = 5.4
fade_in = True
scene.fadeOut = True
scene.hold = 2
scene.text = [(0, 6), (32, 7)]
scene.sound = {
        0 : (Intro_TheSpice(), Intro_Controls(), Intro_TheEmpire()),
        18 : (Intro_Clank()),
        32 : (Intro_WhoEver(), Intro_ControlsDune(), Intro_ControlsTheSpice()),
        53 : (Intro_Brakes_2p()),
        61 : (Intro_Clank())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO10.WSA"
scene.fps = 3.0
scene.hold = 25
scene.text = [(0, 8)]
scene.sound = {
        0 : (Intro_TheEmperor(), Intro_HasProposedAChallenge(), Intro_ToEachOfTheHouses())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO11.WSA"  
scene.fadeOut = True
scene.fps = 8.6
scene.text = [(0, 9), (61, 10), (83, 11)]
scene.textColor = 208
scene.loop = (44, 0, 0, 16)
scene.sound = {
        0 : (Intro_TheHouse(), Intro_ThatProduces(), Intro_TheMostSpice(), Intro_WillControlDune()),
        61 : (Intro_ThereAreNoSet(), Intro_Territories()),
        83 : (Intro_AndNo(), Intro_RulesOfEngagement())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO4.WSA"
scene.fps = 4.1
scene.fadeOut = True
scene.text = [(11, 12)]
scene.sound = {
        11 : (Intro_VastArmies(), Intro_HasArrived())
        }
intro.append(scene)

scene = CutsceneClass()
fade_in = True
scene.fps = 5.8
scene.hold = 38
scene.text = [(0, 13)]
scene.textPosition = (0,0)
scene.textFadeIn = False	
scene.sound = {
        0 : (Intro_AndNow(), Intro_3Houses(), Intro_ForControl(), Intro_OfDune())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO6.WSA"
scene.fps = 10.8
scene.fadeOut = True
scene.text = [(0, 14)]
scene.sound = {
        0  : (Intro_TheNobleAtreides()),
        31 : (Intro_Glass()),
        32 : (Intro_Glass()),
        33 : (Intro_Glass()),
        63 : (Intro_Glass()),
        64 : (Intro_Glass()),
        65 : (Intro_Glass())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO7A.WSA"
scene.fps = 4.4
scene.text = [(0, 15)]
scene.sound = {
        0  : (Intro_TheInsideous(),  Intro_Ordos()),
        2  : (Intro_Missile_8()),
        7  : (Intro_Missile_8()),
        26 : (Intro_Missile_8())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO7B.WSA" 
scene.fps = 7.9
scene.fadeOut = True
scene.continuation = True
scene.hold = 5
scene.sound = {
        10 : (Intro_Missile_8())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO8A.WSA"
scene.fps = 4.0
scene.text = [(0, 16)]
scene.sound = {
        0 : (Intro_AndThe(), Intro_EvilHarkonnen()),
        5 : (Sound_Gun()),
        8 : (Sound_Gun())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO8B.WSA"
scene.fps = 15.0
scene.continuation = True
scene.sound = {
        1 : (Sound_Gun())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO8C.WSA"
scene.fps = 6.4
scene.fadeOut = True
continuation = True
scene.sound = {
        16 : (Sound_ExplosionSmall()),
        25 : (Sound_ExplosionMedium())
        }
intro.append(scene)

scene = CutsceneClass()
scene.filename = "INTRO:INTRO5.WSA"
scene.fps = 4.6
scene.fadeOut = True
scene.loop = (3, 0, 15, 0)
scene.text = [(0, 17)]
scene.sound = {
        0 : (Intro_OnlyOneHouse(), Intro_WillPrevail())
        }
intro.append(scene)

scene = CutsceneClass()
scene.fadeOut = True
scene.fps = 4.9
scene.hold = 22
scene.text = [(0, 18)]
scene.textPosition = (0,0)
scene.textFadeIn = False	
scene.sound = {
        0 : (Intro_Your(), Intro_BattleForDune(), Intro_Begins())
        }
intro.append(scene)

scene = CutsceneClass()
scene.fadeOut = True
scene.hold = 50
scene.text = [(0, 19)]
scene.textPosition = (0,0)
scene.textFadeIn = False
scene.sound = {
        0 : (Intro_Now())
        }
intro.append(scene)

# vim:ts=4:sw=4:et

