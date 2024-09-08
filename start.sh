#!/bin/env bash

set -e

PONG_CLI_PATH=../ft_ascii/./pong_cli


# check if pong_cli is already running and if so dont open alacritty
if pgrep -f "$PONG_CLI_PATH" > /dev/null; then
		echo "pong_cli is already running"
		exit 1
fi

alacritty --config-file ../transcendCLI/custom.toml -e "$PONG_CLI_PATH" &

