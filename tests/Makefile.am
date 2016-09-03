NULL = 

noinst_PROGRAMS = check_scheduler
TESTS = check_scheduler

check_scheduler_SOURCES = scheduler.c
check_scheduler_CFLAGS = \
						 -I $(top_srcdir)/libgsim \
						 $(CFLAGS) \
						 $(LIBGSIM_CFLAGS) \
						 $(NULL)
check_scheduler_LDADD = \
						$(top_builddir)/libgsim/libgsim-1.0.la \
						$(LDFLAGS) \
						$(LIBGSIM_LIBS) \
						$(NULL)
