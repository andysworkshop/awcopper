#!/bin/sh

echo Building awcopper.zip...
rm -f awcopper.zip

if [ -f awcopper.zip ] ; then
		echo unable to remove awcopper.zip, please remove it
		exit 1
fi

cp ../../common/CommandId.h awcopper
zip -r awcopper.zip awcopper 
rm awcopper/CommandId.h

echo Done.
