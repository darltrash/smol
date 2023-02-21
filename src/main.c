// smolanzo!

#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#include "log.h"
#include "map.h"

#include "sokol_gfx.h"
#include "sokol_app.h"
#include "sokol_glue.h"
#include "shader.h"

#include "minilua.h"
#include "smol.h"

sg_bindings smst_bindings;
sg_pipeline smst_pipeline;
sg_pass_action smst_pass_action;

lua_State *smst_lua = NULL;

sg_color smst_color = {1., 1., 1., 1.};
sg_color smst_backcolor = {0., 0., 0., 1.};

int smst_quads;
char *smst_folder = NULL;

sm_mouse_state smst_mouse;
map_char_t smst_keys;

float smst_vertices[SMOL_QUAD_AMOUNT * (2+2+4)];
float smst_camera[2] = { 0.0, 0.0 }; 
float smst_scale = 2.0;

void smst_lua_set_number(const char* str, double v) {
    lua_getglobal(smst_lua, "sm");
    lua_pushstring(smst_lua, str);
    lua_pushnumber(smst_lua, v);
    lua_settable(smst_lua, -3);
    lua_pop(smst_lua, 1);
}

void smst_lua_set_string(const char* str, const char* v) {
    lua_getglobal(smst_lua, "sm");
    lua_pushstring(smst_lua, str);
    lua_pushstring(smst_lua, v);
    lua_settable(smst_lua, -3);
    lua_pop(smst_lua, 1);
}

void init(void)
{
    sg_setup(&(sg_desc){
        .context = sapp_sgcontext()
    });

    map_init(&smst_keys);

    lua_getglobal(smst_lua, "sm");
    lua_getfield(smst_lua, -1, "load");
    if (lua_pcall(smst_lua, 0, 0, 0)) {
        log_fatal("Failed to run sm.load: \n>\t%s\n", lua_tostring(smst_lua, -1));
        exit(1);
    };

    smst_pass_action = (sg_pass_action) {
        .colors[0] = {
            .action = SG_ACTION_CLEAR,
            .value = smst_backcolor
        }
    };

    smst_pipeline = sg_make_pipeline(&(sg_pipeline_desc){
        .shader = sg_make_shader(quad_shader_desc(sg_query_backend())),
        .layout = {
            .attrs = {
                [ATTR_vs_vx_pos_uv].format = SG_VERTEXFORMAT_FLOAT4,
                [ATTR_vs_vx_color].format  = SG_VERTEXFORMAT_FLOAT4
            }
        },
        .colors[0].blend = {
            .enabled = true,
            .src_factor_rgb = SG_BLENDFACTOR_SRC_ALPHA,
            .dst_factor_rgb = SG_BLENDFACTOR_ONE_MINUS_SRC_ALPHA
        },
        .index_type = SG_INDEXTYPE_UINT32,
        .label = "pipeline"
    });

    uint32_t indices[SMOL_QUAD_AMOUNT * 6]; 
    for (int i = 0; i < SMOL_QUAD_AMOUNT; i++) {
        uint32_t tmp[6] = {
            (i*4)+0, (i*4)+1, (i*4)+3, 
            (i*4)+1, (i*4)+2, (i*4)+3
        };
        memcpy(&indices[i*6], &tmp, sizeof(tmp));
    }

    smst_bindings = (sg_bindings) {
        .vertex_buffers[0] = sg_make_buffer(&(sg_buffer_desc){
            .size = SMOL_QUAD_AMOUNT * 36 * sizeof(float),
            .type = SG_BUFFERTYPE_VERTEXBUFFER,
            .usage = SG_USAGE_DYNAMIC
        }),

        .index_buffer = sg_make_buffer(&(sg_buffer_desc){
            .type = SG_BUFFERTYPE_INDEXBUFFER,
            .data = SG_RANGE(indices)
        })
    };
}

void event(const sapp_event *ev)
{
    lua_getglobal(smst_lua, "sm");
    lua_getfield(smst_lua, -1, "event");

    int arguments = 1;
    switch (ev->type) {
        case SAPP_EVENTTYPE_RESIZED: {
            const float w = (float)ev->window_width;
            const float h = (float)ev->window_height;

            lua_pushstring(smst_lua, "resize");
            lua_pushnumber(smst_lua, w);
            lua_pushnumber(smst_lua, h);
            arguments = 3;
            break;
        }
        
        case SAPP_EVENTTYPE_MOUSE_MOVE: {
            smst_mouse.x = ev->mouse_x;
            smst_mouse.y = ev->mouse_y;

            lua_pushstring(smst_lua, "mouse move");
            lua_pushnumber(smst_lua, smst_mouse.x);
            lua_pushnumber(smst_lua, smst_mouse.y);
            arguments = 3;
            break;
        }

        case SAPP_EVENTTYPE_MOUSE_UP:
        case SAPP_EVENTTYPE_MOUSE_DOWN: {
            smst_mouse.buttons[ev->mouse_button] = 
                ev->type == SAPP_EVENTTYPE_MOUSE_DOWN;

            lua_pushstring(smst_lua, (SAPP_EVENTTYPE_MOUSE_DOWN == ev->type) ? "mouse down" : "mouse up");
            lua_pushnumber(smst_lua, ev->mouse_button+1);
            arguments = 2;
            break;
        }

        case SAPP_EVENTTYPE_MOUSE_SCROLL: {
            smst_mouse.scroll[0] = ev->scroll_x;
            smst_mouse.scroll[1] = ev->scroll_y;

            lua_pushstring(smst_lua, "mouse scroll");
            lua_pushnumber(smst_lua, ev->scroll_x);
            lua_pushnumber(smst_lua, ev->scroll_y);
            arguments = 3;
            break;
        }

        case SAPP_EVENTTYPE_KEY_UP:
        case SAPP_EVENTTYPE_KEY_DOWN: {
            const char *key = sm_keymap[ev->key_code];
            const int down = (ev->type == SAPP_EVENTTYPE_KEY_DOWN);
            map_set(&smst_keys, key, down);

            lua_pushstring(smst_lua, down ? "key down" : "key up");
            lua_pushstring(smst_lua, key);
            arguments = 2;
            break;
        }

        case SAPP_EVENTTYPE_MOUSE_ENTER:
        case SAPP_EVENTTYPE_MOUSE_LEAVE: {
            const int enter = (ev->type == SAPP_EVENTTYPE_MOUSE_ENTER);
            lua_pushstring(smst_lua, enter ? "mouse enter" : "mouse leave");
            arguments = 1;
            break;
        }

        case SAPP_EVENTTYPE_FILES_DROPPED: {
            lua_pushstring(smst_lua, "file drop");
            int i = 0;
            char *txt = sapp_get_dropped_file_path(i);
            while(txt[0]) {
                lua_pushstring(smst_lua, txt);
                i++;
                txt = sapp_get_dropped_file_path(i);
            }
            arguments = 1 + i;
            break;
        }

        case SAPP_EVENTTYPE_UNFOCUSED:
        case SAPP_EVENTTYPE_FOCUSED: {
            const int focus = (ev->type == SAPP_EVENTTYPE_FOCUSED);

            lua_pushstring(smst_lua, focus ? "focused" : "unfocused");
            arguments = 1;
            break;
        }

        case SAPP_EVENTTYPE_CHAR: {
            lua_pushstring(smst_lua, "text");
            lua_pushstring(smst_lua, &ev->char_code);
            arguments = 2;
            break;
        }

        default: {
            arguments = 0;
            break;
        }
            
    }

    if (arguments == 0) return;

    if (lua_pcall(smst_lua, arguments, 0, 0)) {
        log_fatal("Failed to run sm.event: \n>\t%s\n", lua_tostring(smst_lua, -1));
        exit(EXIT_FAILURE);
    };
}

void fail(const char *msg)
{
    log_fatal("Something went horribly wrong! %s", msg);
}

void cleanup(void)
{
    sg_shutdown();
}

double timer;
void frame(void)
{

    timer += sapp_frame_duration();
    smst_lua_set_number("width",  sapp_width());
    smst_lua_set_number("height", sapp_height());
    smst_lua_set_number("fps",    1.0/sapp_frame_duration());
    smst_lua_set_number("delta",  sapp_frame_duration());
    smst_lua_set_number("frame",  sapp_frame_count());
    smst_lua_set_number("timer",  timer);
    smst_lua_set_number("quads",  smst_quads);

    lua_getglobal(smst_lua, "sm");
    lua_getfield(smst_lua, -1, "loop");
    lua_pushnumber(smst_lua, sapp_frame_duration());
    if (lua_pcall(smst_lua, 1, 0, 0)) {
        log_fatal("Failed to run sm.loop: \n>\t%s\n", lua_tostring(smst_lua, -1));
        exit(1);
    };

    smst_quads = 0;

    lua_getglobal(smst_lua, "sm");
    lua_getfield(smst_lua, -1, "draw");
    if (lua_pcall(smst_lua, 0, 0, 0)) {
        log_fatal("Failed to run sm.draw: \n>\t%s\n", lua_tostring(smst_lua, -1));
        exit(EXIT_FAILURE);
    };

    smst_pass_action.colors[0].value = smst_backcolor;

    sg_update_buffer(smst_bindings.vertex_buffers[0], &SG_RANGE(smst_vertices));

    sg_begin_default_pass(&smst_pass_action, sapp_width(), sapp_height());

        sg_apply_pipeline(smst_pipeline);
        sg_apply_bindings(&smst_bindings);
        if (smst_quads > 0)
            sg_draw(0, smst_quads*6, 1);

    sg_end_pass();

    sg_commit();

    smst_mouse.scroll[0] = 0;
    smst_mouse.scroll[1] = 0;
}

sapp_desc description = {
    .init_cb = init,
    .frame_cb = frame,
    .cleanup_cb = cleanup,
    .event_cb = event,
    .fail_cb = fail,
    .alpha = 0, 
    .sample_count = 2,
    .width = 600,
    .height = 400,
    .gl_force_gles2 = true,
    .window_title = "smol - Untitled game",
    .icon.sokol_default = true,
};

int sm_setup(lua_State *L) {
    lua_settop(L, 1);
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "title");
    lua_getfield(L, 1, "width");
    lua_getfield(L, 1, "height");
    lua_getfield(L, 1, "fullscreen");

    if (!lua_isnoneornil(L, -4))
        description.window_title = luaL_checkstring(L, -4);

    if (!lua_isnoneornil(L, -3))
        description.width = luaL_checkinteger(L, -3);

    if (!lua_isnoneornil(L, -2))
        description.height = luaL_checkinteger(L, -2);

    if (!lua_isnoneornil(L, -1))
        description.fullscreen = lua_toboolean(L, -1);

    lua_pop(L, 2);
    return 0;
}

int sm_mouse() {
    if (!lua_isnoneornil(smst_lua, 1)) {
        int option = luaL_checkinteger(smst_lua, 1);
        
        if (option > 3 || option < 1)
            luaL_error(smst_lua, "Expected mouse button to be in the range of 1 to 3, got '%i'");

        lua_pushboolean(smst_lua, smst_mouse.buttons[option-1]);

        return 1;
    }

    lua_pushnumber(smst_lua, smst_mouse.x);
    lua_pushnumber(smst_lua, smst_mouse.y);
    return 2;
}

int sm_scroll() {
    lua_pushnumber(smst_lua, smst_mouse.scroll[0]);
    lua_pushnumber(smst_lua, smst_mouse.scroll[1]);
    return 2;
}

int sm_key() {
    int arguments = lua_gettop(smst_lua);
    for (int a = 1; a <= arguments; a++) {
        const char* key = luaL_checkstring(smst_lua, a);
        const char* k = map_get(&smst_keys, key);

        if (k && *k) {
            lua_pushboolean(smst_lua, true);
            return 1;
        }
    }

    lua_pushboolean(smst_lua, false);
    return 1;
}

int sm_title() {
    lua_settop(smst_lua, 1);
    if (!lua_isnoneornil(smst_lua, 1)) {
        const char *title = luaL_checkstring(smst_lua, 1);
        sapp_set_window_title(title);
        description.window_title = title;
    }
    lua_pushstring(smst_lua, description.window_title);
    return 1;
}

int sm_sign() {
    float num = luaL_checknumber(smst_lua, 1);
    lua_pushnumber(smst_lua, (num > 1) ? 1 : (num < 1) ? -1 : 0);
    return 1;
}

int sm_lerp() {
    double a = luaL_checknumber(smst_lua, 1);
    double b = luaL_checknumber(smst_lua, 2);
    double t = luaL_checknumber(smst_lua, 3);

    lua_pushnumber(smst_lua, a + (b - a) * t);
    return 1;
}

int sm_color_internal(sg_color *c) {
    if (!lua_isnoneornil(smst_lua, 1)) {
        if (lua_gettop(smst_lua)==1) {
            int value = luaL_checkinteger(smst_lua, 1);
            c->r = (float)((value & 0xff000000) >> 24)/255;
            c->g = (float)((value & 0x00ff0000) >> 16)/255;
            c->b = (float)((value & 0x0000ff00) >> 8 )/255;
            c->a = (float)((value & 0x000000ff)      )/255;

        } else if (lua_isnumber(smst_lua, 1)) {
            c->r = luaL_checknumber(smst_lua, 1);
            c->g = luaL_checknumber(smst_lua, 2);
            c->b = luaL_checknumber(smst_lua, 3);
            c->a = luaL_checknumber(smst_lua, 4);
        }
    } 

    lua_pushnumber(smst_lua, c->r);
    lua_pushnumber(smst_lua, c->g);
    lua_pushnumber(smst_lua, c->b);
    lua_pushnumber(smst_lua, c->a);

    return 4;
}

int sm_color() {
    return sm_color_internal(&smst_color);
}

int sm_backcolor() {
    return sm_color_internal(&smst_backcolor);
}

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define floorto(a, b) (floorf(a*b)/b)

int sm_rect() {
    // This function appends to a clip-space quad vertex buffer

    // This is probably cheaper than processing later through
    // an orthographic matrix, and it's also much easier to pull off

    if (smst_quads == SMOL_QUAD_AMOUNT) {
        log_error("Internal buffer exceeded! (> %i quads)", SMOL_QUAD_AMOUNT);
        lua_pushboolean(smst_lua, false);
        return 1;
    }

    double x, y, w, h;
    x = luaL_checknumber(smst_lua, -4);
    y = luaL_checknumber(smst_lua, -3);
    w = luaL_checknumber(smst_lua, -2);
    h = luaL_checknumber(smst_lua, -1);

    const double d = 1.0 / (double)(smst_scale);
    const double ww = sapp_widthf()  * d;
    const double wh = sapp_heightf() * d;

    const double sx = (smst_camera[0]+x) / ww;
    const double sy = (smst_camera[1]-y) / wh;
    const double cx = sx + (w / ww);
    const double cy = sy - (h / wh);

    if (true) {
        const float usx = 0.0; //(double)(spr.x)       / SMOL_TEXTURE_WIDTH;
        const float usy = 0.0; //(double)(spr.y)       / SMOL_TEXTURE_HEIGHT;
        const float ucx = 0.0; //(double)(spr.x+spr.w) / SMOL_TEXTURE_WIDTH; 
        const float ucy = 0.0; //(double)(spr.y+spr.h) / SMOL_TEXTURE_HEIGHT;

        // Position (vec2) + UV (vec2) + Color (vec4)
        float q[4 * (2+2+4)] = {
            cx, cy, ucx, ucy, smst_color.r, smst_color.g, smst_color.b, smst_color.a,
            cx, sy, ucx, usy, smst_color.r, smst_color.g, smst_color.b, smst_color.a,
            sx, sy, usx, usy, smst_color.r, smst_color.g, smst_color.b, smst_color.a,
            sx, cy, usx, ucy, smst_color.r, smst_color.g, smst_color.b, smst_color.a
        };

        memcpy(&smst_vertices[smst_quads * 4 * (2+2+4)], &q, sizeof(q));
        smst_quads++;

        lua_pushboolean(smst_lua, true);
        return 1;
    }

    lua_pushboolean(smst_lua, false);
    return 1;
}

void sm_quit() {
    sapp_request_quit();
}

void sm_fullscreen() {
    sapp_toggle_fullscreen();
}

void sm_none() {}

struct reg {const char *name; const void *func;} registers[] = {
    // Get and/or set state info
    {"setup", sm_setup}, 
    {"title", sm_title},
    {"quit", sm_quit},

    // Input
    {"mouse", sm_mouse},
    {"scroll", sm_scroll},
    {"key", sm_key},

    // Do the graphiscscs
    {"color", sm_color},
    {"backcolor", sm_backcolor},
    {"rect", sm_rect},

    // Cool magic
    {"lerp", sm_lerp},
    {"sign", sm_sign},

    // Callback fallbacks!
    {"event", sm_none},
    {"load", sm_none},
    {"loop", sm_none},
    {"draw", sm_none},
    {NULL, NULL}
};

sapp_desc sokol_main(int argc, char *argv[])
{
    if (argc == 0) {
        log_fatal("No path specified!");
        exit(1);
    }

    for (int i = 0; i < argc; i++) {
        DIR* dir = opendir(argv[i]);
        if (dir) {
            closedir(dir);
            smst_folder = argv[i];
        }
    }

    if (smst_folder == NULL) {
        log_fatal("No valid path specified! Check arguments");
        exit(1);
    }

    int status, result, i;

    smst_lua = luaL_newstate();
    luaL_openlibs(smst_lua);

    chdir(smst_folder);
    status = luaL_loadfile(smst_lua, "smol.lua");
    if (status) {
        log_fatal("Couldn't load file: \n>\t%s\n", lua_tostring(smst_lua, -1));
        exit(1);
    }

    lua_newtable(smst_lua); 
    lua_setglobal(smst_lua, "sm");

    smst_lua_set_string("backend", SMOL_BACKEND);
    smst_lua_set_string("OS", SMOL_OS);

    for (int i = 0; i < 99; i++) {    
        if (registers[i].name == NULL)
            break;

        lua_getglobal(smst_lua, "sm");
        lua_pushstring(smst_lua, registers[i].name);
        lua_pushcfunction(smst_lua, registers[i].func);
        lua_settable(smst_lua, -3);

        lua_pop(smst_lua, 1);
    }

    result = lua_pcall(smst_lua, 0, LUA_MULTRET, 0);
    if (result) {
        log_fatal("Failed to run script: \n>\t%s\n", lua_tostring(smst_lua, -1));
        exit(EXIT_FAILURE);
    }

    return description;
}
