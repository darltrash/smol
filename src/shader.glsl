/* quad vertex shader */
@vs vs
in vec4 vx_pos_uv;
in vec4 vx_color;

out vec2 uv;
out vec4 color;

void main() {
    uv = vx_pos_uv.zw;
    color = vx_color;
    
    gl_Position = vec4(vx_pos_uv.xy, 0.0, 1.0);
}
@end

/* quad fragment shader */
@fs fs
in vec2 uv;
in vec4 color;

out vec4 frag_color;

void main() {
    frag_color = color;
}
@end

/* quad shader program */
@program quad vs fs