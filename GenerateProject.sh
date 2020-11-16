architecture=x64
configuration=Debug

for var in "$@"
do
    if [ "$var" = "x86" ]; then
        architecture=x86
    fi

    if [ "$var" = "Release" ]; then
        configuration="Release"
    fi
done

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
cmake .. -DCMAKE_BUILD_TYPE=$configuration -DRL_ARCHITECTURE=$architecture -DGLFW_INSTALL=0 -DGLFW_BUILD_DOCS=0

read -s -n 1 -p "Press any key to continue . . ."
cd ..
echo ""
