PROG=proj

SRCS = proj.c tower.c block.c keyboard.c timer.c videos.c utils.c menus.c mouse.c

# __LCOM_OPTIMIZED__ disables most of the LCF's overhead (and functionality!)
CPPFLAGS += -pedantic #-D __LCOM_OPTIMIZED__

DPADD += ${LIBLCF}
LDADD += -llcf

.include <minix.lcom.mk>
