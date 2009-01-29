def printconf(name, dictionary, level = 0):
	ret = str()
	if level == 0:
		ret += "%s = {\n" % name
	level +=1
	tab = "    "
	rightshift = tab * level
	dictkeys = []
	keys = dictionary.keys()
	keys.sort()
	for key in keys:
		if type(dictionary[key]) == type(dict()):
			dictkeys.append(key)
			continue
		ret += "%s'%s' : %s,\n" % (rightshift, key, dictionary[key].__repr__())

	for key in dictkeys:
		ret += "\n%s'%s' : {\n" % (rightshift, key)
		ret += printconf(name, dictionary[key], level)
		ret += "%s%s\n" % (rightshift, "},")

	level -= 1
	if level == 0:
		ret += "}"
	return ret
