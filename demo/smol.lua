sm.setup{
    title = "Smol demo!"
}

local quads = {}
local r = math.random

function sm.draw()
    if sm.mouse(1) then -- First mouse button pressed
        local x, y = sm.mouse() -- Mouse position 
        x = x - (sm.width / 2)
        y = y - (sm.height / 2)

        for i = 1, math.ceil(sm.delta * 128) do
            table.insert(quads, {
                x = x + r(-5, 5), 
                y = y + r(-5, 5), 
                s = r(1, 30), -- Random size,
                c = r(1, 10)/10
            })
        end
    end
    
    local generation = {}
    for k, v in ipairs(quads) do
        sm.color(1, 1.0 - (v.s/40), 0, v.s/30)

        -- Draw centered rectangles
        sm.rect(v.x-(v.s/2), v.y-(v.s/2), v.s, v.s)

        -- Make rectangles decrease in size
        v.s = v.s - sm.delta * 13
        v.y = v.y - sm.delta * 20 * (v.s/10)
        v.x = v.x + math.sin((sm.timer + k)*16) * (v.s/60)

        -- Only keep rectangles that arent very small
        if v.s > 0 then
            table.insert(generation, v)
        end
    end
end