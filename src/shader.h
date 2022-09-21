#pragma once
/*
    #version:1# (machine generated, don't edit!)

    Generated by sokol-shdc (https://github.com/floooh/sokol-tools)

    Cmdline: sokol-shdc --input src/shader.glsl --output src/shader.h --slang glsl100

    Overview:

        Shader program 'quad':
            Get shader desc: quad_shader_desc(sg_query_backend());
            Vertex shader: vs
                Attribute slots:
                    ATTR_vs_vx_pos_uv = 0
                    ATTR_vs_vx_color = 1
            Fragment shader: fs


    Shader descriptor structs:

        sg_shader quad = sg_make_shader(quad_shader_desc(sg_query_backend()));

    Vertex attribute locations for vertex shader 'vs':

        sg_pipeline pip = sg_make_pipeline(&(sg_pipeline_desc){
            .layout = {
                .attrs = {
                    [ATTR_vs_vx_pos_uv] = { ... },
                    [ATTR_vs_vx_color] = { ... },
                },
            },
            ...});

    Image bind slots, use as index in sg_bindings.vs_images[] or .fs_images[]


*/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#if !defined(SOKOL_SHDC_ALIGN)
  #if defined(_MSC_VER)
    #define SOKOL_SHDC_ALIGN(a) __declspec(align(a))
  #else
    #define SOKOL_SHDC_ALIGN(a) __attribute__((aligned(a)))
  #endif
#endif
#define ATTR_vs_vx_pos_uv (0)
#define ATTR_vs_vx_color (1)
/*
    #version 100
    
    attribute vec4 vx_pos_uv;
    varying vec2 uv;
    varying vec4 color;
    attribute vec4 vx_color;
    
    void main()
    {
        gl_Position = vec4(vx_pos_uv.xy, 1.0, 1.0);
        uv = vx_pos_uv.zw;
        color = vx_color;
        vec4 _37 = color;
        vec2 _39 = _37.xy * uv;
        color.x = _39.x;
        color.y = _39.y;
    }
    
*/
static const char vs_source_glsl100[306] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x31,0x30,0x30,0x0a,0x0a,0x61,0x74,
    0x74,0x72,0x69,0x62,0x75,0x74,0x65,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x78,0x5f,
    0x70,0x6f,0x73,0x5f,0x75,0x76,0x3b,0x0a,0x76,0x61,0x72,0x79,0x69,0x6e,0x67,0x20,
    0x76,0x65,0x63,0x32,0x20,0x75,0x76,0x3b,0x0a,0x76,0x61,0x72,0x79,0x69,0x6e,0x67,
    0x20,0x76,0x65,0x63,0x34,0x20,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x61,0x74,0x74,
    0x72,0x69,0x62,0x75,0x74,0x65,0x20,0x76,0x65,0x63,0x34,0x20,0x76,0x78,0x5f,0x63,
    0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x0a,0x76,0x6f,0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,
    0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,
    0x74,0x69,0x6f,0x6e,0x20,0x3d,0x20,0x76,0x65,0x63,0x34,0x28,0x76,0x78,0x5f,0x70,
    0x6f,0x73,0x5f,0x75,0x76,0x2e,0x78,0x79,0x2c,0x20,0x31,0x2e,0x30,0x2c,0x20,0x31,
    0x2e,0x30,0x29,0x3b,0x0a,0x20,0x20,0x20,0x20,0x75,0x76,0x20,0x3d,0x20,0x76,0x78,
    0x5f,0x70,0x6f,0x73,0x5f,0x75,0x76,0x2e,0x7a,0x77,0x3b,0x0a,0x20,0x20,0x20,0x20,
    0x63,0x6f,0x6c,0x6f,0x72,0x20,0x3d,0x20,0x76,0x78,0x5f,0x63,0x6f,0x6c,0x6f,0x72,
    0x3b,0x0a,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x34,0x20,0x5f,0x33,0x37,0x20,0x3d,
    0x20,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x20,0x20,0x20,0x20,0x76,0x65,0x63,0x32,
    0x20,0x5f,0x33,0x39,0x20,0x3d,0x20,0x5f,0x33,0x37,0x2e,0x78,0x79,0x20,0x2a,0x20,
    0x75,0x76,0x3b,0x0a,0x20,0x20,0x20,0x20,0x63,0x6f,0x6c,0x6f,0x72,0x2e,0x78,0x20,
    0x3d,0x20,0x5f,0x33,0x39,0x2e,0x78,0x3b,0x0a,0x20,0x20,0x20,0x20,0x63,0x6f,0x6c,
    0x6f,0x72,0x2e,0x79,0x20,0x3d,0x20,0x5f,0x33,0x39,0x2e,0x79,0x3b,0x0a,0x7d,0x0a,
    0x0a,0x00,
};
/*
    #version 100
    precision mediump float;
    precision highp int;
    
    varying highp vec4 color;
    varying highp vec2 uv;
    
    void main()
    {
        gl_FragData[0] = color;
    }
    
*/
static const char fs_source_glsl100[156] = {
    0x23,0x76,0x65,0x72,0x73,0x69,0x6f,0x6e,0x20,0x31,0x30,0x30,0x0a,0x70,0x72,0x65,
    0x63,0x69,0x73,0x69,0x6f,0x6e,0x20,0x6d,0x65,0x64,0x69,0x75,0x6d,0x70,0x20,0x66,
    0x6c,0x6f,0x61,0x74,0x3b,0x0a,0x70,0x72,0x65,0x63,0x69,0x73,0x69,0x6f,0x6e,0x20,
    0x68,0x69,0x67,0x68,0x70,0x20,0x69,0x6e,0x74,0x3b,0x0a,0x0a,0x76,0x61,0x72,0x79,
    0x69,0x6e,0x67,0x20,0x68,0x69,0x67,0x68,0x70,0x20,0x76,0x65,0x63,0x34,0x20,0x63,
    0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x76,0x61,0x72,0x79,0x69,0x6e,0x67,0x20,0x68,0x69,
    0x67,0x68,0x70,0x20,0x76,0x65,0x63,0x32,0x20,0x75,0x76,0x3b,0x0a,0x0a,0x76,0x6f,
    0x69,0x64,0x20,0x6d,0x61,0x69,0x6e,0x28,0x29,0x0a,0x7b,0x0a,0x20,0x20,0x20,0x20,
    0x67,0x6c,0x5f,0x46,0x72,0x61,0x67,0x44,0x61,0x74,0x61,0x5b,0x30,0x5d,0x20,0x3d,
    0x20,0x63,0x6f,0x6c,0x6f,0x72,0x3b,0x0a,0x7d,0x0a,0x0a,0x00,
};
#if !defined(SOKOL_GFX_INCLUDED)
  #error "Please include sokol_gfx.h before shader.h"
#endif
static inline const sg_shader_desc* quad_shader_desc(sg_backend backend) {
  if (backend == SG_BACKEND_GLES2) {
    static sg_shader_desc desc;
    static bool valid;
    if (!valid) {
      valid = true;
      desc.attrs[0].name = "vx_pos_uv";
      desc.attrs[1].name = "vx_color";
      desc.vs.source = vs_source_glsl100;
      desc.vs.entry = "main";
      desc.fs.source = fs_source_glsl100;
      desc.fs.entry = "main";
      desc.label = "quad_shader";
    }
    return &desc;
  }
  return 0;
}