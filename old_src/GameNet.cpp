#include "GameNet.h"

#include "DuneConstants.h"
#include "ObjectClass.h"
#include "Net.h"
#include "globals.h"
#include "MapClass.h"
#include "Items.h"
#include "gui/Graphics.h"
#include "Settings.h"
#include "PlayerClass.h"

void doClient()	//check for info from server
{
	char		msg[MAX_MSGLENGTH];
	int			numReady,
				numConverted,
				objectID, playerID, itemID, destObjectID,
				lx, ly, dx, dy, angle, turretAngle, forced;
	ObjectClass	*destObject, *object;

	while ((numReady = SDLNet_CheckSockets(socketSet, 0)) > 0)
	{
		if(SDLNet_SocketReady(server))	//server saying something

		{

			if (SDLNet_TCP_Recv(server, msg, MAX_MSGLENGTH)==MAX_MSGLENGTH)

			{

				numReady--;

				msg[MAX_MSGLENGTH-1] = '\0';

				//showMessage(msg);

				switch (msg[0])

				{

					case MSG_ATTACK:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d,%d", &objectID, &destObjectID, &lx, &ly, &dx, &dy);

						if ((numConverted == 6) && map->cellExists(lx, ly) && map->cellExists(dx, dy) && (objectID != NONE) && (destObjectID != NONE))

						{

							destObject = NULL;

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object)

							{

								COORDTYPE temp = {lx, ly};

								if (object->isAUnit())

								{

									if ((!((UnitClass*)object)->isMoving()

										&& (object->getLocation()->x == object->getDestination()->x)

										&& (object->getLocation()->y == object->getDestination()->y)) 

										|| (blockDistance(object->getLocation(), &temp) > 5))

									{

										//make sure unit is not held by something already

										if (object->hasATarget())	//if so eject them first

										{

											ObjectClass* target = object->getTarget();

											switch(target->getItemID())

											{

											case Structure_Refinery:

												if (!((RefineryClass*)target)->isFree() && (((RefineryClass*)target)->getHarvester() == object))

													((RefineryClass*)target)->deployHarvester();

												break;

											case Structure_RepairYard:

												if (!((RepairYardClass*)target)->isFree() && (((RepairYardClass*)target)->getRepairUnit() == object))

													((RepairYardClass*)target)->deployRepairUnit();

												break;

											case Unit_Carryall:

												((Carryall*)target)->deployUnit((UnitClass*)object);

												break;

											case Unit_Frigate:

												//((Unit_Frigate*)target)->deployUnit((UnitClass*)object);

												break;

											default:

												break;

											}

										}



										map->removeObjectFromMap(object);

										object->setLocation(lx, ly);

										((UnitClass*)object)->resetStartPoint();



									}

									destObject = map->findObjectWidthID(destObjectID, dx, dy);

									object->setTarget(destObject);

									object->setDestination(destObject->getLocation());

									if (object->isAUnit())

									{

										((UnitClass*)object)->clearPath();

										((UnitClass*)object)->clearNextSpotFound();

									}

								}

							}

							else

								showMessage("ERROR: MSG_ATTACK - invalid message received from server.");

						}

						else

						{

							showMessage("ERROR: MSG_ATTACK - invalid message received from server: message incorrectly formatted");

							showMessage(msg);

							fprintf(stderr, "ERROR: MSG_ATTACK - invalid message received from server: message incorrectly formatted");

							fprintf(stderr, msg);

						}

						break;

					case MSG_BUILDCOMMAND:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d", &objectID, &lx, &ly, &dx, &dy);

						if ((numConverted == 5) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAStructure() && ((StructureClass*)object)->isABuilder())

								((BuilderClass*)object)->buildNum(dx ? true : false, dy);

							else

								showMessage("ERROR: MSG_BUILDCOMMAND - invalid message received from server: wrong object type");

						}

						else

							showMessage("ERROR: MSG_BUILDCOMMAND - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_BUILDCOMPLETE:

						numConverted = sscanf(&msg[1], "%d,%d,%d", &objectID, &lx, &ly);

						if ((numConverted == 3) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAStructure() && ((StructureClass*)object)->isABuilder())

								((BuilderClass*)object)->setWaitingToPlace();

							else

								showMessage("ERROR: MSG_BUILDCOMPLETE - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_BUILDCOMPLETE - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_CANCELCOMMAND:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d", &objectID, &lx, &ly, &dx, &dy);

						if ((numConverted == 5) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAStructure() && ((StructureClass*)object)->isABuilder())

								((BuilderClass*)object)->cancelSelection(dx ? true : false, dy);

							else

								showMessage("ERROR: MSG_CANCELCOMMAND - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_CANCELCOMMAND - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_CREATEOBJECT:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d", &playerID, &itemID, &lx, &ly, &angle);

						if ((numConverted == 5) && (playerID >= 0) && (playerID < MAX_PLAYERS))

						{

							UnitClass* unit = (UnitClass*)player[playerID]->createUnit(itemID);

							if (unit)

							{

								if (map->cellExists(lx, ly))

								{

									COORDTYPE pos = {lx, ly};

									unit->deploy(&pos);

								}



								if ((angle >= 0) && (angle < NUM_ANGLES))

									unit->setAngle(angle);

							}

						}

						else

							showMessage("ERROR: MSG_CREATEOBJECT - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_CONSISTANCY:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d", &objectID, &lx, &ly, &angle, &turretAngle);

						if ((numConverted == 5) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object)

							{

								COORDTYPE temp = {lx, ly};

								if (object->isAUnit() && (!((UnitClass*)object)->isMoving() || blockDistance(object->getLocation(), &temp) > 5))

								{

									UnitClass* unit = (UnitClass*)object;



									//make sure unit is not held by something already

									if (object->hasATarget())	//if so eject them first

									{

										ObjectClass* target = object->getTarget();

										switch(target->getItemID())

										{

										case Structure_Refinery:

											if (!((RefineryClass*)target)->isFree() && (((RefineryClass*)target)->getHarvester() == object))

												((RefineryClass*)target)->deployHarvester();

											break;

										case Structure_RepairYard:

											if (!((RepairYardClass*)target)->isFree() && (((RepairYardClass*)target)->getRepairUnit() == object))

												((RepairYardClass*)target)->deployRepairUnit();

											break;

										case Unit_Carryall:

											((Carryall*)target)->deployUnit((UnitClass*)object);

											break;

										case Unit_Frigate:

											((Frigate*)target)->deployUnit((UnitClass*)object);

											break;

										default:

											break;

										}

									}



									map->removeObjectFromMap(object);

									object->setLocation(lx, ly);

									unit->setAngle(angle);

									if (unit->isTurreted() && (turretAngle != 0))

										((TankClass*)unit)->setTurretAngle(turretAngle);

								}

							}

							else

								showMessage("ERROR: MSG_CONSISTANCY - invalid message received from server.");

						}

						else

						{

							showMessage("ERROR: MSG_CONSISTANCY - invalid message received from server: message incorrectly formatted");

							showMessage(msg);

							fprintf(stderr, "ERROR: MSG_CONSISTANCY - invalid message received from server: message incorrectly formatted");

							fprintf(stderr, msg);

						}

						break;

					case MSG_DEVASTATE:

						numConverted = sscanf(&msg[1], "%d,%d,%d", &objectID, &lx, &ly);

						if ((numConverted == 3) && (map->cellExists(lx, ly) || (lx == NONE)) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && (object->getItemID() == Unit_Devastator))

								((DevastatorClass*)object)->startDevastate();

							else

								showMessage("ERROR: MSG_DEVASTATE - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_DEVASTATE - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_DEVIATE:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &objectID, &lx, &ly, &playerID);

						if ((numConverted == 4) && (map->cellExists(lx, ly) || (lx == NONE)) && (objectID != NONE) && ((playerID >= 0) && (playerID < MAX_PLAYERS) && (player[playerID] != NULL)))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAUnit())

								((UnitClass*)object)->deviate(player[playerID]);

							else

								showMessage("ERROR: MSG_DEVIATE - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_DEVIATE - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_MESSAGE:

					{

						char	inMessage[MAX_MSGLENGTH + MAX_NAMELENGTH + 2];



						playerID = (int)msg[1];

						if (((playerID >= 0) && (playerID < MAX_PLAYERS)) && (player[playerID] != NULL))

						{

							sprintf(inMessage, "%s: %s", settings.playerName[playerID], &msg[2]);

							showMessage(inMessage);

						}

						else

							showMessage("ERROR: MSG_MESSAGE - invalid message received from server: message incorrectly formatted");

						break;

					}

					case MSG_OBJECTCOMMAND:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d,%d,%d", &objectID, &destObjectID, &lx, &ly, &dx, &dy, &forced);

						if ((numConverted == 7) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							destObject = NULL;

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object)

							{

								COORDTYPE temp = {lx, ly};

								//showMessage("object command received");

								if (object->isAUnit() && (!((UnitClass*)object)->isMoving() || blockDistance(object->getLocation(), &temp) > 2))

								{

									//make sure unit is not held by something already

									if (object->hasATarget())	//if so eject them first

									{

										ObjectClass* target = object->getTarget();

										switch(target->getItemID())

										{

										case Structure_Refinery:

											if (!((RefineryClass*)target)->isFree() && (((RefineryClass*)target)->getHarvester() == object))

												((RefineryClass*)target)->deployHarvester();

											break;

										case Structure_RepairYard:

											if (!((RepairYardClass*)target)->isFree() && (((RepairYardClass*)target)->getRepairUnit() == object))

												((RepairYardClass*)target)->deployRepairUnit();

											break;

										case Unit_Carryall:

											((Carryall*)target)->deployUnit((UnitClass*)object);

											break;

										case Unit_Frigate:

											((Frigate*)target)->deployUnit((UnitClass*)object);

											break;

										default:

											break;

										}

									}



									map->removeObjectFromMap(object);

									object->setLocation(lx, ly);

									((UnitClass*)object)->resetStartPoint();



								}

								if (destObjectID != NONE)

									destObject = map->findObjectWidthID(destObjectID, dx, dy);

								object->setTarget(destObject);

								if (destObject != NULL)

									object->setDestination(destObject->getLocation());

								object->setDestination(dx, dy);

									

								if (object->isAUnit())

								{

									((UnitClass*)object)->clearPath();

									if (destObject != NULL)

										((UnitClass*)object)->setGuardPoint(destObject->getLocation());

									else if (dx != NONE)

										((UnitClass*)object)->setGuardPoint(dx, dy);

									((UnitClass*)object)->clearNextSpotFound();

								}

								object->setForced(forced == 1);

							}

							else

								showMessage("ERROR: MSG_OBJECTCOMMAND - invalid message received from server.");

						}

						else

						{

							showMessage("ERROR: MSG_OBJECTCOMMAND - invalid message received from server: message incorrectly formatted");

							showMessage(msg);

							fprintf(stderr, "ERROR: MSG_OBJECTCOMMAND - invalid message received from server: message incorrectly formatted");

							fprintf(stderr, msg);

						}

						break;

					case MSG_OBJECTDESTROY:

						numConverted = sscanf(&msg[1], "%d,%d,%d", &objectID, &lx, &ly);

						if ((numConverted == 3) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object)

							{

								//make sure unit is not held by something already

								if (object->isAUnit() && object->hasATarget())	//if so eject them first

								{

									ObjectClass* target = object->getTarget();

									switch(target->getItemID())

									{

									case Structure_Refinery:

										if (!((RefineryClass*)target)->isFree() && (((RefineryClass*)target)->getHarvester() == object))

											((RefineryClass*)target)->deployHarvester();

										break;

									case Structure_RepairYard:

										if (!((RepairYardClass*)target)->isFree() && (((RepairYardClass*)target)->getRepairUnit() == object))

											((RepairYardClass*)target)->deployRepairUnit();

										break;

									case Unit_Carryall:

										((Carryall*)target)->deployUnit((UnitClass*)object);

										break;

									case Unit_Frigate:

										((Frigate*)target)->deployUnit((UnitClass*)object);

										break;

									default:

										break;

									}

								}

								object->destroy();

							}

							else

								showMessage("ERROR: MSG_OBJECTDESTROY - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_OBJECTDESTROY - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_SETATTACKMODE:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &objectID, &lx, &ly, &angle);

						if ((numConverted == 4) && (map->cellExists(lx, ly) || (lx == NONE)) && (objectID != NONE) && ((angle >= 0) && (angle < NUM_ANGLES)))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAUnit())

								((UnitClass*)object)->setAttackMode((ATTACKTYPE)angle);

						}

						else

							showMessage("ERROR: MSG_STARPORTORDER - invalid message received from client: message incorrectly formatted");

						break;

					case MSG_PLACESTRUCTURE:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d,%d,%d,%d", &playerID, &objectID, &lx, &ly, &itemID, &dx, &dy, &angle);

						if ((numConverted == 8) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object

								&& ((object->isAStructure() && ((StructureClass*)object)->isABuilder())

									|| ((object->getItemID() == Unit_MCV) && (itemID == Structure_ConstructionYard))

								&& map->cellExists(lx, ly) && (playerID >= 0) && (playerID < MAX_PLAYERS)))

							{

								StructureClass* structure = (StructureClass*)player[playerID]->placeStructure(objectID, lx, ly, itemID, dx, dy);

								if (structure)

									structure->setJustPlaced();

							}

							else

								showMessage("ERROR: MSG_PLACESTRUCTURE - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_PLACESTRUCTURE - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_STARPORTORDER:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &playerID, &objectID, &lx, &ly);

						if ((numConverted == 4) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAStructure() && (object->getItemID() == Structure_StarPort)

								&& map->cellExists(lx, ly) && (playerID >= 0) && (playerID < MAX_PLAYERS))

								((StarPortClass*)object)->placeOrder();

							else

								showMessage("ERROR: MSG_STARPORTORDER - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_STARPORTORDER - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_STARPORTCANCEL:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &playerID, &objectID, &lx, &ly);

						if ((numConverted == 4) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAStructure() && (object->getItemID() == Structure_StarPort)

								&& map->cellExists(lx, ly) && (playerID >= 0) && (playerID < MAX_PLAYERS))

								((StarPortClass*)object)->cancelOrder();

							else

								showMessage("ERROR: MSG_STARPORTCANCEL - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_STARPORTCANCEL - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_PALACESPECIAL:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d,%d", &playerID, &objectID, &lx, &ly, &dx, &dy);

						if ((numConverted == 6) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAStructure() && (object->getItemID() == Structure_Palace))

							{

								((PalaceClass*)object)->setDeathhandOff(dx, dy);

								((PalaceClass*)object)->doSpecial();

							}

							else

								showMessage("ERROR: MSG_PALACESPECIAL - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_PALACESPECIAL - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_PALACEENABLE:

						numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &playerID, &objectID, &lx, &ly);

						if ((numConverted == 4) && map->cellExists(lx, ly) && (objectID != NONE))

						{

							object = map->findObjectWidthID(objectID, lx, ly);

							if (object && object->isAStructure() && (object->getItemID() == Structure_Palace))

								((PalaceClass*)object)->enableSpecial(true);

							else

								showMessage("ERROR: MSG_PALACEENABLE - invalid message received from server.");

						}

						else

							showMessage("ERROR: MSG_PALACEENABLE - invalid message received from server: message incorrectly formatted");

						break;

					case MSG_QUIT:
						break;
					default:
						break;
				}
			}
			else
			{
				showMessage("Disconnected from server.");

				//close the server socket

				SDLNet_TCP_DelSocket(socketSet, server);

				SDLNet_TCP_Close(server);

				settings.playerType = SINGLE;
			}
		}
	}

	if (numReady == -1)
	{//must be disconnected
		showMessage("Disconnected from server.");

		//close the server socket

		SDLNet_TCP_DelSocket(socketSet, server);

		SDLNet_TCP_Close(server);

		settings.playerType = SINGLE;
	}
}

void doServer()	//check for info from clients
{

	char	msg[MAX_MSGLENGTH+50];

	int		i,
			forced,
			numReady,
			numConverted,
			objectID, playerID, itemID, destObjectID, lx, ly, dx, dy, angle;
	ObjectClass	*destObject, *object;
	COORDTYPE	temp;
	TCPsocket	newClient;

	while ((numReady = SDLNet_CheckSockets(socketSet, 0) > 0))
	{
		if(SDLNet_SocketReady(server))	//someone is trying to join in game

		{

			numReady--;

			newClient = SDLNet_TCP_Accept(server);
			showMessage("player trying to join game in progress.\n");
			SDLNet_TCP_Close(newClient);	//they can't join a game in progress
		}

		for (i = 1; numReady && (i < MAX_PLAYERS); i++)

		{

			if (SDLNet_SocketReady(sockets[i]))

			{

				numReady--;

				if (SDLNet_TCP_Recv(sockets[i], msg, MAX_MSGLENGTH)==MAX_MSGLENGTH)

				{

					msg[MAX_MSGLENGTH-1] = '\0';

					//showMessage(msg);

					switch (msg[0])

					{

						case MSG_BUILDCOMMAND:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d", &objectID, &lx, &ly, &dx, &dy);

							if ((numConverted == 5) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i) && object->isAStructure() && ((StructureClass*)object)->isABuilder())

								{

									if (((BuilderClass*)object)->buildNum(dx ? true : false, dy))

									{

										temp.x = lx; temp.y = ly;

										netServerBuildMessage(objectID, &temp, dx ? true : false, dy);

									}

								}

								else

									showMessage("ERROR: invalid message received from client.");

							}

							else

								showMessage("ERROR: invalid message received from client.");

							break;

						case MSG_CANCELCOMMAND:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d", &objectID, &lx, &ly, &dx, &dy);

							if ((numConverted == 5) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i) && object->isAStructure() && ((StructureClass*)object)->isABuilder())

									((BuilderClass*)object)->netDoCancelCommand(dx ? true : false, dy);

								else

									showMessage("ERROR: invalid message received from client.");

							}

							else

								showMessage("ERROR: invalid message received from client.");

							break;

						case MSG_DEVASTATE:

							numConverted = sscanf(&msg[1], "%d,%d,%d", &objectID, &lx, &ly);

							if ((numConverted == 3) && (map->cellExists(lx, ly) || (lx == NONE)) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i) && (object->getItemID() == Unit_Devastator))

									((DevastatorClass*)object)->netDevastate();

								else

									showMessage("ERROR: MSG_DEVASTATE - invalid message received from client.");

							}

							else

								showMessage("ERROR: MSG_DEVASTATE - invalid message received from client: message incorrectly formatted");

							break;

						case MSG_MESSAGE:

						{

							char	inMessage[MAX_MSGLENGTH + MAX_NAMELENGTH + 2];



							playerID = (int)msg[1];

							if (((playerID >= 0) && (playerID < MAX_PLAYERS)) && (player[playerID] != NULL))

							{

								sprintf(inMessage, "%s: %s", settings.playerName[playerID], &msg[2]);

								showMessage(inMessage);



								netServerMessage(playerID, &msg[2]);

							}

							else

								showMessage("ERROR: MSG_MESSAGE - invalid message received from server: message incorrectly formatted");

							break;

						}

						case MSG_OBJECTCOMMAND:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d,%d,%d", &objectID, &destObjectID, &lx, &ly, &dx, &dy, &forced);

							if ((numConverted == 7) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								destObject = NULL;

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i))

								{

									if (object->isRespondable())

									{

										if (destObjectID != NONE)

											destObject = map->findObjectWidthID(destObjectID, dx, dy);

										object->setTarget(destObject);

										object->setDestination(dx, dy);



										if (object->isAUnit())

										{

											((UnitClass*)object)->clearPath();

											if (destObject != NULL)

												((UnitClass*)object)->setGuardPoint(destObject->getLocation());

											else

												((UnitClass*)object)->setGuardPoint(dx, dy);

											((UnitClass*)object)->clearNextSpotFound();

										}

										object->setForced(forced == 1);



										//forward the message to all clients

										netServerObjectMessage(objectID, destObjectID, object->getX(), object->getY(), dx, dy, forced == 1);

									}

								}

								else

									showMessage("ERROR: invalid message received from client.");

							}

							else

								showMessage("ERROR: invalid message received from client.");

							break;

						case MSG_OBJECTDESTROY:

							numConverted = sscanf(&msg[1], "%d,%d,%d", &objectID, &lx, &ly);

							if ((numConverted == 3) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i))

								{

									object->destroy();

									netServerDestroyObject(objectID, lx, ly);

								}

								else

									showMessage("ERROR: invalid message received from client.");

							}

							else

								showMessage("ERROR: invalid message received from client.");

							break;

						case MSG_PALACESPECIAL:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &playerID, &objectID, &lx, &ly);

							if ((numConverted == 4) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && object->isAStructure() && (object->getItemID() == Structure_Palace) && (object->getOwner()->getPlayerNumber() == i))

								{

									if (((PalaceClass*)object)->specialReady())

									{

										((PalaceClass*)object)->doSpecial();

										netServerPalaceSpecial(playerID, objectID, lx, ly, ((PalaceClass*)object)->getDeathOffX(), ((PalaceClass*)object)->getDeathOffY());

									}

								}

								else

									showMessage("ERROR: MSG_PALACESPECIAL - invalid message received from server.");

							}

							else

								showMessage("ERROR: MSG_PALACESPECIAL - invalid message received from server: message incorrectly formatted");

							break;

						case MSG_PLACESTRUCTURE:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d,%d,%d,%d,%d", &playerID, &objectID, &lx, &ly, &itemID, &dx, &dy, &angle);

							if ((numConverted == 8) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i)

									&& ((object->isAStructure() && ((StructureClass*)object)->isABuilder()) && ((BuilderClass*)object)->isTypeWaitingToPlace(itemID))

										|| ((object->getItemID() == Unit_MCV) && (itemID == Structure_ConstructionYard))

									&& map->cellExists(lx, ly) && (playerID >= 0) && (playerID <= MAX_PLAYERS))

								{

									player[playerID]->netPlaceStructure(objectID, lx, ly, itemID, dx, dy);

									if (object->getItemID() == Unit_MCV)

										object->netDestroy();

								}

								else

									showMessage("ERROR: invalid message received from client.");

							}

							else

								showMessage("ERROR: invalid message received from client.");

							break;

						case MSG_SETATTACKMODE:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &objectID, &lx, &ly, &angle);

							if ((numConverted == 4) && (map->cellExists(lx, ly) || (lx == NONE)) && (objectID != NONE) && ((angle >= 0) && (angle < NUM_ANGLES)))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i) && object->isAUnit())

									((UnitClass*)object)->netSetAttackMode((ATTACKTYPE)angle);

							}

							else

								showMessage("ERROR: MSG_STARPORTORDER - invalid message received from client: message incorrectly formatted");

							break;

						case MSG_STARPORTORDER:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &playerID, &objectID, &lx, &ly);

							if ((numConverted == 4) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i)

									&& object->isAStructure() && (object->getItemID() == Structure_StarPort)

									&& map->cellExists(lx, ly) && (playerID == i))

								{

									((StarPortClass*)object)->placeOrder();

									netServerPlaceOrder(playerID, objectID, lx, ly);

								}

								else

									showMessage("ERROR: MSG_STARPORTORDER - invalid message received from client.");

							}

							else

								showMessage("ERROR: MSG_STARPORTORDER - invalid message received from client: message incorrectly formatted");

							break;

						case MSG_STARPORTCANCEL:

							numConverted = sscanf(&msg[1], "%d,%d,%d,%d", &playerID, &objectID, &lx, &ly);

							if ((numConverted == 4) && map->cellExists(lx, ly) && (objectID != NONE))

							{

								object = map->findObjectWidthID(objectID, lx, ly);

								if (object && (object->getOwner()->getPlayerNumber() == i)

									&& object->isAStructure() && (object->getItemID() == Structure_StarPort) && (playerID == i))

								{

									((StarPortClass*)object)->cancelOrder();

									netServerCancelOrder(playerID, objectID, lx, ly);

								}

								else

									showMessage("ERROR: MSG_STARPORTCANCEL - invalid message received from server.");

							}

							else

								showMessage("ERROR: MSG_STARPORTCANCEL - invalid message received from server: message incorrectly formatted");

							break;

						case MSG_QUIT:
							break;
						default:
							break;
					}
				}
				else
				{
					sprintf(msg, "%s has left the game.", settings.playerName[i]);

					showMessage(msg);

					settings.playerName[i][0] = '\0';

					SDLNet_TCP_DelSocket(socketSet, sockets[i]);

					SDLNet_TCP_Close(sockets[i]);

					sockets[i] = NULL;
				}
			}
		}

		if (numReady == -1)
		{//must be disconnected
			showMessage("Disconnected from clients.\n");

			//close all the sockets

			for(i = 1; i < MAX_PLAYERS; i++)

			if (sockets[i] != NULL)
			{
				settings.playerName[i][0] = '\0';
				SDLNet_TCP_DelSocket(socketSet, sockets[i]);

				SDLNet_TCP_Close(sockets[i]);
			}
			SDLNet_TCP_DelSocket(socketSet, server);

			SDLNet_TCP_Close(server);

			settings.playerType = SINGLE;
		}
	}
}
