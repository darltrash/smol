print(sm.backend, sm.OS)

sm.setup{
    title = "Smol demo!"
}

function sm.draw()
    sm.color(0, 0, 0, 1)
    sm.backcolor(1, 1, 1, 1)
    sm.rect(1, 0, 20, 20)
end