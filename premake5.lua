-- premake5.lua
workspace "NeoTorrentClient"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "NeoTorrentClient"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "WalnutExternal.lua"
include "NeoTorrentClient"