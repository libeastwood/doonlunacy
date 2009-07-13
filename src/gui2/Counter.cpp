#include "gui2/Counter.h"
#include "DataCache.h"
#include "DuneConstants.h"
#include "GameData.h"

Counter::Counter(int min, int max, int value)
{
    char digits[25];
    int size;
    ImagePtr tmp, background;

    sprintf(digits, "%d", max);

    m_min = min;
    m_max = max;
    m_value = value;
    m_maxlen = strlen(digits);
    size = 3 + (m_maxlen*10);

    tmp = DataCache::Instance()->getGameData("UI_CreditsCounter")->getImage();
    background.reset(new Image(UPoint(size + 2, tmp->getSize().y)));

    background->putPixel(UPoint(background->getSize().x-1,0), tmp->getPixel(UPoint(0,0)));
    background->drawVLine(UPoint(background->getSize().x-1,1), tmp->getSize().y-1, tmp->getPixel(UPoint(1, tmp->getSize().y-1)));
    background->blitFrom(tmp->getPictureCrop(Rect(0,0, size, tmp->getSize().y)).get());
    background->blitFrom(tmp->getPictureCrop(Rect(tmp->getSize().x-1,0,1, tmp->getSize().y)).get(), UPoint(background->getSize().x-2, 0));

    changeBackground(background);
    drawDigits();
}

void Counter::decrease()
{
    m_value--;
    if (m_value < m_min)
	m_value = m_min;
    else
	drawDigits();
}

void Counter::increase()
{
    m_value++;
    if (m_value > m_max)
	m_value = m_max;
    else
	drawDigits();
}

void Counter::drawDigits()
{
    char digits[25];
    sprintf(digits, "%d", m_value);
    int len = strlen(digits);
    ImagePtr creditDigits = DataCache::Instance()->getGameData("UI_CreditsDigits")->getImage();



    char digit[2];
    int num;
    Uint32 colorBg = m_surface->getPixel(UPoint(4,4));
    for(int i = 0, j = 0; i < m_maxlen; i++) {
	if(i + len >= m_maxlen) {
	    digit[0] = digits[j++];
	    num = atoi(digit);
	    m_surface->blitFrom(creditDigits->getPictureCrop(Rect(num*8,0, 8,8)).get(),UPoint(4+i*10,4));
	}
	else {
	    Rect r(4+i*10, 4, 8, 8);
	    m_surface->fillRect(colorBg, &r);
	}
    }
}
