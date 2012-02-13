#!/bin/sh

: ==== start ====

D=/testing/scripts/ipsec.conf-restart-01

export IPSEC_CONFS="$D/etc-norestart"
ipsec setup --showonly start | sed -e 's=\(Starting Openswan IPsec \).*\.\.\.=\1DATE...='

export IPSEC_CONFS="$D/etc-restart"
ipsec setup --showonly start | sed -e 's=\(Starting Openswan IPsec \).*\.\.\.=\1DATE...='

: ==== end ====
