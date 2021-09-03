#!/bin/bash

set -e

pio run && pio run --target upload && pio device monitor
