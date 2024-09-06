#!/bin/env bash

make

PONG_CLI_PATH=./pong_cli

alacritty --config-file ~/.config/alacritty/custom.toml -e "$PONG_CLI_PATH" &

