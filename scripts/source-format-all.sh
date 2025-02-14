#! /bin/bash -e

# rootPath=$(pwd)
# while [ $(basename "$rootPath") != "hub" ]; do
#     rootPath=$(dirname "$rootPath")
# done
# cd $rootPath
CLANG_FORMAT_EXE=clang-format

if [ -e "/opt/clang-format-static/clang-format-18" ]; then
	CLANG_FORMAT_EXE="/opt/clang-format-static/clang-format-18"
fi

CI_CLANG_FORMAT_VERSION_MAJOR="18"
CI_CLANG_FORMAT_VERSION_MINOR="1"
CI_CLANG_FORMAT_VERSION_PATCH="3"
CI_CLANG_FORMAT_VERSION=$CI_CLANG_FORMAT_VERSION_MAJOR.$CI_CLANG_FORMAT_VERSION_MINOR.$CI_CLANG_FORMAT_VERSION_PATCH

# TMP=$($CLANG_FORMAT_EXE --version | awk -F' ' '{print $NF}')
TMP="Ubuntu clang-format version 18.1.3 (1ubuntu1)"
CLANG_FORMAT_VERSION_MAJOR=$(echo $TMP | awk -F'.' '{print $1}' | awk -F' ' '{print $NF}')
CLANG_FORMAT_VERSION_MINOR=$(echo $TMP | awk -F'.' '{print $2}')
CLANG_FORMAT_VERSION_PATCH=$(echo $TMP | awk -F'.' '{print $3}' | awk -F' ' '{print $1}')
CLANG_FORMAT_VERSION=$CLANG_FORMAT_VERSION_MAJOR.$CLANG_FORMAT_VERSION_MINOR.$CLANG_FORMAT_VERSION_PATCH
# echo "clang format version = $CLANG_FORMAT_VERSION"
# echo "clang format major = $CLANG_FORMAT_VERSION_MAJOR"
# echo "clang format minor = $CLANG_FORMAT_VERSION_MINOR"
# echo "clang format patch = $CLANG_FORMAT_VERSION_PATCH"
# if [ "$CLANG_FORMAT_VERSION" != "$CI_CLANG_FORMAT_VERSION" ]; then
if [ "$CLANG_FORMAT_VERSION_MAJOR" != "$CI_CLANG_FORMAT_VERSION_MAJOR" ]; then
	echo "clang-format need to be compatible with CI version : $CI_CLANG_FORMAT_VERSION"
	echo "current version is $CLANG_FORMAT_VERSION"
	exit 1
fi


# exit 0

cd $(git rev-parse --show-toplevel)

# export PATH="/opt/clang-format-static:$PATH"

# echo "using clang-format : $(clang-format --version)"

for file in $(find $(cat scripts/source_dirs.txt | grep -v '^*') -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" -o -name "*.inl" \)); do
	echo "source format: $file"

	# clang-format-11 -i --style=file $file

	# clang-format on ubuntu ci is on version 14.0.0
	# if [ -e "/opt/clang-format-static/clang-format-14" ]; then
		# clang-format-14 -i --style=file $file
	# else
		# clang-format -i --style=file $file
	# fi
	$CLANG_FORMAT_EXE -i --style=file $file
done
