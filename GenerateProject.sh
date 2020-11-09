
architecture=x86
configuration=Debug
for arg in "$@"
do
	if [ "$arg"=="x86" ] || [ "$arg"=="x64" ]
		then
			architecture=$arg
			echo Setting Build Architecture to "$arg"
	fi

	if [ "$arg"=="Debug" ] || [ "$arg"=="Release" ]
		then
			configuration=$arg
			echo Setting Build Configuration to "$arg"
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
cmake ..

read -s -n 1 -p "Press any key to continue . . ."
cd ..
echo ""
