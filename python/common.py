class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __getitem__(self, key):
        if key == 0:
            return self.x
        elif key == 1:
            return self.y
        else:
            return None

class Rect(object):
    def __init__(self, x, y, w, h):
        self.x = x
        self.y = y
        self.w = w
        self.h = h

    def __getitem__(self, key):
        if key == 0:
            return self.x
        elif item == 1:
            return self.y
        elif item == 2:
            return self.w
        elif item == 3:
            return self.h
        else:
            return None

# vim:ts=4:sw=4:et
