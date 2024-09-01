#!/bin/env bash

touch /tmp/pong_in
touch /tmp/pong_out

PONG_CLI_PATH=./pong_cli

alacritty --config-file ~/.config/alacritty/custom.toml -e "$PONG_CLI_PATH" &

