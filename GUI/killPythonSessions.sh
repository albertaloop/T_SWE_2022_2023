#!/bin/sh

pkill -x python3


Get-Process | Where Name -Like "python" | Stop-Process