AUTOMAKE_OPTIONS = foreign

# these are flags sent to the C++ compiler                                                
# -Werror: this will treat undefined and unused variables as errors                       
AM_CXXFLAGS = -Wall -Werror -std=c++0x

# This flag will build 'testexternals.C' as a test for the build environment              
BUILT_SOURCES = \
  testexternals.C

# This is where we set the directories where we put our neccessary header files           
AM_CPPFLAGS = \
  -I$(ROOTSYS)/include

lib_LTLIBRARIES = \
  libPIDToy.la

libPIDToy_la_LDFLAGS = \
  -L$(ROOTSYS)/lib `root-config --libs`

AM_LDFLAGS = \
  -L$(ROOTSYS)/lib

# These sources are compiled into the final library                                       
libPIDToy_la_SOURCES = \
  PIDPhoton.C \	
  PIDEvent.C \
  ToyCherenkov.C	

nodist_libPIDToy_la_SOURCES = \
  ToyCherenkov_Dict.C

include_HEADERs = \
  PIDPhoton.h \
  PIDEvent.h \
  ToyCherenkov.h

noinst_PROGRAMS = \
  testexternals

testexternals_LDADD = \
  libPIDToy.la

testexternals.C:
	echo "//*** this is a generated file. Do not commit, do not edit" > $@
	echo "int main()" >> $@
	echo "{" >> $@
	echo "  return 0;" >> $@
	echo "}" >> $@

# This is where the dictionary file is generated                                          
ToyCherenkov_Dict.C: \
	PIDPhoton.h \
	PIDEvent.h \
	ToyCherenkov.h \
	ToyCherenkovLinkDef.h
	rootcint -f $@ -c $(DEFAULT_INCLUDES) $(AM_CPPFLAGS) $^

clean-local:
	rm -f *Dict.*
