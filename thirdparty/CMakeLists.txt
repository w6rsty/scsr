add_subdirectory(lua-5.4.7)
add_subdirectory(fmt)

SET(SDL_SHARED OFF)
SET(SDL_TEST OFF)
add_subdirectory(SDL2 EXCLUDE_FROM_ALL)

add_subdirectory(STB)