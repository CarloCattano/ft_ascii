#!/bin/env bash

set -e

PONG_CLI_PATH=../ft_ascii/./pong_cli

if pgrep -f "pong_cli" > /dev/null; then
		echo "pong_cli is already running"
		exit 1
fi

alacritty --config-file ../transcendCLI/custom.toml -e "$PONG_CLI_PATH" &2>1 > /dev/null

