TILE_NORMAL = 0x00010000
TILE_FLIPH = 0x00100000
TILE_FLIPV = 0x01000000
TILE_ROTATE = 0x10000000

class GameData(object):
    def __init__(self):
        self.filename = ""
        self.crop = None
        self.type = ""
        self.palette = ""
        self.tiles = []
        self.index = -1
        self.map = ""
        self.mapindex = None
        self.tilepos = None
        self.num = 0
        self.row = None
        self.gcobject = None
        self.colorkey = None
        self.putpixel = None
        self.drawvline = None
        self.drawhline = None
        self.fillrect = None
        self.persistent = True

def normal_row(row):
    for i in xrange(0,len(row)):
        row[i] |= TILE_NORMAL
    return row

def airunit_row(i):
    row = [(i+2)|TILE_NORMAL, (i+1)|TILE_NORMAL, i|TILE_NORMAL, (i+1)|TILE_FLIPV,
            (i+2)|TILE_FLIPV, (i+1)|TILE_ROTATE, i|TILE_FLIPH, (i+1)|TILE_FLIPH]
    return row;

def infantry_row(i):
    row = [(i+3)|TILE_NORMAL, i|TILE_NORMAL, (i+3)|TILE_FLIPV, (i+6)|TILE_NORMAL]
    return row;

def groundunit_row(i):
    row = [(i+2)|TILE_NORMAL, (i+1)|TILE_NORMAL, i|TILE_NORMAL, (i+1)|TILE_FLIPV,
            (i+2)|TILE_FLIPV,  (i+3)|TILE_FLIPV,  (i+4)|TILE_NORMAL, (i+3)|TILE_NORMAL]
    return row

def ornithopter_row(i):
    row = [(i+6)|TILE_NORMAL, (i+3)|TILE_NORMAL, i|TILE_NORMAL, (i+3)|TILE_FLIPV,
            (i+6)|TILE_FLIPV, (i+3)|TILE_ROTATE, i|TILE_FLIPH, (i+3)|TILE_FLIPH]
    return row;

def harvestersand_row(i):
    row = [(i+6)|TILE_NORMAL, (i+3)|TILE_NORMAL, i|TILE_NORMAL, (i+3)|TILE_FLIPV,
            (i+6)|TILE_FLIPV, (i+9)|TILE_FLIPV, (i+12)|TILE_NORMAL, (i+9)|TILE_NORMAL]
    return row;

def rocket_row(i):
    row = [(i+4)|TILE_NORMAL, (i+3)|TILE_NORMAL, (i+2)|TILE_NORMAL, (i+1)|TILE_NORMAL,
            i|TILE_NORMAL, (i+1)|TILE_FLIPV, (i+2)|TILE_FLIPV, (i+3)|TILE_FLIPV,
            (i+4)|TILE_FLIPV, (i+3)|TILE_ROTATE, (i+2)|TILE_ROTATE, (i+1)|TILE_ROTATE,
            i|TILE_FLIPH, (i+1)|TILE_FLIPH, (i+2)|TILE_FLIPH, (i+3)|TILE_FLIPH]
    return row

# vim:ts=4:sw=4:et
