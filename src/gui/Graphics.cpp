//#include "../sand.h"

#include "gui/Graphics.h"
#include "ui.h"
#include "Settings.h"
#include "houses.h"
#include "Colours.h"
#include "globals.h"
#include "Game.h"
#include "gui/MessageBox.h"

/*

void catch_stdin()

{

	if (focusedWidget != NULL && focusedWidget->isAnEntry())

	{

		char input[2];

		input[1] = '\0';

		//while(!feof(stdin))

		{

			input[0] = fgetc(stdin);

			((Entry*)focusedWidget)->setText(input);

		}

	}

}

*/

void clearMessage()

{

	int i;

	for (i = 0; i < MESSAGE_BUFFER_SIZE; i++)

	{

		messageBuffer[i][0] = '\0';

		messageTimer[i] = 0;

	}

}


void showMessage(char* newMessage)

{
	if(!edit_mode)
	{
	/*MessagesBox knows if message is already in queue*/
	messageBox.addMessage(newMessage);
	}
	else
	{
	bool found = false;

	int i;

	for (i = 0; i < MESSAGE_BUFFER_SIZE; i++)

	if (strlen(messageBuffer[i]) > 0)

	{

		if (strcmp(messageBuffer[i], newMessage) == 0)

			found = true;

		i = MESSAGE_BUFFER_SIZE;

	}



	if (!found)

	{

		for (i = MESSAGE_BUFFER_SIZE - 1; i > 0; i--)

		{

			strcpy(messageBuffer[i], messageBuffer[i-1]);

			messageTimer[i] = messageTimer[i-1];

		}

		strcpy(messageBuffer[0], newMessage);

		messageTimer[0] = 500;

	}
	};

}
