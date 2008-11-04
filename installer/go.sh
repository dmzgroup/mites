#!/bin/sh
DEPTH=../../..
lmk -m opt -b
cp -r $DEPTH/bin/macos-opt/mites.app $DEPTH
mkdir $DEPTH/mites.app/Contents/Frameworks/Qt
cp $DEPTH/depend/Qt/QtCore $DEPTH/mites.app/Contents/Frameworks/Qt
cp $DEPTH/depend/Qt/QtGui $DEPTH/mites.app/Contents/Frameworks/Qt
cp $DEPTH/depend/Qt/QtXml $DEPTH/mites.app/Contents/Frameworks/Qt
cp $DEPTH/depend/Qt/QtSvg $DEPTH/mites.app/Contents/Frameworks/Qt
cp $DEPTH/depend/Qt/QtOpenGL $DEPTH/mites.app/Contents/Frameworks/Qt
hdiutil create -srcfolder $DEPTH/mites.app $DEPTH/mites-`cat $DEPTH/tmp/macos-opt/mbraapp/buildnumber.txt`.dmg
hdiutil internet-enable -yes -verbose $DEPTH/mites-`cat $DEPTH/tmp/macos-opt/mbraapp/buildnumber.txt`.dmg
rm -rf $DEPTH/mites.app/
