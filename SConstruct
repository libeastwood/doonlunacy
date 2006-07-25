import sys
import os
opts = Options('config.py')

# default to using sdl-config if not on windows
opts.Add('SDL_INCLUDE_PATH', 'include path for SDL', '')
opts.Add('SDL_LIB_PATH', 'lib path for SDL', '')
opts.Add('ZLIB_INCLUDE_PATH', 'include path for zlib', '')
opts.Add('ZLIB_LIB_PATH', 'lib path for zlib', '')
opts.Add('ZZIP_INCLUDE_PATH', 'include path for zlib', '')
opts.Add('ZZIP_LIB_PATH', 'lib path for zlib', '')

opts.Add('BOOST_INCLUDE_PATH', 'include path for boost', '')
# need to get two paths as far as i can tell. these can probably be calculated somehow
opts.Add('BOOST_LIB_PATH', 'lib path for boost', '')


if sys.platform == 'win32':
    env = Environment(options = opts, ENV=os.environ)
else:
    env = Environment(options = opts)

env.Append(CPPPATH="#include")

if sys.platform != "win32":
	env.ParseConfig('sdl-config --cflags --libs')
	env.ParseConfig('pkg-config --cflags --libs zziplib')
	env.Append(CCFLAGS=["-Wall"]) #, "-Werror"])
	#env.Append(CCFLAGS=["-Wall", "-Werror", "-O2", "-ffast-math", "-funroll-loops"])
	#env.Append(LINKFLAGS = ["-ffast-math"])

	if 1:
	  	env.Append(CCFLAGS=["-ggdb"])
else:
	env.Append(LIBS = ["zzipdll", "zdll", "SDLmain"])
	env.Append(LINKFLAGS = ["/SUBSYSTEM:CONSOLE"])
	env.Append(CCFLAGS = ["/MD", "/O2"])

env.Append(LIBS = [ 	"SDL",
			"SDL_mixer", 
                    	"SDL_image",
                    	"SDL_net",
                    	"SDL_ttf",
                  ])

if sys.platform != 'win32':
    env.Append(LIBS=[   "boost_signals",
                        "boost_filesystem",
                    ])

env.Append(CPPPATH = [	"${SDL_INCLUDE_PATH}",
			"${ZLIB_INCLUDE_PATH}",
			"${ZZIP_INCLUDE_PATH}",
			"${BOOST_INCLUDE_PATH}"])
env.Append(LIBPATH = [	"${SDL_LIB_PATH}",
			"${ZLIB_LIB_PATH}",
			"${ZZIP_LIB_PATH}",
			"${BOOST_LIB_PATH}"])

Export('env')

SConscript("src/SConscript")
