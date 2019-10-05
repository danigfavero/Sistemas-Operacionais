#!/bin/bash
#
# Script para a execucação de uma VM com o MINIX 3.1.2a.
#
# Por Marcelo Schimittt
# Licença: GPL-2+
# Bugs? Escreva no PACA, ou fale conosco, ou guarde para você :P
#
# Uso: execute esse script com um shell bash. Ele deverá lançar um terminal
#      já conectado à VM, assim que o boot terminar.
#
# Parâmetros extras:
# -b <BRIDGE>    # Nome da bridge a ser criada e usada.
# -i <IMAGE>     # Imagem de disco HD do Minix a ser usada.
# -d <DELAY>     # Delay que deve ser aguardado entre a criação da VM e ssh.
# -t <TAP>       # Nome do dispositivo TAP que o QEMU deverá criar.
# -v             # Modo Verboso.

# Color special caracters.
GREEN='\033[1;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

CDROM=minix3_1_2a_ide_build2.iso
BRIDGE=minixbr0
IMAGE=minix_disk_compressed1.qcow2
DELAY=4
TAP_DEV=minixtap0
VERBOSE_MODE=0

ok () {
	echo -e "${GREEN}OK: $1${NC}\n"
}

fail () {
	echo -e "${RED}ERROR: $1${NC}\n"
	exit 1
}

configure () {
    # Check if bash is bash
    if [ "x$BASH" = "x" ]; then
        fail "This script must be run by bash!"
    fi

    if [ "$EUID" -ne 0 ]; then
        fail "This script must be run by root! Use sudo!"
    fi

    ip addr >/dev/null
    if [ $? -ne 0 ]; then
        fail "Could not find command 'ip'. Please, install it."
    fi

    qemu-system-x86_64 --version > /dev/null
    if [ $? -ne 0 ]; then
        fail "Could not find QEMU. Please install it."
    fi

    if [ ! -f "$CDROM" ]; then
        fail "Could not find CDROM image $CDROM ."
    fi
}

configure

ME=$(basename $0)

while getopts b:i:d:v option
do
case "${option}" in
	b) BRIDGE=${OPTARG};;
	i) IMAGE=${OPTARG};;
	d) DELAY=${OPTARG};;
	t) TAP_DEV=${OPTARG};;
	v) VERBOSE_MODE=1;;
esac
done

# Log function for verbose mode
log () {
	if [ $VERBOSE_MODE -eq 1 ]; then
		echo "$@"
	fi
}

## Fails script if another instance of this script is running
ensure_uniqueness () {
	local count_s=$(pgrep -c -u root $ME)
    local count_vm=$(pgrep -c -u root "qemu.*$IMAGE")
	if [ $count_s != 1 ] && [ $count_vm != 0 ]; then
		fail "Please, close your previous script instance, and all other minix's qemu!"
	fi
}

launch_qemu () {
	qemu-system-x86_64 \
		--enable-kvm \
		-net nic,model=ne2k_isa \
		-net bridge,br=$BRIDGE \
		-netdev tap,id=minix0,ifname=$TAP_DEV,script=no,downscript=no \
		-m 256 \
		-cdrom $CDROM \
		-drive format=qcow2,file=$IMAGE \
		-boot c
}

set_bridge_conf () {
	if [ ! -f /etc/qemu/bridge.conf ]; then
		log "/etc/qemu/bridge.conf not found! Creating one ..."
		mkdir -p /etc/qemu
		touch /etc/qemu/bridge.conf
	fi
	grep $BRIDGE /etc/qemu/bridge.conf >/dev/null
	if [ $? -ne 0 ]; then
		log "including $BRIDGE on list of qemu whitelisted bridges"
        echo "allow $BRIDGE" | tee -a /etc/qemu/bridge.conf
	fi
}

set_bridge () {
	log "deleting possible unconfigured bridge: $BRIDGE"
	ip link del $BRIDGE >/dev/null
	log "creating new bridge: $BRIDGE"
	ip link add name $BRIDGE type bridge
	ip link set $BRIDGE up
	log "bridge route setup"
	ip route add 172.19.50.0/24 dev $BRIDGE
	ip link set $BRIDGE down
	ip address add 172.19.50.1/24 dev $BRIDGE
	log "bringing $BRIDGE up"
	ip link set $BRIDGE up
	sleep 1
	log "starging VM to bring $TAP_DEV interface up"

	# It is needed to start qemu to bring up tap network interface
	launch_qemu &

	while [ ! -d "/sys/class/net/$TAP_DEV" ]
	do
		log "sleep for $DELAY seconds to give qemu enough time to bring $TAP_DEV up" 
		sleep 4
	done

	log "bind $TAP_DEV net interface to $BRIDGE bridge"
	ip link set dev $TAP_DEV master $BRIDGE
	log "bring $BRIDGE up"
	ip link set $BRIDGE up
	log "bridge $BRIDGE set up finished"

}

connect_to_vm () {
	log "wainting VM ssh server to be available ..."
	PING=1
	while [ $PING -ne 0 ]
	do
		ping -c1 -W 1 172.19.50.2 >/dev/null 2>&1
		PING=$?
		log "ping return: $PING"
	done
	sleep 1
	ok "connecting to VM using 'ssh root@172.19.50.2' ..."
	ssh root@172.19.50.2
}

kill_qemu () {
    local pid=$(pgrep -f $IMAGE)
    echo "killing qemu in 5 seconds..."
    sleep 5
    kill $pid
}

log "bridge: $BRIDGE"
log "disk image: $IMAGE"
log "connect delay $DELAY"
log "tap device name $TAP_DEV"

ensure_uniqueness

set_bridge_conf

set_bridge

connect_to_vm

kill_qemu

# TODO
# se tiver uma bridge com nome diferente e mesmo ip dá conflito
