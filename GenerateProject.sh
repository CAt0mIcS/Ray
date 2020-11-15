# Edit for Architecture and Configuration
# Possible Values: 
#   architecture:   x86   | x64
#   configuration:  Debug | Release

architecture=x86
configuration=Debug


unameOut="$(uname -s)"
case "${unameOut}" in
    Linux*)     machine=Linux;;
    Darwin*)    machine=Mac;;
    *)          machine="UNKNOWN:${unameOut}"
esac

buildPath=build-$configuration-$unameOut-$architecture
echo Selecting Build Settings "$configuration" "$unameOut" and "$architecture"


[ ! -d "./$buildPath" ] && mkdir ./$buildPath
cd ./$buildPath
cmake .. -DCMAKE_BUILD_TYPE=$configuration -DRL_ARCHITECTURE=$architecture

read -s -n 1 -p "Press any key to continue . . ."
cd ..
echo ""
