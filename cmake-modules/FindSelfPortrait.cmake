find_library(SelfPortraitLib NAMES selfportrait)

if (NOT SelfPortraitLib)
	message(FATAL_ERROR "cannot find the SelfPortrait reflection library")
endif()

SET(SELFPC "selfportraitc")

SET(SelfPortrait_FOUND TRUE)
