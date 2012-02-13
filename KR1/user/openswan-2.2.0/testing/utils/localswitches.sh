#!/bin/sh

BUILDTOP=${MYBOX-/c2/freeswan/freeswan-1.92}
export BUILDTOP
OPENSWANSRCDIR=$BUILDTOP 
export OPENSWANSRCDIR

. $BUILDTOP/umlsetup.sh

unset UML_public_CTL
unset UML_west_CTL
unset UML_east_CTL

set -a
source testparams.sh

expect -f $BUILDTOP/testing/utils/localswitches.tcl $*

