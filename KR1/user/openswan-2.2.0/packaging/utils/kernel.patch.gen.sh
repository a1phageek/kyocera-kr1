#!/bin/bash
#
# RCSID $Id: kernel.patch.gen.sh,v 1.1 2004/12/24 07:17:31 rupert Exp $

patchdir=`pwd`
kernelsrc=/usr/src/linux
[ "$1~" = "~" ] || kernelsrc=$1
cd $kernelsrc
# clean out destination file for all patch
#echo "">$patchdir/all

# find files to patch and loop
for i in  `find . -name '*.preipsec'`
do

# strip off '.preipsec' suffix
j=${i%.preipsec}

# strip off './' prefix
k=${j#\.\/}

# single unified diff
#diff -u $i $j >>$patchdir/all

# convert '/' in filename to '.' to avoid subdirectories
sed -e 's/\//\./g' << EOI > /tmp/t
$k
EOI
l=`cat /tmp/t`
rm -f /tmp/t

# *with* path from source root
#echo do diff -u $i $j '>' $patchdir/$l
echo found $i
echo "RCSID \$Id: kernel.patch.gen.sh,v 1.1 2004/12/24 07:17:31 rupert Exp $" >$patchdir/$l
diff -u $i $j >>$patchdir/$l

done

#
# $Log: kernel.patch.gen.sh,v $
# Revision 1.1  2004/12/24 07:17:31  rupert
# +: Add OPENSWANS Package
#
# Revision 1.6  2002/04/25 17:04:16  mcr
# 	resurrected kernel.patch.gen.sh
#
# Revision 1.4  1999/04/06 04:54:30  rgb
# Fix/Add RCSID Id: and Log: bits to make PHMDs happy.  This includes
# patch shell fixes.
#
#
