test = Map:new {
    name = "Test",
    width = 5,
    height = 5,
    TerrainMap = {
        "x_tile", "v_tile", "x_tile", "x_tile", "x_tile",
        "x_tile", "blank_tile", "blank_tile", "blank_tile", "x_tile",
        "x_tile", "blank_tile", "blank_tile", "blank_tile", "x_tile",
        "x_tile", "blank_tile", "blank_tile", "blank_tile", "x_tile",
        "x_tile", "x_tile", "x_tile", "x_tile", "x_tile",
    },
    EnvMap = {
        _, _, _, _, _,
        _, _, _, _, _,
        _, _, _, _, _,
        _, _, _, _, _,
        _, _, _, _, _,
    },
    CharMap = {
        "blank_tile", _, "blank_tile", _, _,
        _, _, _, _, _,
        _, _, _, _, _,
        _, _, _, _, _,
        _, _, _, _, _,
    }}