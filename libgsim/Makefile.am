NULL =

sources_h = \
			gsim-scheduler.h \
			$(NULL)

sources_c = \
			gsim-scheduler.c \
			$(NULL)

lib_LTLIBRARIES = libgsim-1.0.la

libgsim_1_0_la_SOURCES = $(sources_h) $(sources_c)
libgsim_1_0_la_CFLAGS = $(LIBGSIM_CFLAGS)
libgsim_1_0_la_LIBADD = $(LIBGSIM_LIBS)
libgsim_1_0_la_LDFLAGS = -version-info $(LIBGSIM_LT_VERSION) -export-dynamic -no-undefined

libgdict_includedir = $(includedir)/gsim-1.0/gsim
libgdict_include_HEADERS = \
						   $(sources_h) \
						   $(NULL)
