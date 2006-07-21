import sys
opts = Options('config.py')

# default to using sdl-config if not on windows
opts.Add('SDL_INCLUDE_PATH', 'include path for SDL', '')
opts.Add('SDL_LIB_PATH', 'lib path for SDL', '')
opts.Add('ZLIB_INCLUDE_PATH', 'include path for zlib', '')
opts.Add('ZLIB_LIB_PATH', 'lib path for zlib', '')
opts.Add('ZZIP_INCLUDE_PATH', 'include path for zlib', '')
opts.Add('ZZIP_LIB_PATH', 'lib path for zlib', '')

env = Environment(options = opts )

env.Append(CPPPATH="#include")

if sys.platform != "win32":
	env.ParseConfig('sdl-config --cflags --libs')
	env.ParseConfig('pkg-config --cflags --libs zziplib')
	env.Append(CCFLAGS=["-Wall", "-Werror"])

	if 1:
	  	env.Append(CCFLAGS=["-ggdb"])
else:
	env.Append(LIBS = ["zziplib", "zdll", "SDLmain"])
	env.Append(LINKFLAGS = ["/SUBSYSTEM:CONSOLE"])
	env.Append(CCFLAGS = ["/MD", "/Ox"])

env.Append(LIBS = [ 	"SDL",
			"SDL_mixer", 
                    	"SDL_image",
                    	"SDL_net",
                    	"SDL_ttf",
                  ])

env.Append(CPPPATH = [	"${SDL_INCLUDE_PATH}",
			"${ZLIB_INCLUDE_PATH}",
			"${ZZIP_INCLUDE_PATH}"])
env.Append(LIBPATH = [	"${SDL_LIB_PATH}",
			"${ZLIB_LIB_PATH}",
			"${ZZIP_LIB_PATH}"])

Export('env')

SConscript("src/SConscript")
