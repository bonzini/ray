AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libray.la
libray_la_SOURCES = images.cc pngwrite.cc light.cc geom.cc \
	texture.cc scene.cc v3d.cc
noinst_HEADERS = images.h pngwrite.h light.h geom.h \
	texture.h scene.h v3d.h rand.h

noinst_PROGRAMS = scene1 scene2 scene3 scene4 scene5 scene6
CLEANFILES = scene1.png scene2.png scene3.png scene4.png scene5.png scene6.png

scene1_SOURCES = scene1.cc
scene1_DEPENDENCIES = libray.la

scene2_SOURCES = scene2.cc
scene2_DEPENDENCIES = libray.la

scene3_SOURCES = scene3.cc
scene3_DEPENDENCIES = libray.la

scene4_SOURCES = scene4.cc
scene4_DEPENDENCIES = libray.la

scene5_SOURCES = scene5.cc
scene5_DEPENDENCIES = libray.la

scene6_SOURCES = scene6.cc
scene6_DEPENDENCIES = libray.la

AM_CFLAGS = -Wall $(ARCH_CFLAGS)
AM_CXXFLAGS = -Wall $(ARCH_CFLAGS)
AM_LDFLAGS = -static
LDADD = libray.la
