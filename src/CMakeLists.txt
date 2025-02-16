set(GAME_SOURCE_FILES
am_map.c ct_chat.c d_main.c d_net.c f_finale.c g_game.c 
info.c in_lude.c i_sdl.c i_sdlmixer.c i_system.c m_misc.c 
mn_menu.c mus.c p_ceilng.c p_doors.c p_enemy.c p_floor.c 
p_inter.c p_lights.c p_map.c p_maputl.c p_mobj.c p_plats.c 
p_pspr.c p_setup.c p_sight.c p_spec.c p_switch.c p_telept.c 
p_tick.c p_user.c r_bsp.c r_data.c r_draw.c r_main.c r_plane.c 
r_segs.c r_things.c sb_bar.c sounds.c sv_save.c tables.c 
v_video.c w_wad.c z_zone.c)

set(GAME_INCLUDE_DIRS "${CMAKE_CURRENT_BINARY_DIR}")

set(EXTRA_LIBS m SDL2::SDL2main SDL2::SDL2 SDL2::mixer)

add_executable("heretic" ${GAME_SOURCE_FILES})

target_include_directories("heretic" PRIVATE ${GAME_INCLUDE_DIRS})
target_link_libraries("heretic" ${EXTRA_LIBS})

if(MSVC)
    set_target_properties("heretic" PROPERTIES
                          LINK_FLAGS "/MANIFEST:NO")
endif()
