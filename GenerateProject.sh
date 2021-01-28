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

    if [ "$var" = "RelWithDebInfo" ]; then
        configuration="RelWithDebInfo"
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
cmake .. -DCMAKE_BUILD_TYPE=$configuration -DRAY_ARCHITECTURE=$architecture

read -s -n 1 -p "Press any key to continue . . ."
cd ..
echo ""
