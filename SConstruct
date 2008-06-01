import sys
import os
opts = Options('config.py')

# default to using sdl-config if not on windows
opts.Add('SDL_INCLUDE_PATH', 'include path for SDL', '')
opts.Add('SDL_LIB_PATH', 'lib path for SDL', '')

opts.Add('BOOST_INCLUDE_PATH', 'include path for boost', '')
# need to get two paths as far as i can tell. these can probably be calculated somehow
opts.Add('BOOST_LIB_PATH', 'lib path for boost', '')
CC="ccache g++-3.3"

if sys.platform == 'win32':
    env = Environment(options = opts, ENV=os.environ)
else:
    env = Environment(options = opts)

env.Append(CPPPATH="#include")


if sys.platform != "win32":
	env.ParseConfig('pkg-config --cflags sdl samplerate')
	env.Append(CCFLAGS=["-Wall", "-pedantic", "-O0"]) #, "-Werror"])
	#env.Append(CCFLAGS=["-Wall", "-Werror", "-O2", "-ffast-math", "-funroll-loops"])
	#env.Append(LINKFLAGS = ["-ffast-math"])

	if 1:
	  	env.Append(CCFLAGS=["-ggdb"])
else:
	env.Append(LIBS = ["SDLmain"])
	env.Append(LINKFLAGS = ["/SUBSYSTEM:CONSOLE", "/DEBUG"])
	env.Append(CCFLAGS = ["/O2", "/EHsc", "/MD", "/Op", "/DEBUG", "/Zi", "/GR"])

env.Append(LIBS = [ 	"SDL",
                    	"SDL_mixer",
                    	"SDL_net",
                    	"SDL_ttf",
                    	"samplerate",
                  ])


# If boost fails for you make sure you remove -mt from lines 49 and 50
# It's a debian fix.
if sys.platform != 'win32':
    env.Append(LIBS=[   "boost_signals-mt",
                        "boost_filesystem-mt",
                    ])
    env.Append(CCFLAGS=["-DHAVE_LRINT"])
    env.Append(CCFLAGS=["-DHAVE_LRINTF"])
else:
    env.Append(CPPPATH = [	"${SDL_INCLUDE_PATH}",
    			"${ZLIB_INCLUDE_PATH}",
    			"${ZZIP_INCLUDE_PATH}",
    			"${BOOST_INCLUDE_PATH}"])
    env.Append(LIBPATH = [	"${SDL_LIB_PATH}",
    			"${ZLIB_LIB_PATH}",
    			"${ZZIP_LIB_PATH}",
    			"${BOOST_LIB_PATH}"])

#env.Append(CCFLAGS=["-DLOG_DISABLED"])

if os.path.exists('/usr/lib/ccache/bin'):
    os.environ['PATH']         = '/usr/lib/ccache/bin:' + os.environ['PATH']
    env['ENV']['CCACHE_DIR']   = os.environ['CCACHE_DIR']
  
env['ENV']['PATH'] = os.environ['PATH']
env['ENV']['HOME'] = os.environ['HOME']

Export('env')

SConscript("src/SConscript")
