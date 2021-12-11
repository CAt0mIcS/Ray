import os
import sys

path = sys.argv[1] if len(sys.argv) > 1 else "../build-Debug-Windows-x64"


for directory in os.walk(path):
    for filename in os.listdir(directory[0]):
        if filename.endswith(".vcxproj") or filename.endswith(".vcproj"):
            try:
                file_content = ""
                with open(directory[0] + "/" + filename) as stream:
                    try:
                        for line in stream.readlines():
                            file_content += line
                    except UnicodeDecodeError:
                        continue

                file_content = file_content.replace("<PlatformToolset>v139</PlatformToolset>", "<PlatformToolset>v143</PlatformToolset>").replace(
                    "ToolsVersion=\"13.0\"", "ToolsVersion=\"17.0\"")

                file_content = file_content.replace("<PlatformToolset>v140</PlatformToolset>", "<PlatformToolset>v143</PlatformToolset>").replace(
                    "ToolsVersion=\"14.0\"", "ToolsVersion=\"17.0\"")

                file_content = file_content.replace("<PlatformToolset>v141</PlatformToolset>", "<PlatformToolset>v143</PlatformToolset>").replace(
                    "ToolsVersion=\"15.0\"", "ToolsVersion=\"17.0\"")

                file_content = file_content.replace("<PlatformToolset>v142</PlatformToolset>", "<PlatformToolset>v143</PlatformToolset>").replace(
                    "ToolsVersion=\"16.0\"", "ToolsVersion=\"17.0\"")

                with open(directory[0] + "/" + filename, "wt") as stream:
                    stream.write(file_content)
            except PermissionError:
                continue
