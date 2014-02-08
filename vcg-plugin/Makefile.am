# Process this file with automake to produce Makefile.in
#
# Copyright (C) 2009, 2010, 2011 Mingjie Xing, mingjie.xing@gmail.com.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

GCCPLUGINS_DIR = `gcc -print-file-name=plugin`

INCLUDES = -I $(GCCPLUGINS_DIR)/include -I $(srcdir)/../libgdl $(GMP_INC)

AM_CFLAGS = -Wall

pkglib_LTLIBRARIES = vcg-plugin.la

EXTRA_DIST = vcg-plugin.gdbinit

vcg_plugin_la_SOURCES = \
	gcc-cgraph.c \
	gcc-function.c \
	gcc-gimple-hierarchy.c \
	gcc-passes.c \
	gcc-plugin-api.c \
	gcc-tree.c \
	gcc-tree-hierarchy.c \
        vcg-plugin.c vcg-plugin.h
                   

vcg_plugin_la_LDFLAGS = -module -lc

vcg_plugin_la_LIBADD = ../libgdl/libgdl.la
