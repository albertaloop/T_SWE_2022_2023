#!/bin/sh

pkill -x python3



# If you an unlucklily running this on Windows, firstly why? what? how?. Use this code to close all python sessions from Powershell.
# Get-Process | Where Name -Like "python" | Stop-Process