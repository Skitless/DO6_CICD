#!/bin/bash

scp -r src/cat jack@192.168.10.2:/usr/local/bin
scp -r src/grep jack@192.168.10.2:/usr/local/bin

ssh jack@192.168.10.2 ls -lah /usr/local/bin/