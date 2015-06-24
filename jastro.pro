######################################################################
#
# 	QMake Projectfile for the JAstro Project
#
#	Author: Jan Stoefer, jan@stoefer.com
#
#	URL: www.jastro.net
#
######################################################################

TEMPLATE = subdirs

SUBDIRS = swisseph \
          jastroObj \
          dataBase

# desktopUI must be last:
CONFIG += ordered
SUBDIRS += desktopUI 

