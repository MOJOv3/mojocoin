#!/bin/bash

set -e

date
ps axjf

#################################################################
# Update Ubuntu and install prerequisites for running Mojocoin   #
#################################################################
sudo apt-get update
#################################################################
# Build Mojocoin from source                                     #
#################################################################
NPROC=$(nproc)
echo "nproc: $NPROC"
#################################################################
# Install all necessary packages for building Mojocoin           #
#################################################################
sudo apt-get install -y qt4-qmake libqt4-dev libminiupnpc-dev libdb++-dev libdb-dev libcrypto++-dev libqrencode-dev libboost-all-dev build-essential libboost-system-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libboost-filesystem-dev libboost-program-options-dev libboost-thread-dev libssl-dev libdb++-dev libssl-dev ufw git
sudo add-apt-repository -y ppa:bitcoin/bitcoin
sudo apt-get update
sudo apt-get install -y libdb4.8-dev libdb4.8++-dev

cd /usr/local
file=/usr/local/mojocoin
if [ ! -e "$file" ]
then
        sudo git clone https://github.com/MojocoinV3/mojocoin/.git
fi

cd /usr/local/mojocoin/src
file=/usr/local/mojocoin/src/mojocoind
if [ ! -e "$file" ]
then
        sudo make -j$NPROC -f makefile.unix
fi

sudo cp /usr/local/mojocoin/src/mojocoind /usr/bin/mojocoind

################################################################
# Configure to auto start at boot                                      #
################################################################
file=$HOME/.mojocoin
if [ ! -e "$file" ]
then
        sudo mkdir $HOME/.mojocoin
fi
printf '%s\n%s\n%s\n%s\n' 'daemon=1' 'server=1' 'rpcuser=u' 'rpcpassword=p' | sudo tee $HOME/.mojocoin/mojocoin.conf
file=/etc/init.d/mojocoin
if [ ! -e "$file" ]
then
        printf '%s\n%s\n' '#!/bin/sh' 'sudo mojocoind' | sudo tee /etc/init.d/mojocoin
        sudo chmod +x /etc/init.d/mojocoin
        sudo update-rc.d mojocoin defaults
fi

/usr/bin/mojocoind
echo "Mojocoin has been setup successfully and is running..."
exit 0

