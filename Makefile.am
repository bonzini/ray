AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libray.la
libray_la_SOURCES = images.cc pngwrite.cc light.cc geom.cc \
	texture.cc scene.cc v3d.cc
noinst_HEADERS = images.h pngwrite.h light.h geom.h \
	texture.h scene.h v3d.h rand.h

noinst_PROGRAMS = scene1
scene1_SOURCES = scene1.cc
scene1_LDADD = libray.la
scene1_LDFLAGS = -static
scene1_DEPENDENCIES = libray.la

AM_CFLAGS = -Wall $(ARCH_CFLAGS)
AM_CXXFLAGS = -Wall $(ARCH_CFLAGS)
