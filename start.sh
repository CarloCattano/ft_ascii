#!/bin/env bash

set -e

PONG_CLI_PATH=../ft_ascii/./pong_cli

alacritty --config-file ~/.config/alacritty/custom.toml -e "$PONG_CLI_PATH"

