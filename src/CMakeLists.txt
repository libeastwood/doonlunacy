INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

SET( DOONLUNACY_BASE_SRCS
  Application.cpp
  Settings.cpp
  FontManager.cpp
  
  DataCache.cpp
  Gfx.cpp
  GCObject.cpp
  houses.cpp		# just for colors, hope there will be a better place for these (otpetrik)
  Strings.cpp
  Log.cpp
  ResMan.cpp
  SoundPlayer.cpp
  TerrainClass.cpp
  MapClass.cpp
  gui2/MapWidget.cpp
  MapGenerator.cpp
  mmath.cpp
  MapSeed.cpp
  PlayerClass.cpp
  ObjectClass.cpp
  #structures/BarracksClass.cpp
  #structures/ConstructionYardClass.cpp
  #structures/GunTurretClass.cpp
  #structures/HeavyFactoryClass.cpp
  #structures/HighTechFactoryClass.cpp
  #structures/IXClass.cpp
  #structures/LightFactoryClass.cpp
  #structures/PalaceClass.cpp
  #structures/RadarClass.cpp
  #structures/RefineryClass.cpp
  #structures/RepairYardClass.cpp
  #structures/RocketTurretClass.cpp
  #structures/SiloClass.cpp
  #structures/StarPortClass.cpp
  structures/StructureClass.cpp
  structures/WallClass.cpp
  structures/SiloClass.cpp
  structures/ConstructionYardClass.cpp
  units/AirUnit.cpp
  units/GroundUnit.cpp
  units/InfantryClass.cpp
  units/UnitClass.cpp
  GameMan.cpp
  PriorityQ.cpp
  ObjectTree.cpp
  ObjectPointer.cpp
  BulletClass.cpp
)

SET( DOONLUNACY_STATE_SRCS
  #CampaignMenu.cpp
  states/Editor.cpp
  states/HouseChoiceMenu.cpp
  states/HouseChoiceInfoMenu.cpp
  states/CutSceneState2.cpp
  states/GameMenu.cpp
  states/MenuBase.cpp
  states/MainMenu.cpp
  states/MainMenuBase.cpp
  states/MentatMenu.cpp
  states/MentatInfoMenu.cpp
  states/OptionsMenu.cpp
  states/SingleMenu.cpp
  states/SkirmishMenu.cpp
  states/State.cpp
  states/TopLevelState.cpp
)

SET( DOONLUNACY_GUI_SRCS
  gui2/Widget.cpp
  gui2/Container.cpp
  gui2/Button.cpp
  gui2/VBox.cpp
  gui2/Label.cpp
  gui2/Frame.cpp
  gui2/HScale.cpp
)

SET( DOONLUNACY_SRCS
  main.cpp
  ${DOONLUNACY_BASE_SRCS}
  ${DOONLUNACY_STATE_SRCS}
  ${DOONLUNACY_GUI_SRCS}
)

SET( PAKVIEW_SRCS
  ${DOONLUNACY_BASE_SRCS}
  ${DOONLUNACY_GUI_SRCS}
)
  
ADD_EXECUTABLE(../doonlunacy ${DOONLUNACY_SRCS})

TARGET_LINK_LIBRARIES(../doonlunacy ${LIBEASTWOOD_LIBRARIES} )
TARGET_LINK_LIBRARIES(../doonlunacy boost_signals-mt )
TARGET_LINK_LIBRARIES(../doonlunacy boost_filesystem-mt )
TARGET_LINK_LIBRARIES(../doonlunacy config++ )
TARGET_LINK_LIBRARIES(../doonlunacy boost_python )
TARGET_LINK_LIBRARIES(../doonlunacy ${PYTHON_LIBRARY} )



#ADD_EXECUTABLE(../pakview ${PAKVIEW_SRCS})

