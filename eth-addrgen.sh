#!/bin/bash
echo "#####################################"
echo "Welcome to the ETH address generator!"
echo "#####################################"
if [ $# -lt 1 ]
then
echo "input private key (32 bytes, hex format with leading 0x prefix)"
    read priv
else
    priv=$1
fi
priv=$(echo -n $priv | cut -c2-)
## Calculate Public Key
public_key=$(openssl ec -inform DER -text -noout -in <(cat <(echo -n "302e0201010420") <(echo -n $priv) <(echo -n "a00706052b8104000a") | \
    xxd -r -p) 2>/dev/null | tail -6 | head -5 | sed 's/[ :]//g' | tr -d '\n' | cut -c3- && echo)
echo "Public Key: "$public_key
eth_address=$(echo -n $public_key | xxd -p -r | ./keccak -e && echo)
echo "ETH Address:" $eth_address

