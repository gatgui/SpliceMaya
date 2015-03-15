import os, platform
import shutil

spliceEnv = Environment()

def RemoveFolderCmd(target, source, env):
  if os.path.exists(source[0].abspath):
    shutil.rmtree(source[0].abspath)

# define the clean target
if 'clean' in COMMAND_LINE_TARGETS:
  cleanBuild = spliceEnv.Command( spliceEnv.File('cleaning build folder'), spliceEnv.Dir('.build'), RemoveFolderCmd )
  cleanStage = spliceEnv.Command( spliceEnv.File('cleaning stage folder'), spliceEnv.Dir('.stage'), RemoveFolderCmd )
  spliceEnv.Alias('clean', [cleanBuild, cleanStage])
  Return()

# try to setup environment variables from command line arguments
#   with-boost=<dir> sets BOOST_DIR
#   boost-lib-suffix=<str>
#   with-fabric=<dir> sets FABRIC_DIR
#   splice-version=<str> sets FABRIC_SPLICE_VERSION
#   with-maya=<str> sets MAYA_VERSION, MAYA_INCLUDE_DIR, MAYA_LIB_DIR
#   debug=0|1 (optional) sets FABRIC_BUILD_TYPE
#   x86=0|1 (optional) sets FABRIC_BUILD_ARCH
if not "FABRIC_DIR" in os.environ:
  fd = ARGUMENTS.get("with-fabric", None)
  if fd and os.path.isdir(fd):
    os.environ["FABRIC_DIR"] = str(os.path.abspath(fd))

if not "FABRIC_SPLICE_VERSION" in os.environ:
  vs = ARGUMENTS.get("splice-version", None)
  if version:
    try:
      l = map(int, vs.split("."))
      os.environ["FABRIC_SPLICE_VERSION"] = "%d.%d.%d" % (l[0], l[1], l[2])
    except:
      pass

if not "FABRIC_BUILD_OS" in os.environ:
  import sys
  if sys.platform == "win32":
    os.environ["FABRIC_BUILD_OS"] = "Windows"
  elif sys.platform == "darwin":
    os.environ["FABRIC_BUILD_OS"] = "Darwin"
  else:
    os.environ["FABRIC_BUILD_OS"] = "Linux"

if not "FABRIC_BUILD_ARCH" in os.environ:
  try:
    os.environ["FABRIC_BUILD_ARCH"] = "x86" if int(ARGUMENTS.get("x86", "0")) != 0 else "x86_64"
  except:
    pass

if not "FABRIC_BUILD_TYPE" in os.environ:
  try:
    os.environ["FABRIC_BUILD_TYPE"] = "Debug" if int(ARGUMENTS.get("debug", "0")) != 0 else "Release"
  except:
    pass

if not "BOOST_DIR" in os.environ:
  bd = ARGUMENTS.get("with-boost", None)
  if bd and os.path.isdir(bd):
    os.environ["BOOST_DIR"] = str(os.path.abspath(bd))

if not "BOOST_LIB_SUFFIX" in os.environ:
  os.environ["BOOST_LIB_SUFFIX"] = str(ARGUMENTS.get("boost-lib-suffix", ""))

if not "MAYA_INCLUDE_DIR" in os.environ or \
   not "MAYA_LIB_DIR" in os.environ or \
   not "MAYA_VERSION" in os.environ:
  mayaspec = ARGUMENTS.get("with-maya", None)
  if mayaver:
    import re, sys
    
    mayadir = None
    
    if not os.path.isdir(mayaspec):
      if re.match(r"^\d+(\.\d+)?$", mayaspec):
        if sys.platform == "win32":
          mayadir = "C:/Program Files%s/Autodesk/Maya%s" % (" (x86)" if os.environ["FABRIC_BUILD_ARCH"] == "x86" else "", mayaspec)
        elif sys.platform == "darwin":
          mayadir = "/Applications/Autodesk/maya%s" % mayaspec
        else:
          mayadir = "/usr/autodesk/maya%s%s" % (mayaspec, "" if os.environ["FABRIC_BUILD_ARCH"] == "x86" else "-x64")
        
        if not os.path.isdir(mayadir):
          mayadir = None 
    else:
      mayadir = mayaspec
    
    if mayadir:
      mayaver = None
      
      if sys.platform == "darwin":
        mayainc = os.path.join(mayadir, "devkit", "include")
        mayalib = os.path.join(mayadir, "Maya.app", "Contents", "MacOS")
      else:
        mayainc = os.path.join(mayadir, "include")
        mayalib = os.path.join(mayadir, "lib")
      
      mtypes = os.path.join(mayainc, "maya", "MTypes.h")
      
      if os.path.isfile(mtypes):
        defexp = re.compile(r"^\s*#define\s+MAYA_API_VERSION\s+([0-9]+)")
        f = open(mtypes, "r")
        for line in f.readlines():
          m = defexp.match(line)
          if m:
            mayaver = m.group(1)
            break
        f.close()
      
      if mayaver:
        os.environ["MAYA_VERSION"] = str(mayaver)
        os.environ["MAYA_INCLUDE_DIR"] = str(mayainc)
        os.environ["MAYA_LIB_DIR"] = str(mayalib)

# check environment variables
for var in ['FABRIC_DIR', 'FABRIC_SPLICE_VERSION', 'FABRIC_BUILD_OS', 'FABRIC_BUILD_ARCH', 'FABRIC_BUILD_TYPE', 'BOOST_DIR', 'MAYA_INCLUDE_DIR', 'MAYA_LIB_DIR', 'MAYA_VERSION']:
  if not os.environ.has_key(var):
    print 'The environment variable %s is not defined.' % var
    exit(0)
  if var.lower().endswith('_dir'):
    if not os.path.exists(os.environ[var]):
      print 'The path for environment variable %s does not exist.' % var
      exit(0)

spliceEnv = Environment()

if not os.path.exists(spliceEnv.Dir('.stage').Dir('lib').abspath):
  os.makedirs(spliceEnv.Dir('.stage').Dir('lib').abspath)

# determine if we have the SpliceAPI two levels up
spliceApiDir = spliceEnv.Dir('..').Dir('..').Dir('SpliceAPI')

# try to get the Splice API from there
if os.path.exists(spliceApiDir.abspath):

  (spliceAPIAlias, spliceAPIFiles) = SConscript(
    dirs = [spliceApiDir],
    exports = {
      'parentEnv': spliceEnv,
      'FABRIC_DIR': os.environ['FABRIC_DIR'],
      'FABRIC_SPLICE_VERSION': os.environ['FABRIC_SPLICE_VERSION'],
      'FABRIC_BUILD_TYPE': os.environ['FABRIC_BUILD_TYPE'],
      'FABRIC_BUILD_OS': os.environ['FABRIC_BUILD_OS'],
      'FABRIC_BUILD_ARCH': os.environ['FABRIC_BUILD_ARCH'],
      'STAGE_DIR': spliceEnv.Dir('.stage'),
      'BOOST_DIR': os.environ['BOOST_DIR'],
      'BOOST_LIB_SUFFIX': os.environ['BOOST_LIB_SUFFIX']
    },
    variant_dir = spliceEnv.Dir('.build').Dir('SpliceAPI')
  )
  
  spliceApiDir = spliceEnv.Dir('.build').Dir('SpliceAPI').Dir('.stage').abspath
  
else:

  print 'The folder "'+spliceApiDir.abspath+'" does not exist. Please see the README.md for build instructions.'
  exit(0)

(mayaSpliceAlias, mayaSpliceFiles) = SConscript(
  os.path.join('SConscript'),
  exports = {
    'parentEnv': spliceEnv,
    'FABRIC_DIR': os.environ['FABRIC_DIR'],
    'FABRIC_SPLICE_VERSION': os.environ['FABRIC_SPLICE_VERSION'],
    'FABRIC_BUILD_TYPE': os.environ['FABRIC_BUILD_TYPE'],
    'FABRIC_BUILD_OS': os.environ['FABRIC_BUILD_OS'],
    'FABRIC_BUILD_ARCH': os.environ['FABRIC_BUILD_ARCH'],
    'STAGE_DIR': spliceEnv.Dir('.stage').Dir('SpliceIntegrations').Dir('FabricSpliceMaya'+os.environ['MAYA_VERSION']),
    'BOOST_DIR': os.environ['BOOST_DIR'],
    'BOOST_LIB_SUFFIX': os.environ['BOOST_LIB_SUFFIX'],
    'MAYA_INCLUDE_DIR': os.environ['MAYA_INCLUDE_DIR'],
    'MAYA_LIB_DIR': os.environ['MAYA_LIB_DIR'],
    'MAYA_VERSION': os.environ['MAYA_VERSION']
  },
  variant_dir = spliceEnv.Dir('.build').Dir(os.environ['MAYA_VERSION'])
)

allAliases = [mayaSpliceAlias]
spliceEnv.Alias('all', allAliases)
spliceEnv.Default(allAliases)
