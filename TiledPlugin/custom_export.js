/// <reference types="@mapeditor/tiled-api" />

var customMapFormat = {
    name: "Custom map format",
    extension: "lua",

    write: function(map, fileName) {

        var cleanedFileName = fileName.substring(fileName.lastIndexOf('/') + 1, fileName.lastIndexOf('.'));

        var file = new TextFile(fileName, TextFile.WriteOnly);

        file.write(cleanedFileName + " = Map:new {\n");
        file.write("    width = " + map.width + ",\n");
        file.write("    height = " + map.height + ",\n");

        for (var i = 0; i < 3; ++i) {

            if (i == 0) {
                file.write("    TerrainMap = {\n");
            }
            else if (i == 1) {
                file.write("    EnvMap = {\n");
            }
            else if (i == 2) {
                file.write("    CharMap = {\n");
            }

            var layer = map.layerAt(i);
            

            if (layer.isTileLayer) {
                for (y = 0; y < layer.height; ++y) {
                    file.write("        ");
                    var maxWidthIndex = layer.width - 1;
                    for (x = 0; x < layer.width; ++x) {
                        var currTile = layer.tileAt(x, y);
                        if (currTile === null) {
                            file.write("_");
                        }
                        else {
                            file.write(currTile.type);
                        }
                        
                        if (x != maxWidthIndex) {
                            file.write(", ")
                        }
                    }
                    file.write("\n");
                }
                file.write("    }");
                if (i != 2) {
                    file.write(",\n")
                }
            }
        }

        file.write("}");
        file.commit();
    },
}

tiled.registerMapFormat("custom", customMapFormat)