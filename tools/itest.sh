#!/bin/sh
out=$(mktemp)

qemu-system-aarch64 -machine virt -cpu cortex-a53 -smp 4 -m 512M \
    -nographic -kernel build/Image < /dev/null > "$out" 2>&1 &
pid=$!
sleep 4
kill "$pid" 2>/dev/null
wait "$pid" 2>/dev/null

fail=0
for pat in "pyrene booting" "pl011 @ 0x0000000009000000" "cpus: *4" "(512 MiB)"; do
    if ! grep -q -- "$pat" "$out"; then
        echo "FAIL: missing '$pat'"
        fail=1
    fi
done

cat "$out"
rm -f "$out"

if [ "$fail" -eq 0 ]; then
    echo "itest: PASS"
fi
exit "$fail"