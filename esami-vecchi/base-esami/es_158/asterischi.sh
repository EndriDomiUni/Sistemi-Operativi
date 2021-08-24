#!/bin/bash

# /Users/endridomi/Documents/GitHub/Sistemi-Operativi/esami-vecchi/Sol-esami/2020_01_13__es157_biscie_ONLINE

echo ` find /Users/endridomi/Documents/GitHub/Sistemi-Operativi/Sol-esami/2020_01_13__es157_biscie_ONLINE -type f -name "*.h" -exec -grep -H '*' '{}' \; | wc -l` 1>&2