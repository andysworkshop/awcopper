#!/bin/sh

echo Creating staging files...

if [ -a /tmp/awcopper ]; then
		echo "/tmp/awcopper directory exists, please remove it manually"
		exit 1
fi

mkdir -p /tmp/awcopper/utilities/UploadToFlash
mkdir -p /tmp/awcopper/utilities/bm2rgbi


if [ ! -d /tmp/awcopper ]; then
		echo "unexpected failure to create /tmp/awcopper directory"
		exit 1
fi

cp libraries/awcopper/* /tmp/awcopper
cp ../common/CommandId.h /tmp/awcopper
cp -r examples /tmp/awcopper
cp ../pc/UploadToFlash/bin/Release/UploadToFlash.exe /tmp/awcopper/utilities/UploadToFlash
cp ../../stm32plus/utils/bm2rgbi/bm2rgbi/bin/Release/bm2rgbi.exe /tmp/awcopper/utilities/bm2rgbi
cp ../../stm32plus/utils/bm2rgbi/bm2rgbi/bin/Release/lzg.exe /tmp/awcopper/utilities/bm2rgbi

echo Building awcopper.zip...

pushd /tmp
zip -r awcopper.zip awcopper 
rm -rf /tmp/awcopper
popd

mv /tmp/awcopper.zip .

echo Done.
